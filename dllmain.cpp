// attach console and windows library
#include "core/sdk/sdk.hpp"
#include "core/utils/input.h"
#include "core/cheats/hooks/hooks.hpp"
#include "core/cheats/cfg/cfg.h"
#include "core/utils/math.h"

#include "core/cheats/gui/gui.h"
#include "core/cheats/features/event_listener/event_listener.hpp"
#include "core/cheats/features/skins/parser.h"
#include "core/cheats/features/preview_model/preview_model.h"

DWORD WINAPI attach( LPVOID parameter )
{
	try
	{
		while (!GetModuleHandleA( "serverbrowser.dll" ))
			std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

		// timer for injection time
		time_t start, end;
		time( &start );
		{
#ifdef _DEBUG
			csl::attach_console( "csgo-cunny console" );
#endif
			csl::push_color( FOREGROUND_INTENSE_YELLOW );
			csl::print( "starting to setup cheat sdk..." );
			csl::pop_color( );

			if (!csgo::capture_interfaces( ))
				throw std::runtime_error( "failed to capture interfaces" );

			if (!math::setup( ))
				throw std::runtime_error( "failed to capture interfaces" );

			if (!c_netvar_manager::get( ).initialize( "netvar_dump.txt" ))
				throw std::runtime_error( "failed to dump netvar" );

			if (!ipt::setup( ))
				throw std::runtime_error( "failed to set window messages processor" );

			if (!cfg::setup( "default.cfg" ))
				throw std::runtime_error( "failed to setup config system" );

			if (!preview_model->setup( ))
				throw std::runtime_error( "failed to setup preview models" );

			parser::setup_kits( );
			event_listener.setup( { "player_death", "player_hurt" } );

			if (!hooks::setup( ))
				throw std::runtime_error( "failed to setup hooks" );

			if (!props_hook::setup( ))
				throw std::runtime_error( "failed to setup props hook" );

		}
		time( &end );

		windows::is_menu_opened = true;

		csl::push_color( FOREGROUND_INTENSE_GREEN );
		csl::print( "finished setup in {} seconds", static_cast<double>(end - start) );
		csl::pop_color( );
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
		// unload
		FreeLibraryAndExitThread( static_cast<HMODULE>(parameter), EXIT_FAILURE );
	}

	return 1UL;
}

DWORD WINAPI detach( LPVOID parameter )
{
	while (!GetAsyncKeyState( VK_END ))
		std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

	// remove hooks
	hooks::detach( );
	props_hook::detach( );

	// remove listener
	event_listener.release( );
#ifdef _DEBUG
	csl::detach_console( );
#endif // _DEBUG

	FreeLibraryAndExitThread( static_cast<HMODULE>(parameter), EXIT_FAILURE );

	return 1UL;
}


BOOL WINAPI DllMain( HMODULE module_handle, DWORD reason, LPVOID reserved )
{
	if (reason == DLL_PROCESS_ATTACH) {
		// disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH reasons to call
		DisableThreadLibraryCalls( module_handle );

		// basic process check
		if (GetModuleHandleA( "csgo.exe" ) == nullptr)
		{
			MessageBox( nullptr, "this cannot be injected in another process\nopen <csgo.exe> to inject", "cunny-csgo", MB_OK );
			return FALSE;
		}

		// create main thread
		if (auto thread = CreateThread( nullptr, 0U, attach, module_handle, 0UL, nullptr ); thread != nullptr)
			CloseHandle( thread );

		// create detach thread
		if (auto thread = CreateThread( nullptr, 0U, detach, module_handle, 0UL, nullptr ); thread != nullptr)
			CloseHandle( thread );
	}

	return TRUE;
}