#include "surface.h"

void c_surface::setup_font( )
{
    surface_font::verdana = csgo::surface->create_font( );
    csgo::surface->set_font_glyph_set( surface_font::verdana, "verdana", 14, font_weight_medium, 0, 0, font_flag_dropshadow );

    surface_font::tahoma = csgo::surface->create_font( );
    csgo::surface->set_font_glyph_set( surface_font::tahoma, "tahoma", 14, font_weight_medium, 0, 0, font_flag_dropshadow );

}

void c_surface::on_reset( )
{ 
    surface_font::verdana = 0;
    surface_font::tahoma = 0;

    this->setup_font( );
}

void c_surface::draw_filled_rect( const vec2_t& min, const vec2_t& max, const col_t& clr )
{ 
    csgo::surface->draw_set_color( clr );
    csgo::surface->draw_filled_rect( min.x, min.y, max.x, max.y );
}

void c_surface::draw_outlined_rect( const vec2_t& min, const vec2_t& max, const col_t& clr )
{
    csgo::surface->draw_set_color( clr );
    csgo::surface->draw_outlined_rect( min.x, min.y, max.x, max.y );
}

void c_surface::draw_line( const vec2_t& start, const vec2_t& end, const col_t& clr )
{
    csgo::surface->draw_set_color( clr );
    csgo::surface->draw_line( start.x, start.y, end.x, end.y );
}

void c_surface::draw_text( const h_font font, const vec2_t& position, const col_t& clr, std::string str )
{
    std::wstring wstr( str.begin( ), str.end( ) );

    csgo::surface->draw_set_text_font( font );
    csgo::surface->draw_set_text_pos( position.x, position.y );
    csgo::surface->draw_set_text_color( clr );
    csgo::surface->draw_print_text( wstr.c_str( ), wstr.length( ) );
}

vec2_t c_surface::get_text_size( const h_font font, std::string str )
{
    std::wstring wstr( str.begin( ), str.end( ) );
    int w, h;
    csgo::surface->get_text_size( font, wstr.c_str( ), w, h );

    return vec2_t( w, h );
}