#pragma once
#include <functional>

#include "../draw/draw.h"

// custom type
#include "../../utils/keybind.h"
#include "../../sdk/data_types/color.hpp"
#include "../cfg/extra_cfg_struct.h"

namespace ImGui
{
	// custom
    bool TabSelector( const char* label, bool selected );
    bool SubTabSelector( const char* label, bool selected );

    bool HotKey( const char* label, key_t* value );
    bool ListBox( const char* label, int* current_item, std::function<const char* (int)> lambda, int items_count, int height_in_items );
    bool MultiCombo( const char* label, std::vector<bool>& values, const std::string_view* arrItems, int items_count );

	// wrapper for vector values
	bool Combo( const char* label, std::vector<int>& values, int index, const char* items_separated_by_zeros, int height_in_items );
	bool Checkbox( const char* label, std::vector<bool>& values, int index );

	bool SliderFloat( const char* label, std::vector<float>& values, int index, float min, float max, const char* format );
	bool SliderInt( const char* label, std::vector<int>& values, int index, int min, int max, const char* format );

	// custom colorpicker with col_t struct
	bool ColorPicker( const char* label, col_t* clr, bool alpha = false );
	bool ColorPicker( const char* label, std::vector<col_t>& clrs, int index, bool alpha = false );

	// custom hovered tootltip
	void HoveredTooltip( const char* description );
}
