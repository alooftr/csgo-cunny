#include "auto_wall.hpp"
#include "../../../utils/math.h"
#include "../../../sdk/sdk.hpp"

float c_auto_wall::get_damage(c_base_entity* pLocal, const vec3_t& vecPoint, FireBulletData_t* pDataOut)
{
	const vec3_t position = pLocal->get_eye_position();

	// setup data
	FireBulletData_t data = { };
	data.position = position;
	data.direction = (vecPoint - position).normalized();

	if (c_base_combat_weapon* pWeapon = pLocal->get_active_weapon(); pWeapon == nullptr || !simulate_fire_bullet(pLocal, pWeapon, data))
		return -1.0f;

	if (pDataOut != nullptr)
		*pDataOut = data;

	return data.current_damage;
}

void c_auto_wall::scale_damage(const int iHitGroup, c_base_entity* pEntity, const float flWeaponArmorRatio, const float flWeaponHeadShotMultiplier, float& flDamage)
{
	// @ida traceattack: server.dll @ 55 8B EC 83 E4 F8 81 EC C0 00 00 00 56 8B 75 08 57 8B F9 C6 44 24 13 01

	const bool bHeavyArmor = pEntity->has_heavy_armor();

	static c_convar* mp_damage_scale_ct_head = csgo::convar->find_var("mp_damage_scale_ct_head");
	static c_convar* mp_damage_scale_t_head = csgo::convar->find_var("mp_damage_scale_t_head");

	static c_convar* mp_damage_scale_ct_body = csgo::convar->find_var("mp_damage_scale_ct_body");
	static c_convar* mp_damage_scale_t_body = csgo::convar->find_var("mp_damage_scale_t_body");

	float flHeadDamageScale = pEntity->get_team() == team_ct ? mp_damage_scale_ct_head->get_float() : pEntity->get_team() == team_tt ? mp_damage_scale_t_head->get_float() : 1.0f;
	const float flBodyDamageScale = pEntity->get_team() == team_ct ? mp_damage_scale_ct_body->get_float() : pEntity->get_team() == team_tt ? mp_damage_scale_t_body->get_float() : 1.0f;

	if (bHeavyArmor)
		flHeadDamageScale *= 0.5f;

	switch (iHitGroup)
	{
	case hitgroup_head:
		flDamage *= flWeaponHeadShotMultiplier * flHeadDamageScale;
		break;
	case hitgroup_chest:
	case hitgroup_leftarm:
	case hitgroup_rightarm:
	case hitgroup_neck:
		flDamage *= flBodyDamageScale;
		break;
	case hitgroup_stomach:
		flDamage *= 1.25f * flBodyDamageScale;
		break;
	case hitgroup_leftleg:
	case hitgroup_rightleg:
		flDamage *= 0.75f * flBodyDamageScale;
		break;
	default:
		break;
	}

	if (pEntity->is_armored(iHitGroup))
	{
		// @ida ontakedamage: server.dll @ 80 BF ? ? ? ? ? F3 0F 10 5C 24 ? F3 0F 10 35

		const int iArmor = pEntity->get_armor();
		float flHeavyArmorBonus = 1.0f, flArmorBonus = 0.5f, flArmorRatio = flWeaponArmorRatio * 0.5f;

		if (bHeavyArmor)
		{
			flHeavyArmorBonus = 0.25f;
			flArmorBonus = 0.33f;
			flArmorRatio *= 0.20f;
		}

		float flDamageToHealth = flDamage * flArmorRatio;
		if (const float flDamageToArmor = (flDamage - flDamageToHealth) * (flHeavyArmorBonus * flArmorBonus); flDamageToArmor > static_cast<float>(iArmor))
			flDamageToHealth = flDamage - static_cast<float>(iArmor) / flArmorBonus;

		flDamage = flDamageToHealth;
	}
}

// @credits: https://github.com/perilouswithadollarsign/cstrike15_src/blob/master/game/shared/util_shared.cpp#L757
void c_auto_wall::clip_trace_to_players(const vec3_t& vecAbsStart, const vec3_t& vecAbsEnd, const unsigned int fMask, i_trace_filter* pFilter, trace_t* pTrace, const float flMinRange)
{
	// @ida util_cliptracetoplayers: client.dll @ E8 ? ? ? ? 0F 28 84 24 68 02 00 00

	trace_t trace = { };
	float flSmallestFraction = pTrace->fraction;

	const ray_t ray(vecAbsStart, vecAbsEnd);

	for (int i = 1; i < csgo::global_vars->max_clients; i++)
	{
		c_base_entity* pEntity = csgo::entity_list->get<c_base_entity>(i);

		if (pEntity == nullptr || !pEntity->is_alive() || pEntity->is_dormant())
			continue;

		if (pFilter != nullptr && !pFilter->should_hit_entity(pEntity, fMask))
			continue;

		const i_collideable* pCollideable = pEntity->get_collideable();

		if (pCollideable == nullptr)
			continue;

		// get bounding box
		const vec3_t vecMin = pCollideable->obb_mins();
		const vec3_t vecMax = pCollideable->obb_maxs();

		// calculate world space center
		const vec3_t vecCenter = (vecMax + vecMin) * 0.5f;
		const vec3_t position = vecCenter + pEntity->get_origin();

		const vec3_t vecTo = position - vecAbsStart;
		vec3_t direction = vecAbsEnd - vecAbsStart;
		const float flLength = direction.normalize_in_place();

		const float flRangeAlong = direction.dot_product(vecTo);
		float flRange = 0.0f;

		// calculate distance to ray
		if (flRangeAlong < 0.0f)
			// off start point
			flRange = -vecTo.length();
		else if (flRangeAlong > flLength)
			// off end point
			flRange = -(position - vecAbsEnd).length();
		else
			// within ray bounds
			flRange = (position - (direction * flRangeAlong + vecAbsStart)).length();

		constexpr float flMaxRange = 60.f;
		if (flRange < flMinRange || flRange > flMaxRange)
			continue;

		csgo::engine_trace->clip_ray_to_entity(ray, fMask | CONTENTS_HITBOX, pEntity, &trace);

		if (trace.fraction < flSmallestFraction)
		{
			// we shortened the ray - save off the trace
			*pTrace = trace;
			flSmallestFraction = trace.fraction;
		}
	}
}

bool c_auto_wall::trace_to_exit(trace_t& enter_trace, trace_t& exitTrace, const vec3_t& position, const vec3_t& direction, const c_base_entity* pClipPlayer)
{
	// @ida tracetoexit: client.dll @ 55 8B EC 83 EC 4C F3
	// server.dll @ 55 8B EC 83 EC 4C F3 0F 10 75

	float flDistance = 0.0f;
	int iStartContents = 0;

	while (flDistance <= 90.0f)
	{
		// add extra distance to our ray
		flDistance += 4.0f;

		// multiply the direction vector to the distance so we go outwards, add our position to it
		vec3_t vecStart = position + direction * flDistance;

		if (!iStartContents)
			iStartContents = csgo::engine_trace->get_point_contents(vecStart, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);

		const int iCurrentContents = csgo::engine_trace->get_point_contents(vecStart, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);

		if (!(iCurrentContents & MASK_SHOT_HULL) || (iCurrentContents & CONTENTS_HITBOX && iCurrentContents != iStartContents))
		{
			// setup our end position by deducting the direction by the extra added distance
			const vec3_t vecEnd = vecStart - (direction * 4.0f);

			// trace ray to world
			ray_t rayWorld(vecStart, vecEnd);
			csgo::engine_trace->trace_ray(rayWorld, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr, &exitTrace);

			if (static c_convar* sv_clip_penetration_traces_to_players = csgo::convar->find_var("sv_clip_penetration_traces_to_players"); sv_clip_penetration_traces_to_players != nullptr && sv_clip_penetration_traces_to_players->get_bool())
			{
				c_trace_filter filter(pClipPlayer);
				clip_trace_to_players(vecEnd, vecStart, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &exitTrace, -60.f);
			}

			// check if a hitbox is in-front of our enemy and if they are behind of a solid wall
			if (exitTrace.start_solid && exitTrace.surface.flags & SURF_HITBOX)
			{
				// trace ray to entity
				ray_t ray(vecStart, position);
				c_trace_filter filter(exitTrace.hit_entity);

				csgo::engine_trace->trace_ray(ray, MASK_SHOT_HULL, &filter, &exitTrace);

				if (exitTrace.did_hit() && !exitTrace.start_solid)
				{
					vecStart = exitTrace.end;
					return true;
				}

				continue;
			}

			if (exitTrace.did_hit() && !exitTrace.start_solid)
			{
				if (enter_trace.hit_entity->is_breakable() && exitTrace.hit_entity->is_breakable())
					return true;

				if (enter_trace.surface.flags & SURF_NODRAW || (!(exitTrace.surface.flags & SURF_NODRAW) && exitTrace.plane.normal.dot_product(direction) <= 1.0f))
				{
					const float flMultiplier = exitTrace.fraction * 4.0f;
					vecStart -= direction * flMultiplier;
					return true;
				}

				continue;
			}

			if (!exitTrace.did_hit() || exitTrace.start_solid)
			{
				if (enter_trace.hit_entity != nullptr && enter_trace.hit_entity->get_index() != 0 && enter_trace.hit_entity->is_breakable())
				{
					// did hit breakable non world entity
					exitTrace = enter_trace;
					exitTrace.end = vecStart + direction;
					return true;
				}

				continue;
			}
		}
	}

	return false;
}

bool c_auto_wall::handle_bullet_penetration(c_base_entity* pLocal, const ccs_weapon_data* pWeaponData, const surfacedata_t* pEnterSurfaceData, FireBulletData_t& data)
{
	// @ida handlebulletpenetration: client.dll @ E8 ? ? ? ? 83 C4 40 84 C0

	static c_convar* ff_damage_reduction_bullets = csgo::convar->find_var("ff_damage_reduction_bullets");
	static c_convar* ff_damage_bullet_penetration = csgo::convar->find_var("ff_damage_bullet_penetration");

	const float flReductionDamage = ff_damage_reduction_bullets->get_float();
	const float flPenetrateDamage = ff_damage_bullet_penetration->get_float();

	const material_handle_t hEnterMaterial = pEnterSurfaceData->game.hMaterial;

	if (data.penetrate_count == 0 && hEnterMaterial != CHAR_TEX_GRATE && hEnterMaterial != CHAR_TEX_GLASS && !(data.enter_trace.surface.flags & SURF_NODRAW))
		return false;

	if (pWeaponData->penetration <= 0.0f || data.penetrate_count <= 0)
		return false;

	trace_t exitTrace = { };
	if (!trace_to_exit(data.enter_trace, exitTrace, data.enter_trace.end, data.direction, pLocal) && !(csgo::engine_trace->get_point_contents(data.enter_trace.end, MASK_SHOT_HULL, nullptr) & MASK_SHOT_HULL))
		return false;

	const surfacedata_t* pExitSurfaceData = csgo::physics_props->get_surface_data(exitTrace.surface.surface_props);
	const material_handle_t hExitMaterial = pExitSurfaceData->game.hMaterial;

	const float flEnterPenetrationModifier = pEnterSurfaceData->game.flPenetrationModifier;
	const float flExitPenetrationModifier = pExitSurfaceData->game.flPenetrationModifier;

	float flDamageLostModifier = 0.16f;
	float flPenetrationModifier = 0.0f;

	if (hEnterMaterial == CHAR_TEX_GRATE || hEnterMaterial == CHAR_TEX_GLASS)
	{
		flDamageLostModifier = 0.05f;
		flPenetrationModifier = 3.0f;
	}
	else if (((data.enter_trace.contents >> 3) & CONTENTS_SOLID) || ((data.enter_trace.surface.flags >> 7) & SURF_LIGHT))
	{
		flDamageLostModifier = 0.16f;
		flPenetrationModifier = 1.0f;
	}
	else if (hEnterMaterial == CHAR_TEX_FLESH && flReductionDamage == 0.0f && data.enter_trace.hit_entity != nullptr && data.enter_trace.hit_entity->is_player() && (pLocal->get_team() == data.enter_trace.hit_entity->get_team()))
	{
		if (flPenetrateDamage == 0.0f)
			return false;

		// shoot through teammates
		flDamageLostModifier = flPenetrateDamage;
		flPenetrationModifier = flPenetrateDamage;
	}
	else
	{
		flDamageLostModifier = 0.16f;
		flPenetrationModifier = (flEnterPenetrationModifier + flExitPenetrationModifier) * 0.5f;
	}

	if (hEnterMaterial == hExitMaterial)
	{
		if (hExitMaterial == CHAR_TEX_CARDBOARD || hExitMaterial == CHAR_TEX_WOOD)
			flPenetrationModifier = 3.0f;
		else if (hExitMaterial == CHAR_TEX_PLASTIC)
			flPenetrationModifier = 2.0f;
	}

	const float flTraceDistance = (exitTrace.end - data.enter_trace.end).length_sqr();

	// penetration modifier
	const float flModifier = (flPenetrationModifier > 0.0f ? 1.0f / flPenetrationModifier : 0.0f);

	// this calculates how much damage we've lost depending on thickness of the wall, our penetration, damage, and the modifiers set earlier
	const float flLostDamage = (data.current_damage * flDamageLostModifier + (pWeaponData->penetration > 0.0f ? 3.75f / pWeaponData->penetration : 0.0f) * (flModifier * 3.0f)) + ((flModifier * flTraceDistance) / 24.0f);

	// did we loose too much damage?
	if (flLostDamage > data.current_damage)
		return false;

	// we can't use any of the damage that we've lost
	if (flLostDamage > 0.0f)
		data.current_damage -= flLostDamage;

	// do we still have enough damage to deal?
	if (data.current_damage < 1.0f)
		return false;

	data.position = exitTrace.end;
	--data.penetrate_count;
	return true;
}

bool c_auto_wall::simulate_fire_bullet(c_base_entity* pLocal, c_base_combat_weapon* pWeapon, FireBulletData_t& data)
{
	// @ida firebullet: client.dll @ 55 8B EC 83 E4 F0 81 EC ? ? ? ? F3 0F 7E

	ccs_weapon_data* pWeaponData = csgo::weapon_system->get_weapon_data(pWeapon->get_item_definition_index());

	if (pWeaponData == nullptr)
		return false;

	float flMaxRange = pWeaponData->range;

	// the total number of surfaces any bullet can penetrate in a single flight is capped at 4
	data.penetrate_count = 4;
	// set our current damage to what our gun's initial damage reports it will do
	data.current_damage = static_cast<float>(pWeaponData->damage);

	float flTraceLenght = 0.0f;
	c_trace_filter filter(pLocal);

	while (data.penetrate_count > 0 && data.current_damage >= 1.0f)
	{
		// max bullet range
		flMaxRange -= flTraceLenght;

		// end position of bullet
		const vec3_t vecEnd = data.position + data.direction * flMaxRange;

		ray_t ray(data.position, vecEnd);
		csgo::engine_trace->trace_ray(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &data.enter_trace);

		// check for player hitboxes extending outside their collision bounds
		clip_trace_to_players(data.position, vecEnd + data.direction * 40.0f, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &data.enter_trace);

		const surfacedata_t* pEnterSurfaceData = csgo::physics_props->get_surface_data(data.enter_trace.surface.surface_props);
		const float flEnterPenetrationModifier = pEnterSurfaceData->game.flPenetrationModifier;

		// we didn't hit anything, stop tracing shoot
		if (data.enter_trace.fraction == 1.0f)
			break;

		// calculate the damage based on the distance the bullet traveled
		flTraceLenght += data.enter_trace.fraction * flMaxRange;
		data.current_damage *= std::powf(pWeaponData->range_modifier, flTraceLenght / MAX_DAMAGE);

		// check is actually can shoot through
		if (flTraceLenght > 3000.f || flEnterPenetrationModifier < 0.1f)
			break;

		// check is can do damage
		if (data.enter_trace.hit_group != hitgroup_generic && data.enter_trace.hit_group != hitgroup_gear && pLocal->is_enemy(data.enter_trace.hit_entity))
		{
			// we got target - scale damage
			scale_damage(data.enter_trace.hit_group, data.enter_trace.hit_entity, pWeaponData->armor_ratio, pWeaponData->head_shot_multiplier, data.current_damage);
			return true;
		}

		// calling handlebulletpenetration here reduces our penetration ñounter, and if it returns true, we can't shoot through it
		if (!handle_bullet_penetration(pLocal, pWeaponData, pEnterSurfaceData, data))
			break;
	}

	return false;
}
