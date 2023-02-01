#pragma once
#include <array>
#include <vector>
#include <optional>

#include "../data_types/vector.hpp"
#include "../data_types//color.hpp"
#include "../data_types/c_user_cmd.hpp"

#include "../netvar/netvar.hpp"

#include "../enums/enums.hpp"

#include "../../utils.h"

#include "animations.hpp"
#include "bitbuf.hpp"
#include "studio.hpp"
#include "irefcount.hpp"

#include "i_v_model_info.hpp"

using c_base_handle = std::uintptr_t;

#pragma region entities_definitions
#define INVALID_EHANDLE_INDEX		0xFFFFFFFF
#define NUM_ENT_ENTRY_BITS			(11 + 2)
#define NUM_ENT_ENTRIES				(1 << NUM_ENT_ENTRY_BITS)
#define NUM_SERIAL_NUM_BITS			16
#define NUM_SERIAL_NUM_SHIFT_BITS	(32 - NUM_SERIAL_NUM_BITS)
#define ENT_ENTRY_MASK				((1 << NUM_SERIAL_NUM_BITS) - 1)
#pragma endregion

#pragma region entities_enumerations
enum e_data_update_type : int
{
	data_update_created = 0,
	data_update_datatable_changed,
};

enum e_team_id : int
{
	team_unassigned = 0,
	team_spectator,
	team_tt,
	team_ct
};

enum e_think_methods : int
{
	think_fire_all_functions = 0,
	think_fire_base_only,
	think_fire_all_but_base,
};

enum e_item_definition_index : short
{
	weapon_none = 0,
	weapon_deagle = 1,
	weapon_elite = 2,
	weapon_fiveseven = 3,
	weapon_glock = 4,
	weapon_ak47 = 7,
	weapon_aug = 8,
	weapon_awp = 9,
	weapon_famas = 10,
	weapon_g3sg1 = 11,
	weapon_galilar = 13,
	weapon_m249 = 14,
	weapon_m4a1 = 16,
	weapon_mac10 = 17,
	weapon_p90 = 19,
	weapon_zone_repulsor = 20,
	weapon_mp5sd = 23,
	weapon_ump45 = 24,
	weapon_xm1014 = 25,
	weapon_bizon = 26,
	weapon_mag7 = 27,
	weapon_negev = 28,
	weapon_sawedoff = 29,
	weapon_tec9 = 30,
	weapon_taser = 31,
	weapon_hkp2000 = 32,
	weapon_mp7 = 33,
	weapon_mp9 = 34,
	weapon_nova = 35,
	weapon_p250 = 36,
	weapon_shield = 37,
	weapon_scar20 = 38,
	weapon_sg556 = 39,
	weapon_ssg08 = 40,
	weapon_knife_gg = 41,
	weapon_knife = 42,
	weapon_flashbang = 43,
	weapon_hegrenade = 44,
	weapon_smokegrenade = 45,
	weapon_molotov = 46,
	weapon_decoy = 47,
	weapon_incgrenade = 48,
	weapon_c4 = 49,
	weapon_healthshot = 57,
	weapon_knife_t = 59,
	weapon_m4a1_silencer = 60,
	weapon_usp_silencer = 61,
	weapon_cz75a = 63,
	weapon_revolver = 64,
	weapon_tagrenade = 68,
	weapon_fists = 69,
	weapon_breachcharge = 70,
	weapon_tablet = 72,
	weapon_melee = 74,
	weapon_axe = 75,
	weapon_hammer = 76,
	weapon_spanner = 78,
	weapon_knife_ghost = 80,
	weapon_firebomb = 81,
	weapon_diversion = 82,
	weapon_frag_grenade = 83,
	weapon_snowball = 84,
	weapon_bumpmine = 85,
	weapon_knife_bayonet = 500,
	weapon_knife_css = 503,
	weapon_knife_flip = 505,
	weapon_knife_gut = 506,
	weapon_knife_karambit = 507,
	weapon_knife_m9_bayonet = 508,
	weapon_knife_tactical = 509,
	weapon_knife_falchion = 512,
	weapon_knife_survival_bowie = 514,
	weapon_knife_butterfly = 515,
	weapon_knife_push = 516,
	weapon_knife_cord = 517,
	weapon_knife_canis = 518,
	weapon_knife_ursus = 519,
	weapon_knife_gypsy_jackknife = 520,
	weapon_knife_outdoor = 521,
	weapon_knife_stiletto = 522,
	weapon_knife_widowmaker = 523,
	weapon_knife_skeleton = 525,
	glove_studded_brokenfang = 4725,
	glove_studded_bloodhound = 5027,
	glove_t = 5028,
	glove_ct = 5029,
	glove_sporty = 5030,
	glove_slick = 5031,
	glove_leather_handwraps = 5032,
	glove_motorcycle = 5033,
	glove_specialist = 5034,
	glove_studded_hydra = 5035,
	special_agent_blueberries_buckshot = 4619,
	special_agent_two_times_mccoy_tacp = 4680,
	special_agent_commandor_mae_jamison = 4711,
	special_agent_1st_lieutenant_farlow,
	special_agent_john_kask,
	special_agent_bio_haz_specialist,
	special_agent_sergeant_bombson,
	special_agent_chem_haz_specialist,
	special_agent_rezan_the_redshirt = 4718,
	special_agent_sir_bloody_miami_darryl = 4726,
	special_agent_safecracker_voltzmann,
	special_agent_little_kev,
	special_agent_getaway_sally = 4730,
	special_agent_number_k = 4732,
	special_agent_sir_bloody_silent_darryl = 4733,
	special_agent_sir_bloody_skullhead_darryl,
	special_agent_sir_bloody_darryl_royale,
	special_agent_sir_bloody_loudmouth_darryl,
	special_agent_t = 5036,
	special_agent_ct = 5037,
	special_agent_ground_rebel = 5105,
	special_agent_osiris,
	special_agent_shahmat,
	special_agent_muhlik,
	special_agent_soldier = 5205,
	special_agent_enforcer,
	special_agent_slingshot,
	special_agent_street_soldier,
	special_agent_operator = 5305,
	special_agent_markus_delrow,
	special_agent_michael_syfers,
	special_agent_ava,
	special_agent_3rd_commando_company = 5400,
	special_agent_seal_team_6_soldier,
	special_agent_buckshot,
	special_agent_two_times_mccoy_usaf,
	special_agent_ricksaw,
	special_agent_dragomir = 5500,
	special_agent_maximus,
	special_agent_rezan_the_ready,
	special_agent_blackwolf = 5503,
	special_agent_the_doctor,
	special_agent_dragomir_footsoldiers,
	special_agent_b_squadron_officer = 5601
};

enum e_weapon_type : int
{
	weapontype_knife = 0,
	weapontype_pistol = 1,
	weapontype_submachinegun = 2,
	weapontype_rifle = 3,
	weapontype_shotgun = 4,
	weapontype_sniper = 5,
	weapontype_machinegun = 6,
	weapontype_c4 = 7,
	weapontype_placeholder = 8,
	weapontype_grenade = 9,
	weapontype_healthshot = 11,
	weapontype_fists = 12,
	weapontype_breachcharge = 13,
	weapontype_bumpmine = 14,
	weapontype_tablet = 15,
	weapontype_melee = 16
};
#pragma endregion

class i_handle_entity
{
public:
	virtual							~i_handle_entity( ) { }
	virtual void					set_ref_e_handle( const c_base_handle& hRef ) = 0;
	virtual const c_base_handle& get_ref_e_handle( ) const = 0;
};

class i_client_unknown;
class i_collideable
{
public:
	virtual i_handle_entity* get_entity_handle( ) = 0;
	virtual vec3_t& obb_mins( ) const = 0;
	virtual vec3_t& obb_maxs( ) const = 0;
};

class i_client_alpha_property
{
public:
	virtual i_client_unknown* get_i_client_unknown( ) = 0;
	virtual void set_alpha_modulation( std::uint8_t uAlpha ) = 0;
	virtual void set_render_fx( int nRenderFx, int nRenderMode, float flStartTime = FLT_MAX, float flDuration = 0.0f ) = 0;
	virtual void set_fade( float flGlobalFadeScale, float flDistFadeStart, float flDistFadeEnd ) = 0;
	virtual void set_desync_offset( int nOffset ) = 0;
	virtual void enable_alpha_modulation_override( bool bEnable ) = 0;
	virtual void enable_shadow_alpha_modulation_override( bool bEnable ) = 0;
	virtual void set_distance_fade_mode( int nFadeMode ) = 0;
};

class i_client_networkable;
class i_client_renderable;
class i_client_entity;
class c_base_entity;
class i_client_thinkable;
class i_client_unknown : public i_handle_entity
{
public:
	virtual i_collideable* get_collideable( ) = 0;
	virtual i_client_networkable* get_client_networkable( ) = 0;
	virtual i_client_renderable* get_client_renderable( ) = 0;
	virtual i_client_entity* get_i_client_entity( ) = 0;
	virtual c_base_entity* get_base_entity( ) = 0;
	virtual i_client_thinkable* get_client_thinkable( ) = 0;
	virtual i_client_alpha_property* get_client_alpha_property( ) = 0;
};

struct renderable_instance_t
{
	std::uint8_t uAlpha;
};

using client_shadow_handle_t = std::uint16_t;
using client_render_handle_t = std::uint16_t;
using model_instance_handle_t = std::uint16_t;
class i_client_renderable
{
public:
	virtual i_client_unknown* get_i_client_unknown( ) = 0;
	virtual vec3_t& get_render_origin( ) = 0;
	virtual vec3_t& get_render_angles( ) = 0;
	virtual bool					should_draw( ) = 0;
	virtual int						get_render_flags( ) = 0;
	virtual bool					is_transparent( ) = 0;
	virtual client_shadow_handle_t	get_shadow_handle( ) const = 0;
	virtual client_render_handle_t& render_handle( ) = 0;
	virtual const model_t* get_model( ) const = 0;
	virtual int						draw_model( int nFlags, const renderable_instance_t& uInstance ) = 0;
	virtual int						get_body( ) = 0;
	virtual void					get_color_modulation( float* pColor ) = 0;
	virtual bool					lod_test( ) = 0;
	virtual bool					setup_bones( matrix3x4_t* matBoneToWorldOut, int nMaxBones, int fBoneMask, float flCurrentTime ) = 0;
	virtual void					setup_weights( const matrix3x4_t* matBoneToWorld, int nFlexWeightCount, float* flFlexWeights, float* flFlexDelayedWeights ) = 0;
	virtual void					do_animation_events( ) = 0;
	virtual void* get_pvs_notify_interface( ) = 0;
	virtual void					get_render_bounds( vec3_t& vecMins, vec3_t& vecMaxs ) = 0;
	virtual void					get_render_bounds_worldspace( vec3_t& vecMins, vec3_t& vecMaxs ) = 0;
	virtual void					get_shadow_render_bounds( vec3_t& vecMins, vec3_t& vecMaxs, int iShadowType ) = 0;
	virtual bool					should_receive_projected_textures( int nFlags ) = 0;
	virtual bool					get_shadow_cast_distance( float* pDistance, int iShadowType ) const = 0;
	virtual bool					get_shadow_cast_direction( vec3_t* vecDirection, int iShadowType ) const = 0;
	virtual bool					is_shadow_dirty( ) = 0;
	virtual void					mark_shadow_dirty( bool bDirty ) = 0;
	virtual i_client_renderable* get_shadow_parent( ) = 0;
	virtual i_client_renderable* first_shadow_child( ) = 0;
	virtual i_client_renderable* next_shadow_peer( ) = 0;
	virtual int						shadow_cast_type( ) = 0;
	virtual void					unused2( ) {}
	virtual void					create_model_instance( ) = 0;
	virtual model_instance_handle_t	get_model_instance( ) = 0;
	virtual const matrix3x4_t& renderable_to_world_transform( ) = 0;
	virtual int						lookup_attachment( const char* pAttachmentName ) = 0;
	virtual bool					get_attachment( int index, vec3_t& vecOrigin, vec3_t& angView ) = 0;
	virtual bool					get_attachment( int index, matrix3x4_t& matAttachment ) = 0;
	virtual bool					compute_lighting_origin( int nAttachmentIndex, vec3_t vecModelLightingCenter, const matrix3x4_t& matrix, vec3_t& vecTransformedLightingCenter ) = 0;
	virtual float* get_render_clip_plane( ) = 0;
	virtual int						get_skin( ) = 0;
	virtual void					on_threaded_draw_setup( ) = 0;
	virtual bool					uses_flex_delayed_weights( ) = 0;
	virtual void					record_tool_message( ) = 0;
	virtual bool					should_draw_for_split_screen_user( int nSlot ) = 0;
	virtual std::uint8_t			override_alpha_modulation( std::uint8_t uAlpha ) = 0;
	virtual std::uint8_t			override_shadow_alpha_modulation( std::uint8_t uAlpha ) = 0;
	virtual void* get_client_model_renderable( ) = 0;
};

class c_base_client;
class i_client_networkable
{
public:
	virtual i_client_unknown* get_i_client_unknown( ) = 0;
	virtual void					release( ) = 0;
	virtual c_base_client* get_client_class( ) = 0;
	virtual void					notify_should_transmit( int iState ) = 0;
	virtual void					on_pre_data_changed( e_data_update_type updateType ) = 0;
	virtual void					on_data_changed( e_data_update_type updateType ) = 0;
	virtual void					pre_data_update( e_data_update_type updateType ) = 0;
	virtual void					post_data_update( e_data_update_type updateType ) = 0;
	virtual void					on_data_unchanged_in_pvs( ) = 0;
	virtual bool					is_dormant( ) const = 0;
	virtual int						get_index( ) const = 0;
	virtual void					receive_message( e_class_index classIndex, bf_read& msg ) = 0;
	virtual void* get_data_table_base_ptr( ) = 0;
	virtual void					set_destroyed_on_recreate_entities( ) = 0;
};

class c_client_think_handle;
using client_think_handle_t = c_client_think_handle*;
class i_client_thinkable
{
public:
	virtual i_client_unknown* get_i_client_unknown( ) = 0;
	virtual void					client_think( ) = 0;
	virtual client_think_handle_t		get_think_handle( ) = 0;
	virtual void					set_think_handle( client_think_handle_t hThink ) = 0;
	virtual void					release( ) = 0;
};

class i_client_entity : public i_client_unknown, public i_client_renderable, public i_client_networkable, public i_client_thinkable
{
public:
	virtual void					release( ) override = 0;
	virtual const vec3_t& get_abs_origin( ) const = 0;
	virtual const vec3_t& get_abs_angles( ) const = 0;
	virtual void* get_mouth( ) = 0;
	virtual bool					get_sound_spatialization( struct spatialization_info_t& info ) = 0;
	virtual bool					is_blurred( ) = 0;

	void set_abs_origin( const vec3_t& vecOrigin )
	{
		using set_abs_origin_fn = void( __thiscall* )(void*, const vec3_t&);
		static auto o_set_abs_origin = reinterpret_cast<set_abs_origin_fn>(utils::find_pattern( "client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8" ));
		o_set_abs_origin( this, vecOrigin );
	}

	void set_abs_angles( const vec3_t& angView )
	{
		using set_abs_angle_fn = void( __thiscall* )(void*, const vec3_t&);
		static auto o_set_abs_angles = reinterpret_cast<set_abs_angle_fn>(utils::find_pattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" ));
		o_set_abs_angles( this, angView );
	}

	data_map_t* get_data_desc_map( )
	{
		return utils::call_vfunc<data_map_t*>( this, 15 );
	}

	data_map_t* get_prediction_desc_map( )
	{
		return utils::call_vfunc<data_map_t*>( this, 17 );
	}
};

class c_weapon_cs_base;
class i_game_event;
class c_base_entity : public i_client_entity
{
public:
#pragma region DT_BasePlayer
	N_ADD_PVARIABLE( float, get_fall_velocity, "CBasePlayer->m_flFallVelocity" );
	N_ADD_VARIABLE( vec3_t, get_view_punch, "CBasePlayer->m_viewPunchAngle" );
	N_ADD_VARIABLE( vec3_t, get_punch, "CBasePlayer->m_aimPunchAngle" );
	N_ADD_VARIABLE( vec3_t, get_view_offset, "CBasePlayer->m_vecViewOffset[0]" );
	N_ADD_VARIABLE( float, get_friction, "CBasePlayer->m_flFriction" );
	N_ADD_VARIABLE( int, get_tick_base, "CBasePlayer->m_nTickBase" );
	N_ADD_PVARIABLE( int, get_next_think_tick, "CBasePlayer->m_nNextThinkTick" );
	N_ADD_VARIABLE( vec3_t, get_velocity, "CBasePlayer->m_vecVelocity[0]" );

	N_ADD_PVARIABLE_OFFSET( vec3_t, get_view_angles, "CBasePlayer->deadflag", 0x4 );
	N_ADD_VARIABLE( c_base_handle, get_ground_entity_handle, "CBasePlayer->m_hGroundEntity" );
	N_ADD_VARIABLE( int, get_health, "CBasePlayer->m_iHealth" );
	N_ADD_VARIABLE( int, get_life_state, "CBasePlayer->m_lifeState" );
	N_ADD_VARIABLE( float, get_max_speed, "CBasePlayer->m_flMaxspeed" );
	N_ADD_VARIABLE( int, get_flags, "CBasePlayer->m_fFlags" );
	N_ADD_PVARIABLE( int, get_observer_mode, "CBasePlayer->m_iObserverMode" );
	N_ADD_VARIABLE( c_base_handle, get_observer_target_handle, "CBasePlayer->m_hObserverTarget" );
	N_ADD_VARIABLE( c_base_handle, get_view_model_handle, "CBasePlayer->m_hViewModel[0]" );
	N_ADD_PVARIABLE( const char, get_last_place, "CBasePlayer->m_szLastPlaceName" );
	N_ADD_VARIABLE_OFFSET( int, get_button_disabled, "CBasePlayer->m_hViewEntity", -0xC );
	N_ADD_VARIABLE_OFFSET( int, get_button_forced, "CBasePlayer->m_hViewEntity", -0x8 );
	N_ADD_PVARIABLE_OFFSET( c_user_cmd*, get_current_command, "CBasePlayer->m_hViewEntity", -0x4 ); // @ida: client.dll @ [89 BE ? ? ? ? E8 ? ? ? ? 85 FF + 0x2]

	N_ADD_DATAFIELD( int, get_e_flags, this->get_prediction_desc_map( ), "m_iEFlags" );
	N_ADD_PDATAFIELD( int, get_buttons, this->get_prediction_desc_map( ), "m_nButtons" );
	N_ADD_DATAFIELD( int, get_button_last, this->get_prediction_desc_map( ), "m_afButtonLast" );
	N_ADD_DATAFIELD( int, get_button_pressed, this->get_prediction_desc_map( ), "m_afButtonPressed" );
	N_ADD_DATAFIELD( int, get_button_released, this->get_prediction_desc_map( ), "m_afButtonReleased" );
	N_ADD_PDATAFIELD( int, get_impulse, this->get_prediction_desc_map( ), "m_nImpulse" );
	N_ADD_DATAFIELD( float, get_surface_friction, this->get_prediction_desc_map( ), "m_surfaceFriction" );
	N_ADD_DATAFIELD( vec3_t, get_abs_velocity, this->get_prediction_desc_map( ), "m_vecAbsVelocity" );
	N_ADD_VARIABLE( int, get_hide_hud, "CBasePlayer->m_iHideHUD" );

	inline bool is_alive( )
	{
		// @note: https://github.com/rollraw/qo0-base/issues/135
		return (this->get_health( ) > 0);
	}

	int& get_take_damage( )
	{
		static const std::uintptr_t uTakeDamageOffset = *reinterpret_cast<std::uintptr_t*>(utils::find_pattern( "client.dll", "80 BE ? ? ? ? ? 75 46 8B 86" ) + 0x2);
		return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + uTakeDamageOffset);
	}

	c_user_cmd& get_last_command( )
	{
		static const std::uintptr_t uLastCommandOffset = *reinterpret_cast<std::uintptr_t*>(utils::find_pattern( "client.dll", "8D 8E ? ? ? ? 89 5C 24 3C" ) + 0x2);
		return *reinterpret_cast<c_user_cmd*>(reinterpret_cast<std::uintptr_t>(this) + uLastCommandOffset);
	}
#pragma endregion

#pragma region DT_CSPlayer
	N_ADD_VARIABLE( int, get_shots_fired, "CCSPlayer->m_iShotsFired" );
	N_ADD_VARIABLE_OFFSET( float, get_spawn_time, "CCSPlayer->m_iAddonBits", -0x4 ); // @ida: client.dll @ [89 86 ? ? ? ? E8 ? ? ? ? 80 BE ? ? ? ? ? + 0x2]
	N_ADD_VARIABLE( int, get_money, "CCSPlayer->m_iAccount" );
	N_ADD_VARIABLE( int, get_total_hits, "CCSPlayer->m_totalHitsOnServer" );
	N_ADD_VARIABLE( int, get_armor, "CCSPlayer->m_ArmorValue" );
	N_ADD_VARIABLE( vec3_t, get_eye_angles, "CCSPlayer->m_angEyeAngles" );
	N_ADD_VARIABLE( bool, is_defusing, "CCSPlayer->m_bIsDefusing" );
	N_ADD_VARIABLE( bool, is_scoped, "CCSPlayer->m_bIsScoped" );
	N_ADD_VARIABLE_OFFSET( float, collision_change_time, "CCSPlayer->m_bIsScoped", -0x50 ); // @ida: client.dll @ [8B 8E ? ? ? ? F3 0F 10 48 ? E8 ? ? ? ? C7 + 0x2]
	N_ADD_VARIABLE_OFFSET( ccsgo_player_anim_state*, get_animation_state, "CCSPlayer->m_bIsScoped", -0x14 ); // @ida: client.dll @ [8B 8E ? ? ? ? F3 0F 10 48 ? E8 ? ? ? ? C7 + 0x2]
	N_ADD_VARIABLE( bool, is_grabbing_hostage, "CCSPlayer->m_bIsGrabbingHostage" );
	N_ADD_VARIABLE( bool, is_rescuing, "CCSPlayer->m_bIsRescuing" );
	N_ADD_VARIABLE( bool, has_helmet, "CCSPlayer->m_bHasHelmet" );
	N_ADD_VARIABLE( bool, has_heavy_armor, "CCSPlayer->m_bHasHeavyArmor" );
	N_ADD_VARIABLE( bool, has_defuser, "CCSPlayer->m_bHasDefuser" );
	N_ADD_VARIABLE( bool, has_immunity, "CCSPlayer->m_bGunGameImmunity" );
	N_ADD_VARIABLE( bool, is_in_buy_zone, "CCSPlayer->m_bInBuyZone" );
	N_ADD_PVARIABLE( float, get_flash_max_alpha, "CCSPlayer->m_flFlashMaxAlpha" );
	N_ADD_VARIABLE_OFFSET( float, get_flash_alpha, "CCSPlayer->m_flFlashMaxAlpha", -0x8 );
	N_ADD_VARIABLE( float, get_flash_duration, "CCSPlayer->m_flFlashDuration" );
	N_ADD_VARIABLE_OFFSET( int, get_glow_index, "CCSPlayer->m_flFlashDuration", 0x18 );
	N_ADD_VARIABLE( float, get_lower_body_yaw, "CCSPlayer->m_flLowerBodyYawTarget" );
	N_ADD_VARIABLE( int, get_survival_team, "CCSPlayer->m_nSurvivalTeam" );
	N_ADD_VARIABLE_OFFSET( int, is_used_new_anim_state, "CCSPlayer->m_flLastExoJumpTime", 0x8 );
	N_ADD_VARIABLE( float, get_velocity_modifier, "CCSPlayer->m_flVelocityModifier" );
	N_ADD_VARIABLE( float, get_thirdperson_recoil, "CCSPlayer->m_flThirdpersonRecoil" );
	N_ADD_PVARIABLE( void*, get_patch_econ_indices, "CCSPlayer->m_vecPlayerPatchEconIndices" );
	N_ADD_VARIABLE( float, get_duck_amount, "CCSPlayer->m_flDuckAmount" );
	N_ADD_VARIABLE( bool, get_wait_for_no_attack, "CCSPlayer->m_bWaitForNoAttack" );

	inline bool is_armored( const int iHitGroup )
	{
		// @ida isarmored: server.dll @ 55 8B EC 32 D2

		bool bIsArmored = false;

		if (this->get_armor( ) > 0)
		{
			switch (iHitGroup)
			{
			case hitgroup_generic:
			case hitgroup_chest:
			case hitgroup_stomach:
			case hitgroup_leftarm:
			case hitgroup_rightarm:
			case hitgroup_neck:
				bIsArmored = true;
				break;
			case hitgroup_head:
				if (this->has_helmet( ))
					bIsArmored = true;
				[[fallthrough]];
			case hitgroup_leftleg:
			case hitgroup_rightleg:
				if (this->has_heavy_armor( ))
					bIsArmored = true;
				break;
			default:
				break;
			}
		}

		return bIsArmored;
	}
#pragma endregion

#pragma region DT_BaseEntity
	N_ADD_VARIABLE( float, get_animation_time, "CBaseEntity->m_flAnimTime" );
	N_ADD_VARIABLE( float, get_simulation_time, "CBaseEntity->m_flSimulationTime" );
	N_ADD_VARIABLE_OFFSET( float, get_old_simulation_time, "CBaseEntity->m_flSimulationTime", 0x4 );
	N_ADD_VARIABLE( vec3_t, get_origin, "CBaseEntity->m_vecOrigin" );
	N_ADD_VARIABLE( vec3_t, get_rotation, "CBaseEntity->m_angRotation" );
	N_ADD_VARIABLE( int, get_effects, "CBaseEntity->m_fEffects" );
	N_ADD_VARIABLE( int, get_team, "CBaseEntity->m_iTeamNum" );
	N_ADD_VARIABLE( c_base_handle, get_owner_entity_handle, "CBaseEntity->m_hOwnerEntity" );
	N_ADD_PVARIABLE( i_collideable, get_collision, "CBaseEntity->m_Collision" );
	N_ADD_VARIABLE( int, get_collision_group, "CBaseEntity->m_CollisionGroup" );
	N_ADD_PVARIABLE( bool, is_spotted, "CBaseEntity->m_bSpotted" );

	N_ADD_VARIABLE( vec3_t, get_vec_mins, "CBaseEntity->m_vecMins" );
	N_ADD_VARIABLE( vec3_t, get_vec_maxs, "CBaseEntity->m_vecMaxs" );

	N_ADD_DATAFIELD( vec3_t, get_abs_rotation, this->get_data_desc_map( ), "m_angAbsRotation" );
	N_ADD_DATAFIELD( const matrix3x4_t, get_coordinate_frame, this->get_data_desc_map( ), "m_rgflCoordinateFrame" );
	N_ADD_DATAFIELD( int, get_move_type, this->get_prediction_desc_map( ), "m_MoveType" );
	N_ADD_VARIABLE( c_base_handle, get_move_parent, "CBaseEntity->moveparent" );
#pragma endregion

#pragma region DT_BaseCombatCharacter
	N_ADD_VARIABLE( float, get_next_attack, "CBaseCombatCharacter->m_flNextAttack" );
	N_ADD_VARIABLE( c_base_handle, get_active_weapon_handle, "CBaseCombatCharacter->m_hActiveWeapon" );
	N_ADD_PVARIABLE( c_base_handle, get_weapons_handle, "CBaseCombatCharacter->m_hMyWeapons" );
	N_ADD_PVARIABLE( c_base_handle, get_wearables_handle, "CBaseCombatCharacter->m_hMyWearables" );
#pragma endregion

#pragma region DT_BaseAnimating
	N_ADD_VARIABLE( int, get_sequence, "CBaseAnimating->m_nSequence" );
	N_ADD_PVARIABLE_OFFSET( c_bone_accessor, get_bone_accessor, "CBaseAnimating->m_nForceBone", 0x1C );
	N_ADD_VARIABLE( int, get_hitbox_set, "CBaseAnimating->m_nHitboxSet" );
	N_ADD_VARIABLE( bool, is_client_side_animation, "CBaseAnimating->m_bClientSideAnimation" );
	N_ADD_VARIABLE( float, get_cycle, "CBaseAnimating->m_flCycle" );
	N_ADD_VARIABLE( int, get_animating_body, "CBaseAnimating->m_nBody" );

	[[nodiscard]] std::array<float, MAXSTUDIOPOSEPARAM>& get_pose_parameter( )
	{
		static std::uintptr_t m_flPoseParameter = c_netvar_manager::get( ).map_props[ fnv1a::hash_const( "CBaseAnimating->m_flPoseParameter" ) ].offset;
		return *reinterpret_cast<std::array<float, MAXSTUDIOPOSEPARAM>*>(reinterpret_cast<std::uintptr_t>(this) + m_flPoseParameter);
	}

	inline void set_pose_angles( float flYaw, float flPitch )
	{
		auto& arrPose = this->get_pose_parameter( );
		arrPose.at( 11U ) = (flPitch + 90.f) / 180.f;
		arrPose.at( 2U ) = (flYaw + 180.f) / 360.f;
	}

	[[nodiscard]] c_utl_vector<c_animation_layer>& get_animation_overlays( );

	[[nodiscard]] inline c_animation_layer* get_animation_layer( const int nLayer )
	{
		if (nLayer >= 0 && nLayer < MAX_OVERLAYS)
			return &get_animation_overlays( )[ nLayer ];

		return nullptr;
	}

	inline bool is_chocking_packet( )
	{
		return this->get_simulation_time( ) <= this->get_old_simulation_time( );
	}

	c_animation_layer* anim_layers () noexcept
	{
		return *reinterpret_cast<c_animation_layer**>(reinterpret_cast<uintptr_t>(this) + 0x2990);
	}

	int get_animation_layers_count( )
	{
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0x299C);
	}
	
	c_utl_vector<matrix3x4_t>& get_bone_cache( )
	{
		return *reinterpret_cast<c_utl_vector<matrix3x4_t>*>(reinterpret_cast<uintptr_t>(this) + 0x2914);
	}

#pragma endregion

	// DoExtraBonesProcessing
	// pattern @xref: "ankle_L"
	// index @xref: "SetupBones: invalid bone array size (%d - needs %d)\n"

	int is_max_health( )
	{
		// @ida: client.dll @ [FF 90 ? ? ? ? 85 C0 0F 8F ? ? ? ? 80 + 0x2] / sizeof(std::uintptr_t)
		return utils::call_vfunc<int>( this, 123 );
	}

	void think( )
	{
		// @ida: client.dll @ [FF 90 ? ? ? ? FF 35 ? ? ? ? 8B 4C + 0x2] / sizeof(std::uintptr_t)
		utils::call_vfunc<void>( this, 139 );
	}

	const char* get_classname( )
	{
		// @ida: client.dll @ [8B 01 FF 90 ? ? ? ? 90 + 0x4] / sizeof(std::uintptr_t)
		return utils::call_vfunc<const char*>( this, 143 );
	}

	unsigned int physics_solid_mask_for_entity( )
	{
		// @xref: "func_breakable", "func_breakable_surf"
		return utils::call_vfunc<unsigned int>( this, 152 );
	}

	bool is_player( )
	{
		// @xref: "effects/nightvision"
		return utils::call_vfunc<bool>( this, 158 );
	}

	bool is_weapon( )
	{
		// @xref: "effects/nightvision"
		return utils::call_vfunc<bool>( this, 166 );
	}

	[[nodiscard]] vec3_t get_eye_position( bool bShouldCorrect = true )
	{
		vec3_t position = { };

		// get eye position
		utils::call_vfunc<void>( this, 169, std::ref( position ) );

		// correct this like it do weapon_shootpos
		// @ida weapon_shootpos: 55 8B EC 56 8B 75 08 57 8B F9 56 8B 07 FF 90
		if (is_used_new_anim_state( ) && bShouldCorrect)
		{
			if (ccsgo_player_anim_state* anim_state = this->get_animation_state( ); anim_state != nullptr)
				modify_eye_position( anim_state, &position );
		}

		// return corrected position
		return position;
	}

	void set_sequence( int sequence )
	{
		// @ida: FF 90 ? ? ? ? 8B 07 8B CF FF 90 ? ? ? ? 8B CF + 0x2
		utils::call_vfunc<void>( this, 219, sequence );
	}

	void studio_frame_advance( )
	{
		// @ida: FF 90 ? ? ? ? 8B 07 8B CF FF 90 ? ? ? ? 8B CF + 0xC
		utils::call_vfunc<void>( this, 220 );
	}

	float get_layer_sequence_cycle_rate( c_animation_layer* anim_layer, int layer_sequence )
	{
		return utils::call_vfunc<float>( this, 223, anim_layer, layer_sequence );
	}

	void update_client_side_animations( )
	{
		// @xref: "UpdateClientSideAnimations"
		// @ida updateclientsideanimations: 55 8B EC 51 56 8B F1 80 BE ? ? ? ? 00 74 ? 8B 06 FF
		utils::call_vfunc<void>( this, 224 );
	}

	void pre_think( )
	{
		utils::call_vfunc<void>( this, 318 );
	}

	void update_collision_bounds( )
	{
		utils::call_vfunc<void>( this, 340 );
	}

	bool physics_run_think( int nThinkMethod )
	{
		// @xref: from sub with "CLIENT:  %s(%s) thinking for %.02f ms!!!\n"
		using PhysicsRunThinkFn = bool( __thiscall* )(void*, int);
		static auto oPhysicsRunThink = reinterpret_cast<PhysicsRunThinkFn>(utils::find_pattern( "client.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87" ));
		return oPhysicsRunThink( this, nThinkMethod );
	}

	void invalidate_bone_cache( )
	{
		typedef void( __thiscall* OriginalFn )(void*);
		static auto func = reinterpret_cast<OriginalFn>(utils::find_pattern( "client.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81" ));

		if (this && func)
			func( this );
	}

	vec3_t get_aim_punch_angle_scaled( );
	static c_base_entity* get_local_player( );
	int						get_sequence_activity( int sequence );
	int						get_max_health( );
	std::optional<vec3_t>	get_bone_position( int bone_index );
	int						get_bone_by_hash( const fnv1a_t bone_hash ) const;
	std::optional<vec3_t>	get_hitbox_position( const int iHitbox );
	std::optional<vec3_t>	get_hit_group_position( const int iHitGroup );
	void					modify_eye_position( const ccsgo_player_anim_state* anim_state, vec3_t* position ) const;
	void					post_think( );
	bool					is_enemy( c_base_entity* entity );
	bool					is_targeting_local( c_base_entity* pLocal );
	bool					can_shoot( c_weapon_cs_base* base_weapon );
	bool					is_visible( c_base_entity* entity, const vec3_t& end_pos, bool smoke_check = false );
	bool					is_breakable( );
	bool					is_reloading( );
	c_base_combat_weapon*   get_active_weapon( );
	bool					has_c4( );
	vec3_t					get_shoot_pos( bool interpolated = false );
	mstudiobbox_t* get_hitbox( int hb_id );
	void draw_hitbox_overlay( int hitbox, float time = 0.0f, col_t clr = col_t( ) );

	static c_base_entity* get_player_from_event( i_game_event* game_event, const char* name );
	float get_max_feet_yaw( );
	std::string get_player_name( );
};

class ccs_weapon_data
{
public:
	std::byte pad0[ 0x14 ];			// 0x0000
	int max_clip1;					// 0x0014
	int max_clip2;					// 0x0018
	int default_clip1;				// 0x001C
	int default_clip2;				// 0x0020
	int primary_max_reserve_ammo;		// 0x0024
	int secondary_max_reserve_ammo;	// 0x0028
	const char* world_model;		// 0x002C
	const char* view_model;		// 0x0030
	const char* dropped_model;		// 0x0034
	std::byte pad1[ 0x50 ];			// 0x0038
	const char* hud_name;			// 0x0088
	const char* weapon_name;		// 0x008C
	std::byte pad2[ 0x2 ];			// 0x0090
	bool is_melee_weapon;			// 0x0092
	std::byte pad3[ 0x9 ];			// 0x0093
	float weapon_weight;			// 0x009C
	std::byte pad4[ 0x4 ];			// 0x00A0
	int slot;						// 0x00A4
	int position;					// 0x00A8
	std::byte pad5[ 0x1C ];			// 0x00AC
	int weapon_type;				// 0x00C8
	std::byte pad6[ 0x4 ];			// 0x00CC
	int weapon_price;				// 0x00D0
	int kill_award;					// 0x00D4
	const char* animation_prefix;	// 0x00D8
	float cycle_time;				// 0x00DC
	float cycle_time_alt;			// 0x00E0
	std::byte pad8[ 0x8 ];			// 0x00E4
	bool full_auto;					// 0x00EC
	std::byte pad9[ 0x3 ];			// 0x00ED
	int damage;					// 0x00F0
	float head_shot_multiplier;		// 0x00F4
	float armor_ratio;				// 0x00F8
	int iBullets;					// 0x00FC
	float penetration;			// 0x0100
	std::byte pad10[ 0x8 ];			// 0x0104
	float range;					// 0x010C
	float range_modifier;			// 0x0110
	float throw_velocity;			// 0x0114
	std::byte pad11[ 0xC ];			// 0x0118
	bool has_silencer;				// 0x0124
	std::byte pad12[ 0xF ];			// 0x0125
	float max_speed[ 2 ];			// 0x0134
	std::byte pad13[ 0x4 ];			// 0x013C
	float spread[ 2 ];				// 0x0140
	float inaccuracy_crouch[ 2 ];	// 0x0148
	float inaccuracy_stand[ 2 ];		// 0x0150
	std::byte pad14[ 0x8 ];			// 0x0158
	float inaccuracy_jump[ 2 ];		// 0x0160
	float inaccuracy_land[ 2 ];		// 0x0168
	float inaccuracy_ladder[ 2 ];	// 0x0170
	float inaccuracy_fire[ 2 ];		// 0x0178
	float inaccuracy_move[ 2 ];		// 0x0180
	float inaccuracy_reload;		// 0x0188
	int recoil_seed;				// 0x018C
	float recoil_angle[ 2 ];			// 0x0190
	float recoil_angle_variance[ 2 ];	// 0x0198
	float recoil_magnitude[ 2 ];		// 0x01A0
	float recoil_magnitude_variance[ 2 ]; // 0x01A8
	int spread_seed;				// 0x01B0

	bool is_gun( ) const
	{
		switch (this->weapon_type)
		{
		case weapontype_pistol:
		case weapontype_submachinegun:
		case weapontype_rifle:
		case weapontype_shotgun:
		case weapontype_sniper:
		case weapontype_machinegun:
			return true;
		}

		return false;
	}

	bool is_grenade( ) const
	{
		return this->weapon_type == weapontype_grenade;
	}

	bool is_knife( )
	{
		return this->weapon_type == weapontype_knife;
	}
};


class i_ref_counted;
class c_econ_item_view
{
public:
	N_ADD_OFFSET( c_utl_vector<i_ref_counted*>, get_custom_materials, 0x14 );

	c_utl_vector<c_ref_counted*>& get_visuals_data_processors( )
	{
		// @xref: "Original material not found! Name: %s"
		static const std::uintptr_t visuals_data_processors_offset = *reinterpret_cast<std::uintptr_t*>(utils::find_pattern( "client.dll", "81 C7 ? ? ? ? 8B 4F 0C 8B 57 04 89 4C" ) + 0x2);
		return *reinterpret_cast<c_utl_vector<c_ref_counted*>*>(reinterpret_cast<std::uintptr_t>(this) + visuals_data_processors_offset);
	}
};

class c_base_combat_weapon : public c_base_entity
{
public:
#pragma region DT_BaseCombatWeapon
	N_ADD_VARIABLE( float, get_next_primary_attack, "CBaseCombatWeapon->m_flNextPrimaryAttack" );
	N_ADD_VARIABLE( float, get_next_secondary_attack, "CBaseCombatWeapon->m_flNextSecondaryAttack" );
	N_ADD_VARIABLE( int, get_ammo, "CBaseCombatWeapon->m_iClip1" );
	N_ADD_VARIABLE( int, get_ammo_reserve, "CBaseCombatWeapon->m_iPrimaryReserveAmmoCount" );
	N_ADD_VARIABLE( int, get_view_model_index, "CBaseCombatWeapon->m_iViewModelIndex" );
	N_ADD_VARIABLE( int, get_world_model_index, "CBaseCombatWeapon->m_iWorldModelIndex" );
	N_ADD_VARIABLE( c_base_handle, get_world_model_handle, "CBaseCombatWeapon->m_hWeaponWorldModel" );
	N_ADD_VARIABLE( int, get_activity, "CBaseCombatWeapon->m_Activity" );

	N_ADD_DATAFIELD( bool, is_reloading, this->get_prediction_desc_map( ), "m_bInReload" );
	N_ADD_VARIABLE( int, get_model_index, "CBaseViewModel->m_nModelIndex" );

#pragma endregion

#pragma region DT_BaseAttributableItem
	N_ADD_VARIABLE( short, get_item_definition_index, "CBaseAttributableItem->m_iItemDefinitionIndex" );
	N_ADD_VARIABLE( int, get_item_id_high, "CBaseAttributableItem->m_iItemIDHigh" );
	N_ADD_VARIABLE( int, get_item_id_low, "CBaseAttributableItem->m_iItemIDLow" );
	N_ADD_VARIABLE( int, get_account_id, "CBaseAttributableItem->m_iAccountID" );
	N_ADD_VARIABLE( int, get_entity_quality, "CBaseAttributableItem->m_iEntityQuality" );
	N_ADD_PVARIABLE( char, get_custom_name, "CBaseAttributableItem->m_szCustomName" );
	N_ADD_VARIABLE( int, get_owner_xuid_low, "CBaseAttributableItem->m_OriginalOwnerXuidLow" );
	N_ADD_VARIABLE( int, get_owner_xuid_high, "CBaseAttributableItem->m_OriginalOwnerXuidHigh" );
	N_ADD_VARIABLE( int, get_fallback_paint_kit, "CBaseAttributableItem->m_nFallbackPaintKit" );
	N_ADD_VARIABLE( int, get_fallback_seed, "CBaseAttributableItem->m_nFallbackSeed" );
	N_ADD_VARIABLE( float, get_fallback_wear, "CBaseAttributableItem->m_flFallbackWear" );
	N_ADD_VARIABLE( int, get_fallback_stat_trak, "CBaseAttributableItem->m_nFallbackStatTrak" );
	N_ADD_PVARIABLE( c_econ_item_view, get_econ_item_view, "CBaseAttributableItem->m_Item" );
	N_ADD_VARIABLE( bool, is_initialized, "CBaseAttributableItem->m_bInitialized" );
#pragma endregion

	void set_model_index( int nModelIndex )
	{
		utils::call_vfunc<void>( this, 75, nModelIndex );
	}

	[[nodiscard]] bool is_weapon( )
	{
		return utils::call_vfunc<bool>( this, 166 );
	}

	[[nodiscard]] float get_spread( )
	{
		return utils::call_vfunc<float>( this, 453 );
	}

	[[nodiscard]] float get_inaccuracy( )
	{
		return utils::call_vfunc<float>( this, 483 );
	}

	[[nodiscard]] void update_accuracy_penalty( )
	{
		utils::call_vfunc<void>( this, 484 );
	}

	ccs_weapon_data* get_weapon_data( );
};

class cte_fire_bullets
{
public:
	std::byte	pad0[ 0x10 ];				//0x0000
	int			nPlayer;				//0x0010
	int			nItemDefinitionIndex;	//0x0014
	vec3_t		vecOrigin;				//0x0018
	vec3_t		vecAngles;				//0x0024
	int			iWeapon;				//0x0030
	int			nWeaponID;				//0x0034
	int			iMode;					//0x0038
	int			nSeed;					//0x003C
	float		flInaccuracy;			//0x0040
	float		flRecoilIndex;			//0x0044
	float		flSpread;				//0x0048
	int			nSoundType;				//0x004C
}; // Size: 0x0050

class c_weapon_cs_base : public c_base_combat_weapon
{
public:
#pragma region DT_WeaponCSBaseGun
	N_ADD_VARIABLE( int, get_zoom_level, "CWeaponCSBaseGun->m_zoomLevel" );
	N_ADD_VARIABLE( int, get_burst_shots_remaining, "CWeaponCSBaseGun->m_iBurstShotsRemaining" );
	N_ADD_VARIABLE( int, weapon_mode, "CWeaponCSBaseGun->m_weaponMode" );
	N_ADD_VARIABLE( float, get_next_burst_shot, "CWeaponCSBaseGun->m_fNextBurstShot" );
#pragma endregion

#pragma region DT_WeaponCSBase
	N_ADD_VARIABLE( bool, is_burst_mode, "CWeaponCSBase->m_bBurstMode" );
	N_ADD_VARIABLE( float, get_accuracy_penalty, "CWeaponCSBase->m_fAccuracyPenalty" );
	N_ADD_VARIABLE( float, get_fire_ready_time, "CWeaponCSBase->m_flPostponeFireReadyTime" );
#pragma endregion

	c_utl_vector<i_ref_counted*>& get_custom_materials( )
	{
		static auto address = *reinterpret_cast<std::uintptr_t*>(utils::find_pattern( "client.dll", "83 BE ? ? ? ? ? 7F 67" ) + 0x2) - 0xC;
		return *reinterpret_cast<c_utl_vector<i_ref_counted*>*>(reinterpret_cast<std::uintptr_t>(this) + address);
	}

	bool& is_custom_material_initialized( )
	{
		static auto address = *reinterpret_cast<std::uintptr_t*>(utils::find_pattern( "client.dll", "C6 86 ? ? ? ? ? FF 50 04" ) + 0x2);
		return *reinterpret_cast<bool*>(reinterpret_cast<std::uintptr_t>(this) + address);
	}
};

class c_base_cs_grenade : public c_weapon_cs_base
{
public:
#pragma region DT_BaseCSGrenade
	N_ADD_VARIABLE( bool, is_pin_pulled, "CBaseCSGrenade->m_bPinPulled" );
	N_ADD_VARIABLE( float, get_throw_time, "CBaseCSGrenade->m_fThrowTime" );
	N_ADD_VARIABLE( float, get_throw_strength, "CBaseCSGrenade->m_flThrowStrength" );
	N_ADD_VARIABLE( c_base_handle, get_thrower_handle, "CBaseGrenade->m_hThrower" );
#pragma endregion

#pragma region DT_BaseCSGrenadeProjectile
	N_ADD_VARIABLE( int, get_effect_tick_begin, "CBaseCSGrenadeProjectile->m_nExplodeEffectTickBegin" );
	N_ADD_VARIABLE( vec3_t, get_initial_velocity, "CBaseCSGrenadeProjectile->m_vInitialVelocity" );
	N_ADD_VARIABLE( int, get_animation_time, "CBaseCSGrenadeProjectile->m_flAnimTime" );
	N_ADD_VARIABLE( int, get_nade_body, "CBaseCSGrenadeProjectile->m_nBody" );
	N_ADD_VARIABLE( int, get_force_bone, "CBaseCSGrenadeProjectile->m_nForceBone" );
	N_ADD_VARIABLE_OFFSET( float, get_spawn_time, "CBaseCSGrenadeProjectile->m_vecExplodeEffectOrigin", 0xC );

	inline bool is_throwing( )
	{
		if (!this->is_pin_pulled( ))
		{
			float throw_time = this->get_throw_time( );
			if (throw_time > 0.0f)
				return true;
		}
		return false;
	}
#pragma endregion
};

class c_smoke_grenade : public c_base_entity
{
public:
	N_ADD_VARIABLE( int, get_effect_tick_begin, "CSmokeGrenadeProjectile->m_nSmokeEffectTickBegin" );

	inline float get_max_time( )
	{
		return 19.f;
	}
};

class c_inferno : public c_base_entity
{
public:
	N_ADD_VARIABLE( int, get_effect_tick_begin, "CInferno->m_nFireEffectTickBegin" );
	N_ADD_PVARIABLE( bool, get_fire_is_burning, "CInferno->m_bFireIsBurning" );

	N_ADD_VARIABLE( int, get_fire_count, "CInferno->m_fireCount" );

	N_ADD_PVARIABLE( int, get_fire_x_delta, "CInferno->m_fireXDelta" );
	N_ADD_PVARIABLE( int, get_fire_y_delta, "CInferno->m_fireYDelta" );
	N_ADD_PVARIABLE( int, get_fire_z_delta, "CInferno->m_fireZDelta" );

	float get_max_time( );
};

class c_planted_c4
{
public:
#pragma region DT_PlantedC4
	N_ADD_VARIABLE( float, get_blow_time, "CPlantedC4->m_flC4Blow" );
	N_ADD_VARIABLE( float, get_timer_length, "CPlantedC4->m_flTimerLength" );
	N_ADD_VARIABLE( float, get_defuse_length, "CPlantedC4->m_flDefuseLength" );
	N_ADD_VARIABLE( float, get_defuse_count_down, "CPlantedC4->m_flDefuseCountDown" );
	N_ADD_VARIABLE( bool, is_planted, "CPlantedC4->m_bBombTicking" );
	N_ADD_VARIABLE( c_base_handle, get_defuser_handle, "CPlantedC4->m_hBombDefuser" );
	N_ADD_VARIABLE( bool, is_defused, "CPlantedC4->m_bBombDefused" );
	N_ADD_VARIABLE( int, get_bomb_site, "CPlantedC4->m_nBombSite" );

	inline float get_timer( const float flServerTime )
	{
		return std::clamp( this->get_blow_time( ) - flServerTime, 0.0f, this->get_timer_length( ) );
	}

	inline float GetDefuseTimer( const float flServerTime )
	{
		return std::clamp( this->get_defuse_count_down( ) - flServerTime, 0.0f, this->get_defuse_length( ) );
	}
#pragma endregion
};

class c_base_view_model
{
public:
#pragma region DT_BaseViewModel
	N_ADD_VARIABLE( int, get_model_index, "CBaseViewModel->m_nModelIndex" );
	N_ADD_VARIABLE( c_base_handle, get_owner_handle, "CBaseViewModel->m_hOwner" );
	N_ADD_VARIABLE( c_base_handle, get_weapon_handle, "CBaseViewModel->m_hWeapon" );
#pragma endregion

	void send_view_model_matching_sequence( int nSequence )
	{
		utils::call_vfunc<void>( this, 247, nSequence );
	}

	void set_weapon_model( const char* szFileName, c_base_combat_weapon* pWeapon )
	{
		// @ida setweaponmodel: 57 8B F9 8B 97 ? ? ? ? 83 FA FF 74 6A
		utils::call_vfunc<void>( this, 248, szFileName, pWeapon );
	}
};

class c_env_tonemap_controller
{
public:
#pragma region DT_EnvTonemapController
	N_ADD_VARIABLE( bool, is_use_custom_auto_exposure_min, "CEnvTonemapController->m_bUseCustomAutoExposureMin" );
	N_ADD_VARIABLE( bool, is_use_custom_auto_exposure_max, "CEnvTonemapController->m_bUseCustomAutoExposureMax" );
	N_ADD_VARIABLE( bool, is_use_custom_bloom_scale, "CEnvTonemapController->m_bUseCustomBloomScale" );
	N_ADD_VARIABLE( float, get_custom_auto_exposure_min, "CEnvTonemapController->m_flCustomAutoExposureMin" );
	N_ADD_VARIABLE( float, get_custom_auto_exposure_max, "CEnvTonemapController->m_flCustomAutoExposureMax" );
	N_ADD_VARIABLE( float, get_custom_bloom_scale, "CEnvTonemapController->m_flCustomBloomScale" );
	N_ADD_VARIABLE( float, get_custom_bloom_scale_min, "CEnvTonemapController->m_flCustomBloomScaleMinimum" );
	N_ADD_VARIABLE( float, get_bloom_exponent, "CEnvTonemapController->m_flBloomExponent" );
	N_ADD_VARIABLE( float, get_bloom_saturation, "CEnvTonemapController->m_flBloomSaturation" );
#pragma endregion
};

class c_breakable_surface : public c_base_entity, public i_breakable_with_prop_data
{
public:
#pragma region DT_BreakableSurface
	N_ADD_VARIABLE( bool, is_broken, "CBreakableSurface->m_bIsBroken" );
#pragma endregion
};


class c_precipitation : public c_base_entity
{
public:
	N_ADD_VARIABLE( int, get_precipitation_type, "CPrecipitation->m_nPrecipType" );
};

class ccs_game_rules_proxy
{
public:
	N_ADD_VARIABLE( bool, is_valve_secure, "CCSGameRulesProxy->m_bIsValveDS" );
};

class c_player_resources
{
	c_player_resources( ) = delete;
	c_player_resources( c_player_resources&& ) = delete;
	c_player_resources( const c_player_resources& ) = delete;
	c_player_resources& operator=( const c_player_resources& ) = delete;

	N_ADD_VARIABLE( int[ 65 ], music_id, "CCSPlayerResource->m_nMusicID" );
};