// _ReturnAddress
#include <intrin.h>

#include "hooks.hpp"
// setup imgui font...
#include "../draw/draw.h"
// setup surface font...
#include "../draw/surface.h"
// input
#include "../../utils/input.h"
// gui menu
#include "../gui/gui.h"

// including features
#include "../features/prediction/prediction.h"
#include "../features/movement/move.hpp"
#include "../features/spotify/spotify.h"
#include "../features/skins/skins.h"
#include "../features/visuals/visuals.h"
#include "../features/preview_model/preview_model.h"

bool hooks::setup( )
{
    try
    {
        if (MH_Initialize( ) != MH_OK)
            throw std::runtime_error( "failed to initialize minhooks" );

        if (!detour::end_scene.create( utils::get_vfunc( csgo::direct_device, vtable::end_scene ), &hk_end_scene ))
            return false;

        if (!detour::reset.create( utils::get_vfunc( csgo::direct_device, vtable::reset ), &hk_reset ))
            return false;

        if (!detour::lock_cursor.create( utils::get_vfunc( csgo::surface, vtable::lock_cursor ), &hk_lock_cursor ))
            return false;

        if (!detour::paint_traverse.create( utils::get_vfunc( csgo::panel, vtable::paint_traverse ), &hk_paint_traverse ))
            return false;

        if (!detour::alloc_key_values_memory.create( utils::get_vfunc( csgo::key_values_system, vtable::alloc_key_values_memory ), &hk_alloc_key_values_memory ))
            return false;

        if (!detour::create_move_proxy.create( utils::get_vfunc( csgo::client, vtable::create_move_proxy ), &hk_create_move_proxy ))
            return false;

        if (!detour::override_view.create( utils::get_vfunc( csgo::client_mode, vtable::override_view ), &hk_override_view ))
            return false;

        if (!detour::frame_stage_notify.create( utils::get_vfunc( csgo::client, vtable::frame_stage_notify ), &hk_frame_stage_notify ))
            return false;
    }
    catch (const std::exception& ex)
    {
        // print error message
        csl::push_color( FOREGROUND_INTENSE_RED );
        csl::print( "[error] {}", ex.what( ) );
        csl::pop_color( );

#ifdef _DEBUG
        // show error message window (or replace to your exception handler)
        _RPT0( _CRT_ERROR, ex.what( ) );
#endif
    }

    return true;
}

void hooks::detach( )
{
    windows::is_menu_opened = false;
    std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
    // uninstall minhook
    MH_DisableHook( MH_ALL_HOOKS );
    MH_RemoveHook( MH_ALL_HOOKS );

    MH_Uninitialize( );

    // restore the wndproc
    ipt::restore( );
    
    std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
    // reset our skins
    c_skin::get( ).set_update_state( true );
    c_skin::get( ).force_update_skin( globals::local_player );
    csgo::client_state->delta_tick = -1;
}

long D3DAPI hooks::hk_end_scene( IDirect3DDevice9* device )
{
    static auto o_end_scene = detour::end_scene.get_original<decltype(&hk_end_scene)>( );
    static void* used_address = nullptr;

    try {
        // anti obs
        if (used_address == nullptr)
        {
            // search for gameoverlay address
            MEMORY_BASIC_INFORMATION info;
            VirtualQuery( _ReturnAddress( ), &info, sizeof( MEMORY_BASIC_INFORMATION ) );

            char module_name[ MAX_PATH ];
            GetModuleFileName( static_cast<HMODULE>(info.AllocationBase), module_name, MAX_PATH );
            
            // check if its the module we need
            if (strstr( module_name, "gameoverlayrenderer.dll" ) != nullptr)
                used_address = _ReturnAddress( );
        }

        // if we currently at gameoverlayrenderer address
        if (_ReturnAddress( ) == used_address)
        {
            // setup the imgui first
            if (!draw::did_setup)
            {
                draw::setup( device );
                c_surface::get( ).setup_font( );
                tabs::update_accent( ImGui::GetStyle( ), cfg_get( col_t, vars.menu_accent ) );
            }

            // main imgui frame
            ImGui_ImplDX9_NewFrame( );
            ImGui_ImplWin32_NewFrame( );
            ImGui::NewFrame( );
            {
                // draw the threat safe data
                auto background_draw_list = ImGui::GetBackgroundDrawList( );
                background_draw_list->PushClipRectFullScreen( );
                draw::render_draw_data( background_draw_list );
                background_draw_list->PopClipRect( );

                windows::menu( );
            }
            ImGui::EndFrame( );
            ImGui::Render( );

            // render draw lists from draw data
            ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );
        }
    }
    catch (const std::exception& ex) {
        csl::push_color( FOREGROUND_INTENSE_RED );
        csl::print( "[error] {}", ex.what( ) );
        csl::pop_color( );

        // show errors box
#ifdef _DEBUG
        _RPT0( _CRT_ERROR, ex.what( ) );
#endif // _DEBUG
    }

    return o_end_scene( device );
}

#pragma region window_hooks
long D3DAPI hooks::hk_reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters )
{
    static auto o_reset = detour::reset.get_original<decltype(&hk_reset)>( );

    // check for first initialization
    if (!draw::did_setup)
        return o_reset( device, presentation_parameters );

    // invalidate vertex & index buffer, release fonts texture
    ImGui_ImplDX9_InvalidateDeviceObjects( );
    draw::clear_blur( );
    c_skin::get( ).clear_item_icon_textures( );

    const HRESULT rst = o_reset( device, presentation_parameters );

    // get directx device and create fonts texture
    if (rst == D3D_OK)
    {
        ImGui_ImplDX9_CreateDeviceObjects( );
        c_surface::get( ).on_reset( );
    }

    return rst;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );
LRESULT __stdcall hooks::hk_wnd_proc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
    ipt::process( msg, wparam, lparam );

    if (ipt::is_key_released( cfg_get( key_t, vars.menu_open_key ).key))
        windows::is_menu_opened = !windows::is_menu_opened;

    if (ImGui_ImplWin32_WndProcHandler( hwnd, msg, wparam, lparam ))
        return 1L;

    return CallWindowProcW( ipt::old_wnd_proc, hwnd, msg, wparam, lparam );
}
#pragma endregion

void __fastcall hooks::hk_lock_cursor( i_surface* thisptr, int edx )
{
    static auto o_lock_cursor = detour::lock_cursor.get_original<decltype(&hk_lock_cursor)>( );

    if (windows::is_menu_opened)
    {
        // return and unlock the cursor
        return csgo::surface->unlock_cursor( );
    }

    o_lock_cursor( thisptr, edx );
}

void __fastcall hooks::hk_paint_traverse( i_surface* thisptr, int edx, unsigned int panel, bool force_repaint, bool force )
{
    static auto o_paint_traverse = detour::paint_traverse.get_original<decltype(&hk_paint_traverse)>( );
    const fnv1a_t panel_hash = fnv1a::hash( csgo::panel->get_name( panel ) );

    o_paint_traverse( thisptr, edx, panel, force_repaint, force );

    switch (panel_hash)
    {
    case fnv1a::hash_const( "FocusOverlayPanel" ): 
    {
        if (draw::did_setup)
        {
            csgo::panel->set_mouse_input_enabled( panel, windows::is_menu_opened );
            csgo::panel->set_key_board_input_enabled( panel, ImGui::GetIO( ).WantCaptureKeyboard );
        }
        break;
    }
    case fnv1a::hash_const( "MatSystemTopPanel" ): 
    {
        // clear it
        draw::clear_draw_data( );
        {
            // check if we did setup it
            if (draw::did_setup)
            {
                auto local_player = c_base_entity::get_local_player( );

                c_visuals::get( ).run( local_player );
            }
        }
        // swap it here
        draw::swap_draw_data( );

        if ( windows::is_menu_opened && windows::current_tab == 2 )
            preview_model->on_paint( );
        break;
    }
    }
}

void* __fastcall hooks::hk_alloc_key_values_memory( i_key_values_system* thisptr, int edx, int size )
{
    static auto o_alloc_key_values_memory = detour::alloc_key_values_memory.get_original<decltype(&hk_alloc_key_values_memory)>( );

    // return addresses of check function
    // @credits: danielkrupinski
    static const std::uintptr_t alloc_key_values_engine = utils::get_absolute_address( utils::find_pattern( "engine.dll", "E8 ? ? ? ? 83 C4 08 84 C0 75 10 FF 75 0C" ) + 0x1 ) + 0x4A;
    static const std::uintptr_t alloc_key_values_client = utils::get_absolute_address( utils::find_pattern( "client.dll", "E8 ? ? ? ? 83 C4 08 84 C0 75 10" ) + 0x1 ) + 0x3E;

    if (const std::uintptr_t return_address = reinterpret_cast<std::uintptr_t>(_ReturnAddress( )); return_address == alloc_key_values_engine || return_address == alloc_key_values_client)
        return nullptr;

    return o_alloc_key_values_memory( thisptr, edx, size );
}

static void __stdcall create_move( int sequenceNumber, float input_sample_frametime, bool is_active, bool& send_packet )
{
    static auto o_create_move = detour::create_move_proxy.get_original<decltype(&hooks::hk_create_move_proxy)>( );

    o_create_move( csgo::client, 0, sequenceNumber, input_sample_frametime, is_active );

    c_user_cmd* cmd = globals::cmd = csgo::input->get_user_cmd( sequenceNumber );
    c_verified_user_cmd* verified_cmd = csgo::input->get_verified_cmd( sequenceNumber );

    // check do we have valid commands, finished signing on to server and not playing back demos (where our commands are ignored)
    if (cmd == nullptr || verified_cmd == nullptr || !is_active)
        return;

    c_base_entity* local_player = globals::local_player = c_base_entity::get_local_player( );

    if (!local_player)
        return;

    // netchannel pointer
    i_net_channel* net_channel = csgo::client_state->net_channel;
    vec3_t old_view_point = cmd->view_angle;
    const vec3_t current_view_point = cmd->view_angle;
    const c_user_cmd current_cmd{ *cmd };

    try
    {
        c_movement::get( ).pre_prediction( cmd, local_player );

        c_prediction::get( ).start( cmd, local_player );
        {

        }
        c_prediction::get( ).end( cmd, local_player );

        c_movement::get( ).pos_prediction( cmd );

        if (local_player->is_alive( ))
            c_movement::get( ).movement_correction( cmd, cmd->view_angle, old_view_point, local_player );

        if (cfg_get( bool, vars.misc_anti_untrusted ))
        {
            cmd->view_angle.normalize( );
            cmd->view_angle.clamp( );
        }

        globals::send_packet = send_packet;
    }
    catch (const std::exception& ex) {
        csl::push_color( FOREGROUND_INTENSE_RED );
        csl::print( "[error] {}", ex.what( ) );
        csl::pop_color( );

        _RPT0( _CRT_ERROR, ex.what( ) );
    }

    verified_cmd->user_cmd = *cmd;
    verified_cmd->hash_crc = cmd->get_checksum( );
}

__declspec(naked) void __fastcall hooks::hk_create_move_proxy( [[maybe_unused]] i_base_client_dll* thisptr, [[maybe_unused]] int edx, [[maybe_unused]] int sequence_number, [[maybe_unused]] float input_sample_frametime, [[maybe_unused]] bool is_active )
{
    __asm
    {
        push	ebp
        mov		ebp, esp; // store the stack
        push	ebx; // bSendPacket
        push	esp; // restore the stack
        push	dword ptr[ is_active ]; // ebp + 16
        push	dword ptr[ input_sample_frametime ]; // ebp + 12
        push	dword ptr[ sequence_number ]; // ebp + 8
        call	create_move
            pop		ebx
            pop		ebp
            retn	0Ch
    }
}

void __fastcall	hooks::hk_override_view( i_client_mode_shared* thisptr, int edx, c_view_setup* view_setup )
{
    static auto o_override_view = detour::override_view.get_original<decltype(&hk_override_view)>( );

    if (!csgo::engine->is_in_game( ) || !csgo::engine->is_connected( ))
        return o_override_view( thisptr, edx, view_setup );

    // get camera origin
    globals::camera = view_setup->origin;

    o_override_view( thisptr, edx, view_setup );
}

void __fastcall	hooks::hk_frame_stage_notify( i_base_client_dll* thisptr, int edx, e_client_frame_stage stage )
{
    static auto o_frame_stage_notify = detour::frame_stage_notify.get_original<decltype(&hk_frame_stage_notify)>( );

    if (!csgo::engine->is_in_game( ) || !csgo::engine->is_connected( ))
    {
        return o_frame_stage_notify( thisptr, edx, stage );
    }

    c_base_entity* local_player = c_base_entity::get_local_player( );

    if (local_player == nullptr)
        return o_frame_stage_notify( thisptr, edx, stage );

    static vec3_t old_aim_punch = { }, old_view_punch = { };

    switch (stage)
    {
    case frame_net_update_postdataupdate_start:
    {
        /*
         * data has been received and we are going to start calling postdataupdate
         * e.g. resolver or skinchanger and other visuals
         */

        c_skin::get( ).glove_changer( local_player );
        c_skin::get( ).run( local_player );
        c_skin::get( ).force_update_skin( local_player );

        break;
    }
    case frame_net_update_postdataupdate_end:
    {
        /*
         * data has been received and called postdataupdate on all data recipients
         * e.g. now we can modify interpolation, other lagcompensation stuff
         */

        break;
    }
    case frame_net_update_end:
    {
        /*
         * received all packets, now do interpolation, prediction, etc
         * e.g. backtrack stuff
         */

    }
    case frame_render_start:
    {
        /*
         * start rendering the scene
         * e.g. remove visual punch, thirdperson, other render/update stuff
         */


        break;
    }
    case frame_render_end:
    {
        /*
         * finished rendering the scene
         * here we can restore our modified things
         */

        break;
    }
    default:
        break;
    }

    o_frame_stage_notify( thisptr, edx, stage );
}

bool props_hook::setup( )
{
    recv_prop_t* sequence_ptr = c_netvar_manager::get( ).map_props[ fnv1a::hash_const( "CBaseViewModel->m_nSequence" ) ].recv_prop;
    if (!sequence_ptr)
        throw std::runtime_error( _( "failed to hook CBaseViewModel->m_nSequence" ) );

    props::sequence = std::make_shared<c_recv_prop_hook>( sequence_ptr, hk_sequence );

    return true;
}

void props_hook::detach( )
{
    props::sequence->restore( );
}

void props_hook::hk_sequence( const c_recv_proxy_data * data, void* struct_ptr, void* out )
{
    static auto o_sequence = props::sequence->get_original( );

    if (!csgo::engine->is_connected( ) || !csgo::engine->is_in_game( ))
        return o_sequence( data, struct_ptr, out );

    if (!globals::local_player || globals::local_player->get_health( ) == 0)
        return o_sequence( data, struct_ptr, out );

    if (cfg_get( int, vars.skin_knife_model ) == 0)
        return o_sequence( data, struct_ptr, out );

    c_base_view_model* view_model = static_cast<c_base_view_model*>(struct_ptr);
    if (view_model && csgo::entity_list->get( view_model->get_owner_handle( ) ) != nullptr )
    {
        c_recv_proxy_data* proxy_data = const_cast<c_recv_proxy_data*>(data);

        if (auto wpn = globals::local_player->get_active_weapon( ); wpn != nullptr)
        {
            if (wpn->get_client_class( )->class_id == e_class_index::cknife)
            {
                auto knife_model = csgo::model_info->get_model( view_model->get_model_index( ) );
                auto knife_name = csgo::model_info->get_model_name( knife_model );

                if (c_skin::get( ).animation_fix_map.count( knife_name ))
                    proxy_data->value.Int = c_skin::get( ).animation_fix_map.at( knife_name )(proxy_data->value.Int);
            }
        }
    }

    o_sequence( data, struct_ptr, out );
}
