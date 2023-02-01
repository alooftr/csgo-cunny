#pragma once
#include "../data_types/vector.hpp"
#include "../data_types/matrix.hpp"
#include "i_client_entity.hpp"

#pragma region renderbeams_definitions
#define	TE_BEAMPOINTS			0	// beam effect between two points
#define TE_SPRITE				1	// additive sprite, plays 1 cycle
#define TE_BEAMDISK				2	// disk that expands to max radius over lifetime
#define TE_BEAMCYLINDER			3	// cylinder that expands to max radius over lifetime
#define TE_BEAMFOLLOW			4	// create a line of decaying beam segments until entity stops moving
#define TE_BEAMRING				5	// connect a beam ring to two entities
#define TE_BEAMSPLINE			6	
#define TE_BEAMRINGPOINT		7	
#define	TE_BEAMLASER			8	// fades according to viewpoint
#define TE_BEAMTESLA			9
#define MAX_BEAM_ENTS			10
#define NOISE_DIVISIONS			128
#pragma endregion

enum e_beam_type : unsigned int
{
	fbeam_startentity = 0x00000001,
	fbeam_endentity = 0x00000002,
	fbeam_fadein = 0x00000004,
	fbeam_fadeout = 0x00000008,
	fbeam_sinenoise = 0x00000010,
	fbeam_solid = 0x00000020,
	fbeam_shadein = 0x00000040,
	fbeam_shadeout = 0x00000080,
	fbeam_onlynoiseonce = 0x00000100,	// only calculate our noise once
	fbeam_notile = 0x00000200,
	fbeam_use_hitboxes = 0x00000400,	// attachment indices represent hitbox indices instead when this is set.
	fbeam_startvisible = 0x00000800,	// has this client actually seen this beam's start entity yet?
	fbeam_endvisible = 0x00001000,	// has this client actually seen this beam's end entity yet?
	fbeam_isactive = 0x00002000,
	fbeam_forever = 0x00004000,
	fbeam_halobeam = 0x00008000,	// when drawing a beam with a halo, don't ignore the segments and endwidth
	fbeam_reversed = 0x00010000,
};

struct beam_trail_t
{
	beam_trail_t* pNext;
	float			flDie;
	vec3_t			vecOrigin;
	vec3_t vecVelocity;
};

struct beam_t
{
	beam_t( ) = default;

	// Methods of IClientRenderable
	virtual const vec3_t& get_render_origin( ) = 0;
	virtual const vec3_t& get_render_angles( ) = 0;
	virtual const matrix3x4_t& renderable_to_world_transform( ) = 0;
	virtual void			get_render_bounds( vec3_t& vecMins, vec3_t& vecMaxs ) = 0;
	virtual bool			should_draw( ) = 0;
	virtual bool			is_transparent( ) = 0;
	virtual int				draw_model( int nFlags ) = 0;
	virtual void			compute_fx_blend( ) = 0;
	virtual int				get_fx_blend( ) = 0;

	vec3_t			vecMins;
	vec3_t			vecMaxs;
	int* pQueryHandleHalo;
	float			flHaloProxySize;
	beam_t* pNext;
	int				nType;
	int				nFlags;

	// Control points for the beam
	int				nAttachments;
	vec3_t			vecAttachment[ MAX_BEAM_ENTS ];
	vec3_t			vecDelta;

	// 0 .. 1 over lifetime of beam
	float			flTime;
	float			flFrequence;

	// Time when beam should die
	float			flDie;
	float			flWidth;
	float			flEndWidth;
	float			flFadeLength;
	float			flAmplitude;
	float			flLife;

	// Color
	float			r, g, b;
	float			flBrightness;

	// Speed
	float			flSpeed;

	// Animation
	float			flFrameRate;
	float			flFrame;
	int				nSegments;

	// Attachment entities for the beam
	c_base_handle hEntity[ MAX_BEAM_ENTS ];
	int				nAttachmentIndex[ MAX_BEAM_ENTS ];

	// Model info
	int				nModelIndex;
	int				nHaloIndex;
	float			flHaloScale;
	int				iFrameCount;
	float			flRgNoise[ NOISE_DIVISIONS + 1 ];

	// Popcorn trail for beam follows to use
	beam_trail_t* pTrail;

	// for TE_BEAMRINGPOINT
	float			flStartRadius;
	float			flEndRadius;

	// for FBEAM_ONLYNOISEONCE
	bool			bCalculatedNoise;
	float			flHDRColorScale;
};

struct beam_info_t
{
	beam_info_t( )
	{
		nType = TE_BEAMPOINTS;
		nSegments = -1;
		pszModelName = nullptr;
		pszHaloName = nullptr;
		nModelIndex = -1;
		nHaloIndex = -1;
		bRenderable = true;
		nFlags = 0;
	}

	int				nType;

	// Entities
	c_base_entity* pStartEntity;
	int				iStartAttachment;
	c_base_entity* pEndEntity;
	int				iEndAttachment;

	// Points
	vec3_t			vecStart;
	vec3_t			vecEnd;

	int				nModelIndex;
	const char* pszModelName;
	int				nHaloIndex;
	const char* pszHaloName;
	float			flHaloScale;
	float			flLife;
	float			flWidth;
	float			flEndWidth;
	float			flFadeLength;
	float			flAmplitude;
	float			flBrightness;
	float			flSpeed;
	int				iStartFrame;
	float			flFrameRate;
	float			flRed;
	float			flGreen;
	float			flBlue;
	bool			bRenderable;
	int				nSegments;
	int				nFlags;

	// Rings
	vec3_t			vecCenter;
	float			flStartRadius;
	float			flEndRadius;
};

class c_beam;
class i_view_render_beams
{
public:
	virtual void	init_beams( ) = 0;
	virtual void	shutdown_beams( ) = 0;
	virtual void	clear_beams( ) = 0;
	virtual void	update_temp_ent_beams( ) = 0;

	virtual void	draw_beam( c_beam* pBeam, const renderable_instance_t& instance, i_trace_filter* pEntityBeamTraceFilter = nullptr ) = 0;
	virtual void	draw_beam( beam_t* pBeam ) = 0;

	virtual void	kill_dead_beams( c_base_entity* pEntity ) = 0;

	virtual beam_t* create_beam_ents( beam_info_t& beamInfo ) = 0;
	virtual beam_t* create_beam_ent_point( beam_info_t& beamInfo ) = 0;
	virtual	beam_t* create_beam_points( beam_info_t& beamInfo ) = 0;
	virtual beam_t* create_beam_ring( beam_info_t& beamInfo ) = 0;
	virtual beam_t* create_beam_ring_point( beam_info_t& beamInfo ) = 0;
	virtual beam_t* create_beam_circle_points( beam_info_t& beamInfo ) = 0;
	virtual beam_t* create_beam_follow( beam_info_t& beamInfo ) = 0;

	virtual void	free_beam( beam_t* pBeam ) = 0;
	virtual void	update_beam_info( beam_t* pBeam, beam_info_t& beamInfo ) = 0;

	virtual void	create_beam_ents( int iStartEntity, int iEndEntity, int nModelIndex, int iHaloIndex, float flHaloScale,
		float flLife, float flWidth, float flEndWidth, float flFadeLength, float flAmplitude,
		float flBrightness, float flSpeed, int flStartFrame,
		float flFrameRate, float r, float g, float b, int iType = -1 ) = 0;
	virtual void	create_beam_ent_point( int iStartEntity, const vec3_t* pStart, int iEndEntity, const vec3_t* pEnd,
		int nModelIndex, int iHaloIndex, float flHaloScale,
		float flLife, float flWidth, float flEndWidth, float flFadeLength, float flAmplitude,
		float flBrightness, float flSpeed, int iStartFrame,
		float flFrameRate, float r, float g, float b ) = 0;
	virtual void	create_beam_points( vec3_t& vecStart, vec3_t& vecEnd, int nModelIndex, int iHaloIndex, float flHaloScale,
		float flLife, float flWidth, float flEndWidth, float flFadeLength, float flAmplitude,
		float flBrightness, float flSpeed, int iStartFrame,
		float flFrameRate, float r, float g, float b ) = 0;
	virtual void	create_beam_ring( int iStartEntity, int iEndEntity, int nModelIndex, int iHaloIndex, float flHaloScale,
		float flLife, float flWidth, float flEndWidth, float flFadeLength, float flAmplitude,
		float flBrightness, float flSpeed, int iStartFrame,
		float flFrameRate, float r, float g, float b, int iFlags = 0 ) = 0;
	virtual void	create_beam_ring_point( const vec3_t& vecCenter, float flStartRadius, float flEndRadius, int nModelIndex, int iHaloIndex, float flHaloScale,
		float flLife, float flWidth, float flEndWidth, float flFadeLength, float flAmplitude,
		float flBrightness, float flSpeed, int iStartFrame,
		float flFrameRate, float r, float g, float b, int iFlags = 0 ) = 0;
	virtual void	create_beam_circle_points( int iType, vec3_t& vecStart, vec3_t& vecEnd,
		int nModelIndex, int iHaloIndex, float flHaloScale, float flLife, float flWidth,
		float flEndWidth, float flFadeLength, float flAmplitude, float flBrightness, float flSpeed,
		int iStartFrame, float flFrameRate, float r, float g, float b ) = 0;
	virtual void	create_beam_follow( int startEnt, int nModelIndex, int iHaloIndex, float flHaloScale,
		float flLife, float flWidth, float flEndWidth, float flFadeLength, float r, float g, float b,
		float flBrightness ) = 0;
};