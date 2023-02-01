#pragma once
#include "../data_types/key_values.hpp"

using vertex_format_t = std::uint64_t;

#pragma region material_enumerations
enum e_preview_image_ret_val : int
{
	material_preview_image_bad = 0,
	material_preview_image_ok,
	material_no_preview_image,
};

enum e_material_property_type : int
{
	material_property_needs_lightmap = 0,
	material_property_opacity,
	material_property_reflectivity,
	material_property_needs_bumped_lightmaps
};

enum e_material_var_flags : int
{
	material_var_debug = (1 << 0),
	material_var_no_debug_override = (1 << 1),
	material_var_no_draw = (1 << 2),
	material_var_use_in_fillrate_mode = (1 << 3),
	material_var_vertexcolor = (1 << 4),
	material_var_vertexalpha = (1 << 5),
	material_var_selfillum = (1 << 6),
	material_var_additive = (1 << 7),
	material_var_alphatest = (1 << 8),
	material_var_pseudo_translucent = (1 << 9), // used to mark water materials for rendering after opaques but before translucents (with alpha blending but also with depth writes)
	material_var_znearer = (1 << 10),
	material_var_model = (1 << 11),
	material_var_flat = (1 << 12),
	material_var_nocull = (1 << 13),
	material_var_nofog = (1 << 14),
	material_var_ignorez = (1 << 15),
	material_var_decal = (1 << 16),
	material_var_envmapsphere = (1 << 17), // obsolete
	material_var_aoprepass = (1 << 18),
	material_var_envmapcameraspace = (1 << 19), // obsolete
	material_var_basealphaenvmapmask = (1 << 20),
	material_var_translucent = (1 << 21),
	material_var_normalmapalphaenvmapmask = (1 << 22),
	material_var_needs_software_skinning = (1 << 23), // obsolete
	material_var_opaquetexture = (1 << 24),
	material_var_multiply = (1 << 25),
	material_var_suppress_decals = (1 << 26),
	material_var_halflambert = (1 << 27),
	material_var_wireframe = (1 << 28),
	material_var_allowalphatocoverage = (1 << 29),
	material_var_alpha_modified_by_proxy = (1 << 30),
	material_var_vertexfog = (1 << 31)
};

enum e_image_format
{
	image_format_unknown = -1,
	image_format_rgba8888 = 0,
	image_format_abgr8888,
	image_format_rgb888,
	image_format_bgr888,
	image_format_rgb565,
	image_format_i8,
	image_format_ia88,
	image_format_p8,
	image_format_a8,
	image_format_rgb888_bluescreen,
	image_format_bgr888_bluescreen,
	image_format_argb8888,
	image_format_bgra8888,
	image_format_dxt1,
	image_format_dxt3,
	image_format_dxt5,
	image_format_bgrx8888,
	image_format_bgr565,
	image_format_bgrx5551,
	image_format_bgra4444,
	image_format_dxt1_onebitalpha,
	image_format_bgra5551,
	image_format_uv88,
	image_format_uvwq8888,
	image_format_rgba16161616f,
	image_format_rgba16161616,
	image_format_uvlx8888,
	image_format_r32f,			// single-channel 32-bit floating point
	image_format_rgb323232f,	// note: d3d9 does not have this format
	image_format_rgba32323232f,
	image_format_rg1616f,
	image_format_rg3232f,
	image_format_rgbx8888,

	image_format_null,			// dummy format which takes no video memory

	// compressed normal map formats
	image_format_ati2n,			// one-surface ati2n / dxn format
	image_format_ati1n,			// two-surface ati1n format

	image_format_rgba1010102,	// 10 bit-per component render targets
	image_format_bgra1010102,
	image_format_r16f,			// 16 bit fp format

	// depth-stencil texture formats
	image_format_d16,
	image_format_d15s1,
	image_format_d32,
	image_format_d24s8,
	image_format_linear_d24s8,
	image_format_d24x8,
	image_format_d24x4s4,
	image_format_d24fs8,
	image_format_d16_shadow,	// specific formats for shadow mapping
	image_format_d24x8_shadow,	// specific formats for shadow mapping

	// supporting these specific formats as non-tiled for procedural cpu access (360-specific)
	image_format_linear_bgrx8888,
	image_format_linear_rgba8888,
	image_format_linear_abgr8888,
	image_format_linear_argb8888,
	image_format_linear_bgra8888,
	image_format_linear_rgb888,
	image_format_linear_bgr888,
	image_format_linear_bgrx5551,
	image_format_linear_i8,
	image_format_linear_rgba16161616,

	image_format_le_bgrx8888,
	image_format_le_bgra8888,

	num_image_formats
};
#pragma endregion

class i_material_var
{
public:
	i_texture* get_texture( )
	{
		return utils::call_vfunc<i_texture*>( this, 1 );
	}

	void set_float( float flValue )
	{
		utils::call_vfunc<void>( this, 4, flValue );
	}

	void set_int( int iValue )
	{
		utils::call_vfunc<void>( this, 5, iValue );
	}

	void set_string( const char* szValue )
	{
		utils::call_vfunc<void>( this, 6, szValue );
	}

	void set_vector( float x, float y )
	{
		utils::call_vfunc<void>( this, 10, x, y );
	}

	void set_vector( float x, float y, float z )
	{
		utils::call_vfunc<void>( this, 11, x, y, z );
	}

	void set_texture( i_texture* pTexture )
	{
		utils::call_vfunc<void>( this, 15, pTexture );
	}

	void set_vector_component( float flValue, int iComponent )
	{
		utils::call_vfunc<void>( this, 26, flValue, iComponent );
	}
};

class i_material
{
public:
	virtual const char* get_name( ) const = 0;
	virtual const char* get_texture_group_name( ) const = 0;
	virtual e_preview_image_ret_val get_preview_image_properties( int* iWidth, int* iHeight, e_image_format* pImageFormat, bool* bTranslucent ) const = 0;
	virtual e_preview_image_ret_val get_preview_image( unsigned char* pData, int iWidth, int iHeight, e_image_format imageFormat ) const = 0;
	virtual int				get_mapping_width( ) = 0;
	virtual int				get_mapping_height( ) = 0;
	virtual int				get_num_animation_frames( ) = 0;
	virtual bool			in_material_page( ) = 0;
	virtual	void			get_material_offset( float* flOffset ) = 0;
	virtual void			get_material_scale( float* flScale ) = 0;
	virtual i_material* get_material_page( ) = 0;
	virtual i_material_var* find_var( const char* szName, bool* bFound = nullptr, bool bComplain = true ) = 0;
	virtual void			increment_reference_count( ) = 0;
	virtual void			decrement_reference_count( ) = 0;
	inline void				add_reference( ) { increment_reference_count( ); }
	inline void				release( ) { decrement_reference_count( ); }
	virtual int 			get_enumeration_id( ) const = 0;
	virtual void			get_low_res_color_sample( float s, float t, float* arrColor ) const = 0;
	virtual void			recompute_state_snapshots( ) = 0;
	virtual bool			is_translucent( ) = 0;
	virtual bool			is_alpha_tested( ) = 0;
	virtual bool			is_vertex_lit( ) = 0;
	virtual vertex_format_t get_vertex_format( ) const = 0;
	virtual bool			has_proxy( ) const = 0;
	virtual bool			uses_env_cubemap( ) = 0;
	virtual bool			needs_tangent_space( ) = 0;
	virtual bool			needs_power_of_two_frame_buffer_texture( bool bCheckSpecificToThisFrame = true ) = 0;
	virtual bool			needs_full_frame_buffer_texture( bool bCheckSpecificToThisFrame = true ) = 0;
	virtual bool			needs_software_skinning( ) = 0;
	virtual void			alpha_modulate( float flAlpha ) = 0;
	virtual void			color_modulate( float r, float g, float b ) = 0;
	virtual void			set_material_var_flag( e_material_var_flags flag, bool bEnable ) = 0;
	virtual bool			get_material_var_flag( e_material_var_flags flag ) = 0;
	virtual void			get_reflectivity( vec3_t& vecReflect ) = 0;
	virtual bool			get_property_flag( e_material_property_type type ) = 0;
	virtual bool			is_two_sided( ) = 0;
	virtual void			set_shader( const char* szShaderName ) = 0;
	virtual int				get_num_passes( ) = 0;
	virtual int				get_texture_memory_bytes( ) = 0;
	virtual void			refresh( ) = 0;
	virtual bool			needs_lightmap_blend_alpha( ) = 0;
	virtual bool			needs_software_lighting( ) = 0;
	virtual int				shader_param_count( ) const = 0;
	virtual i_material_var** get_shader_params( ) = 0;
	virtual bool			is_error_material( ) const = 0;
	virtual void			set_use_fixed_function_baked_lighting( bool bEnable ) = 0;
	virtual float			get_alpha_modulation( ) = 0;
	virtual void			get_color_modulation( float* r, float* g, float* b ) = 0;
	virtual bool			is_translucent_under_modulation( float flAlphaModulation = 1.0f ) const = 0;
	virtual i_material_var* find_var_fast( char const* szName, unsigned int* puToken ) = 0;
	virtual void			set_shader_and_params( c_key_values* pKeyValues ) = 0;
	virtual const char* get_shader_name( ) const = 0;
	virtual void			delete_if_unreferenced( ) = 0;
	virtual bool			is_sprite_card( ) = 0;
	virtual void			call_bind_proxy( void* pProxyData ) = 0;
	virtual void			refresh_preserving_material_vars( ) = 0;
	virtual bool			was_reloaded_from_whitelist( ) = 0;
	virtual bool			set_temp_excluded( bool bSet, int nExcludedDimensionLimit ) = 0;
	virtual int				get_reference_count( ) const = 0;
};
