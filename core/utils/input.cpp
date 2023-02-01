// std::this_thread
#include <thread>
// window api
#include <windowsx.h>

// setup window handle
#include "../sdk/sdk.hpp"
// setup hook for wndproc
#include "../cheats/hooks/hooks.hpp"

#include "input.h"

bool ipt::setup( )
{
	D3DDEVICE_CREATION_PARAMETERS creation_parameters = { };
	while (FAILED( csgo::direct_device->GetCreationParameters( &creation_parameters ) ))
		std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

	csgo::window_handle = creation_parameters.hFocusWindow;

	if (csgo::window_handle == nullptr)
		return false;

	old_wnd_proc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW( csgo::window_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hooks::hk_wnd_proc) ));

	if (old_wnd_proc == nullptr)
		return false;

	return true;
}

void ipt::restore( )
{
	if (old_wnd_proc != nullptr)
	{
		SetWindowLongPtrW( csgo::window_handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(old_wnd_proc) );
		old_wnd_proc = nullptr;
	}
}

bool ipt::process( UINT msg, WPARAM wparam, LPARAM lparam )
{
	// current active key
	int key = 0;
	// current active key state
	e_key_state state = e_key_state::none;

	switch (msg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (wparam < 256U)
		{
			key = wparam;
			state = e_key_state::down;
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (wparam < 256U)
		{
			key = wparam;
			state = e_key_state::up;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
		key = VK_LBUTTON;
		state = msg == WM_LBUTTONUP ? e_key_state::up : e_key_state::down;
		break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
		key = VK_RBUTTON;
		state = msg == WM_RBUTTONUP ? e_key_state::up : e_key_state::down;
		break;
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
		key = VK_MBUTTON;
		state = msg == WM_MBUTTONUP ? e_key_state::up : e_key_state::down;
		break;
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_XBUTTONDBLCLK:
		key = (GET_XBUTTON_WPARAM( wparam ) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2);
		state = msg == WM_XBUTTONUP ? e_key_state::up : e_key_state::down;
		break;
	default:
		return false;
	}

	// save key states
	if (state == e_key_state::up && key_state.at( key ) == e_key_state::down) // if swap states it will be pressed state
		key_state.at( key ) = e_key_state::released;
	else
		key_state.at( key ) = state;

	return true;
}
