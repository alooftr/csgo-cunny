#include "spotify.h"

// including curl library and dependencies
#define CURL_STATICLIB
#include <curl/curl.h>

#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "advapi32.lib")

// console log, string etc..
#include "../../../utils.h"

void c_spotify::setup_curl( )
{
	curl_global_init( CURL_GLOBAL_DEFAULT );
}

void c_spotify::cleanup_curl( )
{
	curl_global_cleanup( );
}

// bad way
std::string c_spotify::get_current_song( )
{
	auto handle = FindWindowExA( 0, 0, "Chrome_WidgetWin_0", nullptr );
	std::string window_text = "";

	// finding current window
	if (handle != nullptr)
	{
		char buffer[ 255 ] = "";
		GetWindowTextA( handle, buffer, 255 );
		window_text = buffer;

		// we found it
		if (window_text.empty( ))
			return "spotify not open";
		
		// we havent found it so keep looking
		handle = FindWindowExA( 0, handle, "Chrome_WidgetWin_0", nullptr );
	}

	if ( window_text == "Spotify Premium" )
		return "not playing";

	return utils::to_lower(window_text);
}

std::string c_spotify::get_song_from_curl( )
{
	CURL* curl = curl_easy_init( );
	if (!curl)
		return "curl failed";

	return "...";
}
