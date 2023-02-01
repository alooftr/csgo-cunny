#pragma once

#include <map>
#include "utl_vector.hpp"
#include "../../utils.h"
#include "color.hpp"

using fn_command_callback_v1_t = void( __cdecl* )();
using fn_change_callback_t = void( __cdecl* )(void*, const char*, float);
class c_convar
{
public:
	const char* get_name( )
	{
		return utils::call_vfunc<const char*>( this, 5 );
	}

	float get_float( ) // idx @ 11
	{
		std::uint32_t uXored = *reinterpret_cast<std::uint32_t*>(&parent->value_float) ^ reinterpret_cast<std::uint32_t>(this);
		return *reinterpret_cast<float*>(&uXored);
	}

	int get_int( ) // idx @ 12
	{
		return static_cast<int>(parent->value_int ^ reinterpret_cast<int>(this));
	}

	bool get_bool( ) // idx @ 13
	{
		return !!get_int( );
	}

	const char* get_string( ) const
	{
		char const* v = parent->string;
		return v ? v : "";
	}

	void set_value( const char* v )
	{
		return utils::call_vfunc<void>( this, 14, v );
	}

	void set_value( float v )
	{
		return utils::call_vfunc<void>( this, 15, v );
	}

	void set_value( int v )
	{
		return utils::call_vfunc<void>( this, 16, v );
	}

	void set_value( col_t v )
	{
		return utils::call_vfunc<void>( this, 17, v );
	}

public:
	std::byte						pad0[ 0x4 ];			//0x00
	c_convar* next;				//0x04
	bool							registered;		//0x08
	const char* name;				//0x0C
	const char* help_string;		//0x10
	int								flags;				//0x14
	fn_command_callback_v1_t callback;			//0x18
	c_convar* parent;			//0x1C
	const char* default_value;		//0x20
	char* string;			//0x24
	int								string_length;		//0x28
	float							value_float;			//0x2C
	int								value_int;				//0x30
	bool							has_min;			//0x34
	float							min_value;			//0x38
	bool							has_max;			//0x3C
	float							max_value;			//0x40
	c_utl_vector<void( __cdecl* )(void*, const char*, float)>	change_callbacks;	//0x44
};
