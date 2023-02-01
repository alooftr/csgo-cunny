#pragma once
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/materialsystem/imaterialsystem.h

// used: imatrendercontext, stencilstate, material, texture
#include "i_material_context.hpp"
// used: texture_group definitions
#include "../enums/enums.hpp"

#include "../data_types/key_values.hpp"
using material_handle_t = std::uint16_t;

#pragma region materialsystem_enumerations
enum e_create_render_target_flags : unsigned int
{
	createrendertargetflags_hdr = 0x00000001,
	createrendertargetflags_automipmap = 0x00000002,
	createrendertargetflags_unfilterable_ok = 0x00000004,
	createrendertargetflags_noedram = 0x00000008,
	createrendertargetflags_temp = 0x00000010
};

enum e_texture_flags : unsigned int
{
	textureflags_pointsample = 0x00000001,
	textureflags_trilinear = 0x00000002,
	textureflags_clamps = 0x00000004,
	textureflags_clampt = 0x00000008,
	textureflags_anisotropic = 0x00000010,
	textureflags_hint_dxt5 = 0x00000020,
	textureflags_srgb = 0x00000040,
	textureflags_normal = 0x00000080,
	textureflags_nomip = 0x00000100,
	textureflags_nolod = 0x00000200,
	textureflags_all_mips = 0x00000400,
	textureflags_procedural = 0x00000800,
	textureflags_onebitalpha = 0x00001000,
	textureflags_eightbitalpha = 0x00002000,
	textureflags_envmap = 0x00004000,
	textureflags_rendertarget = 0x00008000,
	textureflags_depthrendertarget = 0x00010000,
	textureflags_nodebugoverride = 0x00020000,
	textureflags_singlecopy = 0x00040000,
	textureflags_unused_00080000 = 0x00080000,
	textureflags_unused_00100000 = 0x00100000,
	textureflags_unused_00200000 = 0x00200000,
	textureflags_unused_00400000 = 0x00400000,
	textureflags_nodepthbuffer = 0x00800000,
	textureflags_unused_01000000 = 0x01000000,
	textureflags_clampu = 0x02000000,
	textureflags_vertextexture = 0x04000000,
	textureflags_ssbump = 0x08000000,
	textureflags_unused_10000000 = 0x10000000,
	textureflags_border = 0x20000000,
	textureflags_unused_40000000 = 0x40000000,
	textureflags_unused_80000000 = 0x80000000
};

enum e_clear_flags : unsigned int
{
	view_clear_color = 0x1,
	view_clear_depth = 0x2,
	view_clear_full_target = 0x4,
	view_no_draw = 0x8,
	view_clear_obey_stencil = 0x10,
	view_clear_stencil = 0x20
};

enum e_render_target_size_mode : int
{
	rt_size_no_change = 0,					// only allowed for render targets that don't want a depth buffer
	rt_size_default,						// don't play with the specified width and height other than making sure it fits in the framebuffer.
	rt_size_picmip,							// apply picmip to the render target's width and height.
	rt_size_hdr,							// frame_buffer_width / 4
	rt_size_full_frame_buffer,				// same size as frame buffer, or next lower power of 2 if we can't do that.
	rt_size_offscreen,						// target of specified size, don't mess with dimensions
	rt_size_full_frame_buffer_rounded_up,	// same size as the frame buffer, rounded up if necessary for systems that can't do non-power of two textures.
	rt_size_replay_screenshot,				// rounded down to power of 2, essentially...
	rt_size_literal							// use the size passed in. don't clamp it to the frame buffer size. really.
};

enum e_material_render_target_depth : unsigned int
{
	material_rt_depth_shared = 0x0,
	material_rt_depth_separate = 0x1,
	material_rt_depth_none = 0x2,
	material_rt_depth_only = 0x3
};
#pragma endregion

// @credits: https://github.com/pmrowla/hl2sdk-csgo/blob/master/public/materialsystem/materialsystem_config.h
struct material_video_mode_t
{
	int				iWidth;
	int				iHeight;
	e_image_format Format;
	int				iRefreshRate;
};

struct MaterialSystemConfig_t
{
	material_video_mode_t VideoMode;
	float				flMonitorGamma;
	float				flGammaTVRangeMin;
	float				flGammaTVRangeMax;
	float				flGammaTVExponent;
	bool				bGammaTVEnabled;
	bool				bTripleBuffered;
	int					nAASamples;
	int					nForceAnisotropicLevel;
	int					iSkipMipLevels;
	int					nDxSupportLevel;
	unsigned int		uFlags;
	bool				bEditMode;
	unsigned char		dProxiesTestMode;
	bool				bCompressedTextures;
	bool				bFilterLightmaps;
	bool				bFilterTextures;
	bool				bReverseDepth;
	bool				bBufferPrimitives;
	bool				bDrawFlat;
	bool				bMeasureFillRate;
	bool				bVisualizeFillRate;
	bool				bNoTransparency;
	bool				bSoftwareLighting;
	bool				bAllowCheats;
	char				nShowMipLevels;
	bool				bShowLowResImage;
	bool				bShowNormalMap;
	bool				bMipMapTextures;
	unsigned char		uFullbright;
	bool				bFastNoBump;
	bool				bSuppressRendering;
	bool				bDrawGray;
	bool				bShowSpecular;
	bool				bShowDiffuse;
	std::uint32_t		uWindowedSizeLimitWidth;
	std::uint32_t		uWindowedSizeLimitHeight;
	int					nAAQuality;
	bool				bShadowDepthTexture;
	bool				bMotionBlur;
	bool				bSupportFlashlight;
	bool				bPaintEnabled;
	std::byte			pad0[ 0xC ];
};

class i_material_system
{
public:
	e_image_format get_back_buffer_format( )
	{
		return utils::call_vfunc<e_image_format>( this, 36 );
	}

	i_material* create_material( const char* szName, c_key_values* pKeyValues )
	{
		return utils::call_vfunc<i_material*>( this, 83, szName, pKeyValues );
	}

	i_material* find_material( char const* szMaterialName, const char* szTextureGroupName = TEXTURE_GROUP_MODEL, bool bComplain = true, const char* pComplainPrefix = nullptr )
	{
		return utils::call_vfunc<i_material*>( this, 84, szMaterialName, szTextureGroupName, bComplain, pComplainPrefix );
	}

	material_handle_t first_material( )
	{
		return utils::call_vfunc<material_handle_t>( this, 86 );
	}

	material_handle_t next_material( material_handle_t hMaterial )
	{
		return utils::call_vfunc<material_handle_t>( this, 87, hMaterial );
	}

	material_handle_t invalid_material( )
	{
		return utils::call_vfunc<material_handle_t>( this, 88 );
	}

	i_material* get_material( material_handle_t hMaterial )
	{
		return utils::call_vfunc<i_material*>( this, 89, hMaterial );
	}

	int	get_num_materials( )
	{
		return utils::call_vfunc<int>( this, 90 );
	}

	i_texture* find_texture( char const* szTextureName, const char* szTextureGroupName, bool bComplain = true, int nAdditionalCreationFlags = 0 )
	{
		return utils::call_vfunc<i_texture*>( this, 91, szTextureName, szTextureGroupName, bComplain, nAdditionalCreationFlags );
	}

	void begin_render_target_allocation( )
	{
		utils::call_vfunc<void>( this, 94 );
	}

	void end_render_target_allocation( )
	{
		utils::call_vfunc<void>( this, 95 );
	}

	void force_begin_render_target_allocation( )
	{
		const bool bOldState = disable_render_target_allocation_forever( );

		disable_render_target_allocation_forever( ) = false;
		begin_render_target_allocation( );
		disable_render_target_allocation_forever( ) = bOldState;
	}

	void force_end_render_target_allocation( )
	{
		const bool bOldState = disable_render_target_allocation_forever( );

		disable_render_target_allocation_forever( ) = false;
		end_render_target_allocation( );
		disable_render_target_allocation_forever( ) = bOldState;
	}

	i_texture* create_render_target_texture( int iWidth, int iHeight, e_render_target_size_mode sizeMode, e_image_format format, e_material_render_target_depth depth = material_rt_depth_shared )
	{
		return utils::call_vfunc<i_texture*>( this, 96, iWidth, iHeight, sizeMode, format, depth );
	}

	i_texture* create_named_render_target_texture_ex( const char* szName, int iWidth, int iHeight, e_render_target_size_mode sizeMode, e_image_format format, e_material_render_target_depth depth = material_rt_depth_shared, unsigned int fTextureFlags = 0U, unsigned int fRenderTargetFlags = createrendertargetflags_hdr )
	{
		return utils::call_vfunc<i_texture*>( this, 97, szName, iWidth, iHeight, sizeMode, format, depth, fTextureFlags, fRenderTargetFlags );
	}

	i_texture* create_named_render_target_texture( const char* szName, int iWidth, int iHeight, e_render_target_size_mode sizeMode, e_image_format format, e_material_render_target_depth depth = material_rt_depth_shared, bool bClampTexCoords = true, bool bAutoMipMap = false )
	{
		return utils::call_vfunc<i_texture*>( this, 98, szName, iWidth, iHeight, sizeMode, format, depth, bClampTexCoords, bAutoMipMap );
	}

	i_texture* create_named_render_target_texture_ex2( const char* szName, int iWidth, int iHeight, e_render_target_size_mode sizeMode, e_image_format format, e_material_render_target_depth depth = material_rt_depth_shared, unsigned int fTextureFlags = 0U, unsigned int fRenderTargetFlags = createrendertargetflags_hdr )
	{
		return utils::call_vfunc<i_texture*>( this, 99, szName, iWidth, iHeight, sizeMode, format, depth, fTextureFlags, fRenderTargetFlags );
	}

	i_mat_render_context* get_render_context( )
	{
		return utils::call_vfunc<i_mat_render_context*>( this, 115 );
	}

	void finish_render_target_allocation( )
	{
		utils::call_vfunc<void>( this, 136 );
	}

	// i realize if i call this all textures will be unloaded and load time will suffer horribly
	void re_enable_render_target_allocation( )
	{
		utils::call_vfunc<void>( this, 137 );
	}

	bool& disable_render_target_allocation_forever( )
	{
		// @xref: "Tried BeginRenderTargetAllocation after game startup. If I let you do this, all users would suffer.\n"
		static auto uDisableRenderTargetAllocationForever = *reinterpret_cast<std::uintptr_t*>(utils::find_pattern( "materialsystem.dll", "80 B9 ? ? ? ? ? 74 0F" ) + 0x2);
		return *reinterpret_cast<bool*>(reinterpret_cast<std::uintptr_t>(this) + uDisableRenderTargetAllocationForever);
	}
};

class c_mat_render_context_ptr : public c_ref_ptr<i_mat_render_context>
{
	typedef c_ref_ptr<i_mat_render_context> c_base_class;
public:
	c_mat_render_context_ptr( ) = default;

	c_mat_render_context_ptr( i_mat_render_context* pInit ) : c_base_class( pInit )
	{
		if (c_base_class::pObject != nullptr)
			c_base_class::pObject->begin_render( );
	}

	c_mat_render_context_ptr( i_material_system* pFrom ) : c_base_class( pFrom->get_render_context( ) )
	{
		if (c_base_class::pObject != nullptr)
			c_base_class::pObject->begin_render( );
	}

	~c_mat_render_context_ptr( )
	{
		if (c_base_class::pObject != nullptr)
			c_base_class::pObject->end_render( );
	}

	i_mat_render_context* operator=( i_mat_render_context* pSecondContext )
	{
		if (pSecondContext != nullptr)
			pSecondContext->begin_render( );

		return c_base_class::operator=( pSecondContext );
	}

	void safe_release( )
	{
		if (c_base_class::pObject != nullptr)
			c_base_class::pObject->end_render( );

		c_base_class::safe_release( );
	}

	void assign_add_reference( i_mat_render_context* pFrom )
	{
		if (c_base_class::pObject)
			c_base_class::pObject->end_render( );

		c_base_class::assign_add_reference( pFrom );
		c_base_class::pObject->begin_render( );
	}

	void get_from( i_material_system* pFrom )
	{
		assign_add_reference( pFrom->get_render_context( ) );
	}

private:
	c_mat_render_context_ptr( const c_mat_render_context_ptr& pRefPtr );
	void operator=( const c_mat_render_context_ptr& pSecondRefPtr );
};
