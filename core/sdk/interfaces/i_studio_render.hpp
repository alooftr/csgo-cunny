#pragma once
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/istudiorender.h
#include "i_material_context.hpp"

#pragma region studiorender_enumerations
enum
{
	studiorender_draw_entire_model = 0,
	studiorender_draw_opaque_only = 0x01,
	studiorender_draw_translucent_only = 0x02,
	studiorender_draw_group_mask = 0x03,
	studiorender_draw_no_flexes = 0x04,
	studiorender_draw_static_lighting = 0x08,
	studiorender_draw_accuratetime = 0x10,
	studiorender_draw_no_shadows = 0x20,
	studiorender_draw_get_perf_stats = 0x40,
	studiorender_draw_wireframe = 0x80,
	studiorender_draw_item_blink = 0x100,
	studiorender_shadowdepthtexture = 0x200,
	studiorender_ssaodepthtexture = 0x1000,
	studiorender_generate_stats = 0x8000,
};

enum e_override_type : int
{
	override_normal = 0,
	override_build_shadows,
	override_depth_write,
	override_ssao_depth_write
};
#pragma endregion

struct material_lighting_state_t
{
	vec3_t			vecAmbientCube[ 6 ];
	vec3_t		vecLightingOrigin;
	int				nLocalLightCount;
	light_desc_t		localLightDesc[ 4 ];
};

struct draw_model_results_t;
struct color_mesh_info_t;
struct studio_decal_handle_t { int iUnused; };
struct draw_model_info_t
{
	studiohdr_t* pStudioHdr;
	studiohwdata_t* pHardwareData;
	studio_decal_handle_t hDecals;
	int						iSkin;
	int						iBody;
	int						iHitboxSet;
	i_client_renderable* pClientEntity;
	int						iLOD;
	color_mesh_info_t* pColorMeshes;
	bool					bStaticLighting;
	material_lighting_state_t lightingState;
};

class i_studio_render
{
public:
	void set_ambient_light_colors( const vec4_t* ambient_only_colors )
	{
		utils::call_vfunc<void>( this, 20, ambient_only_colors );
	}

	void set_local_lights( int32_t num_lights, const void* lights )
	{
		utils::call_vfunc<void>( this, 22, num_lights, lights );
	}

	void set_color_modulation( float const* arrColor )
	{
		utils::call_vfunc<void>( this, 27, arrColor );
	}

	void set_alpha_modulation( float flAlpha )
	{
		utils::call_vfunc<void>( this, 28, flAlpha );
	}

	void forced_material_override( i_material* pMaterial, e_override_type nOverrideType = override_normal, int nOverrides = 0 )
	{
		utils::call_vfunc<void>( this, 33, pMaterial, nOverrideType, nOverrides );
	}
};
