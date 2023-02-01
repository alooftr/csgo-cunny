#pragma once
#include "../../../sdk/interfaces/i_physics_surface_props.hpp"
#include "../../../sdk/interfaces/i_client_entity.hpp"

struct FireBulletData_t
{
	vec3_t			position = { };
	vec3_t			direction = { };
	trace_t         enter_trace = { };
	float			current_damage = 0.0f;
	int				penetrate_count = 0;
};

// @credits: outlassn
class c_auto_wall
{
public:
	// Get
	/* returns damage at point and simulated bullet data (if given) */
	static float get_damage(c_base_entity* pLocal, const vec3_t& vecPoint, FireBulletData_t* pDataOut = nullptr);
	/* calculates damage factor */
	static void scale_damage(const int iHitGroup, c_base_entity* pEntity, const float flWeaponArmorRatio, const float flWeaponHeadShotMultiplier, float& flDamage);
	/* simulates fire bullet to penetrate up to 4 walls, return true when hitting player */
	static bool simulate_fire_bullet(c_base_entity* pLocal, c_base_combat_weapon* pWeapon, FireBulletData_t& data);

private:
	// Main
	static void clip_trace_to_players(const vec3_t& vecAbsStart, const vec3_t& vecAbsEnd, const unsigned int fMask, i_trace_filter* pFilter, trace_t* pTrace, const float flMinRange = 0.0f);
	static bool trace_to_exit(trace_t& enterTrace, trace_t& exitTrace, const vec3_t& vecPosition, const vec3_t& vecDirection, const c_base_entity* pClipPlayer);
	static bool handle_bullet_penetration(c_base_entity* pLocal, const ccs_weapon_data* pWeaponData, const surfacedata_t* pEnterSurfaceData, FireBulletData_t& data);
};
