#include "parser.h"
#include "../../../sdk/sdk.hpp"

parser::PaintKit::PaintKit( int id, const std::string& name, int rarity ) noexcept : id{ id }, name{ name }, rarity{ rarity }
{
    name_upper_case = utils::to_upper( utils::to_wide_string( name ) );
}

parser::PaintKit::PaintKit( int id, std::string&& name, int rarity ) noexcept : id{ id }, name{ std::move( name ) }, rarity{ rarity }
{
    name_upper_case = utils::to_upper( utils::to_wide_string( this->name ) );
}

parser::PaintKit::PaintKit( int id, std::wstring&& name, std::string&& icon, int rarity ) noexcept : id{ id }, name_upper_case{ std::move( name ) }, icon{ std::move( icon ) }, rarity{ rarity }
{
    this->name = csgo::localize->convert_unicode_to_ansi( name_upper_case.c_str( ) );
    std::transform( this->name.begin( ), this->name.end( ), this->name.begin( ),
        [ ]( unsigned char c )
        {
            return std::tolower( c );
        } );

    name_upper_case = utils::to_upper( utils::to_wide_string( this->name ) );
}

parser::PaintKit::PaintKit( int id, std::wstring&& name, int rarity ) noexcept : id{ id }, name_upper_case{ std::move( name ) }, rarity{ rarity }
{
    this->name = csgo::localize->convert_unicode_to_ansi( name_upper_case.c_str( ) );
    std::transform( this->name.begin( ), this->name.end( ), this->name.begin( ),
        [ ]( unsigned char c )
        {
            return std::tolower( c );
        } );


    name_upper_case = utils::to_upper( name_upper_case );
}

void parser::setup_kits( )
{
    const auto item_schema = csgo::item_system( )->get_item_schema( );

    struct kit_weapon_t
    {
        kit_weapon_t( int paintKit, e_item_definition_index weaponId, const char* icon ) noexcept : paintKit{ paintKit }, weaponId{ weaponId }, icon{ icon }
        {}
        int paintKit;
        e_item_definition_index weaponId;
        const char* icon;
    };

    std::vector<kit_weapon_t> kits_weapons;
    kits_weapons.reserve( item_schema->alternateIcons.numElements );

    for (const auto& node : item_schema->alternateIcons)
    {
        // https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/shared/econ/econ_item_schema.cpp#L325-L329
        if (const auto encoded = node.key; (encoded & 3) == 0)
            kits_weapons.emplace_back( int( (encoded & 0xFFFF) >> 2 ), e_item_definition_index( encoded >> 16 ), node.value.simpleName.data( ) );
    }

    std::sort( kits_weapons.begin( ), kits_weapons.end( ), [ ]( const auto& a, const auto& b )
        {
            return a.paintKit < b.paintKit;
        } );

    skin_kits.reserve( item_schema->paintKits.lastAlloc );
    glove_kits.reserve( item_schema->paintKits.lastAlloc );
    for (const auto& node : item_schema->paintKits)
    {
        const auto paintKit = node.value;

        if (paintKit->id == 0 || paintKit->id == 9001) // ignore workshop_default
            continue;

        if (paintKit->id >= 10000)
        {
            std::wstring name;
            std::string icon;

            name += csgo::localize->find_safe( paintKit->itemName.data( ) + 1 );

            if (const auto it = std::lower_bound( kits_weapons.begin( ), kits_weapons.end( ), paintKit->id, [ ]( const auto& p, auto id )
                {
                    return p.paintKit < id;
                } ); it != kits_weapons.end( ) && it->paintKit == paintKit->id)
            {
                if (const auto item_def = item_schema->get_item_definition_interface( it->weaponId ))
                {
                    name += L" | ";
                    name += csgo::localize->find_safe( item_def->get_item_base_name( ) );
                }
                icon = it->icon;
            }

                glove_kits.emplace_back( paintKit->id, std::move( name ), std::move( icon ), paintKit->rarity );
        }
        else
        {
            for (auto it = std::lower_bound( kits_weapons.begin( ), kits_weapons.end( ), paintKit->id, [ ]( const auto& p, auto id )
                {
                    return p.paintKit < id;
                } ); it != kits_weapons.end( ) && it->paintKit == paintKit->id; ++it)
            {

                const auto item_def = item_schema->get_item_definition_interface( it->weaponId );
                if (!item_def)
                    continue;

                std::wstring name = csgo::localize->find_safe( paintKit->itemName.data( ) + 1 );
                name += L" | ";
                name += csgo::localize->find_safe( item_def->get_item_base_name( ) );
                

                skin_kits.emplace_back( paintKit->id, std::move( name ), it->icon, std::clamp( item_def->get_rarity( ) + paintKit->rarity - 1, 0, (paintKit->rarity == 7)?7:6 ) );
            }
        }
    }

    std::sort( skin_kits.begin( ) + 1, skin_kits.end( ) );
    skin_kits.shrink_to_fit( );
    csl::print( "found {} skin kits", skin_kits.size( ) );

    std::sort( glove_kits.begin( ), glove_kits.end( ) );
    glove_kits.shrink_to_fit( );
    csl::print( "found {} glove kits", glove_kits.size( ) );
}