#pragma once

#include <Windows.h>
#include <format>
#include <fstream>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <string>
#include <xstring>

#define FOREGROUND_WHITE		    (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_YELLOW       	(FOREGROUND_RED | FOREGROUND_GREEN)
#define FOREGROUND_CYAN		        (FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_MAGENTA	        (FOREGROUND_RED | FOREGROUND_BLUE)
#define FOREGROUND_BLACK		    0

#define FOREGROUND_INTENSE_RED		(FOREGROUND_RED | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_GREEN	(FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_BLUE		(FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_WHITE	(FOREGROUND_WHITE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_YELLOW	(FOREGROUND_YELLOW | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_CYAN		(FOREGROUND_CYAN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_MAGENTA	(FOREGROUND_MAGENTA | FOREGROUND_INTENSITY)

#include "encrypt/xor.h"

namespace csl
{
	inline FILE* stream;
	inline std::uint16_t console_color = FOREGROUND_WHITE;

	bool attach_console( const char* title );
	void detach_console( );

	template <typename ... Args_t>
	void print( const std::string_view text, const Args_t& ... argList )
	{
#ifdef _DEBUG
		const std::string time = std::vformat( _( "[{:%d-%m-%Y %X}] " ), std::make_format_args( std::chrono::system_clock::now( ) ) );

		// print to console
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), FOREGROUND_INTENSE_CYAN );
		std::cout << time;
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), console_color );

		if constexpr (sizeof...(argList) > 0)
			std::cout << std::vformat( text, std::make_format_args( argList... ) ) << std::endl;
		else
			std::cout << text << std::endl;
#endif
	}

	inline void push_color( const std::uint16_t col_t )
	{
		console_color = col_t;
	}

	inline void pop_color( )
	{
		console_color = FOREGROUND_WHITE;
	}
}