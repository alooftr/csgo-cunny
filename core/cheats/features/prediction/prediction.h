#pragma once

#include "../../../sdk/sdk.hpp"

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/server/player_command.cpp#L315
class c_prediction : public c_singleton<c_prediction>
{
public:
	c_prediction( )
	{
		prediction_random_seed = *reinterpret_cast< unsigned int** >( utils::find_pattern( "client.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04" ) + 0x2 ); // @xref: "SelectWeightedSequence"
		prediction_player = *reinterpret_cast< c_base_entity*** >( utils::find_pattern( "client.dll", "89 35 ? ? ? ? F3 0F 10 48 20" ) + 0x2 );
	}

	// Get
	void start( c_user_cmd* cmd, c_base_entity* local_player );
	void end( c_user_cmd* cmd, c_base_entity* local_player ) const;
	void restore_entity_to_predicted_frame( int predicted_frame );

	/*
	 * required cuz prediction works on frames, not ticks
	 * corrects tickbase if your framerate goes below tickrate and m_nTickBase won't update every tick
	 */
	static int get_tick_base( c_user_cmd* cmd, c_base_entity* local_player );
private:
	// Values
	/* prediction seed */
	unsigned int* prediction_random_seed = nullptr;
	/* current predictable player */
	c_base_entity** prediction_player = nullptr;
	/* encapsulated input parameters to player movement */
	c_move_data move_data = { };

	/* backup */
	float old_current_time = 0.f;
	float old_frame_time = 0.f;
	int old_tick_count = 0;
};