#pragma once
#include <cwctype>
#include <d3d9.h>
#pragma warning(disable:4996)

#include "utils/console_log.h"

#include "sdk/data_types/vector.hpp"

namespace utils
{
	template <typename T = void*>
	constexpr T get_vfunc( void* this_ptr, std::size_t index )
	{
		return (*static_cast<T**>(this_ptr))[ index ];
	}

	template <typename T, typename ... Args_t>
	constexpr T call_vfunc( void* this_ptr, std::size_t index, Args_t... arg_list )
	{
		using virtual_fn = T( __thiscall* )(void*, decltype(arg_list)...);
		return (*static_cast<virtual_fn**>(this_ptr))[ index ]( this_ptr, arg_list... );
	}

	template <typename T>
	static constexpr auto relative_to_absolute( uintptr_t address ) noexcept
	{
		return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
	}

	inline std::uintptr_t get_absolute_address( const std::uintptr_t relative_address )
	{
		return relative_address + 0x4 + *reinterpret_cast<std::int32_t*>(relative_address);
	}

	inline std::filesystem::path get_game_path( )
	{
		char progname[ MAX_PATH ] = "client.dll";
		GetModuleFileName( NULL, progname, MAX_PATH );

		std::string path = progname;
		// remove csgo.exe from the string
		path.erase( path.find( "csgo.exe" ) );

		return path;
	}

	inline void flash_window( HWND hwnd ) {
		FLASHWINFO fi;
		fi.cbSize = sizeof( FLASHWINFO );
		fi.hwnd = hwnd;
		fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
		fi.uCount = 0;
		fi.dwTimeout = 0;
		FlashWindowEx( &fi );
	}

	inline std::wstring to_wide_string( const std::string& str ) noexcept
	{
		std::wstring upperCase( str.length( ), L'\0' );
		if (const auto newLen = std::mbstowcs( upperCase.data( ), str.c_str( ), upperCase.length( ) ); newLen != static_cast<std::size_t>(-1))
			upperCase.resize( newLen );
		return upperCase;
	}

	inline std::wstring to_upper( std::wstring str ) noexcept
	{
		std::transform( str.begin( ), str.end( ), str.begin( ), [ ]( wchar_t w ) -> wchar_t
			{
				if (w >= 0 && w <= 127)
				{
					if (w >= 'a' && w <= 'z')
						return w - ('a' - 'A');
					return w;
				}

		return std::towupper( w );
			} );
		return str;
	}

	inline std::wstring to_lower( std::wstring str ) noexcept
	{
		std::transform( str.begin( ), str.end( ), str.begin( ), ::tolower );

		return str;
	}

	inline std::string to_lower( std::string str ) noexcept
	{
		std::transform( str.begin( ), str.end( ), str.begin( ), ::tolower );

		return str;
	}

	template <typename T, std::size_t s>
	inline std::vector<T> get_filled_vector( const T& fill )
	{
		std::vector<T> temp( s );
		std::fill( temp.begin( ), temp.begin( ) + s, fill );
		return temp;
	}

	std::uintptr_t find_pattern( const std::string_view module_name, const std::string_view pattern );
	std::uintptr_t find_pattern( const std::uint8_t* region_start, const std::uintptr_t region_size, const std::string_view pattern );
	std::uintptr_t* find_hud_element( const char* name );
	bool line_goes_through_smoke( const vec3_t& start, const vec3_t& end, const bool grenade_bloat = true );
	const char8_t* get_weapon_icon( short item_definition_index );

	LPDIRECT3DTEXTURE9 get_steam_avatar( std::uint64_t steam_id = 76561198433738750 );
	int get_fps( );
	inline void copy_convert( const uint8_t* rgba, uint8_t* out, const size_t size )
	{
		auto in = reinterpret_cast<const uint32_t*>(rgba);
		auto buf = reinterpret_cast<uint32_t*>(out);
		for (auto i = 0u; i < (size / 4); ++i)
		{
			const auto pixel = *in++;
			*buf++ = (pixel & 0xFF00FF00) | ((pixel & 0xFF0000) >> 16) | ((pixel & 0xFF) << 16);
		}
	}
}