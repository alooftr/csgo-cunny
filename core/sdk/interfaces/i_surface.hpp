#pragma once

typedef unsigned long h_scheme, h_panel, h_texture, h_cursor, h_font;

#pragma region surface_enumerations
enum e_font_draw_type : int
{
	font_draw_default = 0,
	font_draw_nonadditive,
	font_draw_additive,
	font_draw_type_count = 2
};

enum e_font_flags
{
	font_flag_none,
	font_flag_italic = 0x001,
	font_flag_underline = 0x002,
	font_flag_strikeout = 0x004,
	font_flag_symbol = 0x008,
	font_flag_antialias = 0x010,
	font_flag_gaussianblur = 0x020,
	font_flag_rotary = 0x040,
	font_flag_dropshadow = 0x080,
	font_flag_additive = 0x100,
	font_flag_outline = 0x200,
	font_flag_custom = 0x400,
	font_flag_bitmap = 0x800,
};

enum e_font_weight
{
	font_weight_thin = 100,
	font_weight_extra_light = 200,
	font_weight_light = 300,
	font_weight_normal = 400,
	font_weight_medium = 500,
	font_weight_semi_bold = 600,
	font_weight_bold = 700,
	font_weight_extra_bold = 800,
	font_weight_heavy = 900,
};
#pragma endregion

struct vertex_t
{
	vertex_t( ) = default;

	vertex_t( const vec2_t& position, const vec2_t& coordinate = vec2_t( 0, 0 ) )
	{
		this->position = position;
		this->coordinate = coordinate;
	}

	void init( const vec2_t& position, const vec2_t& coordinate = vec2_t( 0, 0 ) )
	{
		this->position = position;
		this->coordinate = coordinate;
	}

	vec2_t position = { };
	vec2_t coordinate = { };
};

class i_surface
{
public:
	void draw_set_color( col_t clr )
	{
		utils::call_vfunc<void>( this, 14, clr );
	}

	void draw_set_color( int r, int g, int b, int a )
	{
		utils::call_vfunc<void>( this, 15, r, g, b, a );
	}

	void draw_filled_rect( int x0, int y0, int x1, int y1 )
	{
		utils::call_vfunc<void>( this, 16, x0, y0, x1, y1 );
	}

	void draw_filled_rect_fade( int x0, int y0, int x1, int y1, uint32_t alpha1, uint32_t alpha2, bool horizontal )
	{
		utils::call_vfunc<void>( this, 123, x0, y0, x1, y1, alpha1, alpha2, horizontal );
	}

	void draw_outlined_rect( int x0, int y0, int x1, int y1 )
	{
		utils::call_vfunc<void>( this, 18, x0, y0, x1, y1 );
	}

	void draw_line( int x0, int y0, int x1, int y1 )
	{
		utils::call_vfunc<void>( this, 19, x0, y0, x1, y1 );
	}

	void draw_poly_line( int* x, int* y, int points )
	{
		utils::call_vfunc<void>( this, 20, x, y, points );
	}

	void draw_set_text_font( h_font font )
	{
		utils::call_vfunc<void>( this, 23, font );
	}

	void draw_set_text_color( col_t col_t )
	{
		utils::call_vfunc<void>( this, 24, col_t );
	}

	void draw_set_text_color( int r, int g, int b, int a )
	{
		utils::call_vfunc<void>( this, 25, r, g, b, a );
	}

	void draw_set_text_pos( int x, int y )
	{
		utils::call_vfunc<void>( this, 26, x, y );
	}

	void draw_print_text( const wchar_t* text, int text_length, e_font_draw_type draw_type = font_draw_default )
	{
		utils::call_vfunc<void>( this, 28, text, text_length, draw_type );
	}

	void draw_set_texture_rgba( int index, const unsigned char* rgba, int wide, int iTall )
	{
		utils::call_vfunc<void>( this, 37, index, rgba, wide, iTall );
	}

	void draw_set_texture( int index )
	{
		utils::call_vfunc<void>( this, 38, index );
	}

	int create_new_texture_id( bool procedural = false )
	{
		return utils::call_vfunc<int>( this, 43, procedural );
	}

	void unlock_cursor( )
	{
		return utils::call_vfunc<void>( this, 66 );
	}

	void lock_cursor( )
	{
		return utils::call_vfunc<void>( this, 67 );
	}

	h_font create_font( )
	{
		return utils::call_vfunc<h_font>( this, 71 );
	}

	bool set_font_glyph_set( h_font font, const char* windows_font_name, int tall, int weight, int blur, int scan_lines, int flags, int range_min = 0, int range_max = 0 )
	{
		return utils::call_vfunc<bool>( this, 72, font, windows_font_name, tall, weight, blur, scan_lines, flags, range_min, range_max );
	}

	void get_text_size( h_font font, const wchar_t* text, int& wide, int& tall )
	{
		utils::call_vfunc<void>( this, 79, font, text, std::ref( wide ), std::ref( tall ) );
	}

	void play_sound_surface( const char* file_name )
	{
		utils::call_vfunc<void>( this, 82, file_name );
	}

	void draw_outlined_circle( int x, int y, int radius, int segments )
	{
		utils::call_vfunc<void>( this, 103, x, y, radius, segments );
	}

	void draw_textured_polygon( int n, vertex_t* vertice, bool clip_vertices = true )
	{
		utils::call_vfunc<void>( this, 106, n, vertice, clip_vertices );
	}
};