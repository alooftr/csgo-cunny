#include "extra_cfg_struct.h"
// color picker
#include "../gui/extra_imgui.h"
// ImRect
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../../../ext/imgui/imgui_internal.h"
// including config accent
#include "var.h"

void esp_variable_t::draw_menu_item( std::string label, bool show_position, bool show_background, bool show_glow )
{ 
	ImGui::Checkbox( label.c_str( ), &this->enable );

	ImGui::OpenPopupOnItemClick( ( label + " extras" ).c_str( ), ImGuiPopupFlags_MouseButtonRight );

	ImGui::HoveredTooltip( "right-click for more options!" );

	this->draw_popup_menu( label, show_position, show_background, show_glow );
}

void esp_variable_t::draw_popup_menu( std::string label, bool show_position, bool show_background, bool show_glow )
{
	if ( ImGui::BeginPopup( ( label + " extras" ).c_str( ) ) )
	{
		if ( show_position )
		{
			ImGui::SetNextItemWidth( ImGui::GetContentRegionAvail( ).x - ImGui::GetCursorPos( ).x );
			ImGui::Combo( "position", &this->position, "top\0left\0right\0bot" );
		}

		ImGui::ColorPicker( "primary color", &this->primary_color );

		if ( show_background )
			ImGui::ColorPicker( "background color", &this->background_color );

		ImGui::ColorPicker( "outline color", &this->outline_color );

		if ( show_glow )
		{
			ImGui::Checkbox( "glow", &this->glow );
			ImGui::SameLine( ImGui::CalcItemWidth( ) - ImGui::GetCursorPos( ).x );
			ImGui::ColorPicker( "##glow color", &this->glow_color );
		}

		ImGui::EndPopup( );
	}
}

void esp_variable_t::draw_preview( std::string label, int type )
{ 
	if ( !this->enable )
		return;


}
