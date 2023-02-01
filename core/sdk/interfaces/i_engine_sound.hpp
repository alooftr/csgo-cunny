#pragma once
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/engine/IEngineSound.h

#pragma region enginesound_enumerations
/* channel */
enum
{
	chan_replace = -1,
	chan_auto = 0,
	chan_weapon = 1,
	chan_voice = 2,
	chan_item = 3,
	chan_body = 4,
	chan_stream = 5,		// allocate stream channel from the static or dynamic area
	chan_static = 6,		// allocate channel from the static area 
	chan_voice2 = 7,
	chan_voice_base = 8,	// allocate channel for network voice data
	chan_user_base = (chan_voice_base + 128) // anything >= this number is allocated to game code.
};

enum e_sound_level : int
{
	sndlvl_none = 0,
	sndlvl_20db = 20,			// rustling leaves
	sndlvl_25db = 25,			// whispering
	sndlvl_30db = 30,			// library
	sndlvl_35db = 35,
	sndlvl_40db = 40,
	sndlvl_45db = 45,			// refrigerator
	sndlvl_50db = 50,	// 3.9	// average home
	sndlvl_55db = 55,	// 3.0

	sndlvl_idle = 60,	// 2.0	
	sndlvl_60db = 60,	// 2.0	// normal conversation, clothes dryer
	sndlvl_65db = 65,	// 1.5	// washing machine, dishwasher

	sndlvl_static = 66,	// 1.25
	sndlvl_70db = 70,	// 1.0	// car, vacuum cleaner, mixer, electric sewing machine

	sndlvl_norm = 75,
	sndlvl_75db = 75,	// 0.8	// busy traffic
	sndlvl_80db = 80,	// 0.7	// mini-bike, alarm clock, noisy restaurant, office tabulator, outboard motor, passing snowmobile

	sndlvl_talking = 80,// 0.7
	sndlvl_85db = 85,	// 0.6	// average factory, electric shaver
	sndlvl_90db = 90,	// 0.5	// screaming child, passing motorcycle, convertible ride on frw
	sndlvl_95db = 95,
	sndlvl_100db = 100,	// 0.4	// subway train, diesel truck, woodworking shop, pneumatic drill, boiler shop, jackhammer
	sndlvl_105db = 105,			// helicopter, power mower
	sndlvl_110db = 110,			// snowmobile drvrs seat, inboard motorboat, sandblasting
	sndlvl_120db = 120,			// auto horn, propeller aircraft
	sndlvl_130db = 130,			// air raid siren

	sndlvl_gunfire = 140,// 0.27	// threshold of pain, gunshot, jet engine
	sndlvl_140db = 140,	// 0.2
	sndlvl_150db = 150,	// 0.2
	sndlvl_180db = 180,			// rocket launching
};

enum e_sound_flags : int
{
	snd_noflags = 0,				// to keep the compiler happy
	snd_change_vol = (1 << 0),		// change sound vol
	snd_change_pitch = (1 << 1),	// change sound pitch
	snd_stop = (1 << 2),			// stop the sound
	snd_spawning = (1 << 3),		// we're spawning, used in some cases for ambients
	// not sent over net, only a param between dll and server.
	snd_delay = (1 << 4),			// sound has an initial delay
	snd_stop_looping = (1 << 5),	// stop all looping sounds on the entity.
	snd_speaker = (1 << 6),			// being played again by a microphone through a speaker
	snd_shouldpause = (1 << 7),		// this sound should be paused if the game is paused
	snd_ignore_phonemes = (1 << 8),
	snd_ignore_name = (1 << 9),		// used to change all sounds emitted by an entity, regardless of scriptname
	snd_do_not_overwrite_existing_on_channel = (1 << 10)
};
#pragma endregion

#pragma region enginesound_definitions
/* volume */
#define VOL_NORM			1.0f

/* attenuation */
#define ATTN_NONE			0.0f
#define ATTN_NORM			0.8f
#define ATTN_IDLE			2.0f
#define ATTN_STATIC			1.25f 
#define ATTN_RICOCHET		1.5f
#define ATTN_GUNFIRE		0.27f
#define MAX_ATTENUATION		3.98f

/* soundlevel limits */
#define MAX_SNDLVL_BITS		9
#define MIN_SNDLVL_VALUE	0
#define MAX_SNDLVL_VALUE	((1 << MAX_SNDLVL_BITS) - 1)

/* soundlevel */
#define SNDLEVEL_TO_COMPATIBILITY_MODE( x )		static_cast<e_sound_level>(static_cast<int>(x + 256))
#define SNDLEVEL_FROM_COMPATIBILITY_MODE( x )	static_cast<e_sound_level>(static_cast<int>(x - 256))
#define SNDLEVEL_IS_COMPATIBILITY_MODE( x )		( x ) >= 256

/* convertation */
#define ATTN_TO_SNDLVL( a ) static_cast<e_sound_level>(static_cast<int>( a ) ? (50 + 20 / (static_cast<float>( a ))) : 0)
#define SNDLVL_TO_ATTN( a ) ((a > 50) ? (20.0f / static_cast<float>(a - 50)) : 4.0)

/* sound */
#define SND_FLAG_BITS_ENCODE 11

#define MAX_SOUND_DELAY_MSEC_ENCODE_BITS	13
#define MAX_SOUND_INDEX_BITS				14
#define	MAX_SOUNDS							(1 << MAX_SOUND_INDEX_BITS)
#define MAX_SOUND_DELAY_MSEC				(1 << (MAX_SOUND_DELAY_MSEC_ENCODE_BITS - 1))    // 4096 msec or about 4 seconds

/* source */
#define SOUND_FROM_UI_PANEL			-2
#define SOUND_FROM_LOCAL_PLAYER		-1
#define SOUND_FROM_WORLD			0

/* pitch */
#define PITCH_LOW			95
#define	PITCH_NORM			100
#define PITCH_HIGH			120

#define DEFAULT_SOUND_PACKET_VOLUME 1.0f
#define DEFAULT_SOUND_PACKET_PITCH	100
#define DEFAULT_SOUND_PACKET_DELAY	0.0f
#pragma endregion

using file_name_handle_t = void*;
struct sound_info_t
{
	int			guid;
	file_name_handle_t file_name;
	int			sound_source;
	int			channel;
	int			speaker_entity;
	float		volume;
	float		last_spatialized_volume;
	float		radius;
	int			pitch;
	vec3_t* origin;
	vec3_t* direction;
	bool		update_positions;
	bool		is_sentence;
	bool		dry_mix;
	bool		speaker;
	bool		special_dsp;
	bool		from_server;
};

class c_sfx_table;
struct start_sound_params_t
{
	bool			static_sound;
	int				user_data;
	int				sound_source;
	int				entity_channel;
	c_sfx_table* sfx;
	vec3_t			origin;
	vec3_t			direction;
	bool			update_positions;
	float			volume;
	e_sound_level		SoundLevel;
	int				flags;
	int				pitch;
	bool			from_server;
	float			delay;
	int				speaker_entity;
	bool			suppress_recording;
	int				initial_stream_position;
};

class i_recipient_filter
{
public:
	virtual			~i_recipient_filter( ) { }

	virtual bool	is_reliable( ) const = 0;
	virtual bool	is_init_message( ) const = 0;

	virtual int		get_recipient_count( ) const = 0;
	virtual int		get_recipient_index( int nSlot ) const = 0;
};

class i_engine_sound
{
public:
	virtual bool	precache_sound( const char* szSample, bool bPreload = false, bool bIsUISound = false ) = 0;
	virtual bool	is_sound_precached( const char* szSample ) = 0;
	virtual void	prefetch_sound( const char* szSample ) = 0;
	virtual bool	is_looping_sound( const char* szSample ) = 0;
	virtual float	get_sound_duration( const char* szSample ) = 0;

	virtual int emit_sound( i_recipient_filter& filter, int nEntityIndex, int iChannel, const char* szSoundEntry, unsigned int uSoundEntryHash, const char* szSample,
		float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch,
		const vec3_t* vecOrigin, const vec3_t* vecDirection, c_utl_vector<vec3_t>* pUtlVecOrigins, bool bUpdatePositions, float flSoundTime, int nSpeakerEntity, start_sound_params_t& parameters ) = 0;

	virtual int emit_sound( i_recipient_filter& filter, int nEntityIndex, int iChannel, const char* szSoundEntry, unsigned int uSoundEntryHash, const char* szSample,
		float flVolume, e_sound_level nSoundLevel, int nSeed, int iFlags, int iPitch,
		const vec3_t* vecOrigin, const vec3_t* vecDirection, c_utl_vector<vec3_t>* pUtlVecOrigins, bool bUpdatePositions, float flSoundTime, int nSpeakerEntity, start_sound_params_t& parameters ) = 0;

	virtual void emit_sentence_by_index( i_recipient_filter& filter, int nEntityIndex, int iChannel, int nSentenceIndex,
		float flVolume, e_sound_level nSoundLevel, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
		const vec3_t* vecOrigin = nullptr, const vec3_t* vecDirection = nullptr, c_utl_vector<vec3_t>* pUtlVecOrigins = nullptr, bool bUpdatePositions = true, float flSoundTime = 0.0f, int nSpeakerEntity = -1 ) = 0;

	virtual void	stop_sound( int nEntityIndex, int iChannel, const char* szSample, unsigned int uSoundEntryHash ) = 0;
	virtual void	stop_all_sounds( bool bClearBuffers ) = 0;
	virtual void	set_room_type( i_recipient_filter& filter, int iRoomType ) = 0;
	virtual void	set_player_dsp( i_recipient_filter& filter, int iDspType, bool bFastReset ) = 0;
	virtual void	emit_ambient_sound( const char* szSample, float flVolume, int iPitch = PITCH_NORM, int iFlags = 0, float flSoundTime = 0.0f ) = 0;
	virtual float	get_dist_gain_from_sound_level( e_sound_level soundLevel, float flDistance ) = 0;
	virtual int		get_guid_for_last_sound_emitted( ) = 0;
	virtual bool	is_sound_still_playing( int nGUID ) = 0;
	virtual void	stop_sound_by_guid( int nGUID, bool bForceSync ) = 0;
	virtual void	set_volume_by_guid( int nGUID, float flVolume ) = 0;
private:
	virtual void	function0( ) = 0;
public:
	virtual void	get_active_sounds( c_utl_vector<sound_info_t>& vecSoundList ) = 0;
	virtual void	precache_sentence_group( const char* szGroupName ) = 0;
	virtual void	notify_begin_movie_playback( ) = 0;
	virtual void	notify_end_movie_playback( ) = 0;
	virtual bool	get_sound_channel_volume( const char* szSound, float& flVolumeLeft, float& flVolumeRight ) = 0;
	virtual float	get_elapsed_time_by_guid( int nGUID ) = 0;
	virtual bool	get_prevent_sound( ) = 0;
	virtual void	set_replay_sound_fade( float flReplayVolume ) = 0;
	virtual float	get_replay_sound_fade( ) const = 0;
};