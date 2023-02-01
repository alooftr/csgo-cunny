#pragma once
// used: std::function
#include <functional>

// used: matrix3x4_t
#include "../data_types/matrix.hpp"
#include "../data_types/vector.hpp"
// used: utlvector
#include "../data_types/utl_vector.hpp"

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/engine/IEngineTrace.h

#define CONTENTS_EMPTY					0
#define CONTENTS_SOLID					0x1
#define CONTENTS_WINDOW					0x2
#define CONTENTS_AUX					0x4
#define CONTENTS_GRATE					0x8
#define CONTENTS_SLIME					0x10
#define CONTENTS_WATER					0x20
#define CONTENTS_BLOCKLOS				0x40
#define CONTENTS_OPAQUE					0x80
#define CONTENTS_TESTFOGVOLUM			0x100
#define CONTENTS_UNUSED					0x200
#define CONTENTS_BLOCKLIGHT				0x400
#define CONTENTS_TEAM1					0x800
#define CONTENTS_TEAM2					0x1000
#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000
#define CONTENTS_MOVEABLE				0x4000
#define CONTENTS_AREAPORTAL				0x8000
#define CONTENTS_PLAYERCLIP				0x10000
#define CONTENTS_MONSTERCLIP			0x20000
#define CONTENTS_CURRENT_0				0x40000
#define CONTENTS_CURRENT_90				0x80000
#define CONTENTS_CURRENT_180			0x100000
#define CONTENTS_CURRENT_270			0x200000
#define CONTENTS_CURRENT_UP				0x400000
#define CONTENTS_CURRENT_DOWN			0x800000
#define CONTENTS_ORIGIN					0x1000000
#define CONTENTS_MONSTER				0x2000000
#define CONTENTS_DEBRIS					0x4000000
#define CONTENTS_DETAIL					0x8000000
#define CONTENTS_TRANSLUCENT			0x10000000
#define CONTENTS_LADDER					0x20000000
#define CONTENTS_HITBOX					0x40000000
#define LAST_VISIBLE_CONTENTS			CONTENTS_OPAQUE
#define ALL_VISIBLE_CONTENTS			(LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define SURF_LIGHT						0x0001
#define SURF_SKY2D						0x0002
#define SURF_SKY						0x0004
#define SURF_WARP						0x0008
#define SURF_TRANS						0x0010
#define SURF_NOPORTAL					0x0020
#define SURF_TRIGGER					0x0040
#define SURF_NODRAW						0x0080
#define SURF_HINT						0x0100
#define SURF_SKIP						0x0200
#define SURF_NOLIGHT					0x0400
#define SURF_BUMPLIGHT					0x0800
#define SURF_NOSHADOWS					0x1000
#define SURF_NODECALS					0x2000
#define SURF_NOCHOP						0x4000
#define SURF_HITBOX						0x8000

#define MASK_ALL						(0xFFFFFFFF)
#define MASK_SOLID						(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define MASK_PLAYERSOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define MASK_NPCSOLID					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define MASK_NPCFLUID					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define MASK_WATER						(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
#define MASK_OPAQUE						(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
#define MASK_OPAQUE_AND_NPCS			(MASK_OPAQUE|CONTENTS_MONSTER)
#define MASK_BLOCKLOS					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
#define MASK_BLOCKLOS_AND_NPCS			(MASK_BLOCKLOS|CONTENTS_MONSTER)
#define MASK_VISIBLE					(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define MASK_VISIBLE_AND_NPCS			(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define MASK_SHOT						(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
#define MASK_SHOT_BRUSHONLY				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
#define MASK_SHOT_HULL					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
#define MASK_SHOT_PORTAL				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define MASK_SOLID_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
#define MASK_PLAYERSOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
#define MASK_NPCSOLID_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define MASK_NPCWORLDSTATIC				(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define MASK_NPCWORLDSTATIC_FLUID		(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
#define MASK_SPLITAREAPORTAL			(CONTENTS_WATER|CONTENTS_SLIME)
#define MASK_CURRENT					(CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)
#define MASK_DEADSOLID					(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)


#pragma region enginetrace_enumerations
enum e_disp_surf_flags : int
{
	dispsurf_flag_surface = (1 << 0),
	dispsurf_flag_walkable = (1 << 1),
	dispsurf_flag_buildable = (1 << 2),
	dispsurf_flag_surfprop1 = (1 << 3),
	dispsurf_flag_surfprop2 = (1 << 4)
};

enum e_trace_type : int
{
	trace_everything = 0,
	trace_world_only,
	trace_entities_only,
	trace_everything_filter_props,
	trace_filtersky
};

enum e_debug_trace_counter_behavior : int
{
	trace_counter_set = 0,
	trace_counter_inc,
};
#pragma endregion

struct brush_side_info_t
{
	vector_aligned	plane;	// the plane of the brush side
	std::uint16_t	bevel;		// bevel plane?
	std::uint16_t	thin;		// thin?
};

struct cplane_t
{
	vec3_t		normal;
	float		distance;
	std::byte	type;
	std::byte	sign_bits;
	std::byte	pad[ 0x2 ];
};

struct csurface_t
{
	const char* name;
	short			surface_props;
	std::uint16_t flags;
};

class c_base_trace
{
public:
	c_base_trace( ) { }

	vec3_t			start;		// start position
	vec3_t			end;			// final position
	cplane_t		plane;			// surface normal at impact
	float			fraction;		// time completed, 1.0 = didn't hit anything
	int				contents;		// contents on other side of surface hit
	std::uint16_t	disp_flags;		// displacement flags for marking surfaces with data
	bool			all_solid;		// if true, plane is not valid
	bool			start_solid;	// if true, the initial point was in a solid area
};

class c_base_entity;
class c_game_trace : public c_base_trace
{
public:
	c_game_trace( ) : hit_entity( nullptr ) { }

	float				fraction_left_solid;	// time we left a solid, only valid if we started in solid
	csurface_t			surface;				// surface hit (impact surface)
	int					hit_group;				// 0 == generic, non-zero is specific body part
	short				physics_bone;			// physics bone hit by trace in studio
	std::uint16_t		world_surface_index;		// index of the msurface2_t, if applicable
	c_base_entity* hit_entity;				// entity hit by trace
	int					hitbox;				// box hit by trace in studio

	inline bool did_hit( ) const
	{
		return (fraction < 1.0f || all_solid || start_solid);
	}

	inline bool is_visible( ) const
	{
		return (fraction > 0.97f);
	}

	inline void clear( ) {
		std::memset( this, 0, sizeof( c_game_trace ) );

		this->fraction = 1.f;
		this->surface.name = "**empty**";
	}
private:
	c_game_trace( const c_game_trace& other )
	{
		this->start = other.start;
		this->end = other.end;
		this->plane = other.plane;
		this->fraction = other.fraction;
		this->contents = other.contents;
		this->disp_flags = other.disp_flags;
		this->all_solid = other.all_solid;
		this->start_solid = other.start_solid;
		this->fraction_left_solid = other.fraction_left_solid;
		this->surface = other.surface;
		this->hit_group = other.hit_group;
		this->physics_bone = other.physics_bone;
		this->world_surface_index = other.world_surface_index;
		this->hit_entity = other.hit_entity;
		this->hitbox = other.hitbox;
	}
};

using trace_t = c_game_trace;

struct ray_t
{
	ray_t( const vec3_t& vecStart, const vec3_t& end_pos ) :
		start( vecStart ), delta( end_pos - vecStart ), world_axis_transform( nullptr ), is_ray( true )
	{
		this->is_swept = (this->delta.length_sqr( ) != 0.f);
	}

	ray_t( const vec3_t& vecStart, const vec3_t& end_pos, const vec3_t& vecMins, const vec3_t& vecMaxs )
	{
		this->delta = end_pos - vecStart;

		this->world_axis_transform = nullptr;
		this->is_swept = (this->delta.length_sqr( ) != 0.f);

		this->extents = vecMaxs - vecMins;
		this->extents *= 0.5f;
		this->is_ray = (this->extents.length_sqr( ) < 1e-6f);

		this->start_offset = vecMins + vecMaxs;
		this->start_offset *= 0.5f;
		this->start = vecStart + this->start_offset;
		this->start_offset *= -1.0f;
	}

	vector_aligned		start;
	vector_aligned		delta;
	vector_aligned		start_offset;
	vector_aligned		extents;
	const matrix3x4_t* world_axis_transform;
	bool				is_ray;
	bool				is_swept;
};

class i_handle_entity;
class i_trace_filter
{
public:
	virtual bool should_hit_entity( i_handle_entity* entity, int fContentsMask ) = 0;
	virtual e_trace_type get_trace_type( ) const = 0;
};

class c_trace_filter : public i_trace_filter
{
	using filter_callback_fn = std::function<bool( i_handle_entity*, int )>;

public:
	// @todo: sig ctracefiltersimple constructor and use it

	c_trace_filter( const i_handle_entity* pSkientity, e_trace_type iTraceType = trace_everything )
		: skip( pSkientity ), trace_type( iTraceType ) { }

	c_trace_filter( filter_callback_fn&& checkCallback, e_trace_type iTraceType = trace_everything )
		: check_callback( std::move( checkCallback ) ), trace_type( iTraceType ) { }

	bool should_hit_entity( i_handle_entity* pHandleEntity, int fContentsMask ) override
	{
		// if given user defined callback - check it
		if (check_callback != nullptr)
			return check_callback( pHandleEntity, fContentsMask );

		// otherwise skip entity if given
		return pHandleEntity != skip;
	}

	e_trace_type get_trace_type( ) const override
	{
		return trace_type;
	}

private:
	const i_handle_entity* skip = nullptr;
	filter_callback_fn check_callback = nullptr;
	e_trace_type trace_type = trace_everything;
};

class c_trace_world_only : public i_trace_filter
{
public:
	bool should_hit_entity( i_handle_entity* pHandleEntity, int fContentsMask ) override
	{
		return false;
	}

	e_trace_type get_trace_type( ) const
	{
		return trace_everything;
	}

	void* pSkip1;
};

class i_trace_list_data
{
public:
	virtual			~i_trace_list_data( ) { }
	virtual void	reset( ) = 0;
	virtual bool	is_empty( ) = 0;
	virtual bool	can_trace_ray( const ray_t& ray ) = 0;
};

class i_entity_enumerator
{
public:
	// this gets called with each handle
	virtual bool enum_entity( i_handle_entity* pHandleEntity ) = 0;
};

struct virtualmeshlist_t;
struct aabb_t;
class i_collideable;
class c_phys_collide;
class c_brush_query;
class i_engine_trace
{
public:
	virtual int get_point_contents( const vec3_t& vecAbsPosition, int fContentsMask = MASK_ALL, i_handle_entity** pentity = nullptr ) = 0;
	virtual int get_point_contents_world_only( const vec3_t& vecAbsPosition, int fContentsMask = MASK_ALL ) = 0;
	virtual int get_point_contents_collideable( i_collideable* pCollide, const vec3_t& vecAbsPosition ) = 0;
	virtual void clip_ray_to_entity( const ray_t& ray, unsigned int fMask, i_handle_entity* entity, trace_t* pTrace ) = 0;
	virtual void clip_ray_to_collideable( const ray_t& ray, unsigned int fMask, i_collideable* pCollide, trace_t* pTrace ) = 0;
	virtual void trace_ray( const ray_t& ray, unsigned int fMask, i_trace_filter* pTraceFilter, trace_t* pTrace ) = 0;
	virtual void setup_leaf_and_entity_list_ray( const ray_t& ray, i_trace_list_data* pTraceData ) = 0;
	virtual void setup_leaf_and_entity_list_box( const vec3_t& vecBoxMin, const vec3_t& vecBoxMax, i_trace_list_data* pTraceData ) = 0;
	virtual void trace_ray_against_leaf_and_entity_list( const ray_t& ray, i_trace_list_data* pTraceData, unsigned int fMask, i_trace_filter* pTraceFilter, trace_t* pTrace ) = 0;
	virtual void sweep_collideable( i_collideable* pCollide, const vec3_t& vecAbsStart, const vec3_t& vecAbsEnd, const vec3_t& vecAngles, unsigned int fMask, i_trace_filter* pTraceFilter, trace_t* pTrace ) = 0;
	virtual void enumerate_entities( const ray_t& ray, bool bTriggers, i_entity_enumerator* pEnumerator ) = 0;
	virtual void enumerate_entities( const vec3_t& vecAbsMins, const vec3_t& vecAbsMaxs, i_entity_enumerator* pEnumerator ) = 0;
	virtual i_collideable* get_collideable( i_handle_entity* entity ) = 0;
	virtual int get_stat_by_index( int index, bool bClear ) = 0;
	virtual void get_brushes_in_aabb( const vec3_t& vecMins, const vec3_t& vecMaxs, c_utl_vector<int>* pOutput, int fContentsMask = MASK_ALL ) = 0;
	virtual c_phys_collide* get_collidable_from_displacements_in_aabb( const vec3_t& vecMins, const vec3_t& vecMaxs ) = 0;
	virtual int get_num_displacements( ) = 0;
	virtual void get_displacement_mesh( int index, virtualmeshlist_t* pMeshTriList ) = 0;
	virtual bool get_brush_info( int iBrush, c_utl_vector<brush_side_info_t>* pBrushSideInfoOut, int* pContentsOut ) = 0;
	virtual bool point_outside_world( const vec3_t& vecPoint ) = 0;
	virtual int get_leaf_containing_point( const vec3_t& vecPoint ) = 0;
	virtual i_trace_list_data* alloc_trace_list_data( ) = 0;
	virtual void free_trace_list_data( i_trace_list_data* pListData ) = 0;
	virtual int get_set_debug_trace_counter( int iValue, e_debug_trace_counter_behavior behavior ) = 0;
	virtual int get_meshes_from_displacements_in_aabb( const vec3_t& vecMins, const vec3_t& vecMaxs, virtualmeshlist_t* pOutputMeshes, int nMaxOutputMeshes ) = 0;
	virtual void get_brushes_in_collideable( i_collideable* pCollideable, c_brush_query& BrushQuery ) = 0;
	virtual bool is_fully_occluded( int nOcclusionKey, const aabb_t& aabb1, const aabb_t& aabb2, const vec3_t& vecShadow ) = 0;
	virtual void suspend_occlusion_tests( ) = 0;
	virtual void resume_occlusion_tests( ) = 0;
	virtual void flush_occlusion_queries( ) = 0;

public:
	void trace_line( const vec3_t& src, const vec3_t& dst, int mask, i_handle_entity* ent, int collision_group, trace_t* trace );
	void trace_hull( const vec3_t& src, const vec3_t& dst, const vec3_t& mins, const vec3_t& maxs, int mask, i_handle_entity* ent, int collision_group, trace_t* trace );
};
