#pragma once

#define EVENT_DEBUG_ID_INIT 42
#define EVENT_DEBUG_ID_SHUTDOWN 13

class i_game_event
{
public:
	virtual					~i_game_event( ) { }
	virtual const char* get_name( ) const = 0;

	virtual bool			is_reliable( ) const = 0;
	virtual bool			is_local( ) const = 0;
	virtual bool			is_empty( const char* szKeyName = nullptr ) = 0;

	virtual bool			get_bool( const char* szKeyName = nullptr, bool bDefault = false ) = 0;
	virtual int				get_int( const char* szKeyName = nullptr, int iDefault = 0 ) = 0;
	virtual std::uint64_t	get_uint64( const char* szKeyName = nullptr, std::uint64_t uDefault = 0 ) = 0;
	virtual float			get_float( const char* szKeyName = nullptr, float flDefault = 0.f ) = 0;
	virtual const char* get_string( const char* szKeyName = nullptr, const char* szDefault = "" ) = 0;
	virtual const wchar_t* get_wstring( const char* szKeyName = nullptr, const wchar_t* szDefault = L"" ) = 0;
	virtual const void* get_ptr( const char* szKeyName = nullptr ) const = 0;

	virtual void			set_bool( const char* szKeyName, bool bValue ) = 0;
	virtual void			set_int( const char* szKeyName, int iValue ) = 0;
	virtual void			set_uint64( const char* szKeyName, std::uint64_t uValue ) = 0;
	virtual void			set_float( const char* szKeyName, float flValue ) = 0;
	virtual void			set_string( const char* szKeyName, const char* szValue ) = 0;
	virtual void			set_wstring( const char* szKeyName, const wchar_t* szValue ) = 0;
	virtual void			set_ptr( const char* szKeyName, const void* pValue ) = 0;

	virtual bool			for_event_data( void* pEvent ) const = 0;
};

class i_game_event_listener2
{
public:
	virtual			~i_game_event_listener2( ) { }
	virtual void	fire_game_event( i_game_event* pEvent ) = 0;
	virtual int		get_event_debug_id( )
	{
		return nDebugID;
	}
public:
	int	nDebugID;
};

class csvc_msg_game_event;
class i_game_event_manager2
{
public:
	virtual				~i_game_event_manager2( ) { }
	virtual int			load_events_from_file( const char* szFileName ) = 0;
	virtual void		reset( ) = 0;
	virtual bool		add_listener( i_game_event_listener2* pListener, const char* szName, bool bServerSide ) = 0;
	virtual bool		find_listener( i_game_event_listener2* pListener, const char* szName ) = 0;
	virtual void		remove_listener( i_game_event_listener2* pListener ) = 0;
	virtual void		add_listener_global( i_game_event_listener2* pListener, bool bServerSide ) = 0;
	virtual i_game_event* create_new_event( const char* szName, bool bForce = false, int* unknown = nullptr ) = 0;
	virtual bool		fire_event( i_game_event* pEvent, bool bDontBroadcast = false ) = 0;
	virtual bool		fire_event_client_side( i_game_event* pEvent ) = 0;
	virtual i_game_event* duplicate_event( i_game_event* pEvent ) = 0;
	virtual void		free_event( i_game_event* pEvent ) = 0;
	virtual bool		serialize_event( i_game_event* pEvent, csvc_msg_game_event* pEventMsg ) = 0;
	virtual i_game_event* unserialize_event( const csvc_msg_game_event& eventMsg ) = 0;
	virtual c_key_values* get_event_data_types( i_game_event* pEvent ) = 0;
};

