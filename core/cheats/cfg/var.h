#pragma once

#include "cfg.h"

struct variables_t
{
#pragma region menu
	add_var( bool, menu_draw_blur, true );
	add_var( int, menu_draw_blur_alpha, 255 );
	add_var( bool, menu_draw_shadow, true );
	add_var( col_t, menu_draw_shadow_clr, col_t( 121, 109, 255 ) );
	add_var( float, menu_draw_shadow_thickness, 64.0f );
	add_var( key_t, menu_open_key, key_t( VK_INSERT, key_state_toggle ) );
	add_var( col_t, menu_accent, col_t( 105, 100, 205 ) );
#pragma endregion

#pragma region misc
	add_var( bool, misc_auto_bhop, false );
	add_var( int, auto_bhop_chance, 100 );
	add_var( bool, misc_fast_stop, false );
	add_var( bool, misc_auto_strafe, false );
	add_var( int, misc_auto_strafe_smoothness, 80 );
	add_var( key_t, misc_auto_strafe_key, key_t( ) );
	add_var( bool, misc_infinite_duck, false );

	add_var( bool, misc_reveal_rank, false );
	add_var( bool, misc_anti_untrusted, false );

#pragma endregion

#pragma region skin
	add_var( bool, skin_enable, false );
	add_var( int, skin_knife_model, 0 );

	add_var( int, skin_glove_model, 0 );
	add_var( int, skin_glove_paint_kit, 0 );
	add_var( float, skin_glove_wear, 1.0f );

	add_var_vector( int, 33, skin_selected_paint_kit, 0 );
	add_var_vector( float, 33, skin_selected_wear, 1.0f );
	add_var_vector( int, 33, skin_selected_seed, 0 );
	add_var_vector( int, 33, skin_selected_stat_track, 0 );
#pragma endregion

#pragma region visuals
	add_var( bool, vis_esp_enable, false );
	add_var( esp_variable_t, vis_esp_enemy_box, esp_variable_t( ) );
	add_var( esp_variable_t, vis_esp_teammate_box, esp_variable_t( ) );
	add_var( esp_variable_t, vis_esp_local_box, esp_variable_t( ) );

	add_var( esp_variable_t, vis_esp_enemy_name, esp_variable_t( esp_position_top, col_t( 5, 255, 47 ) ) );
	add_var( esp_variable_t, vis_esp_teammate_name, esp_variable_t( esp_position_top, col_t( 5, 255, 47 ) ) );
	add_var( esp_variable_t, vis_esp_local_name, esp_variable_t( esp_position_top, col_t( 5, 255, 47 ) ) );

	// hp bar
	add_var( esp_variable_t, vis_esp_enemy_hp_bar, esp_variable_t( esp_position_left, col_t( 150, 176, 255 ) ) );
	add_var( esp_variable_t, vis_esp_teammate_hp_bar, esp_variable_t( esp_position_left, col_t( 150, 176, 255 ) ) );
	add_var( esp_variable_t, vis_esp_local_hp_bar, esp_variable_t( esp_position_left, col_t( 150, 176, 255 ) ) );

	add_var( esp_variable_t, vis_esp_enemy_ammo_bar, esp_variable_t( esp_position_bot, col_t( 105, 0, 105 ) ) );
	add_var( esp_variable_t, vis_esp_teammate_ammo_bar, esp_variable_t( esp_position_bot, col_t( 105, 0, 105 ) ) );
	add_var( esp_variable_t, vis_esp_local_ammo_bar, esp_variable_t( esp_position_bot, col_t( 105, 0, 105 ) ) );

	add_var( esp_variable_t, vis_esp_enemy_weapon_icon, esp_variable_t( esp_position_bot, col_t( 246, 255, 69 ) ) );
	add_var( esp_variable_t, vis_esp_teammate_weapon_icon, esp_variable_t( esp_position_bot, col_t( 246, 255, 69 ) ) );
	add_var( esp_variable_t, vis_esp_local_weapon_icon, esp_variable_t( esp_position_bot, col_t( 246, 255, 69 ) ) );

	/* player flags 
	// bot
	// armor
	// has c4
	// is zoom ( scoped )
	// reloading
	// flashed
	*/

	add_var( esp_variable_t, vis_esp_enemy_flags_bot, esp_variable_t( esp_position_right, col_t( 255, 0, 0 ) ) );
	add_var( esp_variable_t, vis_esp_teammate_flags_bot, esp_variable_t( esp_position_right, col_t( 255, 0, 0 ) ) );
	add_var( esp_variable_t, vis_esp_local_flags_bot, esp_variable_t( esp_position_right, col_t( 255, 0, 0 ) ) );

	add_var( esp_variable_t, vis_esp_enemy_flags_armor, esp_variable_t( esp_position_right, col_t( 0, 0, 255 ) ) );
	add_var( esp_variable_t, vis_esp_teammate_flags_armor, esp_variable_t( esp_position_right, col_t( 0, 0, 255 ) ) );
	add_var( esp_variable_t, vis_esp_local_flags_armor, esp_variable_t( esp_position_right, col_t( 0, 0, 255 ) ) );

	add_var( esp_variable_t, vis_esp_enemy_flags_c4, esp_variable_t( esp_position_right, col_t( 205, 0, 0 ) ) );
	add_var( esp_variable_t, vis_esp_teammate_flags_c4, esp_variable_t( esp_position_right, col_t( 205, 0, 0 ) ) );
	add_var( esp_variable_t, vis_esp_local_flags_c4, esp_variable_t( esp_position_right, col_t( 205, 0, 0 ) ) );

	add_var( esp_variable_t, vis_esp_enemy_flags_zoom, esp_variable_t( esp_position_right, col_t( 205, 0, 205 ) ) );
	add_var( esp_variable_t, vis_esp_teammate_flags_zoom, esp_variable_t( esp_position_right, col_t( 205, 0, 205 ) ) );
	add_var( esp_variable_t, vis_esp_local_flags_zoom, esp_variable_t( esp_position_right, col_t( 205, 0, 205 ) ) );

	add_var( esp_variable_t, vis_esp_enemy_flags_reload, esp_variable_t( esp_position_right, col_t( 205, 205, 0 ) ) );
	add_var( esp_variable_t, vis_esp_teammate_flags_reload, esp_variable_t( esp_position_right, col_t( 205, 205, 0 ) ) );
	add_var( esp_variable_t, vis_esp_local_flags_reload, esp_variable_t( esp_position_right, col_t( 205, 205, 0 ) ) );

	add_var( esp_variable_t, vis_esp_enemy_flags_flashed , esp_variable_t( esp_position_right, col_t( 205, 205, 205 ) ) );
	add_var( esp_variable_t, vis_esp_teammate_flags_flashed, esp_variable_t( esp_position_right, col_t( 205, 205, 205 ) ) );
	add_var( esp_variable_t, vis_esp_local_flags_flashed, esp_variable_t( esp_position_right, col_t( 205, 205, 205 ) ) );
#pragma endregion
};

inline variables_t vars;
	
#define cfg_get(type, index) cfg::get<type>(index)
#define cfg_get_vector(type, name, index) cfg::get<std::vector<type>>(name).at(index)