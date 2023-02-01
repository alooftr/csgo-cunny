#pragma region convar_enumerations
// command to convars and concommands
enum e_con_var_flag : int
{
	// convar systems
	fcvar_none = 0,
	fcvar_unregistered = (1 << 0),	// if this is set, don't add to linked list, etc.
	fcvar_developmentonly = (1 << 1),	// hidden in released products. flag is removed automatically if allow_development_cvars is defined.
	fcvar_gamedll = (1 << 2),	// defined by the game dll
	fcvar_clientdll = (1 << 3),	// defined by the client dll
	fcvar_hidden = (1 << 4),	// hidden. doesn't appear in find or autocomplete. like developmentonly, but can't be compiled out.

	// convar only
	fcvar_protected = (1 << 5),	// it's a server cvar, but we don't send the data since it's a password, etc.  sends 1 if it's not bland/zero, 0 otherwise as value
	fcvar_sponly = (1 << 6),	// this cvar cannot be changed by clients connected to a multiplayer server.
	fcvar_archive = (1 << 7),	// set to cause it to be saved to vars.rc
	fcvar_notify = (1 << 8),	// notifies players when changed
	fcvar_userinfo = (1 << 9),	// changes the client's info string
	fcvar_cheat = (1 << 14),	// only useable in singleplayer / debug / multiplayer & sv_cheats
	fcvar_printableonly = (1 << 10),	// this cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
	fcvar_unlogged = (1 << 11),	// if this is a fcvar_server, don't log changes to the log file / console if we are creating a log
	fcvar_never_as_string = (1 << 12),	// never try to print that cvar

	// it's a convar that's shared between the client and the server.
	// at signon, the values of all such convars are sent from the server to the client (skipped for local client, ofc )
	// if a change is requested it must come from the console (i.e., no remote client changes)
	// if a value is changed while a server is active, it's replicated to all connected clients
	fcvar_server = (1 << 13),	// server setting enforced on clients, replicated
	fcvar_demo = (1 << 16),	// record this cvar when starting a demo file
	fcvar_dontrecord = (1 << 17),	// don't record these command in demofiles
	fcvar_reload_materials = (1 << 20),	// if this cvar changes, it forces a material reload
	fcvar_reload_textures = (1 << 21),	// if this cvar changes, if forces a texture reload
	fcvar_not_connected = (1 << 22),	// cvar cannot be changed by a client that is connected to a server
	fcvar_material_system_thread = (1 << 23),	// indicates this cvar is read from the material system thread
	fcvar_archive_xbox = (1 << 24),	// cvar written to config.cfg on the xbox
	fcvar_accessible_from_threads = (1 << 25),	// used as a debugging tool necessary to check material system thread convars
	fcvar_server_can_execute = (1 << 28),	// the server is allowed to execute this command on clients via clientcommand/net_stringcmd/cbaseclientstate::processstringcmd.
	fcvar_server_cannot_query = (1 << 29),	// if this is set, then the server is not allowed to query this cvar's value (via iserverpluginhelpers::startquerycvarvalue).
	fcvar_clientcmd_can_execute = (1 << 30),	// ivengineclient::clientcmd is allowed to execute this command. 
	fcvar_material_thread_mask = (fcvar_reload_materials | fcvar_reload_textures | fcvar_material_system_thread)
};
#pragma endregion

using c_var_dll_identifier_t = int;
class c_conbase;
class c_convar;
class c_concmd;
class i_convar : public i_app_system
{
public:
	virtual c_var_dll_identifier_t allocate_dll_identifier( ) = 0;
	virtual void			register_con_command( c_convar* pCommandBase, int iDefaultValue = 1 ) = 0;
	virtual void			unregister_con_command( c_convar* pCommandBase ) = 0;
	virtual void			unregister_con_commands( c_var_dll_identifier_t id ) = 0;
	virtual const char* get_command_line_value( const char* szVariableName ) = 0;
	virtual c_conbase* find_command_base( const char* szName ) = 0;
	virtual const c_conbase* find_command_base( const char* szName ) const = 0;
	virtual c_convar* find_var( const char* szVariableName ) = 0;
	virtual const c_convar* find_var( const char* szVariableName ) const = 0;
	virtual c_concmd* find_com_mand( const char* szName ) = 0;
	virtual const c_concmd* find_command( const char* szName ) const = 0;
	virtual void			install_global_change_callback( fn_change_callback_t callback ) = 0;
	virtual void			remove_global_change_callback( fn_change_callback_t callback ) = 0;
	virtual void			call_global_change_callbacks( c_convar* pVar, const char* szOldString, float flOldValue ) = 0;
	virtual void			install_console_display_func( void* pDisplayFunc ) = 0;
	virtual void			remove_console_display_func( void* pDisplayFunc ) = 0;
	virtual void			console_color_printf( const col_t& col_t, const char* pFormat, ... ) const = 0;
	virtual void			console_printf( const char* pFormat, ... ) const = 0;
	virtual void			console_d_printf( const char* pFormat, ... ) const = 0;
	virtual void			revert_flagged_con_vars( int nFlag ) = 0;
};
