#include "sdk.hpp"

#define capture_ptr(i, ptr) i = ptr; if (!i) return false; else csl::push_color(FOREGROUND_CYAN); csl::print("captured {} interface -> {:#08x}", typeid(i).name(), reinterpret_cast<std::uintptr_t>(i)); csl::pop_color();

template<typename T>
T* capture( const std::string_view moduleName, const std::string_view name )
{
	const auto get_register_list = [ &moduleName ]( ) -> c_interface_register*
	{
		void* o_create_interface = nullptr;

		if ( const auto module_handle = GetModuleHandleA( moduleName.data( ) ); module_handle != nullptr )
			o_create_interface = GetProcAddress( module_handle, "CreateInterface" );

		if ( o_create_interface == nullptr )
			throw std::runtime_error( "failed get CreateInterface address" );

		const std::uintptr_t create_interface_relative = reinterpret_cast< std::uintptr_t >( o_create_interface ) + 0x5;
		const std::uintptr_t create_interface = create_interface_relative + 4U + *reinterpret_cast< std::int32_t* >( create_interface_relative );
		return **reinterpret_cast< c_interface_register*** >( create_interface + 0x6 );
	};

	for ( auto register_list = get_register_list( ); register_list != nullptr; register_list = register_list->next )
	{
		// found needed interface
		if ( ( std::string_view( register_list->name ).compare( 0U, name.length( ), name ) == 0 &&
			// and it have digits after name
			std::atoi( register_list->name + name.length( ) ) > 0 ) ||
			// or given full name with hardcoded digits
			name.compare( register_list->name ) == 0 )
		{
			// capture our interface
			auto interface_pointer = register_list->create_fn( );

			// log interface address
			csl::push_color( FOREGROUND_CYAN );
			csl::print( "captured {} interface -> {:#08x}", register_list->name, reinterpret_cast< std::uintptr_t >( interface_pointer ) );
			csl::pop_color( );

			return static_cast< T* >( interface_pointer );
		}
	}

#ifdef _DEBUG
	csl::push_color( FOREGROUND_INTENSE_RED );
	csl::print( "failed to find interface \"{}\" in \"{}\"", name, moduleName );
	csl::pop_color( );
#endif

	return nullptr;
}


bool csgo::capture_interfaces( )
{
	try
	{
		client = capture<i_base_client_dll>( "client.dll", "VClient" );
		surface = capture<i_surface>( "vguimatsurface.dll", "VGUI_Surface" );
		panel = capture<iv_panel>( "vgui2.dll", "VGUI_Panel" );
		engine = capture<i_engine_client>( "engine.dll", "VEngineClient" );
		entity_list = capture<i_client_entity_list>( "client.dll", "VClientEntityList" );
		model_info = capture<iv_model_info>( "engine.dll", "VModelInfoClient" );
		model_render = capture<iv_model_render>( "engine.dll", "VEngineModel" );
		engine_trace = capture<i_engine_trace>( "engine.dll", "EngineTraceClient" );
		mdl_cache = capture<i_mdl_cache>( "datacache.dll", "MDLCache" );
		convar = capture<i_convar>( "vstdlib.dll", "VEngineCvar" );
		game_types = capture<i_game_types>( "matchmaking.dll", "VENGINE_GAMETYPES_VERSION" );
		game_movement = capture<i_game_movement>( "client.dll", "GameMovement" );;
		prediction = capture<i_prediction>( "client.dll", "VClientPrediction" );;
		engine_sound = capture<i_engine_sound>( "engine.dll", "IEngineSoundClient" );
		effects = capture<i_effects>( "client.dll", "IEffects" );
		debug_overlay = capture<iv_debug_overlay>( "engine.dll", "VDebugOverlay" );
		game_console = capture<i_game_console>( "client.dll", "GameConsole" );
		event_manager = capture<i_game_event_manager2>( "engine.dll", "GAMEEVENTSMANAGER002" );
		localize = capture<i_localize>( "localize.dll", "Localize_" );
		material_system = capture<i_material_system>( "materialsystem.dll", "VMaterialSystem" );
		file_system = capture<i_file_system>( "filesystem_stdio.dll", "VBaseFileSystem011" );
		physics_props = capture<i_physics_surface_props>( "vphysics.dll", "VPhysicsSurfaceProps" );
		input_system = capture<i_input_system>( "inputsystem.dll", "InputSystemVersion" );
		studio_render = capture<i_studio_render>( "studiorender.dll", "VStudioRender" );
		leaf_system = capture<i_client_leaf_system_engine>( "client.dll", "ClientLeafSystem002" );
		render_view = capture<i_render_view>( "engine.dll", "VEngineRenderView014" );

		capture_ptr( direct_device, **( IDirect3DDevice9*** )( utils::find_pattern( "shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C" ) + 0x1 ) );
		capture_ptr( global_vars, **reinterpret_cast< i_global_vars_base*** >( utils::get_vfunc<std::uintptr_t>( client, 11 ) + 0xA ) );
		capture_ptr( client_state, **reinterpret_cast< i_client_state*** >( utils::get_vfunc<std::uintptr_t>( engine, 12 ) + 0x10 ) );
		capture_ptr( client_mode, **reinterpret_cast< i_client_mode_shared*** >( utils::get_vfunc<std::uintptr_t>( client, 10 ) + 0x5 ) );
		capture_ptr( key_values_system, reinterpret_cast< key_values_system_fn >( GetProcAddress( GetModuleHandleA( "vstdlib.dll" ), "KeyValuesSystem" ) )( ) );
		capture_ptr( weapon_system, *reinterpret_cast< i_weapon_system** >( utils::find_pattern( "client.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0" ) + 0x2 ) );
		capture_ptr( input, *reinterpret_cast< i_input** >( utils::get_vfunc<std::uintptr_t>( client, 16 ) + 0x1 ) );
		capture_ptr( glow_manager, *reinterpret_cast< i_glow_object_manager** >( utils::find_pattern( "client.dll", "0F 11 05 ? ? ? ? 83 C8 01" ) + 0x3 ) );
		capture_ptr( view_setup, *reinterpret_cast< void** >( utils::find_pattern( "client.dll", "A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10" ) + 0x1 ) );
		capture_ptr( mem_alloc, *reinterpret_cast< i_mem_alloc** >( GetProcAddress( GetModuleHandleA( "tier0.dll" ), "g_pMemAlloc" ) ) );
		capture_ptr( item_system, utils::relative_to_absolute<decltype( item_system )>( utils::find_pattern( "client.dll", "E8 ? ? ? ? 0F B7 0F" ) + 0x1 ) );
		capture_ptr( view_render_beams, *reinterpret_cast< i_view_render_beams** >( utils::find_pattern( "client.dll", "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9" ) + 0x1 ) );

		// functions
		capture_ptr( fn::equip_wearable, reinterpret_cast< decltype( fn::equip_wearable ) >( utils::find_pattern( "client.dll", "55 8B EC 83 EC 10 53 8B 5D 08 57 8B F9" ) ) );

		capture_ptr( fn::player_resources, *reinterpret_cast< c_player_resources*** >( utils::find_pattern( "client.dll", "74 30 8B 35 ? ? ? ? 85 F6" ) + 0x4 ) );
		capture_ptr( fn::get_decorated_player_name, utils::relative_to_absolute<decltype( fn::get_decorated_player_name )>( utils::find_pattern( "client.dll", "E8 ? ? ? ? 66 83 3E" ) + 0x1 ) );
		capture_ptr( fn::has_c4, reinterpret_cast< bool( __thiscall* )( void* ) >( utils::find_pattern( "client.dll", "56 8B F1 85 F6 74 31" ) ) );
		fn::animation_overlays_offset = *reinterpret_cast< std::uintptr_t* >( utils::find_pattern( "client.dll", "8B 89 ? ? ? ? 8D 0C D1" ) + 0x2 );

		// steam api
		const auto steam_api_ctx = engine->get_steam_api_context( );
		if ( !steam_api_ctx )
			return false;

		steam_user = steam_api_ctx->steam_user;
		steam_friend = steam_api_ctx->steam_friends;
		steam_utils = steam_api_ctx->steam_utils;

		if ( !steam_user || !steam_friend || !steam_utils )
			return false;

		csl::print( "steam_user->logged_on( )  is {}", steam_user->logged_on( ) ? "true" : "false" );
		csl::print( "steam_user->get_h_steam_user( )  is {}", steam_user->get_h_steam_user( ) );
	}
	catch ( const std::exception& ex )
	{
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
