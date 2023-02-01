#pragma once
#pragma region gametypes_enumerations

enum e_game_type : int
{
	gametype_unknown = -1,
	gametype_classic,
	gametype_gungame,
	gametype_training,
	gametype_custom,
	gametype_cooperative,
	gametype_skirmish,
	gametype_freeforall
};

// @credits: https://developer.valvesoftware.com/wiki/CSGO_Game_Mode_Commands
enum e_game_mode : int
{
	gamemode_unknown = 0,
	gamemode_casual,
	gamemode_competitive,
	gamemode_wingman,
	gamemode_armsrace,
	gamemode_demolition,
	gamemode_deathmatch,
	gamemode_guardian,
	gamemode_coopstrike,
	gamemode_dangerzone
};
#pragma endregion

// @credits: https://github.com/scen/ionlib/blob/master/src/sdk/hl2_csgo/public/matchmaking/igametypes.h
class i_game_types
{
public:
	int get_current_game_type( )
	{
		return utils::call_vfunc<int>( this, 8 );
	}

	int get_current_game_mode( )
	{
		return utils::call_vfunc<int>( this, 9 );
	}

	const char* get_current_map_name( )
	{
		return utils::call_vfunc<const char*>( this, 10 );
	}

	const char* get_current_game_type_name_id( )
	{
		return utils::call_vfunc<const char*>( this, 11 );
	}

	const char* get_current_game_mode_name_id( )
	{
		return utils::call_vfunc<const char*>( this, 13 );
	}
};
