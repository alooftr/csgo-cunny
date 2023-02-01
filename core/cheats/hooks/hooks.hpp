#pragma once
#include "../../sdk/sdk.hpp"
#include "../../utils/detour.h"

namespace vtable
{
	enum {
		reset = 16,
		end_scene = 42,
		lock_cursor = 67,
		paint_traverse = 41,
		alloc_key_values_memory = 2,
		create_move_proxy = 22,
		override_view = 18,
		frame_stage_notify = 37,
	};
}

namespace detour
{
	inline c_detour_hook end_scene;
	inline c_detour_hook reset;
	inline c_detour_hook lock_cursor;
	inline c_detour_hook paint_traverse;
	inline c_detour_hook alloc_key_values_memory;
	inline c_detour_hook create_move_proxy;
	inline c_detour_hook override_view;
	inline c_detour_hook frame_stage_notify;

}

namespace hooks
{
	bool setup( );
	void detach( );

	long	D3DAPI		hk_end_scene( IDirect3DDevice9* device );
	long	D3DAPI		hk_reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters );
	LRESULT __stdcall	hk_wnd_proc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
	void	__fastcall	hk_lock_cursor( i_surface* thisptr, int edx );
	void	__fastcall	hk_paint_traverse( i_surface* thisptr, int edx, unsigned int panel, bool force_repaint, bool force );
	void*	__fastcall	hk_alloc_key_values_memory( i_key_values_system* thisptr, int edx, int size );
	void	__fastcall	hk_create_move_proxy( i_base_client_dll* thisptr, int edx, int sequence_number, float input_sample_frametime, bool is_active );
	void	__fastcall	hk_override_view( i_client_mode_shared* thisptr, int edx, c_view_setup* view_setup );
	void	__fastcall	hk_frame_stage_notify( i_base_client_dll* thisptr, int edx, e_client_frame_stage stage );
}

namespace props
{
	inline std::shared_ptr<c_recv_prop_hook> sequence;
}

namespace props_hook
{
	// Get
	bool	setup( );
	void	detach( );

	// Handlers
	void	hk_sequence( const c_recv_proxy_data* fata, void* struct_ptr, void* out );
}