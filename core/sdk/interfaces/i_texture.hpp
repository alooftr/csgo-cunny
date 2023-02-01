#pragma once
#include <d3d9.h>
#include "../../utils.h"

struct texture_t
{
private:
	std::byte			pad0[ 0xC ];		// 0x0000
public:
	IDirect3DTexture9* raw_texture;	// 0x000C
};

class i_texture
{
private:
	std::byte	pad0[ 0x50 ];		 // 0x0000
public:
	texture_t** texture_handles; // 0x0050

	int get_actual_width( )
	{
		return utils::call_vfunc<int>( this, 3 );
	}

	int get_actual_height( )
	{
		return utils::call_vfunc<int>( this, 4 );
	}

	void increment_reference_count( )
	{
		utils::call_vfunc<void>( this, 10 );
	}

	void decrement_reference_count( )
	{
		utils::call_vfunc<void>( this, 11 );
	}
};
