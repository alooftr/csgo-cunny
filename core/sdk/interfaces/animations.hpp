#pragma once

/* max animation layers */
#define MAX_OVERLAYS 15

enum e_sequence_activity : int
{
	act_csgo_null = 957,
	act_csgo_defuse,
	act_csgo_defuse_with_kit,
	act_csgo_flashbang_reaction,
	act_csgo_fire_primary,
	act_csgo_fire_primary_opt_1,
	act_csgo_fire_primary_opt_2,
	act_csgo_fire_secondary,
	act_csgo_fire_secondary_opt_1,
	act_csgo_fire_secondary_opt_2,
	act_csgo_reload,
	act_csgo_reload_start,
	act_csgo_reload_loop,
	act_csgo_reload_end,
	act_csgo_operate,
	act_csgo_deploy,
	act_csgo_catch,
	act_csgo_silencer_detach,
	act_csgo_silencer_attach,
	act_csgo_twitch,
	act_csgo_twitch_buyzone,
	act_csgo_plant_bomb,
	act_csgo_idle_turn_balanceadjust,
	act_csgo_idle_adjust_stoppedmoving,
	act_csgo_alive_loop,
	act_csgo_flinch,
	act_csgo_flinch_head,
	act_csgo_flinch_molotov,
	act_csgo_jump,
	act_csgo_fall,
	act_csgo_climb_ladder,
	act_csgo_land_light,
	act_csgo_land_heavy,
	act_csgo_exit_ladder_top,
	act_csgo_exit_ladder_bottom
};

enum e_anim_layer {
	animation_layer_aimmatrix,
	animation_layer_weapon_action,
	animation_layer_weapon_action_recrouch,
	animation_layer_adjust,
	animation_layer_movement_jump_or_fall,
	animation_layer_movement_land_or_climb,
	animation_layer_movement_move,
	animation_layer_movement_strafechange,
	animation_layer_whole_body,
	animation_layer_flashed,
	animation_layer_flinch,
	animation_layer_aliveloop,
	animation_layer_lean,
	animation_layer_count
};

class matrix3x4a_t;
class c_base_animating;
class c_bone_accessor
{
public:
	const c_base_animating* animating;		//0x00
	matrix3x4a_t* mat_bones;		//0x04
	int						readable_bones;	//0x08
	int						writable_bones;	//0x0C
}; // Size: 0x10

class c_animation_layer
{
public:
	float			animation_time;		//0x00
	float			fade_out;				//0x04
	void* studio_hdr;				//0x08
	int				dispatched_src;			//0x0C
	int				dispatched_dst;			//0x10
	int				order;					//0x14
	std::uintptr_t  sequence;				//0x18
	float			prev_cycle;			//0x1C
	float			weight;				//0x20
	float			weight_delta_rate;		//0x24
	float			playback_rate;			//0x28
	float			cycle;				//0x2C
	void* owner;					//0x30
	int				invalidate_physics_bits;	//0x34

	void reset( )
	{
		this->sequence = 0;
		this->weight = 0;
		this->weight_delta_rate = 0;
		this->playback_rate = 0;
		this->prev_cycle = 0;
		this->cycle = 0;
	}
}; // Size: 0x38

class c_base_entity;
class c_base_combat_weapon;
class ccsgo_player_anim_state
{
public:
	void create( c_base_entity* entity )
	{
		using CreateAnimationStateFn = void( __thiscall* )(void*, c_base_entity*);
		static auto oCreateAnimationState = reinterpret_cast<CreateAnimationStateFn>(utils::find_pattern( "client.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46" )); // @xref: "ggprogressive_player_levelup"

		if (oCreateAnimationState == nullptr)
			return;

		oCreateAnimationState( this, entity );
	}

	void update( vec3_t view_angles )
	{
		using UpdateAnimationStateFn = void( __vectorcall* )(void*, void*, float, float, float, void*);
		static auto oUpdateAnimationState = reinterpret_cast<UpdateAnimationStateFn>(utils::find_pattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24" )); // @xref: "%s_aim"

		if (oUpdateAnimationState == nullptr)
			return;

		oUpdateAnimationState( this, nullptr, 0.0f, view_angles.y, view_angles.x, nullptr );
	}

	void reset( )
	{
		using ResetAnimationStateFn = void( __thiscall* )(void*);
		static auto oResetAnimationState = reinterpret_cast<ResetAnimationStateFn>(utils::find_pattern( "client.dll", "56 6A 01 68 ? ? ? ? 8B F1" )); // @xref: "player_spawn"

		if (oResetAnimationState == nullptr)
			return;

		oResetAnimationState( this );
	}

	std::byte	pad0[ 0x60 ]; // 0x00
	c_base_entity* entity; // 0x60
	c_base_combat_weapon* active_weapon; // 0x64
	c_base_combat_weapon* last_active_weapon; // 0x68
	float		last_update_time; // 0x6C
	int			last_update_frame; // 0x70
	float		last_update_increment; // 0x74
	float		eye_yaw; // 0x78
	float		eye_pitch; // 0x7C
	float		goal_feet_yaw; // 0x80
	float		last_feet_yaw; // 0x84
	float		move_yaw; // 0x88
	float		last_move_yaw; // 0x8C // changes when moving/jumping/hitting ground
	float		lean_amount; // 0x90
	float		lower_body_yaw_align_time; // 0x94
	float		feet_cycle; // 0x98 0 to 1
	float		move_weight; // 0x9C 0 to 1
	float		move_weight_smoothed; // 0xA0
	float		duck_amount; // 0xA4
	float		hit_ground_cycle; // 0xA8
	float		recrouch_weight; // 0xAC
	vec3_t	origin; // 0xB0
	vec3_t	last_origin;// 0xBC
	vec3_t	velocity; // 0xC8
	vec3_t	velocity_normalized; // 0xD4
	vec3_t	velocity_normalized_non_zero; // 0xE0
	float		velocity_lenght2_d; // 0xEC
	float		jump_fall_velocity; // 0xF0
	float		speed_normalized; // 0xF4 // clamped velocity from 0 to 1 
	float		running_speed; // 0xF8
	float		ducking_speed; // 0xFC
	float		duration_moving; // 0x100
	float		duration_still; // 0x104
	bool		on_ground; // 0x108
	bool		hit_ground_animation; // 0x109
	std::byte	pad2[ 0x2 ]; // 0x10A
	float		next_lower_body_yaw_update_time; // 0x10C
	float		duration_in_air; // 0x110
	float		left_ground_height; // 0x114
	float		hit_ground_weight; // 0x118 // from 0 to 1, is 1 when standing
	float		walk_to_run_transition; // 0x11C // from 0 to 1, doesnt change when walking or crouching, only running
	std::byte	pad3[ 0x4 ]; // 0x120
	float		affected_fraction; // 0x124 // affected while jumping and running, or when just jumping, 0 to 1
	std::byte	pad4[ 0x208 ]; // 0x128
	float		min_body_yaw; // 0x330
	float		max_body_yaw; // 0x334
	float		min_pitch; //0x338
	float		max_pitch; // 0x33C
	int			animset_version; // 0x340
}; // Size: 0x344
