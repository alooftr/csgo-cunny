#pragma once
#include "../data_types/c_user_cmd.hpp"

#define MULTIPLAYER_BACKUP 150

class i_input
{
public:
	std::byte			pad0[ 0xC ];				// 0x00
	bool				bTrackIRAvailable;		// 0x0C
	bool				bMouseInitialized;		// 0x0D
	bool				bMouseActive;			// 0x0E
	std::byte			pad1[ 0x9A ];				// 0x0F
	bool				bCameraInThirdPerson;	// 0xA9
	std::byte			pad2[ 0x2 ];				// 0xAA
	vec3_t				vecCameraOffset;		// 0xAC
	std::byte			pad3[ 0x38 ];				// 0xB8
	c_user_cmd* pCommands;				// 0xF0
	c_verified_user_cmd* pVerifiedCommands;		// 0xF4

	[[nodiscard]] c_user_cmd* get_user_cmd( const int sequence_number ) const
	{
		return &pCommands[ sequence_number % MULTIPLAYER_BACKUP ];
	}

	[[nodiscard]] c_verified_user_cmd* get_verified_cmd( const int sequence_number ) const
	{
		return &pVerifiedCommands[ sequence_number % MULTIPLAYER_BACKUP ];
	}
};
static_assert(sizeof( i_input ) == 0xF8);