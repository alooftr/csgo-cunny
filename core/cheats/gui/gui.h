#pragma once
#include "extra_imgui.h"

class c_tab
{
public:
    const char* name = nullptr;
    std::function<void( )> render_function = nullptr;
};

namespace tabs
{
    template <std::size_t s> // not a best way, but we stay using cheaper, fixed-sized arrays instead dynamic vectors
    void render( const char* tab_name, const std::array<c_tab, s> tabs, int* current_tab );

    template <std::size_t s> // not a best way, but we stay using cheaper, fixed-sized arrays instead dynamic vectors
    void render_sub_tab( const char* subtab_name, const std::array<c_tab, s> tabs, int* current_tab );

    template <std::size_t s> // not a best way, but we stay using cheaper, fixed-sized arrays instead dynamic vectors
    void render_inner_sub_tab( const char* inner_subtab_name, const std::array<c_tab, s> tabs, int* current_tab );

    void rage_bot( );
    void anti_aim( );
    void visuals( );
    void miscellaneous( );
    void players( );
    void lua( );

    void update_accent( ImGuiStyle& style, col_t& accent );

    inline int selected_config = 0;
    inline std::string config_file = "";

    // for esp preview
    inline int current_player_sub_tabs = 0;
}

namespace windows
{
    void menu( );
    void esp_preview( ImDrawList* draw );

    // make it a global value
    inline ImVec2 menu_pos = ImVec2( );
    inline ImVec2 menu_size = ImVec2( );

    inline bool is_menu_opened = false;
    inline int current_tab = 4;
}
