#pragma once
#include "../enums/enums.hpp"

class c_user_cmd;
class i_global_vars_base
{
public:
	float			real_time;					//0x00
	int				frame_count;				//0x04
	float			abs_frame_time;				//0x08
	float			abs_frame_start_time;		//0x0C
	float			current_time;				//0x10
	float			frame_time;				//0x14
	int				max_clients;				//0x18
	int				tick_count;					//0x1C
	float			interval_per_tick;			//0x20
	float			interpolation_amount;		//0x24
	int				frame_simulation_ticks;		//0x28
	int				network_protocol;			//0x2C
	void*			save_data;					//0x30
	bool			client;					//0x34
	bool		    remote_client;				//0x35
	int				timestamp_networking_base;	//0x36
	int				timestamp_randomize_window;	//0x3A
	float absolute_time( ) {
		return (float)(clock( ) / (float)1000.f);
	}
}; // Size: 0x3E

class i_global_vars : public i_global_vars_base
{
public:
	string_t		map_name;					//0x3E
	string_t		map_group_name;				//0x42
	int				map_version;				//0x46
	string_t		start_spot;				//0x4A
	int				load_type;					//0x4E
	bool			map_load_failed;				//0x52
	bool			deathmatch;				//0x53
	bool			cooperative;				//0x54
	bool			teamplay;					//0x55
	int				max_entities;				//0x56
	int				server_count;				//0x5A
	void* edicts;					//0x5E
}; // Size: 0x62