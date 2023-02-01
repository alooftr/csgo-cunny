#include "prediction.h"

void c_prediction::start( c_user_cmd* cmd, c_base_entity* local_player )
{
	// update
	if ( csgo::client_state->delta_tick > 0 )
		csgo::prediction->update( csgo::client_state->delta_tick, csgo::client_state->delta_tick > 0, csgo::client_state->last_command_ack, csgo::client_state->last_outgoing_command + csgo::client_state->choked_commands );

	// @xref: "CPrediction::ProcessMovement"
	if ( !local_player->is_alive( ) || csgo::move_helper == nullptr )
		return;

	globals::in_prediction = true;

	// start command
	*local_player->get_current_command( ) = cmd;
	local_player->get_last_command( ) = *cmd;

	// random seed was already generated in CInput::CreateMove
	*prediction_random_seed = cmd->random_seed;
	// set ourselves as a predictable entity
	*prediction_player = local_player;

	// backup globals
	old_current_time = csgo::global_vars->current_time;
	old_frame_time = csgo::global_vars->frame_time;
	old_tick_count = csgo::global_vars->tick_count;

	// backup tickbase
	const int old_tick_base = local_player->get_tick_base( );

	// backup prediction states
	const bool old_is_first_prediction = csgo::prediction->bIsFirstTimePredicted;
	const bool old_in_prediction = csgo::prediction->bInPrediction;

	// set corrected values
	csgo::global_vars->current_time = TICKS_TO_TIME( get_tick_base( cmd, local_player ) );
	csgo::global_vars->frame_time = csgo::prediction->bEnginePaused ? 0.f : TICK_INTERVAL;
	csgo::global_vars->tick_count = get_tick_base( cmd, local_player );

	csgo::prediction->bIsFirstTimePredicted = false;
	csgo::prediction->bInPrediction = true;

	/* skipped weapon select and vehicle predicts */

	// synchronize m_afButtonForced & m_afButtonDisabled
	cmd->buttons |= local_player->get_button_forced( );
	cmd->buttons &= ~( local_player->get_button_disabled( ) );

	csgo::game_movement->start_track_prediction_errors( local_player );

	// update button state
	const int buttons = cmd->buttons;
	const int local_buttons = *local_player->get_buttons( );
	const int buttons_changed = buttons ^ local_buttons;

	// synchronize m_afButtonLast
	local_player->get_button_last( ) = local_buttons;

	// synchronize m_nButtons
	*local_player->get_buttons( ) = buttons;

	// synchronize m_afButtonPressed
	local_player->get_button_pressed( ) = buttons_changed & buttons;

	// synchronize m_afButtonReleased
	local_player->get_button_released( ) = buttons_changed & ( ~buttons );

	// check if the player is standing on a moving entity and adjusts velocity and basevelocity appropriately
	csgo::prediction->check_moving_ground( local_player, csgo::global_vars->frame_time );

	// copy angles from command to player
	csgo::prediction->set_local_view_angles( cmd->view_angle );

	// run prethink
	if ( local_player->physics_run_think( think_fire_all_functions ) )
		local_player->pre_think( );

	// run think
	if ( int* next_think_tick = local_player->get_next_think_tick( ); *next_think_tick > 0 && *next_think_tick <= get_tick_base( cmd, local_player ) )
	{
		*next_think_tick = TICK_NEVER_THINK;

		/*
		 * handle no think function
		 * pseudo i guess didnt seen before but not sure, most likely unnecessary
		nEFlags = player->GetEFlags();
		result = player->GetEFlags() & EFL_NO_THINK_FUNCTION;
		if (!result)
		{
			result = [&]()
			{
				if (player->GetNextThinkTick() > 0)
					return 1;
				v3 = *(_DWORD *)(player + 0x2BC);
				v4 = 0;
				if (v3 > 0)
				{
				v5 = (_DWORD *)(*(_DWORD *)(player + 0x2B0) + 0x14);
				while (*v5 <= 0)
				{
					++v4;
					v5 += 8;
					if (v4 >= v3)
						return 0;
				}
				return 1;
			}();
			if (!result)
				player->GetEFlags() = nEFlags | EFL_NO_THINK_FUNCTION;
		}
		 */

		local_player->think( );
	}

	// set host player
	csgo::move_helper->set_host( local_player );

	// setup move
	csgo::prediction->setup_move( local_player, cmd, csgo::move_helper, &move_data );
	csgo::game_movement->process_movement( local_player, &move_data );

	// finish move
	csgo::prediction->finish_move( local_player, cmd, &move_data );
	csgo::move_helper->process_impacts( );

	// run post think
	local_player->post_think( );

	// restore tickbase
	local_player->get_tick_base( ) = old_tick_base;

	// restore prediction states
	csgo::prediction->bInPrediction = old_in_prediction;
	csgo::prediction->bIsFirstTimePredicted = old_is_first_prediction;
}

void c_prediction::end( c_user_cmd* cmd, c_base_entity* local_player ) const
{
	if ( !local_player->is_alive( ) || csgo::move_helper == nullptr )
		return;

	csgo::game_movement->finish_track_prediction_errors( local_player );

	// reset host player
	csgo::move_helper->set_host( nullptr );

	// restore globals
	csgo::global_vars->current_time = old_current_time;
	csgo::global_vars->frame_time = old_frame_time;
	csgo::global_vars->tick_count = old_tick_count;

	// finish command
	*local_player->get_current_command( ) = nullptr;

	// reset prediction seed
	*prediction_random_seed = -1;

	// reset prediction entity
	*prediction_player = nullptr;

	// reset move
	csgo::game_movement->reset( );

	globals::in_prediction = false;
}

void c_prediction::restore_entity_to_predicted_frame( int predicted_frame )
{
	using restore_entity_to_predicted_frame_fn = void( __stdcall* )( int, int );
	static auto fn = reinterpret_cast< restore_entity_to_predicted_frame_fn >( utils::find_pattern( "client.dll", "55 8B EC 8B 4D ? 56 E8 ? ? ? ? 8B 75" ) );
	fn( 0, predicted_frame );
}

/*
 * required cuz prediction works on frames, not ticks
 * corrects tickbase if your framerate goes below tickrate and m_nTickBase won't update every tick
 */
int c_prediction::get_tick_base( c_user_cmd* cmd, c_base_entity* local_player )
{
	static int iTick = 0;

	if ( cmd != nullptr )
	{
		static c_user_cmd* pLastCmd = nullptr;

		// if command was not predicted - increment tickbase
		if ( pLastCmd == nullptr || pLastCmd->has_been_predicted )
			iTick = local_player->get_tick_base( );
		else
			iTick++;

		pLastCmd = cmd;
	}

	return iTick;
}