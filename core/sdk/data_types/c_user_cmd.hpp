#pragma once
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/shared/usercmd.h

// used: crc32 hashing
#include "../../utils/hash/crc32.h"
// used: vector
#include "vector.hpp"

#pragma region usercmd_enumerations
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/shared/in_buttons.h
enum e_command_buttons : int
{
	in_attack = (1 << 0),
	in_jump = (1 << 1),
	in_duck = (1 << 2),
	in_forward = (1 << 3),
	in_back = (1 << 4),
	in_use = (1 << 5),
	in_cancel = (1 << 6),
	in_left = (1 << 7),
	in_right = (1 << 8),
	in_moveleft = (1 << 9),
	in_moveright = (1 << 10),
	in_second_attack = (1 << 11),
	in_run = (1 << 12),
	in_reload = (1 << 13),
	in_left_alt = (1 << 14),
	in_right_alt = (1 << 15),
	in_score = (1 << 16),
	in_speed = (1 << 17),
	in_walk = (1 << 18),
	in_zoom = (1 << 19),
	in_first_weapon = (1 << 20),
	in_second_weapon = (1 << 21),
	in_bullrush = (1 << 22),
	in_first_grenade = (1 << 23),
	in_second_grenade = (1 << 24),
	in_middle_attack = (1 << 25),
	in_use_or_reload = (1 << 26)
};
#pragma endregion

#pragma pack(push, 4)
class c_user_cmd
{
public:
	virtual			~c_user_cmd( ) { }		// 0x00
	int				cmd_num;		// 0x04
	int				tick_count;			// 0x08
	vec3_t			view_angle;		// 0x0C
	vec3_t			aim_direction;	// 0x18
	float			forward_move;		// 0x24
	float			side_move;			// 0x28
	float			up_move;			// 0x2C
	int				buttons;			// 0x30
	std::uint8_t	impulse;			// 0x34
	int				weapon_select;		// 0x38
	int				weapon_sub_type;		// 0x3C
	int				random_seed;		// 0x40
	short			mouse_delta_x;		// 0x44
	short			mouse_delta_y;		// 0x46
	bool			has_been_predicted;	// 0x48
	vec3_t			head_angles;		// 0x4C
	vec3_t			head_offset;		// 0x58

	[[nodiscard]] crc32_t get_checksum( ) const
	{
		crc32_t hash_crc = 0UL;

		crc32::init( &hash_crc );
		crc32::process_buffer( &hash_crc, &cmd_num, sizeof( cmd_num ) );
		crc32::process_buffer( &hash_crc, &tick_count, sizeof( tick_count ) );
		crc32::process_buffer( &hash_crc, &view_angle, sizeof( view_angle ) );
		crc32::process_buffer( &hash_crc, &aim_direction, sizeof( aim_direction ) );
		crc32::process_buffer( &hash_crc, &forward_move, sizeof( forward_move ) );
		crc32::process_buffer( &hash_crc, &side_move, sizeof( side_move ) );
		crc32::process_buffer( &hash_crc, &up_move, sizeof( up_move ) );
		crc32::process_buffer( &hash_crc, &buttons, sizeof( buttons ) );
		crc32::process_buffer( &hash_crc, &impulse, sizeof( impulse ) );
		crc32::process_buffer( &hash_crc, &weapon_select, sizeof( weapon_select ) );
		crc32::process_buffer( &hash_crc, &weapon_sub_type, sizeof( weapon_sub_type ) );
		crc32::process_buffer( &hash_crc, &random_seed, sizeof( random_seed ) );
		crc32::process_buffer( &hash_crc, &mouse_delta_x, sizeof( mouse_delta_x ) );
		crc32::process_buffer( &hash_crc, &mouse_delta_y, sizeof( mouse_delta_y ) );
		crc32::final( &hash_crc );

		return hash_crc;
	}
};
static_assert(sizeof( c_user_cmd ) == 0x64);

class c_verified_user_cmd
{
public:
	c_user_cmd	user_cmd;	// 0x00
	crc32_t		hash_crc;	// 0x64
};
static_assert(sizeof( c_verified_user_cmd ) == 0x68);
#pragma pack(pop)
