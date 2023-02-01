#pragma once
#include "../data_types/vector.hpp"
#include "../data_types/utl_vector.hpp"
#include "i_client_entity.hpp"

// @credits: https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/shared/igamemovement.h
class c_move_data
{
public:
	bool			bFirstRunOfFunctions : 1;
	bool			bGameCodeMovedPlayer : 1;
	bool			bNoAirControl : 1;
	std::uintptr_t	hPlayerHandle;		// edict index on server, client entity handle on client=
	int				nImpulseCommand;	// impulse command issued.
	vec3_t			angViewAngles;		// command view angles (local space)
	vec3_t			angAbsViewAngles;	// command view angles (world space)
	int				nButtons;			// attack buttons.
	int				nOldButtons;		// from host_client->oldbuttons;
	float			flForwardMove;
	float			flSideMove;
	float			flUpMove;
	float			flMaxSpeed;
	float			flClientMaxSpeed;
	vec3_t			vecVelocity;		// edict::velocity	// current movement direction.
	vec3_t			vecTrailingVelocity;
	float			flTrailingVelocityTime;
	vec3_t			vecAngles;			// edict::angles
	vec3_t			vecOldAngles;
	float			flOutStepHeight;	// how much you climbed this move
	vec3_t			vecOutWishVel;		// this is where you tried 
	vec3_t			vecOutJumpVel;		// this is your jump velocity
	vec3_t			vecConstraintCenter;
	float			flConstraintRadius;
	float			flConstraintWidth;
	float			flConstraintSpeedFactor;
	bool			bConstraintPastRadius;
	vec3_t			vecAbsOrigin;
};

// @credits: https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/shared/imovehelper.h
class c_base_entity;
class i_physics_surface_props;
class c_game_trace;
enum e_sound_level;
class i_move_helper
{
public:
	virtual	const char* get_name( void* hEntity ) const = 0;
	virtual void				set_host( c_base_entity* pHost ) = 0;
	virtual void				reset_touch_list( ) = 0;
	virtual bool				add_to_touched( const c_game_trace& trace, const vec3_t& vecImpactVelocity ) = 0;
	virtual void				process_impacts( ) = 0;
	virtual void				con_n_printf( int index, char const* fmt, ... ) = 0;
	virtual void				start_sound( const vec3_t& vecOrigin, int iChannel, char const* szSample, float flVolume, e_sound_level soundlevel, int fFlags, int iPitch ) = 0;
	virtual void				start_sound( const vec3_t& vecOrigin, const char* soundname ) = 0;
	virtual void				playback_event_full( int fFlags, int nClientIndex, unsigned short uEventIndex, float flDelay, vec3_t& vecOrigin, vec3_t& vecAngles, float flParam1, float flParam2, int iParam1, int iParam2, int bParam1, int bParam2 ) = 0;
	virtual bool				player_falling_damage( ) = 0;
	virtual void				player_set_animation( int playerAnimation ) = 0;
	virtual i_physics_surface_props* get_surface_props( ) = 0;
	virtual bool				is_world_entity( const unsigned long& hEntity ) = 0;
};

class i_game_movement
{
public:
	virtual						~i_game_movement( ) { }
	virtual void				process_movement( c_base_entity* pEntity, c_move_data* pMove ) = 0;
	virtual void				reset( ) = 0;
	virtual void				start_track_prediction_errors( c_base_entity* pEntity ) = 0;
	virtual void				finish_track_prediction_errors( c_base_entity* pEntity ) = 0;
	virtual void				diff_print( char const* fmt, ... ) = 0;
	virtual vec3_t const& get_player_mins( bool bDucked ) const = 0;
	virtual vec3_t const& get_player_maxs( bool bDucked ) const = 0;
	virtual vec3_t const& get_player_view_offset( bool bDucked ) const = 0;
	virtual bool				is_moving_player_stuck( ) const = 0;
	virtual c_base_entity* get_moving_player( ) const = 0;
	virtual void				unblock_pusher( c_base_entity* pEntity, c_base_entity* pPusher ) = 0;
	virtual void				setup_movement_bounds( c_move_data* pMove ) = 0;
};

class i_prediction
{
public:
	std::byte		pad0[ 0x4 ];						// 0x0000
	std::uintptr_t	hLastGround;					// 0x0004
	bool			bInPrediction;					// 0x0008
	bool			bOldCLPredictValue;				// 0x000B
	bool			bIsFirstTimePredicted;			// 0x0009
	bool			bEnginePaused;					// 0x000A
	int				iPreviousStartFrame;			// 0x000C
	int				nIncomingPacketNumber;			// 0x0010
	float			flLastServerWorldTimeStamp;		// 0x0014

	struct split_t
	{
		bool		bIsFirstTimePredicted;			// 0x0018
		std::byte	pad0[ 0x3 ];						// 0x0019
		int			nCommandsPredicted;				// 0x001C
		int			nServerCommandsAcknowledged;	// 0x0020
		int			iPreviousAckHadErrors;			// 0x0024
		float		flIdealPitch;					// 0x0028
		int			iLastCommandAcknowledged;		// 0x002C
		bool		bPreviousAckErrorTriggersFullLatchReset; // 0x0030
		c_utl_vector<c_base_handle> vecEntitiesWithPredictionErrorsInLastAck; // 0x0031
		bool		bPerformedTickShift;			// 0x0045
	};

	split_t			Split[ 1 ];						// 0x0018
	// SavedGlobals 0x4C

public:
	void update( int iStartFrame, bool bValidFrame, int nIncomingAcknowledged, int nOutgoingCommand )
	{
		utils::call_vfunc<void>( this, 3, iStartFrame, bValidFrame, nIncomingAcknowledged, nOutgoingCommand );
	}

	void get_local_view_angles( vec3_t& angView )
	{
		utils::call_vfunc<void>( this, 12, std::ref( angView ) );
	}

	void set_local_view_angles( vec3_t& angView )
	{
		utils::call_vfunc<void>( this, 13, std::ref( angView ) );
	}

	void check_moving_ground( c_base_entity* pEntity, double dbFrametime )
	{
		utils::call_vfunc<void>( this, 18, pEntity, dbFrametime );
	}

	void setup_move( c_base_entity* pEntity, c_user_cmd* pCmd, i_move_helper* pHelper, c_move_data* pMoveData )
	{
		utils::call_vfunc<void>( this, 20, pEntity, pCmd, pHelper, pMoveData );
	}

	void finish_move( c_base_entity* pEntity, c_user_cmd* pCmd, c_move_data* pMoveData )
	{
		utils::call_vfunc<void>( this, 21, pEntity, pCmd, pMoveData );
	}
};
