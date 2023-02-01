// use assert
#include <assert.h>

#include "utils.h"

#include "../core/sdk/sdk.hpp"

std::uintptr_t utils::find_pattern( const std::string_view module_name, const std::string_view pattern )
{
    void* module_base = GetModuleHandleA( module_name.data( ) );

    if (module_base == nullptr)
        throw std::runtime_error( std::vformat( "Failed to get handle for: {}", std::make_format_args( module_name ) ) );

    const std::uint8_t* module_address = static_cast<std::uint8_t*>(module_base);
    const IMAGE_DOS_HEADER* dos_header = static_cast<IMAGE_DOS_HEADER*>(module_base);
    const IMAGE_NT_HEADERS* nt_headers = reinterpret_cast<const IMAGE_NT_HEADERS*>(module_address + dos_header->e_lfanew);

    return find_pattern( module_address, nt_headers->OptionalHeader.SizeOfImage, pattern );
}


std::vector<std::optional<std::uint8_t>> pattern_to_bytes( const std::string_view pattern )
{
    std::vector<std::optional<std::uint8_t>> bytes = { };
    auto begin = pattern.cbegin( );
    const auto end = pattern.cend( );

    // convert pattern into bytes
    while (begin < end)
    {
        // check is current byte a wildcard
        if (*begin == '?')
        {
            // check is two-character wildcard
            if (++begin; *begin == '?')
                ++begin;

            // ignore that
            bytes.emplace_back( std::nullopt );
        }
        // check is not space
        else if (*begin != ' ')
        {
            // convert current 4 bits to hex
            std::uint8_t byte = static_cast<std::uint8_t>(((*begin >= 'A' ? (((*begin - 'A') & (~('a' ^ 'A'))) + 10) : (*begin <= '9' ? *begin - '0' : 0x0)) | 0xF0) << 4);

            // convert next 4 bits to hex and assign to byte
            if (++begin; *begin != ' ')
                byte |= static_cast<std::uint8_t>(*begin >= 'A' ? (((*begin - 'A') & (~('a' ^ 'A'))) + 10) : (*begin <= '9' ? *begin - '0' : 0x0));

            bytes.emplace_back( byte );
        }

        ++begin;
    }

    return bytes;
}

std::uintptr_t utils::find_pattern( const std::uint8_t* region_start, const std::uintptr_t region_size, const std::string_view pattern )
{
    const std::vector<std::optional<std::uint8_t>> bytes = pattern_to_bytes( pattern );

    // check for bytes sequence match
    for (std::uintptr_t i = 0U; i < region_size - bytes.size( ); ++i)
    {
        bool found = true;

        for (std::uintptr_t s = 0U; s < bytes.size( ); ++s)
        {
            // compare byte and skip if wildcard
            if (bytes[ s ].has_value( ) && region_start[ i + s ] != bytes[ s ].value( ))
            {
                found = false;
                break;
            }
        }

        // return valid address
        if (found)
            return reinterpret_cast<std::uintptr_t>(&region_start[ i ]);
    }

    csl::push_color( FOREGROUND_INTENSE_RED );
    csl::print( "pattern not found: [{}]", pattern );
    csl::pop_color( );
    return 0U;
}

std::uintptr_t* utils::find_hud_element( const char* name )
{
    // @note: https://www.unknowncheats.me/forum/counterstrike-global-offensive/342743-finding-sigging-chud-pointer-chud-findelement.html

    static auto hud = *reinterpret_cast<void**>(utils::find_pattern( "client.dll", "B9 ? ? ? ? 68 ? ? ? ? E8 ? ? ? ? 89" ) + 0x1); // @xref: "CHudWeaponSelection"

    using find_hud_element_fn = std::uintptr_t* (__thiscall*)(void*, const char*);
    static auto o_find_hud_element = reinterpret_cast<find_hud_element_fn>(utils::find_pattern( "client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28" )); // @xref: "[%d] Could not find Hud Element: %s\n"
    assert( o_find_hud_element != nullptr );

    return o_find_hud_element( hud, name );
}

bool utils::line_goes_through_smoke( const vec3_t& start, const vec3_t& end, const bool grenade_bloat )
{
    using line_goes_through_smoke_fn = bool( __cdecl* )(vec3_t, vec3_t, bool);
    static auto o_line_goes_through_smoke = reinterpret_cast<line_goes_through_smoke_fn>(utils::find_pattern( "client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0" )); // @xref: "effects/overlaysmoke"
    assert( o_line_goes_through_smoke != nullptr );

    return o_line_goes_through_smoke( start, end, grenade_bloat );
}

LPDIRECT3DTEXTURE9 utils::get_steam_avatar( std::uint64_t steam_id )
{
    // to return it later
    LPDIRECT3DTEXTURE9 temp{};

    int img = csgo::steam_friend->get_large_friend_avatar( steam_id );
    if (img == -1)
        return nullptr;

    unsigned int avatar_width, avatar_height;
    if (!csgo::steam_utils->get_image_size( img, &avatar_width, &avatar_height ))
        return nullptr;

    const int image_size_in_bytes = avatar_width * avatar_height * 4u;
    unsigned char* avatar_rgba = new unsigned char[ image_size_in_bytes ];
    if (!csgo::steam_utils->get_image_rgba( img, avatar_rgba, image_size_in_bytes ))
    {
        delete[ ] avatar_rgba;
        return nullptr;
    }

    auto res = csgo::direct_device->CreateTexture( avatar_width, avatar_height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &temp, nullptr );

    std::vector<uint8_t> tex_data;
    tex_data.resize( image_size_in_bytes );

    copy_convert( avatar_rgba, tex_data.data( ), image_size_in_bytes );
    D3DLOCKED_RECT rect;
    res = temp->LockRect( 0, &rect, nullptr, D3DLOCK_DISCARD );
    auto src = tex_data.data( );
    auto dst = reinterpret_cast<uint8_t*>(rect.pBits);
    for (auto y = 0u; y < avatar_height; ++y)
    {
        std::copy( src, src + (avatar_width * 4), dst );

        src += avatar_width * 4;
        dst += rect.Pitch;
    }
    res = temp->UnlockRect( 0 );
    delete[ ] avatar_rgba;

    return temp;
}

int utils::get_fps( )
{
    return static_cast<int>( 1.f / csgo::global_vars->frame_time );
}

const char8_t* utils::get_weapon_icon( short item_definition_index )
{
	/*
	 * @note: icon code = weapon item definition index in hex
	 * list of other icons:
	 *	"E210" - kevlar
	 *	"E20E" - helmet
	 *	"E20F" - defuser kit
	 *	"E211" - banner
	 *	"E212" - target
	 */

	switch ( item_definition_index )
	{
		case weapon_deagle:
			return u8"\uE001";
		case weapon_elite:
			return u8"\uE002";
		case weapon_fiveseven:
			return u8"\uE003";
		case weapon_glock:
			return u8"\uE004";
		case weapon_ak47:
			return u8"\uE007";
		case weapon_aug:
			return u8"\uE008";
		case weapon_awp:
			return u8"\uE009";
		case weapon_famas:
			return u8"\uE00A";
		case weapon_g3sg1:
			return u8"\uE00B";
		case weapon_galilar:
			return u8"\uE00D";
		case weapon_m249:
			return u8"\uE00E";
		case weapon_m4a1:
			return u8"\uE010";
		case weapon_mac10:
			return u8"\uE011";
		case weapon_p90:
			return u8"\uE013";
		case weapon_mp5sd:
			return u8"\uE017";
		case weapon_ump45:
			return u8"\uE018";
		case weapon_xm1014:
			return u8"\uE019";
		case weapon_bizon:
			return u8"\uE01A";
		case weapon_mag7:
			return u8"\uE01B";
		case weapon_negev:
			return u8"\uE01C";
		case weapon_sawedoff:
			return u8"\uE01D";
		case weapon_tec9:
			return u8"\uE01E";
		case weapon_taser:
			return u8"\uE01F";
		case weapon_hkp2000:
			return u8"\uE020";
		case weapon_mp7:
			return u8"\uE021";
		case weapon_mp9:
			return u8"\uE022";
		case weapon_nova:
			return u8"\uE023";
		case weapon_p250:
			return u8"\uE024";
		case weapon_scar20:
			return u8"\uE026";
		case weapon_sg556:
			return u8"\uE027";
		case weapon_ssg08:
			return u8"\uE028";
		case weapon_knife:
			return u8"\uE02A";
		case weapon_flashbang:
			return u8"\uE02B";
		case weapon_hegrenade:
			return u8"\uE02C";
		case weapon_smokegrenade:
			return u8"\uE02D";
		case weapon_molotov:
			[[fallthrough]];
		case weapon_firebomb:
			return u8"\uE02E";
		case weapon_decoy:
			[[fallthrough]];
		case weapon_diversion:
			return u8"\uE02F";
		case weapon_incgrenade:
			return u8"\uE030";
		case weapon_c4:
			return u8"\uE031";
		case weapon_healthshot:
			return u8"\uE039";
		case weapon_knife_gg:
			[[fallthrough]];
		case weapon_knife_t:
			return u8"\uE03B";
		case weapon_m4a1_silencer:
			return u8"\uE03C";
		case weapon_usp_silencer:
			return u8"\uE03D";
		case weapon_cz75a:
			return u8"\uE03F";
		case weapon_revolver:
			return u8"\uE040";
		case weapon_tagrenade:
			return u8"\uE044";
		case weapon_fists:
			return u8"\uE045";
		case weapon_tablet:
			return u8"\uE048";
		case weapon_melee:
			return u8"\uE04A";
		case weapon_axe:
			return u8"\uE04B";
		case weapon_hammer:
			return u8"\uE04C";
		case weapon_spanner:
			return u8"\uE04E";
		case weapon_knife_bayonet:
			return u8"\uE1F4";
		case weapon_knife_css:
			return u8"\uE1F7";
		case weapon_knife_flip:
			return u8"\uE1F9";
		case weapon_knife_gut:
			return u8"\uE1FA";
		case weapon_knife_karambit:
			return u8"\uE1FB";
		case weapon_knife_m9_bayonet:
			return u8"\uE1FC";
		case weapon_knife_tactical:
			return u8"\uE1FD";
		case weapon_knife_falchion:
			return u8"\uE200";
		case weapon_knife_survival_bowie:
			return u8"\uE202";
		case weapon_knife_butterfly:
			return u8"\uE203";
		case weapon_knife_push:
			return u8"\uE204";
		case weapon_knife_cord:
			return u8"\uE205";
		case weapon_knife_canis:
			return u8"\uE206";
		case weapon_knife_ursus:
			return u8"\uE207";
		case weapon_knife_gypsy_jackknife:
			return u8"\uE208";
		case weapon_knife_outdoor:
			return u8"\uE209";
		case weapon_knife_stiletto:
			return u8"\uE20A";
		case weapon_knife_widowmaker:
			return u8"\uE20B";
		case weapon_knife_skeleton:
			return u8"\uE20D";
		default:
			return u8"\u003F";
	}
}