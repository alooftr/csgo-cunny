#pragma once

#include "../data_types/matrix.hpp"
#include "../data_types/utl_vector.hpp"

using RadianEuler = float[ 3 ];
using Quaternion = float[ 4 ];

#pragma region studio_definitions
#define MAXSTUDIOBONECTRLS			4
#define MAXSTUDIOPOSEPARAM			24
#define MAXSTUDIOSKINS				32		// total textures
#define MAXSTUDIOFLEXCTRL			96		// maximum number of flexcontrollers (input sliders)
#define MAXSTUDIOBONES				128		// total bones actually used
#define MAXSTUDIOANIMBLOCKS			256
#define MAXSTUDIOFLEXDESC			1024	// maximum number of low level flexes (actual morph targets)

#define BONE_CALCULATE_MASK			0x1F
#define BONE_PHYSICALLY_SIMULATED	0x01
#define BONE_PHYSICS_PROCEDURAL		0x02
#define BONE_ALWAYS_PROCEDURAL		0x04
#define BONE_SCREEN_ALIGN_SPHERE	0x08
#define BONE_SCREEN_ALIGN_CYLINDER	0x10

#define BONE_USED_MASK				0x0007FF00
#define BONE_USED_BY_ANYTHING		0x0007FF00
#define BONE_USED_BY_HITBOX			0x00000100
#define BONE_USED_BY_ATTACHMENT		0x00000200
#define BONE_USED_BY_VERTEX_MASK	0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0	0x00000400
#define BONE_USED_BY_VERTEX_LOD1	0x00000800
#define BONE_USED_BY_VERTEX_LOD2	0x00001000
#define BONE_USED_BY_VERTEX_LOD3	0x00002000
#define BONE_USED_BY_VERTEX_LOD4	0x00004000
#define BONE_USED_BY_VERTEX_LOD5	0x00008000
#define BONE_USED_BY_VERTEX_LOD6	0x00010000
#define BONE_USED_BY_VERTEX_LOD7	0x00020000
#define BONE_USED_BY_BONE_MERGE		0x00040000
#define BONE_ALWAYS_SETUP			0x00080000

#define BONE_USED_BY_VERTEX_AT_LOD( iLOD ) ( BONE_USED_BY_VERTEX_LOD0 << ( iLOD ) )
#define BONE_USED_BY_ANYTHING_AT_LOD( iLOD ) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD( iLOD ) )

#define MAX_NUM_LODS				8

#define BONE_TYPE_MASK				0x00F00000
#define BONE_FIXED_ALIGNMENT		0x00100000
#define BONE_HAS_SAVEFRAME_POS		0x00200000
#define BONE_HAS_SAVEFRAME_ROT64	0x00400000
#define BONE_HAS_SAVEFRAME_ROT32	0x00800000
#pragma endregion

struct vcollide_t;
struct virtualmodel_t;
struct vertexfileheader_t;

struct studiohdr_t;
class c_studio_hdr
{
public:
	studiohdr_t* studio_hdr;
	virtualmodel_t* virtual_model;
	void* softbody;
	mutable c_utl_vector<const studiohdr_t*> studio_hdr_cache;
	mutable int		mutable_frame_unlock_counter;
	int* pointer_frame_unlock_counter;
	std::byte		pad0[ 0x8 ];
	c_utl_vector<int>	bone_flags;
	c_utl_vector<int>	bone_parent;
	void* activity_to_sequence;
};

struct mstudiobone_t
{
	int			name_index;
	int			parent;			// parent bone
	int			bone_controller[ 6 ]; // bone controller index, -1 == none

	// default values
	vec3_t	position;
	Quaternion	world;
	RadianEuler rad_rotation;

	// compression scale
	vec3_t	position_scale;
	vec3_t	euler_scale;

	matrix3x4_t pose_to_bone;
	Quaternion	alignment;
	int			flags;
	int			procedure_type;
	int			procedure_index;	// procedural rule
	mutable int physics_bone;		// index into physically simulated bone
	int			surface_prop_index;	// index into string table for property name
	int			contents;			// see bspflags.h for the contents flags
	int			surface_prop;		// this index must be cached by the loader, not saved in the file
	std::byte	pad0[ 0x1C ];

	inline const char* get_name( )
	{
		if (!name_index) return nullptr;
		return (char*)this + name_index;
	}

	inline void* GetProcedure( ) const
	{
		if (!procedure_index) return nullptr;
		return (std::uint8_t*)this + procedure_index;
	}

	inline const char* get_surface_prop_name( ) const
	{
		if (!surface_prop_index) return nullptr;
		return (char*)this + surface_prop_index;
	}
};

struct mstudiobonecontroller_t
{
	int			bone; // -1 == 0
	int			type; // X, Y, Z, XR, YR, ZR, M
	float		start;
	float		end;
	int			rest; // byte index value at rest
	int			input_field; // 0-3 user set controller, 4 mouth
	std::byte	pad0[ 0x40 ];
};

struct mstudiobbox_t
{
	int			bone;
	int			group; // intersection group
	vec3_t	bb_min; // bounding box
	vec3_t	bb_max;
	int			hitbox_name_index; // offset to the name of the hitbox
	vec3_t	offset_orientation;
	float		radius;
	std::byte	pad0[ 0x10 ];

	inline const char* get_hitbox_name( ) const
	{
		if (!hitbox_name_index) return nullptr;
		return (const char*)this + hitbox_name_index;
	}
};

struct mstudiohitboxset_t
{
	int name_index;
	int hitboxes;
	int hitbox_index;

	inline const char* get_name( ) const
	{
		if (!name_index) return nullptr;
		return (char*)this + name_index;
	}

	inline mstudiobbox_t* get_hitbox( int iHitBox ) const
	{
		if (iHitBox < 0 || iHitBox >= hitboxes) return nullptr;
		return (mstudiobbox_t*)((std::uint8_t*)this + hitbox_index) + iHitBox;
	}
};

class virtualgroup_t
{
public:
	void* pCache;
	c_utl_vector<int> bone_map;
	c_utl_vector<int> master_bone;
	c_utl_vector<int> master_sequence;
	c_utl_vector<int> master_anim;
	c_utl_vector<int> master_attachment;
	c_utl_vector<int> master_pose;
	c_utl_vector<int> master_node;
};

struct virtualsequence_t
{
	int	flags;
	int activity;
	int group;
	int index;
};

struct virtualgeneric_t
{
	int group;
	int index;
};

struct virtualmodel_t
{
	inline virtualgroup_t* get_anim_group( const int animation )
	{
		// Note: user must manage mutex for this
		return &group[ anim[ animation ].group ];
	}

	inline virtualgroup_t* GetSequenceGroup( const int sequence_number )
	{
		// Note: user must manage mutex for this
		return &group[ sequence[ sequence_number ].group ];
	}

	std::byte pad0[ 0x8 ]; // CThreadFastMutex
	c_utl_vector<virtualsequence_t> sequence;
	c_utl_vector<virtualgeneric_t> anim;
	c_utl_vector<virtualgeneric_t> attachment;
	c_utl_vector<virtualgeneric_t> pose;
	c_utl_vector<virtualgroup_t> group;
	c_utl_vector<virtualgeneric_t> node;
	c_utl_vector<virtualgeneric_t> i_k_lock;
	c_utl_vector<unsigned short> autoplay_sequences;
};

struct studiohdr_t;
struct mstudioseqdesc_t
{
	int base_studio;
	inline studiohdr_t* get_studio_hdr( ) const
	{
		return (studiohdr_t*)((std::uint8_t*)this + base_studio);
	}

	int	label_index;
	inline const char* get_label( ) const
	{
		return (char*)this + label_index;
	}

	int	activity_name_index;
	inline const char* get_activity_name( ) const
	{
		return (char*)this + activity_name_index;
	}

	int	flags;

	int	activity;
	int	activity_weight;

	int events;
	int event_index;
	inline void* GetEvent( int iEvent ) const
	{
		if (iEvent < 0 || iEvent >= events) return nullptr;
		return ((std::uint8_t*)this + event_index) + iEvent;
	}

	vec3_t bb_min;
	vec3_t bb_max;

	int	blends;
	int	blend_index;

	inline int get_anim_value( int x, int y ) const
	{
		if (x >= group_size[ 0 ])
			x = group_size[ 0 ] - 1;

		if (y >= group_size[ 1 ])
			y = group_size[ 1 ] - 1;

		int offset = y * group_size[ 0 ] + x;
		short* blends = (short*)((std::uint8_t*)this + blend_index);
		return (int)blends[ offset ];
	}

	int	movement_index;		// [blend] float array for blended movement
	int group_size[ 2 ];
	int param_index[ 2 ];		// X, Y, Z, XR, YR, ZR
	float param_start[ 2 ];	// local (0..1) starting value
	float param_end[ 2 ];	// local (0..1) ending value
	int	param_parent;

	float fade_in_time;		// ideal cross fate in time (0.2 default)
	float fade_out_time;	// ideal cross fade out time (0.2 default)

	int	entry_node;			// transition node at entry
	int	exit_node;			// transition node at exit
	int	node_flags;			// transition rules

	float entry_phase;		// used to match entry gait
	float exit_phase;		// used to match exit gait

	float last_frame;		// frame that should generation EndOfSequence

	int	next_sequence;		// auto advancing sequences
	int	pose;				// index of delta animation between end and nextseq

	int	i_k_rules;

	int	auto_layers;
	int	auto_layer_index;
	inline void* get_auto_layer( int add_auto_layer ) const
	{
		if (add_auto_layer < 0 || add_auto_layer >= auto_layers) return nullptr;
		return ((std::uint8_t*)this + auto_layer_index) + add_auto_layer;
	}

	int	weight_list_index;
	inline float* get_bone_weight( int bone ) const
	{
		return ((float*)((std::uint8_t*)this + weight_list_index) + bone);
	}

	int	pose_key_index;
	inline float* get_pose_key( int parameter, int animation ) const
	{
		return (float*)((std::uint8_t*)this + pose_key_index) + parameter * group_size[ 0 ] + animation;
	}

	int	nIKLocks;
	int	nIKLockIndex;
	inline void* get_ik_lock( int iIK ) const
	{
		if (iIK < 0 || iIK >= nIKLocks) return nullptr;
		return ((std::uint8_t*)this + nIKLockIndex) + iIK;
	}

	int	nKeyValueIndex;
	int	iKeyValueSize;
	inline const char* key_value_text( ) const
	{
		if (!iKeyValueSize) return nullptr;
		return (char*)this + nKeyValueIndex;
	}

	int	nCyclePoseIndex;

	int nActivityModifierIndex;
	int	nActivityModifiers;

	int	nAnimTagIndex;
	int	nAnimTags;

	int	nRootDriverIndex;
	std::byte pad[ 0x8 ];
};

struct mstudioposeparamdesc_t
{
	int name_index;
	int nFlags;
	float flStart; // starting value
	float flEnd; // ending value
	float flLoop; // looping range, 0 for no looping, 360 for rotations, etc.

	inline const char* get_name( ) const
	{
		return (char*)this + name_index;
	}
};

struct studiohwdata_t;
struct studiohdr_t
{
	int	nID;
	int	iVersion;
	int iChecksum;
	char szName[ 64 ];
	int	iLength;

	vec3_t vecEyePosition;
	vec3_t vecIllumPosition;
	vec3_t vecHullMin;
	vec3_t vecHullMax;
	vec3_t vecViewBBMin;
	vec3_t vecViewBBMax;

	int	iFlags;

	int	nBones;
	int	nBoneIndex;
	inline mstudiobone_t* get_bone( int bone ) const
	{
		if (bone < 0 || bone >= nBones) return nullptr;
		return (mstudiobone_t*)((std::uint8_t*)this + nBoneIndex) + bone;
	}

	int	nBoneControllers;
	int	nBoneControllerIndex;
	inline void* get_bone_controller( int bone_indexController ) const
	{
		if (bone_indexController < 0 || bone_indexController >= nBoneControllers) return nullptr;
		return ((std::uint8_t*)this + nBoneControllerIndex) + bone_indexController;
	}

	int	nHitboxSets;
	int	nHitboxSetIndex;
	inline mstudiohitboxset_t* get_hitbox_set( int iSet ) const
	{
		if (iSet < 0 || iSet >= nHitboxSets) return nullptr;
		return (mstudiohitboxset_t*)((std::uint8_t*)this + nHitboxSetIndex) + iSet;
	}

	inline mstudiobbox_t* get_hitbox( int iHitBox, int iSet ) const
	{
		const mstudiohitboxset_t* pHitboxSet = get_hitbox_set( iSet );

		if (pHitboxSet == nullptr)
			return nullptr;

		return pHitboxSet->get_hitbox( iHitBox );
	}

	inline int get_hitbox_count( int iSet ) const
	{
		const mstudiohitboxset_t* pHitboxSet = get_hitbox_set( iSet );

		if (pHitboxSet == nullptr)
			return 0;

		return pHitboxSet->hitboxes;
	}

	int	nLocalAnimations;
	int	nLocalAnimationIndex;
	inline void* get_anim_description( int animation ) const
	{
		if (animation < 0 || animation >= nLocalAnimations) animation = 0;
		return ((std::uint8_t*)this + nLocalAnimationIndex) + animation;
	}

	int nLocalSequences;
	int nLocalSequenceIndex;
	inline mstudioseqdesc_t* get_local_sequence_description( int sequence ) const
	{
		if (sequence < 0 || sequence >= nLocalSequences) sequence = 0;
		return (mstudioseqdesc_t*)((std::uint8_t*)this + nLocalSequenceIndex) + sequence;
	}

	inline mstudioseqdesc_t& get_sequence_description( int sequence, virtualmodel_t* pVirtualModel = nullptr, const studiohdr_t* pStudioHdr = nullptr ) const
	{
		if (nIncludeModels == 0 || pVirtualModel == nullptr || pStudioHdr == nullptr)
			return *get_local_sequence_description( sequence );

		return *pStudioHdr->get_local_sequence_description( pVirtualModel->sequence[ sequence ].index );
	}

	mutable int miActivityListVersion;
	mutable int miEventsIndexed;

	int	nTextures;
	int	nTextureIndex;
	inline void* get_texture( int index ) const
	{
		if (index < 0 || index >= nTextures) return nullptr;
		return ((std::uint8_t*)this + nTextureIndex) + index;
	}

	int	nCdTextures;
	int	nCdTextureIndex;
	inline char* get_cd_texture( int index ) const
	{
		if (index < 0 || index >= nCdTextures) return nullptr;
		return (((char*)this) + *((int*)((std::uint8_t*)this + nCdTextureIndex) + index));
	}

	int	nSkinRefs;
	int	nSkinFamilies;
	int	nSkinIndex;
	inline short* get_skin_ref( int index ) const
	{
		if (index < 0 || index >= nSkinRefs) return nullptr;
		return (short*)((std::uint8_t*)this + nSkinIndex) + index;
	}

	int	nBodyParts;
	int	nBodyPartIndex;
	inline void* get_body_part( int index ) const
	{
		if (index < 0 || index >= nBodyParts) return nullptr;
		return ((std::uint8_t*)this + nBodyPartIndex) + index;
	}

	int	nAttachments;
	int	nAttachmentIndex;
	inline void* get_attachment( int index ) const
	{
		if (index < 0 || index >= nAttachments) return nullptr;
		return ((std::uint8_t*)this + nAttachmentIndex) + index;
	}

	int	nTransitions;
	int	nTransitionIndex;
	int	nTransitionNameIndex;
	inline std::uint8_t* get_transition( int index ) const
	{
		if (index < 0 || index >= (nTransitions * nTransitions)) return nullptr;
		return (std::uint8_t*)((std::uint8_t*)this + nTransitionIndex) + index;
	}

	int	nFlexDescs;
	int	nFlexDescIndex;
	inline void* get_flex_description( int index ) const
	{
		if (index < 0 || index >= nFlexDescs) return nullptr;
		return ((std::uint8_t*)this + nFlexDescIndex) + index;
	}

	int	nFlexControllers;
	int	nFlexControllerIndex;
	inline void* get_flex_controller( int index ) const
	{
		if (index < 0 || index >= nFlexControllers) return nullptr;
		return ((std::uint8_t*)this + nFlexControllerIndex) + index;
	}

	int	nFlexRules;
	int	nFlexRuleIndex;
	inline void* get_flex_rule( int index ) const
	{
		if (index < 0 || index >= nFlexRules) return nullptr;
		return ((std::uint8_t*)this + nFlexRuleIndex) + index;
	}

	int	nIkChains;
	int	nIkChainIndex;
	inline void* get_ik_chain( int index ) const
	{
		if (index < 0 || index >= nIkChains) return nullptr;
		return ((std::uint8_t*)this + nIkChainIndex) + index;
	}

	int	nMouths;
	int	nMouthIndex;
	inline void* get_mouth( int index ) const
	{
		if (index < 0 || index >= nMouths) return nullptr;
		return ((std::uint8_t*)this + nMouthIndex) + index;
	}

	int	nPoseParameters;
	int	nPoseParameterIndex;
	inline void* get_pose_parameter( int index ) const
	{
		if (index < 0 || index >= nPoseParameters) return nullptr;
		return ((std::uint8_t*)this + nPoseParameterIndex) + index;
	}

	int	nSurfacePropIndex;
	inline const char* get_surface_prop( ) const
	{
		if (!nSurfacePropIndex) return nullptr;
		return (char*)this + nSurfacePropIndex;
	}

	int	nKeyValueIndex;
	int	nKeyValueSize;
	inline const char* key_value_text( ) const
	{
		if (!nKeyValueSize) return nullptr;
		return (char*)this + nKeyValueIndex;
	}

	int	nIkAutoplayLocks;
	int	nIkAutoplayLockIndex;
	inline void* get_ik_autoplay_lock( int index ) const
	{
		if (index < 0 || index >= nIkAutoplayLocks) return nullptr;
		return ((std::uint8_t*)this + nIkAutoplayLockIndex) + index;
	}

	float flMass;
	int	iContents;

	int	nIncludeModels;
	int	nIncludeModelIndex;
	inline void* get_model_group( int index ) const
	{
		if (index < 0 || index >= nIncludeModels) return nullptr;
		return ((std::uint8_t*)this + nIncludeModelIndex) + index;
	}

	int	iVirtualModel;

	int	nAnimBlockNameIndex;
	inline const char* get_anim_block_name( ) const
	{
		return ((char*)this) + nAnimBlockNameIndex;
	}

	int	nAnimBlocks;
	int	nAnimBlockIndex;
	inline void* get_anim_block( int index ) const
	{
		if (index < 0 || index >= nAnimBlocks) return nullptr;
		return (((std::uint8_t*)this) + nAnimBlockIndex) + index;
	}

	int	iAnimBlockModel;

	int	nBoneTableByNameIndex;
	inline const std::uint8_t* get_bone_table_sorted_by_name( ) const
	{
		return (std::uint8_t*)this + nBoneTableByNameIndex;
	}

	int iVertexBase;
	int nIndexBase;

	std::uint8_t uDirectionalLightDot;
	std::uint8_t uRootLOD;
	std::uint8_t nAllowedRootLODs;
	std::byte pad0[ 0x5 ];

	int	nFlexControllerUI;
	int	nFlexcontrollerUIIndex;

	float flVertAnimFixedPointScale;
	mutable int miSurfacePropLookup;
	int	nStudioHdr2Index;

	std::byte pad1[ 0x4 ];
};
