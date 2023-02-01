#pragma once
#include <d3d9.h>

#include "../utils.h"
#include "enums/enums.hpp"

#include "data_types/vector.hpp"
#include "data_types/matrix.hpp"
#include "data_types/color.hpp"
#include "data_types/convar.hpp"
#include "data_types/key_values.hpp"

#include "interfaces/i_base_client_dll.hpp"
#include "interfaces/i_surface.hpp"
#include "interfaces/i_panel.hpp"
#include "interfaces/i_engine_client.hpp"
#include "interfaces/i_global_vars.hpp"
#include "interfaces/i_client_entity_list.hpp"
#include "interfaces/i_client_entity.hpp"
#include "interfaces/i_mdl_cache.hpp"
#include "interfaces/i_engine_trace.hpp"
#include "interfaces/i_client_mode_shared.hpp"
#include "interfaces/i_convar.hpp"
#include "interfaces/i_game_types.hpp"
#include "interfaces/i_prediction.hpp"
#include "interfaces/i_key_values_system.hpp"
#include "interfaces/i_weapon_system.hpp"
#include "interfaces/i_engine_sound.hpp"
#include "interfaces/i_input.hpp"
#include "interfaces/i_effects.hpp"
#include "interfaces/i_v_debug_overlay.hpp"
#include "interfaces/i_game_console.hpp"
#include "interfaces/i_v_model_render.hpp"
#include "interfaces/i_game_event.hpp"
#include "interfaces/i_localize.hpp"
#include "interfaces/i_material_system.hpp"
#include "interfaces/i_net_channel.hpp"
#include "interfaces/i_file_system.hpp"
#include "interfaces/i_physics_surface_props.hpp"
#include "interfaces/i_glow_manager.hpp"
#include "interfaces/i_input_system.hpp"
#include "interfaces/i_mem_alloc.hpp"
#include "interfaces/i_view_render_beams.hpp"
#include "interfaces/i_studio_render.hpp"
#include "interfaces/c_item_schema.h"
#include "interfaces/i_client_leaf_system.hpp"
#include "interfaces/i_steam_api.hpp"
#include "interfaces/i_render_view.hpp"

class c_interface_register
{
public:
	instantiate_interface_fn	create_fn;		//0x0
	const char*					name;			//0x4
	c_interface_register*		next;			//0x8
};

namespace csgo
{
	bool capture_interfaces( );

	// game interfaces
	inline i_base_client_dll* client = nullptr;
	inline i_surface* surface = nullptr;
	inline iv_panel* panel = nullptr;
	inline i_engine_client* engine = nullptr;
	inline i_global_vars_base* global_vars = nullptr;
	inline i_client_entity_list* entity_list = nullptr;
	inline iv_model_info* model_info = nullptr;
	inline iv_model_render* model_render = nullptr;
	inline i_engine_trace* engine_trace = nullptr;
	inline i_mdl_cache* mdl_cache = nullptr;
	inline i_client_state* client_state = nullptr;
	inline i_client_mode_shared* client_mode = nullptr;
	inline i_convar* convar = nullptr;
	inline i_game_types* game_types = nullptr;
	inline i_game_movement* game_movement = nullptr;
	inline i_move_helper* move_helper = nullptr;
	inline i_prediction* prediction = nullptr;
	inline i_key_values_system* key_values_system;
	inline i_weapon_system* weapon_system = nullptr;
	inline i_engine_sound* engine_sound = nullptr;
	inline i_input* input = nullptr;
	inline i_effects* effects = nullptr;
	inline iv_debug_overlay* debug_overlay = nullptr;
	inline i_game_console* game_console = nullptr;
	inline i_game_event_manager2* event_manager = nullptr;
	inline i_localize* localize = nullptr;
	inline i_material_system* material_system = nullptr;
	inline i_file_system* file_system = nullptr;
	inline i_physics_surface_props* physics_props;
	inline i_glow_object_manager* glow_manager = nullptr;
	inline i_input_system* input_system = nullptr;
	inline i_mem_alloc* mem_alloc = nullptr;
	inline i_view_render_beams* view_render_beams = nullptr;
	inline i_studio_render* studio_render = nullptr;
	inline std::add_pointer_t<c_item_system* __cdecl()> item_system = nullptr;
	inline i_client_leaf_system_engine* leaf_system = nullptr;
	inline i_render_view* render_view = nullptr;

	inline i_steam_user* steam_user = nullptr;
	inline i_steam_friends* steam_friend = nullptr;
	inline i_steam_utils* steam_utils = nullptr;

	inline void* view_setup = nullptr;

	inline IDirect3DDevice9* direct_device = nullptr;

	inline HWND window_handle = nullptr;

	namespace fn
	{
		inline int( __thiscall* equip_wearable )(c_base_combat_weapon* wearable, c_base_entity* player) = nullptr;
		inline const wchar_t* ( __thiscall* get_decorated_player_name )( c_player_resources* pr, int index, wchar_t* buffer, int buffsize, int flags );
		inline c_player_resources** player_resources = nullptr;
		inline bool( __thiscall* has_c4)( void* ) = nullptr;
		inline std::uintptr_t animation_overlays_offset = 0;
	}
}

namespace globals
{
	inline c_base_entity* local_player = nullptr;
	inline c_user_cmd* cmd = nullptr;
	inline vec3_t camera = vec3_t( );
	inline bool in_prediction = false;

	inline float absolute_time( ) {
		return (float)(clock( ) / (float)1000.f);
	}

	inline bool send_packet = false;
	inline vec2_t mouse_pos = vec2_t( );
}

#define get_server_time(ent) TICKS_TO_TIME( csgo::client_state->clock_drift_mgr.server_tick )