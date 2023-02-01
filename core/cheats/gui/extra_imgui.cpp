#include <unordered_map>
// clamp
#include <algorithm>

#include "extra_imgui.h"

#include "../../utils/input.h"

float content_anim = 0.f;

static constexpr std::array<const char*, 166U> key_names =
{
	"",
	"mouse 1", "mouse 2", "cancel", "mouse 3", "mouse 4", "mouse 5", "",
	"backspace", "tab", "", "", "clear", "enter", "", "",
	"shift", "control", "alt", "pause", "caps", "", "", "", "", "", "",
	"escape", "", "", "", "", "space", "page up", "page down",
	"end", "home", "left", "up", "right", "down", "", "", "",
	"print", "insert", "delete", "",
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	"", "", "", "", "", "", "",
	"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
	"l", "m", "n", "o", "p", "q", "r", "s", "t", "u",
	"v", "w", "x", "y", "z", "lwin", "rwin", "", "", "",
	"num0", "num1", "num2", "num3", "num4", "num5",
	"num6", "num7", "num8", "num9",
	"*", "+", "", "-", ".", "/",
	"f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8",
	"f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16",
	"f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24",
	"", "", "", "", "", "", "", "",
	"num lock", "scroll lock",
	"", "", "", "", "", "", "",
	"", "", "", "", "", "", "",
	"lshift", "rshift", "lctrl",
	"rctrl", "lmenu", "rmenu"
};


ImColor FastColorLerp( ImColor start, ImColor end, float stage )
{
    ImVec4 lerp = ImLerp(
        ImVec4( start.Value.x, start.Value.y, start.Value.z, start.Value.w ),
        ImVec4( end.Value.x, end.Value.y, end.Value.z, end.Value.w ),
        stage );

    return ImGui::ColorConvertFloat4ToU32( lerp );
}

float FastFloatLerp( std::string identifier, bool state, float min, float max, float speed ) {

    static std::unordered_map<std::string, float> valuesMapFloat;
    auto value = valuesMapFloat.find( identifier );

    if (value == valuesMapFloat.end( )) {
        valuesMapFloat.insert( { identifier, min } );
        value = valuesMapFloat.find( identifier );
    }

    const float frameRateSpeed = speed * (1.f - ImGui::GetIO( ).DeltaTime);

    if (state) {
        if (value->second < max)
            value->second += frameRateSpeed;
    }
    else {
        if (value->second > min)
            value->second -= frameRateSpeed;
    }

    value->second = std::clamp( value->second, min, max );

    return value->second;
}

bool ImGui::TabSelector( const char* label, bool selected )
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow( );

    ImGuiID id = window->GetID( label );

    ImVec2 p = window->DC.CursorPos;
    ImVec2 size( { window->Size.x, 24 } );
    ImRect bb( p, p + size );

    ItemSize( bb );
    ItemAdd( bb, id );

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held );

    float anim = FastFloatLerp( std::string( label ).append( "tab.anim" ), selected, 0.f, 1.f, 0.04f );
    auto col = FastColorLerp( GetColorU32( ImGuiCol_TextDisabled ), GetColorU32( ImGuiCol_Text ), anim );

    if (pressed)
        content_anim = 0.f;

    static float line_pos = 0.f;
    line_pos = ImLerp( line_pos, selected ? bb.Min.y - window->Pos.y : line_pos, 0.04f );

    window->DrawList->AddRectFilled( { bb.Max.x - 2, window->Pos.y + line_pos }, { bb.Max.x, window->Pos.y + line_pos + size.y }, GetColorU32( ImGuiCol_FrameBgActive, anim ), 2, ImDrawFlags_RoundCornersLeft );

    PushStyleColor( ImGuiCol_Text, col.Value );
    ImGui::PushFont( imgui_fonts::menu_icon );
    RenderText( bb.GetCenter( ) - CalcTextSize( label, 0, true ) / 2, label );
    ImGui::PopFont( );
    PopStyleColor( );

    return pressed;
}

bool ImGui::SubTabSelector( const char* label, bool selected )
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow( );

    ImGuiID id = window->GetID( label );

    ImVec2 p = window->DC.CursorPos;
    ImVec2 size( { CalcTextSize( label, 0, true ).x + 10, window->Size.y } );
    ImRect bb( p, { p.x + size.x, p.y + size.y } );

    ItemSize( bb );
    ItemAdd( bb, id );

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held );

    float anim = FastFloatLerp( std::string( label ).append( "subtab.anim" ), selected, 0.f, 1.f, 0.04f );
    auto col = FastColorLerp( GetColorU32( ImGuiCol_TextDisabled ), GetColorU32( ImGuiCol_Text ), anim );

    if (hovered || held)
        window->DrawList->AddRectFilled( { bb.Min.x, bb.Max.y - 2 }, bb.Max, GetColorU32( ImGuiCol_FrameBgHovered ), 2, ImDrawFlags_RoundCornersTop );

    window->DrawList->AddRectFilled( { bb.Min.x, bb.Max.y - 2 }, bb.Max, GetColorU32( ImGuiCol_FrameBgActive, anim ), 2, ImDrawFlags_RoundCornersTop );

    PushStyleColor( ImGuiCol_Text, col.Value );
    RenderText( bb.GetCenter( ) - CalcTextSize( label, 0, true ) / 2, label );
    PopStyleColor( );

    return pressed;
}

bool ImGui::HotKey( const char* label, key_t* value )
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* pWindow = g.CurrentWindow;

	if (pWindow->SkipItems)
		return false;

	ImGuiIO& io = g.IO;
	const ImGuiStyle& style = g.Style;
	const ImGuiID index = pWindow->GetID( label );

	const float flWidth = CalcItemWidth( );
	const ImVec2 vecLabelSize = CalcTextSize( label, nullptr, true );

	const ImRect rectFrame( pWindow->DC.CursorPos + ImVec2( vecLabelSize.x > 0.0f ? style.ItemInnerSpacing.x + GetFrameHeight( ) : 0.0f, 0.0f ), pWindow->DC.CursorPos + ImVec2( flWidth, vecLabelSize.x > 0.0f ? vecLabelSize.y + style.FramePadding.y : 0.f ) );
	const ImRect rectTotal( rectFrame.Min, rectFrame.Max );

	ItemSize( rectTotal, style.FramePadding.y );
	if (!ItemAdd( rectTotal, index, &rectFrame ))
		return false;

	const bool bHovered = ItemHoverable( rectFrame, index );
	if (bHovered)
	{
		SetHoveredID( index );
		g.MouseCursor = ImGuiMouseCursor_TextInput;
	}

	const bool bClicked = bHovered && io.MouseClicked[ 0 ];
	const bool bDoubleClicked = bHovered && g.IO.MouseDoubleClicked[ 0 ];
	if (bClicked || bDoubleClicked)
	{
		if (g.ActiveId != index)
		{
			memset( io.MouseDown, 0, sizeof( io.MouseDown ) );
			memset( io.KeysDown, 0, sizeof( io.KeysDown ) );
			value->key = 0;
		}

		SetActiveID( index, pWindow );
		FocusWindow( pWindow );
	}

	bool bValueChanged = false;
	if (int nKey = value->key; g.ActiveId == index)
	{
		for (int n = 0; n < IM_ARRAYSIZE( io.MouseDown ); n++)
		{
			if (IsMouseDown( n ))
			{
				switch (n)
				{
				case 0:
					nKey = VK_LBUTTON;
					break;
				case 1:
					nKey = VK_RBUTTON;
					break;
				case 2:
					nKey = VK_MBUTTON;
					break;
				case 3:
					nKey = VK_XBUTTON1;
					break;
				case 4:
					nKey = VK_XBUTTON2;
					break;
				}

				bValueChanged = true;
				ClearActiveID( );
			}
		}

		if (!bValueChanged)
		{
			for (int n = VK_BACK; n <= VK_RMENU; n++)
			{
				if (ipt::is_key_down( n ))
				{
					nKey = n;
					bValueChanged = true;
					ClearActiveID( );
				}
			}
		}

		if (ipt::is_key_released( VK_ESCAPE ))
		{
			value->key = 0;
			ClearActiveID( );
		}
		else
			value->key = nKey;
	}

	OpenPopupOnItemClick( "state_popup", ImGuiPopupFlags_MouseButtonRight );

	if (BeginPopup( "state_popup" ) )
	{
		if (ImGui::Selectable( "hold", value->key_state == key_state_hold ))
			value->key_state = key_state_hold;

		if (ImGui::Selectable( "toggle", value->key_state == key_state_toggle ))
			value->key_state = key_state_toggle;

		if (ImGui::Selectable( "always-on", value->key_state == key_state_on ))
			value->key_state = key_state_on;
		EndPopup( );
	}


	char chBuffer[ 64 ] = {};
	sprintf_s( chBuffer, sizeof( chBuffer ), "[ % s ]", value->key_state == key_state_on ? "on" : value->key != 0 && g.ActiveId != index ? key_names.at( value->key ) : g.ActiveId == index ? "press" : "none" );

	PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( style.FramePadding.x, -1 ) );
	pWindow->DrawList->AddText( ImVec2( rectFrame.Max.x - CalcTextSize( chBuffer ).x, rectTotal.Min.y + style.FramePadding.y ), GetColorU32( g.ActiveId == index ? ImGuiCol_Text : ImGuiCol_TextDisabled ), chBuffer );

	if (vecLabelSize.x > 0.f)
		RenderText( ImVec2( rectTotal.Min.x, rectTotal.Min.y + style.FramePadding.y ), label );

	PopStyleVar( );
	return bValueChanged;
}


bool ImGui::ListBox( const char* label, int* current_item, std::function<const char* (int)> lambda, int items_count, int height_in_items )
{
	return ListBox( label, current_item, [ ]( void* pData, int index, const char** szOutText )
		{
			*szOutText = (*static_cast<std::function<const char* (int)>*>(pData))(index);
	return true;
		}, &lambda, items_count, height_in_items );
}

bool ImGui::MultiCombo( const char* label, std::vector<bool>& values, const std::string_view* arrItems, int items_count )
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* pWindow = g.CurrentWindow;

	if (pWindow->SkipItems)
		return false;

	const ImGuiStyle& style = g.Style;
	const ImVec2 vecLabelSize = CalcTextSize( label, nullptr, true );
	const float flActiveWidth = CalcItemWidth( ) - (vecLabelSize.x > 0.0f ? style.ItemInnerSpacing.x + GetFrameHeight( ) : 0.0f);

	std::vector<std::string_view> vecActiveItems = { };

	// collect active items
	for (int i = 0; i < items_count; i++)
	{
		if (values[ i ])
			vecActiveItems.push_back( arrItems[ i ] );
	}

	// fuck it, stl still haven't boost::join, fmt::join replacement
	std::string szBuffer = { };
	for (std::size_t i = 0U; i < vecActiveItems.size( ); i++)
	{
		szBuffer.append( vecActiveItems[ i ] );

		if (i < vecActiveItems.size( ) - 1U)
			szBuffer.append( ", " );
	}

	if (szBuffer.empty( ))
		szBuffer.assign( "none" );
	else
	{
		const char* szWrapPosition = g.Font->CalcWordWrapPositionA( GetCurrentWindow( )->FontWindowScale, &szBuffer[ 0 ], szBuffer.data( ) + szBuffer.length( ), flActiveWidth - style.FramePadding.x * 2.0f );
		const std::size_t nWrapLength = szWrapPosition - &szBuffer[ 0 ];

		if (nWrapLength > 0U && nWrapLength < szBuffer.length( ))
		{
			szBuffer.resize( nWrapLength );
			szBuffer.append( "..." );
		}
	}

	bool bValueChanged = false;
	if (BeginCombo( label, szBuffer.c_str( ) ))
	{
		for (int i = 0; i < items_count; i++)
		{
			if (Selectable( arrItems[ i ].data( ), values[ i ], ImGuiSelectableFlags_DontClosePopups ))
			{
				values[ i ] = !values[ i ];
				bValueChanged = true;
			}
		}

		EndCombo( );
	}

	return bValueChanged;
}

bool ImGui::Combo( const char* label, std::vector<int>& values, int index, const char* items_separated_by_zeros, int height_in_items )
{
	int current_value = values[ index ];
	const bool value_changed = Combo( label, &current_value, items_separated_by_zeros, height_in_items );

	if ( value_changed )
		values[ index ] = current_value;

	return value_changed;
}

bool ImGui::Checkbox( const char* label, std::vector<bool>& values, int index )
{
	bool current_value = values[ index ];
	const bool value_changed = Checkbox( label, &current_value );

	if ( value_changed )
		values[ index ] = current_value;

	return value_changed;
}

bool ImGui::SliderFloat( const char* label, std::vector<float>& values, int index, float min, float max, const char* format )
{
	float current_value = values[ index ];
	const bool value_changed = SliderFloat( label, &current_value, min, max, format );

	if ( value_changed )
		values[ index ] = current_value;

	return value_changed;
}

bool ImGui::SliderInt( const char* label, std::vector<int>& values, int index, int min, int max, const char* format )
{
	int current_value = values[ index ];
	const bool value_changed = SliderInt( label, &current_value, min, max, format );

	if ( value_changed )
		values[ index ] = current_value;

	return value_changed;
}

bool ImGui::ColorPicker( const char* label, col_t* clr, bool alpha )
{
	auto flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_DisplayHex;
	auto vec = clr->get_vec4( );

	if ( ColorEdit4( label, &vec.x, flags | ( alpha ? ImGuiColorEditFlags_AlphaBar : ImGuiColorEditFlags_NoAlpha ) ) )
	{
		*clr = col_t( vec.x, vec.y, vec.z, vec.w );

		return true;
	}

	return false;
}

bool ImGui::ColorPicker( const char* label, std::vector<col_t>& clrs, int index, bool alpha )
{
	col_t clr = clrs[ index ];
	const bool value_changed = ColorPicker( label, &clr, alpha );

	if ( value_changed )
		clrs[ index ] = clr;

	return value_changed;
}

void ImGui::HoveredTooltip( const char* description )
{
	if ( IsItemHovered( ) )
	{
		BeginTooltip( );
		PushTextWrapPos( 450.f );
		TextUnformatted( description );
		PopTextWrapPos( );
		EndTooltip( );
	}
}