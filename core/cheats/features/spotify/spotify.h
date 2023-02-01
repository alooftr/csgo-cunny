#pragma once

#include <string>
#include "../../../utils/singleton.h"

class c_spotify : public c_singleton< c_spotify >
{
public:
	void setup_curl( );
	void cleanup_curl( );
	std::string get_current_song( );

	std::string get_song_from_curl( );
private:
	const char* id = "5a7cc2c0636a423aa45f61638afb0617";
	const char* secret = "c2fed60c9a6245a282595c20e4e9ca5b";
};