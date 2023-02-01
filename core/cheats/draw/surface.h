#pragma once

#include "../../sdk/sdk.hpp"
#include "../../utils/singleton.h"

namespace surface_font
{
	inline h_font verdana;
	inline h_font tahoma;
}

class c_surface : public c_singleton<c_surface>
{
public:
	// we will call it at the same time setuping d3d fonts from imgui so no need to check
	// same as when device reset we recreate the font again tho we clear it first
	void setup_font( );

	void on_reset( );

	// drawing function
	void draw_filled_rect( const vec2_t& min, const vec2_t& max, const col_t& clr );
	void draw_outlined_rect( const vec2_t& min, const vec2_t& max, const col_t& clr );
	void draw_line( const vec2_t& start, const vec2_t& end, const col_t& clr );

	// text
	void draw_text( const h_font font, const vec2_t& position, const col_t& clr, std::string str );

	vec2_t get_text_size( const h_font font, std::string str );
};