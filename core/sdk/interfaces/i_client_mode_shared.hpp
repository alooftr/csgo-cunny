#pragma once

#pragma region clientmode_definitions
#define SIGNONSTATE_NONE		0	// no state yet, about to connect
#define SIGNONSTATE_CHALLENGE	1	// client challenging server, all OOB packets
#define SIGNONSTATE_CONNECTED	2	// client is connected to server, netchans ready
#define SIGNONSTATE_NEW			3	// just got serverinfo and string tables
#define SIGNONSTATE_PRESPAWN	4	// received signon buffers
#define SIGNONSTATE_SPAWN		5	// ready to receive entity packets
#define SIGNONSTATE_FULL		6	// we are fully connected, first non-delta packet received (in-game check)
#define SIGNONSTATE_CHANGELEVEL	7	// server is changing level, please wait
#pragma endregion

class c_view_setup
{
public:
	int			x;
	int			unscaled_x;
	int			y;
	int			unscaled_y;
	int			width;
	int			unscaled_width;
	int			height;
	int			unscaled_height;
	bool		ortho;
	float		ortho_left;
	float		ortho_top;
	float		ortho_right;
	float		ortho_bottom;
private:
	std::byte	pad0[ 0x7C ];
public:
	float		fov;
	float		view_model_fov;
	vec3_t		origin;
	vec3_t		view;
	float		near_z;
	float		far_z;
	float		near_viewmodel_z;
	float		far_viewmodel_z;
	float		aspect_ratio;
	float		near_blur_depth;
	float		near_focus_depth;
	float		far_focus_depth;
	float		far_blur_depth;
	float		near_blur_radius;
	float		far_blur_radius;
	float		do_f_quality;
	int			motion_blur_mode;
	float		shutter_time;
	vec3_t		shutter_open_position;
	vec3_t		shutter_open_angles;
	vec3_t		shutter_close_position;
	vec3_t		shutter_close_angles;
	float		off_center_top;
	float		off_center_bottom;
	float		off_center_left;
	float		off_center_right;
	bool		off_center : 1;
	bool		render_to_subrect_of_larger_screen : 1;
	bool		do_bloom_and_tone_mapping : 1;
	bool		do_depth_of_field : 1;
	bool		hdr_target : 1;
	bool		draw_world_normal : 1;
	bool		cull_font_faces : 1;
	bool		cache_full_scene_state : 1;
	bool		csm_view : 1;
};

class i_hud_chat;
class i_client_mode_shared
{
private:
	std::byte	pad0[ 0x1B ];
public:
	void* viewport;
	i_hud_chat* chat_element;
	h_cursor cursor_none;
	void* weapon_selection;
	int			root_size[ 2 ];
};

class i_app_system
{
private:
	virtual void function0( ) = 0;
	virtual void function1( ) = 0;
	virtual void function2( ) = 0;
	virtual void function3( ) = 0;
	virtual void function4( ) = 0;
	virtual void function5( ) = 0;
	virtual void function6( ) = 0;
	virtual void function7( ) = 0;
	virtual void function8( ) = 0;
};

class c_event_info
{
public:
	short				class_id;		// 0x00 // 0 implies not in use
	float				fire_delay;	// 0x02 // if non-zero, the delay time when the event should be fired ( fixed up on the client )
	const void* send_table;		// 0x06
	const c_base_client* client_class;	// 0x0A // clienclass pointer
	void* data;			// 0x0E // raw event data
	std::intptr_t		packed_bits;	// 0x12
	int					flags;			// 0x16
private:
	std::byte			pad0[ 0x16 ];		// 0x1A
	std::byte pad1[ 0x1C ];
public:
	c_event_info* next;
}; // Size: 0x30

class c_clock_drift_mgr
{
public:
	enum
	{
		// This controls how much it smoothes out the samples from the server.
		NUM_CLOCKDRIFT_SAMPLES = 16
	};

	// This holds how many ticks the client is ahead each time we get a server tick.
	// We average these together to get our estimate of how far ahead we are.
	float clock_offsets[ NUM_CLOCKDRIFT_SAMPLES ]; //0x0128
	int cur_clock_offset; // 0x0168

	int server_tick; // 0x016C		// Last-received tick from the server.
	int client_tick; // 0x0170		// The client's own tick counter (specifically, for interpolation during rendering).
	// The server may be on a slightly different tick and the client will drift towards it.
}; //Size: 76

class i_net_channel;
class i_client_state
{
public:
	std::byte		pad0[ 0x9C ];				// 0x0000
	i_net_channel*  net_channel;			// 0x009C
	int				challenge_nr;			// 0x00A0
	std::byte		pad1[ 0x64 ];				// 0x00A4
	int				signon_state;			// 0x0108
	std::byte		pad2[ 0x8 ];				// 0x010C
	float			next_cmd_time;			// 0x0114
	int				server_count;			// 0x0118
	int				current_sequence;		// 0x011C
	char pad_0120[ 8 ]; //0x0120
	c_clock_drift_mgr clock_drift_mgr; //0x0128
	int				delta_tick;				// 0x0174
	bool			paused;				// 0x0178
	std::byte		pad4[ 0x7 ];				// 0x0179
	int				view_entity;			// 0x0180
	int				player_slot;			// 0x0184
	char			level_name[ MAX_PATH ];	// 0x0188
	char			level_name_short[ 80 ];	// 0x028C
	char			map_group_name[ 80 ];		// 0x02DC
	char			last_level_name_short[ 80 ]; // 0x032C
	std::byte		pad5[ 0xC ];				// 0x037C
	int				max_clients;			// 0x0388 
	std::byte		pad6[ 0x498C ];			// 0x038C
	float			last_server_tick_time;	// 0x4D18
	bool			in_simulation;			// 0x4D1C
	std::byte		pad7[ 0x3 ];				// 0x4D1D
	int				old_tickcount;			// 0x4D20
	float			tick_remainder;		// 0x4D24
	float			frame_time;			// 0x4D28
	int				last_outgoing_command;	// 0x4D2C
	int				choked_commands;		// 0x4D30
	int				last_command_ack;		// 0x4D34
	int				command_ack;			// 0x4D38
	int				sound_sequence;			// 0x4D3C
	std::byte		pad8[ 0x50 ];				// 0x4D40
	vec3_t			view_point;			// 0x4D90
	std::byte		pad9[ 0xD0 ];				// 0x4D9C
	c_event_info* events;				// 0x4E6C
}; // Size: 0x4E70
