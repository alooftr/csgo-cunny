#pragma once
#include <string>

#include "../../sdk/data_types/color.hpp"

enum e_esp_position : unsigned
{
	esp_position_top = 0,
	esp_position_left,
	esp_position_right,
	esp_position_bot
};

enum e_esp_draw_preview : unsigned
{
	esp_draw_preview_box = 0,
	esp_draw_preview_name,
	esp_draw_preview_flags,
	esp_draw_preview_bar
};

/* custom esp variables struct 
// for show pos, drawing item, previews...
//
*/
struct esp_variable_t
{
	esp_variable_t(
		int position = 0, col_t primary_color = col_t( 255, 255, 255 ), bool enable = false,
		col_t background_color = col_t( 0, 0, 0, 125 ), col_t outline_color = col_t( 105, 100, 205 ),
		bool glow = false, col_t glow_color = col_t( 105, 100, 205 ) )
		: enable{ enable }, position{ position }, primary_color{ primary_color }, background_color{ background_color }, outline_color{ outline_color }, glow{ glow }, glow_color{ glow_color }
	{}
	
	bool enable = false;
	int position = esp_position_top;
	col_t primary_color = col_t( 255, 255, 255 );
	col_t background_color = col_t( 0, 0, 0, 125 );
	col_t outline_color = col_t( 105, 100, 205, 255 );
	
	bool glow = false;
	col_t glow_color = col_t( 105, 100, 205, 255 );
	void draw_menu_item( std::string label, bool show_position = true, bool show_background = true, bool show_glow = true );
	void draw_preview( std::string label, int type = esp_draw_preview_box );

private:
	void draw_popup_menu( std::string label, bool show_position = true, bool show_background = true, bool show_glow = true );
};