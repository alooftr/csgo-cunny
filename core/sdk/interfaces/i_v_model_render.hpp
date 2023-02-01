#pragma once

#include "i_studio_render.hpp"

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/engine/ivmodelrender.h
using model_instance_handle_t = std::uint16_t;

struct model_render_info_t
{
	vec3_t					vecOrigin;
	vec3_t					angAngles;
	std::byte				pad0[ 0x4 ];
	i_client_renderable* pRenderable;
	const model_t* pModel;
	const matrix3x4_t* pModelToWorld;
	const matrix3x4_t* pLightingOffset;
	const vec3_t* pLightingOrigin;
	int						iFlags;
	int						nEntityIndex;
	int						iSkin;
	int						iBody;
	int						iHitboxSet;
	model_instance_handle_t hInstance;
};

struct renderable_info_t
{
	i_client_renderable* pRenderable;		// 0x00
	i_client_alpha_property* pAlphaProperty;		// 0x04
	int						iEnumCount;			// 0x08
	int						nRenderFrame;		// 0x0C
	unsigned short			uFirstShadow;		// 0x10
	unsigned short			uLeafList;			// 0x12
	short					sArea;				// 0x14
	std::uint16_t			uFlags;				// 0x16
	std::uint16_t			uFlags2;			// 0x18
	vec3_t					vecBloatedAbsMins;	// 0x1A
	vec3_t					vecBloatedAbsMaxs;	// 0x26
	vec3_t					vecAbsMins;			// 0x32
	vec3_t					vecAbsMaxs;			// 0x3E
	std::byte				pad0[ 0x4 ];			// 0x4A
}; // Size: 0x4E

struct studiohwdata_t;
struct studio_decal_handle_t;
struct draw_model_state_t
{
	studiohdr_t* pStudioHdr;
	studiohwdata_t* pStudioHWData;
	i_client_renderable* pRenderable;
	const matrix3x4_t* pModelToWorld;
	studio_decal_handle_t* pDecals;
	int						iDrawFlags;
	int						iLOD;
};

class iv_model_render
{
public:
	void forced_material_override( i_material* pMaterial, e_override_type overrideType = override_normal, int nOverrides = 0 )
	{
		return utils::call_vfunc<void>( this, 1, pMaterial, overrideType, nOverrides );
	}

	bool is_forced_material_override( )
	{
		return utils::call_vfunc<bool>( this, 2 );
	}

	void suppress_engine_lighting( bool suppress )
	{
		return utils::call_vfunc<void>( this, 24, suppress );
	}
};
