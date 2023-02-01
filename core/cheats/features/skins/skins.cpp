#include "skins.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../../../ext/imgui/imstb_image.h"

#include "../../cfg/var.h"
#include "../../../utils/math.h"

ImTextureID c_skin::get_icon_texture( const std::string& path )
{
    if (path.empty( ))
        return 0;

    if (icon_textures[ path ].get( ))
        return icon_textures[ path ].get( );

    if (icon_textures.size( ) >= 50)
        icon_textures.erase( icon_textures.begin( ) );

    if (const auto handle = csgo::file_system->open( ("resource/flash/" + path + "_large.png").c_str( ), "r", "GAME" ))
    {
        if (const auto size = csgo::file_system->size( handle ); size > 0)
        {
            const auto buffer = std::make_unique<std::uint8_t[ ]>( size );
            if (csgo::file_system->read( buffer.get( ), size, handle ) > 0)
            {
                int width, height;
                stbi_set_flip_vertically_on_load_thread( false );

                if (const auto data = stbi_load_from_memory( (const stbi_uc*)buffer.get( ), size, &width, &height, nullptr, STBI_rgb_alpha ))
                {
                    icon_textures[ path ].create( width, height, data );
                    stbi_image_free( data );
                }
                else
                {
                    assert( false );
                }
            }
        }
        csgo::file_system->close( handle );
    }
    else
    {
        assert( false );
    }

    return icon_textures[ path ].get( );
}

void c_skin::clear_item_icon_textures( )
{
	this->icon_textures.clear( );
}

std::vector<int> weapons_id = {
    weapon_ak47,
    weapon_aug,
    weapon_awp,
    weapon_cz75a,
    weapon_deagle,
    weapon_elite,
    weapon_famas,
    weapon_fiveseven,
    weapon_g3sg1,
    weapon_galilar,
    weapon_glock,
    weapon_m249,
    weapon_m4a1_silencer,
    weapon_m4a1,
    weapon_mac10,
    weapon_mag7,
    weapon_mp5sd,
    weapon_mp7,
    weapon_mp9,
    weapon_negev,
    weapon_nova,
    weapon_hkp2000,
    weapon_p250,
    weapon_p90,
    weapon_bizon,
    weapon_revolver,
    weapon_sawedoff,
    weapon_scar20,
    weapon_ssg08,
    weapon_sg556,
    weapon_tec9,
    weapon_ump45,
    weapon_usp_silencer,
    weapon_xm1014,
    weapon_knife,
    weapon_knife_t
};

int c_skin::get_config_index( short index )
{
    switch (index)
    {
    case weapon_deagle:
        return 0;
    case weapon_elite:
        return 1;
    case weapon_fiveseven:
        return 2;
    case weapon_glock:
        return 3;
    case weapon_ak47:
        return 4;
    case weapon_aug:
        return 5;
    case weapon_awp:
        return 6;
    case weapon_famas:
        return 7;
    case weapon_g3sg1:
        return 8;
    case weapon_galilar:
        return 9;
    case weapon_m249:
        return 10;
    case weapon_m4a1:
        return 11;
    case weapon_mac10:
        return 12;
    case weapon_p90:
        return 13;
    case weapon_mp5sd:
        return 14;
    case weapon_ump45:
        return 15;
    case weapon_xm1014:
        return 16;
    case weapon_bizon:
        return 17;
    case weapon_mag7:
        return 18;
    case weapon_negev:
        return 19;
    case weapon_sawedoff:
        return 20;
    case weapon_tec9:
        return 21;
    case weapon_mp7:
        return 22;
    case weapon_mp9:
        return 23;
    case weapon_nova:
        return 24;
    case weapon_p250:
        return 25;
    case weapon_scar20:
        return 26;
    case weapon_sg556:
        return 27;
    case weapon_ssg08:
        return 28;
    case weapon_knife:
        return 29;
    case weapon_usp_silencer:
        return 30;
    case weapon_cz75a:
        return 31;
    case weapon_revolver:
        return 32;
    default:
        return 0;
    }
}

c_base_combat_weapon* make_glove( int entry, int serial )
{
    static const auto create_wearable = [ ]
    {
        create_client_class_fn fn = nullptr;
        for (auto client_class = csgo::client->get_all_classes( ); client_class; client_class = client_class->next)
        {
            if (client_class->class_id == e_class_index::ceconwearable)
            {
                fn = client_class->create_fn;
                break;
            }
        }
        return fn;
    }();

    if (!create_wearable)
        return nullptr;

    if (const auto wearable = create_wearable( entry, serial ))
        return reinterpret_cast<c_base_combat_weapon*>(std::uintptr_t( wearable ) - 2 * sizeof( std::uintptr_t ));

    return nullptr;
}


void c_skin::run( c_base_entity* local_player )
{
    if (!local_player || !local_player->is_alive( ))
        return;

    if (!cfg_get( bool, vars.skin_enable ))
        return;

    auto active_wpn = local_player->get_active_weapon( );
    if (!active_wpn)
        return;

    player_info_t info;
    if (!csgo::engine->get_player_info( local_player->get_index( ), &info ))
        return;

    auto weapons_handle = local_player->get_weapons_handle( );
    for (int i = 0; weapons_handle[ i ] != INVALID_EHANDLE_INDEX; i++)
    {
        auto wpn = csgo::entity_list->get<c_base_combat_weapon>( weapons_handle[ i ] );
        if (!wpn)
            continue;

        // filter out weapons that aren't our own.
        if (wpn->get_owner_xuid_low( ) != info.xuid_low)
            continue;

        // changing our knife model
        if (wpn->get_client_class( )->class_id == e_class_index::cknife && cfg_get( int, vars.skin_knife_model ) != 0)
        {
            this->knife_changer( local_player, wpn );
        }

        // so it wont go away after we force update
        // shouldnt do this tho
        for (size_t j = 0; j < weapons_id.size( ); j++)
        {
            if (wpn->get_item_definition_index( ) == weapons_id[ j ])
            {
                if (wpn->get_client_class( )->class_id == e_class_index::cknife && cfg_get( int, vars.skin_knife_model ) != 0)
                {
                    wpn->get_item_definition_index( ) = this->knife_object[ cfg_get( int, vars.skin_knife_model ) ].id;
                    wpn->set_model_index( csgo::model_info->get_model_index( this->knife_object[ cfg_get( int, vars.skin_knife_model ) ].model_name ) );
                    wpn->get_entity_quality( ) = 3;

                    wpn->get_fallback_paint_kit( ) = parser::skin_kits.at( cfg_get_vector( int, vars.skin_selected_paint_kit, 29 ) ).id;
                    wpn->get_fallback_wear( ) = 1.0f - cfg_get_vector( float, vars.skin_selected_wear, 29 );
                    wpn->get_fallback_seed( ) = cfg_get_vector( int, vars.skin_selected_seed, 29 ) == 0 ? cfg_get_vector( int, vars.skin_selected_seed, 29 ) : math::random_int( 0, 10000 );
                    wpn->get_account_id( ) = wpn->get_owner_xuid_low( );

                    auto world_handle = wpn->get_world_model_handle( );
                    if (world_handle == INVALID_EHANDLE_INDEX)
                        continue;

                    auto world_model = csgo::entity_list->get<c_base_combat_weapon>( world_handle );
                    if (!world_model)
                        continue;

                    // shadow dagger world model need + 2 else it will just follow your origin
                    if (cfg_get( int, vars.skin_knife_model ) == 12)
                        world_model->set_model_index( csgo::model_info->get_model_index( this->knife_object.at( cfg_get( int, vars.skin_knife_model ) ).model_name ) + 2 );
                    else
                        world_model->set_model_index( csgo::model_info->get_model_index( this->knife_object.at( cfg_get( int, vars.skin_knife_model ) ).model_name ) + 1 );
                }
                else
                {
                    auto cfg_index = this->get_config_index( wpn->get_item_definition_index( ) );

                    wpn->get_fallback_paint_kit( ) = parser::skin_kits.at( cfg_get_vector( int, vars.skin_selected_paint_kit, cfg_index ) ).id;
                    wpn->get_fallback_wear( ) = 1.0f - cfg_get_vector( float, vars.skin_selected_wear, cfg_index );
                    wpn->get_fallback_seed( ) = cfg_get_vector( int, vars.skin_selected_seed, cfg_index ) == 0 ? cfg_get_vector( int, vars.skin_selected_seed, cfg_index ) : math::random_int( 0, 10000 );
                    wpn->get_account_id( ) = info.xuid_low;
                }

                wpn->get_item_id_high( ) = wpn->get_item_id_low( ) = -1;
            }
        }
    }
}

void c_skin::glove_changer( c_base_entity* local_player )
{
    if (!local_player)
        return;

    if (cfg_get( int, vars.skin_glove_model ) == 0)
        return;

    c_base_handle* wearables = local_player->get_wearables_handle( );
    if (!wearables)
        return;

    static uintptr_t glove_handle = uintptr_t( 0 );
    auto glove = csgo::entity_list->get<c_base_combat_weapon>( wearables[ 0 ] );

    if (glove)
    {
        auto our_glove = csgo::entity_list->get<c_base_combat_weapon>( glove_handle );
        if (our_glove)
        {
            wearables[ 0 ] = glove_handle;
            glove = our_glove;
        }
    }

    if (!local_player->is_alive( ) && glove)
    {
        glove->set_destroyed_on_recreate_entities( );
        glove->release( );
        return;
    }

    if (!glove)
    {
        auto entry = csgo::entity_list->get_highest_entity_index( ) + 1;
        const auto serial = rand( ) % 0x1000;
        glove = make_glove( entry, serial );   // He he
        assert( glove );
        if (glove)
        {
            glove->set_abs_origin( { 10000.f, 10000.f, 10000.f } );
            glove->is_initialized( ) = true;
            wearables[ 0 ] = entry | serial << 16;
            glove_handle = wearables[ 0 ]; // Let's store it in case we somehow lose it.
        }
    }

    if (glove)
    {
        // make glove follow us
        //csgo::fn::equip_wearable( glove, local_player );
        //local_player->get_animating_body( ) = 1;
        //// apply attributes

        player_info_t info;
        if (!csgo::engine->get_player_info( local_player->get_index( ), &info ))
            return;

        // force fallback values to be used.
        glove->get_item_id_high( ) = -1;
        glove->get_item_id_low( ) = -1;

        // Set the owner of the weapon to our lower XUID. (fixes StatTrak)
        glove->get_account_id( ) = info.xuid_low;
        glove->get_entity_quality( ) = 3;

        *reinterpret_cast<int*>(uintptr_t( glove ) + 0x64) = -1;

        glove->get_item_definition_index( ) = this->glove_object.at( cfg_get( int, vars.skin_glove_model ) ).id;

        // set paintkit
        glove->get_fallback_paint_kit( ) = parser::glove_kits.at(cfg_get( int, vars.skin_glove_paint_kit )).id;

        // set wear
        glove->get_fallback_wear( ) = 1.0f - cfg_get( float, vars.skin_glove_wear );

        // set the model
        glove->set_model_index( csgo::model_info->get_model_index( this->glove_object.at( cfg_get( int, vars.skin_glove_model ) ).model_name ) );

        glove->pre_data_update( data_update_created );
    }
}

void c_skin::knife_changer( c_base_entity* local_player, c_base_combat_weapon* wpn )
{
    if (!local_player || !local_player->is_alive( ) || !wpn)
        return;

    auto view_model = reinterpret_cast<c_base_view_model*>(csgo::entity_list->get( local_player->get_view_model_handle( ) ));
    if (!view_model)
        return;

    c_base_handle weapon_handle = view_model->get_weapon_handle( );
    if (weapon_handle == INVALID_EHANDLE_INDEX)
        return;

    auto view_model_weapon = csgo::entity_list->get<c_base_combat_weapon>( weapon_handle );
    if (view_model_weapon != wpn)
        return;

    view_model->get_model_index( ) = csgo::model_info->get_model_index( this->knife_object.at( cfg_get( int, vars.skin_knife_model ) ).model_name );
}

void c_skin::force_update_skin( c_base_entity* local_player )
{
    if (!this->update_skin)
        return;

    if (!local_player || !local_player->is_alive( ))
        return;

    //auto data = wpn->get_weapon_data( );
    //if (!data)
    //    return;

    //if (data->is_grenade( ) || wpn->get_item_definition_index( ) == weapon_taser)
    //    return;

    ////finally normal weapon
    //auto econ_item_view = wpn->get_econ_item_view( );

    //reinterpret_cast<c_weapon_cs_base*>(wpn)->is_custom_material_initialized( ) = wpn->get_fallback_paint_kit( ) <= 0;
    //reinterpret_cast<c_weapon_cs_base*>(wpn)->get_custom_materials( ).remove_all( );
    //econ_item_view->get_custom_materials( ).remove_all( );
    //
    //for (auto& elem : econ_item_view->get_visuals_data_processors( ))
    //{
    //    if (elem)
    //    {
    //        elem->release( );
    //        elem = nullptr;
    //    }
    //}
    //econ_item_view->get_visuals_data_processors( ).remove_all( );

    //wpn->post_data_update( data_update_created );
    //wpn->on_data_changed( data_update_created );

    // update hud
    using clear_hud_weapon_icon_fn = int( __thiscall* )(void*, int);
    static auto o_clear_hud_weapon_icon = reinterpret_cast<clear_hud_weapon_icon_fn>(utils::find_pattern( "client.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B" )); // @xref: "WeaponIcon--itemcount"
    assert( o_clear_hud_weapon_icon != nullptr );
    if (const auto hud_weapons = utils::find_hud_element( "CCSGO_HudWeaponSelection" ) - 0x28; hud_weapons != nullptr)
    {
        // go through all weapons
        for (std::size_t i = 0; i < *(hud_weapons + 0x20); i++)
            i = o_clear_hud_weapon_icon( hud_weapons, i );
    }

    csgo::client_state->delta_tick = -1;

    this->update_skin = false;
}

void c_skin::on_player_hurt( i_game_event* game_event )
{
    if (cfg_get( int, vars.skin_knife_model ) == 0)
        return;

    auto attack_id = csgo::engine->get_player_for_user_id( game_event->get_int( "attacker" ));
    if (!attack_id)
        return;

    auto attacker = csgo::entity_list->get<c_base_entity>( attack_id );
    if (!attacker || attacker != globals::local_player)
        return;

    if (!strcmp( game_event->get_string( "weapon" ),"knife" ) || !strcmp( game_event->get_string( "weapon" ), "knife_t" ))
        game_event->set_string( "weapon", this->knife_object.at( cfg_get( int, vars.skin_knife_model ) ).kill_icon );
}

c_skin::c_skin( )
{
    this->animation_fix_map = {
        { ("models/weapons/v_knife_butterfly.mdl"), [ ]( int sequence ) -> int
        {
            switch (sequence)
                {
                case 0:
                    return math::random_int( 0, 1 );
                case 12:
                    return math::random_int( 13, 15 );
                default:
                    return sequence + 1;
                }
        } },
        { ("models/weapons/v_knife_falchion_advanced.mdl"), [ ]( int sequence ) -> int
        {
            switch (sequence)
                {
                case 2:
                    return 1;
                case 9:
                    return math::random_int( 8, 9 );
                case 12:
                    return math::random_int( 12, 13 );
                case 0:
                case 1:
                    return sequence;
                default:
                    return sequence - 1;
                }
        } },
        { ("models/weapons/v_knife_css.mdl"), [ ]( int sequence ) -> int
        {
            switch (sequence)
            {
            case 12:
                return 15;
            default:
                return sequence;
            }
        } },
        { ("models/weapons/v_knife_push.mdl"), [ ]( int sequence ) -> int
        {
            switch (sequence)
                {
                case 2:
                    return 1;
                case 3:
                case 4:
                    return math::random_int( 2, 6 );
                case 9:
                    return math::random_int( 11, 12 );
                case 10:
                case 11:
                case 12:
                    return sequence + 3;
                case 0:
                case 1:
                    return sequence;
                default:
                    return sequence + 2;
                }
        } },
        { ("models/weapons/v_knife_survival_bowie.mdl"), [ ]( int sequence ) -> int
        {
            switch (sequence)
                {
                case 0:
                case 1:
                    return sequence;
                case 2:
                    return 1;
                default:
                    return sequence - 1;
                }
        } },
        { ("models/weapons/v_knife_ursus.mdl"), [ ]( int sequence ) -> int
        {
            switch (sequence)
                {
                case 0:
                    return math::random_int( 0, 1 );
                case 12:
                    return math::random_int( 13, 14 );
                default:
                    return sequence + 1;
                }
        } },
        { ("models/weapons/v_knife_stiletto.mdl"), [ ]( int sequence ) -> int
        {
            switch (sequence)
                {
                case 12:
                    return math::random_int( 12, 13 );
                default:
                    return sequence;
                }
        } },
        { ("models/weapons/v_knife_widowmaker.mdl"), [ ]( int sequence ) -> int
        {
            switch (sequence)
                {
                case 12:
                    return math::random_int( 14, 15 );
                default:
                    return sequence;
                }
        } },
        { ("models/weapons/v_knife_cord.mdl"), [ ]( int sequence ) -> int
        {
            switch (sequence)
                {
                case 0:
                    return math::random_int( 0, 1 );
                case 12:
                    return math::random_int( 13, 14 );
                default:
                    return sequence + 1;
                }
        } },
        { ("models/weapons/v_knife_canis.mdl"), [ ]( int sequence ) -> int
        {
                switch (sequence)
                {
                case 0:
                    return math::random_int( 0, 1 );
                case 12:
                    return math::random_int( 13, 14 );
                default:
                    return sequence + 1;
                }
        } },
        { ("models/weapons/v_knife_outdoor.mdl"), [ ]( int sequence ) -> int
        {
                switch (sequence)
                {
                case 0:
                    return math::random_int( 0, 1 );
                case 12:
                    return math::random_int( 13, 14 );
                default:
                    return sequence + 1;
                }
        } },
        { ("models/weapons/v_knife_skeleton.mdl"), [ ]( int sequence ) -> int
        {
                switch (sequence)
                {
                case 0:
                    return math::random_int( 0, 1 );
                case 12:
                    return math::random_int( 13, 14 );
                default:
                    return sequence + 1;
                }
        } }
    };
}
