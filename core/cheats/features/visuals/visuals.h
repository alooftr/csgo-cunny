#pragma once
#include "../../cfg/var.h"
#include "../../../sdk/sdk.hpp"
#include "../../../utils/singleton.h"

class radar_player_t
{
public:
	vec3_t pos;
	vec3_t angle;
	vec3_t spotted_map_angle_related;
	DWORD tab_related;
	char pad_0x0028[ 0xC ];
	float spotted_time;
	float spotted_fraction;
	float time;
	char pad_0x0040[ 0x4 ];
	__int32 player_index;
	__int32 entity_index;
	char pad_0x004C[ 0x4 ];
	__int32 health;
	char name[ 32 ];
	char pad_0x0074[ 0x75 ];
	unsigned char spotted;
	char pad_0x00EA[ 0x8A ];
};

class ccsgo_hud_radar
{
public:
	char pad_0x0000[ 0x14C ];
	radar_player_t radar_info[ 65 ];
};

class c_visuals : public c_singleton<c_visuals>
{
	friend class c_singleton<c_visuals>;
public:
	// call this on paint tranverse hook
	void run( c_base_entity* local_player );
	
private:
	struct bbox_t
	{
		float left = 0.f, top = 0.f, right = 0.f, bottom = 0.f, width = 0.f, height = 0.f;
	};

	enum e_padding_direction : unsigned int
	{
		pad_left = 0,
		pad_top,
		pad_right,
		pad_bottom,
		pad_max
	};

	struct context_t
	{
		context_t( c_base_entity* entity ) : entity{ entity } {}

		// bounding box
		bbox_t bbox = { };
		// summary of all directions paddings
		std::array<ImVec2, pad_max> padding = { };
		// ent
		c_base_entity* entity = nullptr;

		float dormant_alpha = 1.0f;

		int health = 100;
	};
	bool calc_bound_box( c_base_entity* ent, bbox_t* bbox_in ) const;
	void process_dormancy( context_t* ctx, ccsgo_hud_radar* hud_radar );

	void player_esp( context_t& ctx, c_base_entity* local_player, float cam_dist );

	void player_bounding_box( context_t& ctx, const esp_variable_t* config );

	void draw_bar_factor( context_t& ctx, const esp_variable_t* config, float factor );
	void draw_text( context_t& ctx, const esp_variable_t* config, std::string str, ImFont* font );
};