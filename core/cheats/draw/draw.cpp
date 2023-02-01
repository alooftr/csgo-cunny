// D3DXCreateTexture 
#include <D3dx9tex.h>

#include "draw.h"
// csgo window_handle
#include "../../sdk/sdk.hpp"

// including memory font
#include "../../../resources/whitney.h"
#include "../../../resources/smallest_pixel.h"
#include "../../../resources/weapon_icons.h"
#include "../../../resources/menu_icon.h"
#include "../../../resources/avatar.h"

// blur memory texture
#include "../../../resources/blur_texture.h"

void draw::setup( IDirect3DDevice9* device )
{
	ImGui::CreateContext( );
	ImGui_ImplWin32_Init( csgo::window_handle );
	ImGui_ImplDX9_Init( device );

	// color style
	ImGui::StyleColorsClassic( );
	auto& style = ImGui::GetStyle( );

#pragma region imgui_style
	style.Alpha = 1.0f;
	style.WindowPadding = ImVec2( 8, 8 );
	style.WindowRounding = 8.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2( 32, 32 );
	style.WindowTitleAlign = ImVec2( 0.5f, 0.5f );
	style.ChildRounding = 6.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 8.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2( 4, 2 );
	style.FrameRounding = 4.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2( 8, 4 );
	style.ItemInnerSpacing = ImVec2( 4, 4 );
	style.IndentSpacing = 6.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 6.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 0.0f;
	style.GrabRounding = 4.0f;
	style.TabRounding = 8.0f;
	style.TabBorderSize = 1.0f;
	style.ButtonTextAlign = ImVec2( 0.5f, 0.5f );
	style.SelectableTextAlign = ImVec2( 0.0f, 0.5f );
	style.MouseCursorScale = 0.75f;
	style.AntiAliasedFill = true;
	style.AntiAliasedLines = true;
	style.AntiAliasedLinesUseTex = true;
#pragma endregion

	auto& io = ImGui::GetIO( );

	ImFontConfig whitney_config;
	whitney_config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting;
	imgui_fonts::whitney = io.Fonts->AddFontFromMemoryCompressedTTF( whitney_compressed_data, whitney_compressed_size, 16.f, &whitney_config, io.Fonts->GetGlyphRangesCyrillic( ) );

	ImFontConfig menu_icon_config;
	menu_icon_config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting;
	imgui_fonts::menu_icon = io.Fonts->AddFontFromMemoryCompressedTTF( menu_icon_compressed_data, menu_icon_compressed_size, 20.f, &menu_icon_config );

	ImFontConfig arial_config;
	arial_config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome;
	imgui_fonts::arial = io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\ArialUni.ttf", 14.f, &arial_config, io.Fonts->GetGlyphRangesJapanese( ) );

	ImFontConfig verdana_config;
	verdana_config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome;
	imgui_fonts::verdana = io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\Verdana.ttf", 10.f, &verdana_config, io.Fonts->GetGlyphRangesJapanese( ) );
		
	ImFontConfig smallest_pixel_config;
	smallest_pixel_config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting;
	imgui_fonts::smallest_pixel = io.Fonts->AddFontFromMemoryCompressedTTF( smallest_pixel_compressed_data, smallest_pixel_compressed_size, 10.f, &smallest_pixel_config, io.Fonts->GetGlyphRangesCyrillic( ) );

	ImFontConfig weapon_icons_config;
	weapon_icons_config.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_LightHinting;
	constexpr ImWchar icon_ranges[ ] =
	{
		0xE000, 0xF8FF, // Private Use Area
		0
	};
	imgui_fonts::weapon_icons = io.Fonts->AddFontFromMemoryCompressedTTF( weapon_icons_compressed_data, weapon_icons_compressed_size, 14.f, &weapon_icons_config, icon_ranges );


	if (images::avatar == nullptr)
		D3DXCreateTextureFromFileInMemoryEx( device, avatar_data, sizeof( avatar_data ), 55, 55, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &images::avatar );

	did_setup = ImGuiFreeType::BuildFontAtlas( io.Fonts );
}

void draw::render_draw_data( ImDrawList* draw_list )
{
	std::unique_lock<std::shared_mutex> lock( draw_mutex );

	if (safe_draw_data.empty( ))
		return;

	for (const auto& data : safe_draw_data)
	{
		if (!data.object.has_value( ))
			continue;

		switch (data.type)
		{
		case e_draw_type::line:
		{
			const auto& object = std::any_cast<line_object_t>(data.object);
			draw_list->AddLine( object.start, object.end, object.clr, object.thickness );
			break;
		}
		case e_draw_type::rect:
		{
			const auto& object = std::any_cast<rect_object_t>(data.object);
			if ((object.pos_min.x == object.pos_max.x && object.pos_min.y == object.pos_max.y) || object.pos_min.x == NULL || object.pos_min.y == NULL)
				break;

			add_draw_list_rect( draw_list, object.pos_min, object.pos_max, object.clr, object.flags, object.clr_outline, object.rounding, object.rounding_corners, object.thickness );
			break;
		}
		case e_draw_type::rect_multicolor:
		{
			const auto& object = std::any_cast<rect_multi_color_object_t>(data.object);
			draw_list->AddRectFilledMultiColorRounded( object.pos_min, object.pos_max, object.clr1, object.horizontal ? object.clr1 : object.clr2, object.clr2, object.horizontal ? object.clr2 : object.clr1, object.ronuding, object.rounding_corners );
			break;
		}
		case e_draw_type::circle:
		{
			const auto& object = std::any_cast<circle_object_t>(data.object);

			if (object.flags & draw_circle_filled)
				draw_list->AddCircleFilled( object.center, object.radius, object.clr, object.segments );
			else
				draw_list->AddCircle( object.center, object.radius, object.clr, object.segments, object.thickness );

			if (object.flags & draw_circle_outline)
				draw_list->AddCircle( object.center, object.radius + 1.0f, object.clr_outline, object.segments, object.thickness + 1.0f );

			break;
		}
		case e_draw_type::triangle:
		{
			const auto& object = std::any_cast<triangle_object_t>(data.object);

			if (object.flags & draw_triangle_filled)
				draw_list->AddTriangleFilled( object.first, object.second, object.third, object.clr );
			else
				draw_list->AddTriangle( object.first, object.second, object.third, object.clr, object.thickness );

			if (object.flags & draw_triangle_outline)
				draw_list->AddTriangle( object.first, object.second, object.third, object.clr_outline, object.thickness + 1.0f );

			break;
		}
		case e_draw_type::polygon:
		{
			const auto& object = std::any_cast<polygon_object_t>(data.object);

			if (object.flags & draw_polygon_filled)
				draw_list->AddConvexPolyFilled( object.points.data( ), object.points.size( ), object.clr );
			else
				draw_list->AddPolyline( object.points.data( ), object.points.size( ), object.clr, object.closed, object.thickness );

			if (object.flags & draw_polygon_outline)
				draw_list->AddPolyline( object.points.data( ), object.points.size( ), object.clr_outline, object.closed, object.thickness + 1.0f );

			break;
		}
		case e_draw_type::text:
		{
			const auto& object = std::any_cast<text_object_t>(data.object);
			add_draw_list_text( draw_list, object.font, object.position, object.text, object.clr, object.flags, object.clr_outline );
			break;
		}
		case e_draw_type::image:
		{
			const auto& object = std::any_cast<image_object_t>(data.object);
			draw_list->AddImageRounded( object.texture, object.pos_min, object.pos_max, ImVec2( 0, 0 ), ImVec2( 1, 1 ), object.clr, object.rounding, object.rounding_corners );
			break;
		}
		case e_draw_type::shadow_rect:
		{
			const auto& object = std::any_cast<shadow_rect_object_t>(data.object);
			draw_list->AddShadowRect( object.pos_min, object.pos_max, object.clr, object.thickness, ImVec2( 0, 0 ), object.rounding_corners, object.rounding );
			break;
		}
		default:
			break;
		}
	}
}

void draw::clear_draw_data( )
{
	if (!draw_data.empty( ))
		draw_data.clear( );
}

void draw::swap_draw_data( )
{
	std::unique_lock<std::shared_mutex> lock( draw_mutex );
	draw_data.swap( safe_draw_data );
}

void draw::add_line( const ImVec2& start, const ImVec2& end, const col_t& clr, float thickness )
{
	draw_data.emplace_back( e_draw_type::line, std::make_any<line_object_t>( start, end, clr.get_u32( ), thickness ) );
}

void draw::add_rect( const ImVec2& pos_min, const ImVec2& pos_max, const col_t& clr, unsigned int flags, const col_t& clr_outline, float rounding, ImDrawCornerFlags rounding_corners, float thickness )
{
	draw_data.emplace_back( e_draw_type::rect, std::make_any<rect_object_t>( pos_min, pos_max, clr.get_u32( ), flags, clr_outline.set<col_a>( clr.get<col_a>( ) ).get_u32( ), rounding, rounding_corners, thickness ) );
}

void draw::add_rect_multi_color( const ImVec2& pos_min, const ImVec2& pos_max, const col_t& clr1, const col_t& clr2, bool horizontal, float rounding, ImDrawFlags rounding_corners )
{
	draw_data.emplace_back( e_draw_type::rect_multicolor, std::make_any<rect_multi_color_object_t>( pos_min, pos_max, clr1.get_u32( ), clr2.get_u32( ), horizontal, rounding, rounding_corners ) );
}

void draw::add_circle( const ImVec2& center, float radius, const col_t& clr, int segments, unsigned int flags, const col_t& clr_outline, float thickness )
{
	draw_data.emplace_back( e_draw_type::circle, std::make_any<circle_object_t>( center, radius, clr.get_u32( ), segments, flags, clr_outline.set<col_a>( clr.get<col_a>( ) ).get_u32( ), thickness ) );
}

void draw::add_triangle( const ImVec2& first, const ImVec2& second, const ImVec2& third, const col_t& clr, unsigned int flags, const col_t& clr_outline, float thickness )
{
	draw_data.emplace_back( e_draw_type::triangle, std::make_any<triangle_object_t>( first, second, third, clr.get_u32( ), flags, clr_outline.set<col_a>( clr.get<col_a>( ) ).get_u32( ), thickness ) );
}

void draw::add_polygon( std::vector<ImVec2>& points, const col_t& clr, unsigned int flags, const col_t& clr_outline, bool closed, float thickness )
{
	draw_data.emplace_back( e_draw_type::polygon, std::make_any<polygon_object_t>( std::move( points ), clr.get_u32( ), flags, clr_outline.set<col_a>( clr.get<col_a>( ) ).get_u32( ), closed, thickness ) );
}

void draw::add_text( const ImFont* font, const ImVec2& pos, const std::string& str, const col_t& clr, unsigned int flags, const col_t& clr_outline )
{
	if (font->ContainerAtlas == nullptr)
		return;

	// check is only one flag selected
	IM_ASSERT( ImIsPowerOfTwo( flags == draw_text_none || flags & (draw_text_dropshadow | draw_text_outline) ) );

	draw_data.emplace_back( e_draw_type::text, std::make_any<text_object_t>( font, font->FontSize, pos, str, clr.get_u32( ), flags, clr_outline.set<col_a>( clr.get<col_a>( ) ).get_u32( ) ) );
}

void draw::add_image( ImTextureID texture, const ImVec2& pos_min, const ImVec2& pos_max, const col_t& clr, float rounding, ImDrawFlags rounding_corners )
{
	draw_data.emplace_back( e_draw_type::image, std::make_any<image_object_t>( texture, pos_min, pos_max, clr.get_u32( ), rounding, rounding_corners ) );
}

void draw::add_shadow_rect( const ImVec2& pos_min, const ImVec2 pos_max, const col_t& clr, float thickness, float rounding, ImDrawFlags rounding_corners )
{
	draw_data.emplace_back( e_draw_type::shadow_rect, std::make_any<shadow_rect_object_t>( pos_min, pos_max, clr.get_u32( ), thickness, rounding, rounding_corners ) );
}

void draw::add_draw_list_rect( ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max, ImU32 clr, unsigned int flags, ImU32 clr_outline, float rounding, ImDrawCornerFlags rounding_corners, float thickness )
{
	if (flags & draw_rect_border)
		draw_list->AddRect( ImVec2( pos_min.x + 1.0f, pos_min.y + 1.0f ), ImVec2( pos_max.x - 1.0f, pos_max.y - 1.0f ), clr_outline, rounding, rounding_corners, thickness );

	if (flags & draw_rect_outline)
		draw_list->AddRect( ImVec2( pos_min.x - 1.0f, pos_min.y - 1.0f ), ImVec2( pos_max.x + 1.0f, pos_max.y + 1.0f ), clr_outline, rounding, rounding_corners, thickness );

	if (flags & draw_rect_filled)
		draw_list->AddRectFilled( pos_min, pos_max, clr, rounding, rounding_corners );
	else
		draw_list->AddRect( pos_min, pos_max, clr, rounding, rounding_corners, thickness );
}

void draw::add_draw_list_text( ImDrawList* draw_list, const ImFont* font, const ImVec2& pos, const std::string& str, ImU32 clr, unsigned int flags, ImU32 clr_outline )
{
	// set font texture
	draw_list->PushTextureID( font->ContainerAtlas->TexID );

	if (flags & draw_text_dropshadow)
		draw_list->AddText( font, font->FontSize, ImVec2( pos.x - 1.0f, pos.y + 1.0f ), clr_outline, str.c_str( ) );
	else if (flags & draw_text_outline)
	{
		draw_list->AddText( font, font->FontSize, ImVec2( pos.x + 1.0f, pos.y - 1.0f ), clr_outline, str.c_str( ) );
		draw_list->AddText( font, font->FontSize, ImVec2( pos.x - 1.0f, pos.y + 1.0f ), clr_outline, str.c_str( ) );
	}

	draw_list->AddText( font, font->FontSize, pos, clr, str.data( ) );
	draw_list->PopTextureID( );
}

static IDirect3DSurface9* rt_backup = nullptr;
static IDirect3DPixelShader9* blur_shader_x = nullptr;
static IDirect3DPixelShader9* blur_shader_y = nullptr;
static IDirect3DTexture9* blur_texture = nullptr;
static int back_buffer_width = 0;
static int back_buffer_height = 0;

static void begin_blur( const ImDrawList* parent_list, const ImDrawCmd* cmd )
{
	const auto device = reinterpret_cast<IDirect3DDevice9*>(cmd->UserCallbackData);

	if (!blur_shader_x)
	{
		device->CreatePixelShader( reinterpret_cast<const DWORD*>(blur_x.data( )), &blur_shader_x );
	}

	if (!blur_shader_y)
	{
		device->CreatePixelShader( reinterpret_cast<const DWORD*>(blur_y.data( )), &blur_shader_y );
	}

	IDirect3DSurface9* back_buffer;
	device->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &back_buffer );
	D3DSURFACE_DESC desc;
	back_buffer->GetDesc( &desc );

	if (back_buffer_width != desc.Width || back_buffer_height != desc.Height)
	{
		if (blur_texture)
		{
			blur_texture->Release( );
		}

		back_buffer_width = desc.Width;
		back_buffer_height = desc.Height;
		device->CreateTexture( desc.Width, desc.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &blur_texture, nullptr );
	}

	device->GetRenderTarget( 0, &rt_backup );

	{
		IDirect3DSurface9* surface;
		blur_texture->GetSurfaceLevel( 0, &surface );
		device->StretchRect( back_buffer, nullptr, surface, nullptr, D3DTEXF_NONE );
		device->SetRenderTarget( 0, surface );
		surface->Release( );
	}

	back_buffer->Release( );

	device->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	device->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
}

static void first_blur_pass( const ImDrawList* parent_list, const ImDrawCmd* cmd )
{
	const auto device = reinterpret_cast<IDirect3DDevice9*>(cmd->UserCallbackData);

	device->SetPixelShader( blur_shader_x );
	const float params[ 4 ] = { 1.0f / back_buffer_width };
	device->SetPixelShaderConstantF( 0, params, 1 );
}

static void second_blur_pass( const ImDrawList* parent_list, const ImDrawCmd* cmd )
{
	const auto device = reinterpret_cast<IDirect3DDevice9*>(cmd->UserCallbackData);

	device->SetPixelShader( blur_shader_y );
	const float params[ 4 ] = { 1.0f / back_buffer_height };
	device->SetPixelShaderConstantF( 0, params, 1 );
}

static void end_blur( const ImDrawList* parent_list, const ImDrawCmd* cmd )
{
	const auto device = reinterpret_cast<IDirect3DDevice9*>(cmd->UserCallbackData);

	device->SetRenderTarget( 0, rt_backup );
	rt_backup->Release( );

	device->SetPixelShader( nullptr );
	device->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	device->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
}

void draw::clear_blur( )
{
	if (blur_texture)
	{
		blur_texture->Release( );
		blur_texture = nullptr;
	}
}

void draw::add_blur( ImDrawList* draw_list, int alpha, const ImVec2& pos_min, const ImVec2& pos_max )
{
	auto drawList = ImGui::GetBackgroundDrawList( );

	drawList->PushClipRect( pos_min, pos_max );
	drawList->AddCallback( begin_blur, csgo::direct_device );

	for (int i = 0; i < 8; ++i)
	{
		drawList->AddCallback( first_blur_pass, csgo::direct_device );
		drawList->AddImage( blur_texture, { 0.0f, 0.0f }, { back_buffer_width * 1.0f, back_buffer_height * 1.0f } );
		drawList->AddCallback( second_blur_pass, csgo::direct_device );
		drawList->AddImage( blur_texture, { 0.0f, 0.0f }, { back_buffer_width * 1.0f, back_buffer_height * 1.0f } );
	}

	drawList->AddCallback( end_blur, csgo::direct_device );
	drawList->AddImage( blur_texture, { 0.0f, 0.0f }, { back_buffer_width * 1.0f, back_buffer_height * 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, IM_COL32( 255, 255, 255, alpha ) );
	drawList->PopClipRect( );
}
