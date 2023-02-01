#pragma once

#include <cwctype>
#include <string>
#include <vector>
#include <algorithm>

namespace parser
{
    struct PaintKit
    {
        PaintKit( int id, const std::string& name, int rarity = 0 ) noexcept;
        PaintKit( int id, std::string&& name, int rarity = 0 ) noexcept;
        PaintKit( int id, std::wstring&& name, std::string&& icon, int rarity = 0 ) noexcept;
        PaintKit( int id, std::wstring&& name, int rarity = 0 ) noexcept;

        int id;
        int rarity;
        std::string name;
        std::wstring name_upper_case;
        std::string icon;

        auto operator<( const PaintKit& other ) const noexcept
        {
            return name_upper_case < other.name_upper_case;
        }
    };

    void setup_kits( );

    inline std::vector<PaintKit> skin_kits{ { 0, "default" } };
    inline std::vector<PaintKit> glove_kits;
}