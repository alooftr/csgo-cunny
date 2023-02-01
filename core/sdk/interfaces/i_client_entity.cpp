#include "i_client_entity.hpp"
#include "../sdk.hpp"
#include "../../utils/math.h"

#pragma region entity_baseentity
vec3_t c_base_entity::get_aim_punch_angle_scaled( )
{
	if (!this)
		return vec3_t( );

	static auto a = csgo::convar->find_var( "weapon_recoil_scale" );
	return this->get_punch( ) * a->get_float( );
}
c_base_entity* c_base_entity::get_local_player( )
{
	return csgo::entity_list->get<c_base_entity>( csgo::engine->get_local_player( ) );
}

int c_base_entity::get_sequence_activity( int sequence )
{
	studiohdr_t* studio_hdr = csgo::model_info->get_studio_model( this->get_model( ) );

	if (studio_hdr == nullptr)
		return -1;

	using get_sequence_activity_fn = int( __fastcall* )(void*, void*, int);
	static auto o_get_sequence_activity = reinterpret_cast<get_sequence_activity_fn>(utils::find_pattern( "client.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83" )); // @xref: "Need to handle the activity %d\n"
	assert( o_get_sequence_activity != nullptr );

	return o_get_sequence_activity( this, studio_hdr, sequence );
}

int c_base_entity::get_max_health( )
{
	//check is dangerzone
	if (csgo::game_types->get_current_game_type( ) == gametype_freeforall)
		return 120;

	return 100;
}

std::optional<vec3_t> c_base_entity::get_bone_position( int bone_index )
{
	assert( bone_index > bone_invalid && bone_index < MAXSTUDIOBONES ); // given invalid bone index for getboneposition

	std::array<matrix3x4_t, MAXSTUDIOBONES> arrBonesToWorld = { };

	if (this->setup_bones( arrBonesToWorld.data( ), arrBonesToWorld.size( ), BONE_USED_BY_ANYTHING, 0.f ))
		return arrBonesToWorld.at( bone_index ).at( 3 );

	return std::nullopt;
}

int c_base_entity::get_bone_by_hash( const fnv1a_t bone_hash ) const
{
	if (const auto model = this->get_model( ); model != nullptr)
	{
		if (const auto studio_hdr = csgo::model_info->get_studio_model( model ); studio_hdr != nullptr)
		{
			for (int i = 0; i < studio_hdr->nBones; i++)
			{
				if (const auto pBone = studio_hdr->get_bone( i ); pBone != nullptr && fnv1a::hash( pBone->get_name( ) ) == bone_hash)
					return i;
			}
		}
	}

	return bone_invalid;
}

std::optional<vec3_t> c_base_entity::get_hitbox_position( const int iHitbox )
{
	assert( iHitbox > hitbox_invalid && iHitbox < hitbox_max ); // given invalid hitbox index for gethitboxposition

	std::array<matrix3x4_t, MAXSTUDIOBONES> arrBonesToWorld = { };

	if (const auto pHitbox = this->get_hitbox( iHitbox ); pHitbox != nullptr)
	{
		if (this->setup_bones( arrBonesToWorld.data( ), arrBonesToWorld.size( ), BONE_USED_BY_HITBOX, 0.f ))
		{
			// get mins/maxs by bone
			vec3_t vecMin, vecMax;
			math::transform_vector( pHitbox->bb_min, arrBonesToWorld.at( pHitbox->bone ), vecMin );
			math::transform_vector( pHitbox->bb_max, arrBonesToWorld.at( pHitbox->bone ), vecMax );

			// get center
			return (vecMin + vecMax) * 0.5f;
		}
	}

	return std::nullopt;
}

std::optional<vec3_t> c_base_entity::get_hit_group_position( const int iHitGroup )
{
	assert( iHitGroup >= hitgroup_generic && iHitGroup <= hitgroup_gear ); // given invalid hitbox index for gethitgroupposition

	std::array<matrix3x4_t, MAXSTUDIOBONES> arrBonesToWorld = { };

	if (const auto model = this->get_model( ); model != nullptr)
	{
		if (const auto studio_hdr = csgo::model_info->get_studio_model( model ); studio_hdr != nullptr)
		{
			if (const auto pHitboxSet = studio_hdr->get_hitbox_set( this->get_hitbox_set( ) ); pHitboxSet != nullptr)
			{
				if (this->setup_bones( arrBonesToWorld.data( ), arrBonesToWorld.size( ), BONE_USED_BY_HITBOX, 0.f ))
				{
					mstudiobbox_t* pHitbox = nullptr;
					for (int i = 0; i < pHitboxSet->hitboxes; i++)
					{
						pHitbox = pHitboxSet->get_hitbox( i );

						// check is reached needed group
						if (pHitbox->group == iHitGroup)
							break;
					}

					if (pHitbox != nullptr)
					{
						// get mins/maxs by bone
						vec3_t vecMin, vecMax;
						math::transform_vector( pHitbox->bb_min, arrBonesToWorld.at( pHitbox->bone ), vecMin );
						math::transform_vector( pHitbox->bb_max, arrBonesToWorld.at( pHitbox->bone ), vecMax );

						// get center
						return (vecMin + vecMax) * 0.5f;
					}
				}
			}
		}
	}

	return std::nullopt;
}

void c_base_entity::modify_eye_position( const ccsgo_player_anim_state* anim_state, vec3_t* position ) const
{
	// @ida modifyeyeposition: client.dll @ 55 8B EC 83 E4 F8 83 EC 70 56 57 8B F9 89 7C 24 38

	if (csgo::engine->is_hltv( ) || csgo::engine->is_playing_demo( ))
		return;

	c_base_entity* pBaseEntity = anim_state->entity;

	if (pBaseEntity == nullptr)
		return;

	i_client_entity* pGroundEntity = csgo::entity_list->get_client_entity_from_handle( pBaseEntity->get_ground_entity_handle( ) );

	if (!anim_state->hit_ground_animation && anim_state->duck_amount == 0.f && pGroundEntity != nullptr)
		return;

	if (const auto headPosition = pBaseEntity->get_bone_position( pBaseEntity->get_bone_by_hash( fnv1a::hash_const( "head_0" ) ) ); headPosition.has_value( ))
	{
		vec3_t vecHead = headPosition.value( );
		vecHead.z += 1.7f;

		if (vecHead.z < position->z)
		{
			float flFactor = 0.f;
			const float flDelta = std::fabsf( position->z - vecHead.z );
			const float flOffset = (flDelta - 4.0f) / 6.0f;

			if (flOffset >= 0.f)
				flFactor = std::min( flOffset, 1.0f );

			const float flFactorSquared = (flFactor * flFactor);
			position->z += ((vecHead.z - position->z) * ((flFactorSquared * 3.0f) - ((flFactorSquared * 2.0f) * flFactor)));
		}
	}
}

void c_base_entity::post_think( )
{
	// @ida postthink: client.dll 56 8B 35 ? ? ? ? 57 8B F9 8B CE 8B 06 FF 90 ? ? ? ? 8B 07

	using post_think_v_physics_fn = bool( __thiscall* )(c_base_entity*);
	static auto o_post_think_v_physics = reinterpret_cast<post_think_v_physics_fn>(utils::find_pattern( "client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB" ));
	assert( o_post_think_v_physics != nullptr );

	using simulate_player_simulated_entities_fn = void( __thiscall* )(c_base_entity*);
	static auto o_simulate_player_simulated_entities = reinterpret_cast<simulate_player_simulated_entities_fn>(utils::find_pattern( "client.dll", "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74" ));
	assert( o_simulate_player_simulated_entities != nullptr );

	csgo::mdl_cache->begin_lock( );

	if (this->is_alive( ))
	{
		this->update_collision_bounds( );

		if (this->get_flags( ) & fl_onground)
			*this->get_fall_velocity( ) = 0.f;

		if (this->get_sequence( ) == -1)
			this->set_sequence( 0 );

		this->studio_frame_advance( );
		o_post_think_v_physics( this );
	}

	o_simulate_player_simulated_entities( this );

	csgo::mdl_cache->end_lock( );
}

bool c_base_entity::is_enemy( c_base_entity* entity )
{
	//check is dangerzone
	if (csgo::game_types->get_current_game_type( ) == gametype_freeforall)
		// check is not teammate
		return (this->get_survival_team( ) != entity->get_survival_team( ));

	static c_convar* mp_teammates_are_enemies = csgo::convar->find_var( "mp_teammates_are_enemies" );

	// check is deathmatch
	if (mp_teammates_are_enemies != nullptr && mp_teammates_are_enemies->get_bool( ) && this->get_team( ) == entity->get_team( ) && this != entity)
		return true;

	if (this->get_team( ) != entity->get_team( ))
		return true;

	return false;
}

bool c_base_entity::is_targeting_local( c_base_entity* pLocal )
{
	vec3_t vecForward = { };
	const vec3_t angView = this->get_eye_angles( );
	math::angle_vectors( angView, &vecForward );
	vecForward *= MAX_DISTANCE;

	const vec3_t vecStart = this->get_eye_position( );
	const vec3_t end_pos = vecStart + vecForward;

	const ray_t ray( vecStart, end_pos );
	c_trace_filter filter( this );

	trace_t trace = { };
	csgo::engine_trace->trace_ray( ray, MASK_SHOT, &filter, &trace );

	if (trace.hit_entity == pLocal)
		return true;

	return false;
}

bool c_base_entity::can_shoot( c_weapon_cs_base* base_weapon )
{
	const float server_time = TICKS_TO_TIME( this->get_tick_base( ) );

	if (!base_weapon)
		return false;

	// check is have ammo
	if (base_weapon->get_ammo( ) <= 0)
		return false;

	// is player ready to shoot
	if (this->get_next_attack( ) > server_time)
		return false;

	const short nDefinitionIndex = base_weapon->get_item_definition_index( );

	// check is weapon with burst mode
	if ((nDefinitionIndex == weapon_famas || nDefinitionIndex == weapon_glock) &&
		// check is burst mode
		base_weapon->is_burst_mode( ) && base_weapon->get_burst_shots_remaining( ) > 0)
		return true;

	// is weapon ready to shoot
	if (base_weapon->get_next_primary_attack( ) > server_time)
		return false;

	// check for revolver cocking ready
	if (nDefinitionIndex == weapon_revolver && base_weapon->get_fire_ready_time( ) > server_time)
		return false;

	return true;
}

bool c_base_entity::is_visible( c_base_entity* entity, const vec3_t& end_pos, bool smoke_check )
{
	const vec3_t vecStart = this->get_eye_position( false );

	const ray_t ray( vecStart, end_pos );
	c_trace_filter filter( this );

	trace_t trace = { };
	csgo::engine_trace->trace_ray( ray, MASK_SHOT, &filter, &trace );

	// trace check
	if ((trace.is_visible( ) || trace.hit_entity == entity) &&
		// smoke check
		!(smoke_check && utils::line_goes_through_smoke( vecStart, end_pos )))
		return true;

	return false;
}

bool c_base_entity::is_reloading( )
{
	auto animLayer = this->get_animation_layer( 1 );

	auto sequence = animLayer->sequence;
	if (sequence) {
		auto activity = this->get_sequence_activity( sequence );

		return activity == act_csgo_reload && animLayer->weight;
	}

	return false;
}

c_base_combat_weapon* c_base_entity::get_active_weapon( )
{
	return csgo::entity_list->get<c_base_combat_weapon>( this->get_active_weapon_handle( ) );
}
bool c_base_entity::has_c4( )
{
	return csgo::fn::has_c4( this );
}

vec3_t c_base_entity::get_shoot_pos( bool interpolated )
{
	if (!this)
		return vec3_t( );

	if (!interpolated)
		return this->get_origin( ) + this->get_view_offset( );
	else
	{
		vec3_t temp{};
		utils::call_vfunc<void>( this, 169, &temp );
		return temp;
	}
}

mstudiobbox_t* c_base_entity::get_hitbox( int hb_id )
{
	assert( hb_id > hitbox_invalid && hb_id < hitbox_max ); // given invalid hitbox index for gethitboxposition

	std::array<matrix3x4_t, MAXSTUDIOBONES> arrBonesToWorld = { };

	if (const auto model = this->get_model( ); model != nullptr)
	{
		if (const auto studio_hdr = csgo::model_info->get_studio_model( model ); studio_hdr != nullptr)
		{
			return studio_hdr->get_hitbox( hb_id, 0 );
		}
	}

	return nullptr;
}

void c_base_entity::draw_hitbox_overlay( int hitbox, float time, col_t clr )
{
	assert( hitbox > hitbox_invalid && hitbox < hitbox_max ); // given invalid hitbox index for gethitboxposition
	
	if (!this)
		return;

	auto model = this->get_model( );
	if (!model)
		return;

	auto studio_model = csgo::model_info->get_studio_model( model );
	if (!studio_model)
		return;

	auto set = studio_model->get_hitbox_set( this->get_hitbox_set( ) );
	if (!set)
		return;

	auto hb = set->get_hitbox( hitbox );
	if (!hb)
		return;

	matrix3x4_t bones[ MAXSTUDIOBONES ];
	memcpy( bones, this->get_bone_cache( ).base( ), this->get_bone_cache( ).size( ) * sizeof( matrix3x4_t ) );

	vec3_t min, max;
	math::vector_transform( hb->bb_min, bones[ hb->bone ], min );
	math::vector_transform( hb->bb_max, bones[ hb->bone ], max );

	std::uint8_t r, g, b, a;
	clr.get( r, g, b, a );
	csgo::debug_overlay->add_capsule_overlay( min, max, hb->radius, r, g, b, a, 4.0f );
}

c_base_entity* c_base_entity::get_player_from_event( i_game_event* game_event, const char* name )
{
	if (!game_event)
		return nullptr;

	auto user_id = game_event->get_int( name );
	auto id = csgo::engine->get_player_for_user_id( user_id );

	return csgo::entity_list->get<c_base_entity>( id );
}

float c_base_entity::get_max_feet_yaw( )
{
	static auto angle_diff = [ ]( float dest) {
		float delta = static_cast<float>(fmod( dest - 360.0f, 360.0f ));
		if (dest > delta)
		{
			if (delta >= 180.0f)
				delta -= 360.0f;
		}
		else
		{
			if (delta <= -180.0f)
				delta += 360.0f;
		}

		return delta;
	};

	auto anim_state = this->get_animation_state( );
	if (!anim_state)
		return 0.0f;

	auto eyes_angle = this->get_eye_angles( );
	auto eye_goalfeet_delta = angle_diff( anim_state->eye_yaw - anim_state->goal_feet_yaw );

	if (eye_goalfeet_delta < 0.0f || anim_state->max_body_yaw == 0.0f)
		return (eye_goalfeet_delta / anim_state->min_body_yaw) * (anim_state->min_body_yaw != 0.0f ? -58.0f : 58.0f);
	
	return 0.0f;
}

[[nodiscard]] static wchar_t* remove_newline_chars( wchar_t* begin, wchar_t* end ) noexcept
{
	return std::remove( begin, end, L'\n' );
}

[[nodiscard]] static wchar_t* remove_color_markup( wchar_t* begin, wchar_t* end ) noexcept
{
	return std::remove_if( begin, end, [ ]( wchar_t c )
		{
			return c > 0 && c < 17;
		} );
}

[[nodiscard]] static wchar_t* remove_consecutive_spaces( wchar_t* begin, wchar_t* end ) noexcept
{
	return std::unique( begin, end, [ ]( wchar_t a, wchar_t b )
		{
			return a == L' ' && a == b;
		} );
}

std::string c_base_entity::get_player_name( )
{
	if ( !this )
		return "unknown";

	wchar_t wide[ 128 ];
	csgo::fn::get_decorated_player_name( *csgo::fn::player_resources, this->get_client_networkable( )->get_index( ), wide, sizeof( wide ), 4 );

	char out [ 128 ];
	auto end = remove_newline_chars( wide, wide + wcslen( wide ) );
	end = remove_color_markup( wide, end );
	end = remove_consecutive_spaces( wide, end );

	*end = L'\0';

	csgo::localize->convert_unicode_to_ansi( wide, out, 128 );

	return std::string( out );
}

bool c_base_entity::is_breakable( )
{
	// @ida isbreakableentity: client.dll @ 55 8B EC 51 56 8B F1 85 F6 74 68

	const int iHealth = this->get_health( );

	// first check to see if it's already broken
	if (iHealth < 0 && this->is_max_health( ) > 0)
		return true;

	if (this->get_take_damage( ) != damage_yes)
	{
		const e_class_index nClassIndex = this->get_client_class( )->class_id;

		// force pass cfuncbrush
		if (nClassIndex != e_class_index::cfuncbrush)
			return false;
	}

	if (const int nCollisionGroup = this->get_collision_group( ); nCollisionGroup != collision_group_pushaway && nCollisionGroup != collision_group_breakable_glass && nCollisionGroup != collision_group_none)
		return false;

	if (iHealth > 200)
		return false;

	if (i_multiplayer_physics* pPhysicsInterface = dynamic_cast<i_multiplayer_physics*>(this); pPhysicsInterface != nullptr)
	{
		if (pPhysicsInterface->get_multiplayer_physics_mode( ) != physics_multiplayer_solid)
			return false;
	}
	else
	{
		if (const char* szClassName = this->get_classname( ); !strcmp( szClassName, "func_breakable" ) || !strcmp( szClassName, "func_breakable_surf" ))
		{
			if (!strcmp( szClassName, "func_breakable_surf" ))
			{
				c_breakable_surface* pSurface = static_cast<c_breakable_surface*>(this);

				// don't try to break it if it has already been broken
				if (pSurface->is_broken( ))
					return false;
			}
		}
		else if (this->physics_solid_mask_for_entity( ) & CONTENTS_PLAYERCLIP)
		{
			// hostages and players use CONTENTS_PLAYERCLIP, so we can use it to ignore them
			return false;
		}
	}

	if (i_breakable_with_prop_data* pBreakableInterface = dynamic_cast<i_breakable_with_prop_data*>(this); pBreakableInterface != nullptr)
	{
		// bullets don't damage it - ignore
		if (pBreakableInterface->get_dmg_mod_bullet( ) <= 0.0f)
			return false;
	}

	return true;
}
#pragma endregion

ccs_weapon_data* c_base_combat_weapon::get_weapon_data( )
{
	if (!this)
		return nullptr;

	return csgo::weapon_system->get_weapon_data( this->get_item_definition_index( ) );
}

float c_inferno::get_max_time( )
{
	static auto inferno_flame_lifetime = csgo::convar->find_var( "inferno_flame_lifetime " );
	if (!inferno_flame_lifetime)
		return 7.0f;

	return inferno_flame_lifetime->get_float( );
}

c_utl_vector<c_animation_layer>& c_base_entity::get_animation_overlays( )
{
	return *reinterpret_cast< c_utl_vector<c_animation_layer>* >( reinterpret_cast< std::uintptr_t >( this ) + csgo::fn::animation_overlays_offset );
}
