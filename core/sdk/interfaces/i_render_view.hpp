#pragma once

class i_mat_render_context;
class c_view_setup;
class i_texture;
struct cplane_t;

struct v_plane_t
{
	vec3_t        normal;
	vec3_t		  dist;
};
#define FRUSTUM_NUMPLANES    6
typedef v_plane_t frustum_t[ FRUSTUM_NUMPLANES ];

class i_world_render_list /*: public IRefCounted*/
{};

struct visible_fog_volume_info_t
{
    int            visible_fog_volume;
    int            visible_fog_volume_leaf;
    bool        eye_in_fog_volume;
    float       distance_to_water;
    float       water_height;
    i_material* fog_volume_material;
};

typedef unsigned short leaf_index_t;
struct world_list_leaf_data_t
{
    leaf_index_t     leaf_index;    // 16 bits
    int16_t         water_data;
    uint16_t        first_translucent_surface;    // engine-internal list index
    uint16_t        translucent_surface_count;    // count of translucent surfaces+disps
};

struct world_list_info_t
{
    int                     view_fog_volume;
    int                     leaf_count;
    bool                    has_water;
    world_list_leaf_data_t* leaf_data_list;
};

struct VisOverrideData_t
{
    vec3_t        vis_origin;                    // The point to to use as the viewpoint for area portal backface cull checks.
    float        dist_to_area_portal_tolerance;    // The distance from an area portal before using the full screen as the viewable portion.
};

struct brush_vertex_t
{
    vec3_t        pos;
    vec3_t        normal;
    vec3_t        tangent_s;
    vec3_t        tangent_t;
    vec2_t        tex_coord;
    vec2_t        lightmap_coord;

private:
    brush_vertex_t( const brush_vertex_t& src );
};

class c_brush_surface
{
public:
    // Computes texture coordinates + lightmap coordinates given a world position
    virtual void compute_texture_coordinate( vec3_t const& worldPos, vec2_t& texCoord ) = 0;
    virtual void compute_lightmap_coordinate( vec3_t const& worldPos, vec2_t& lightmapCoord ) = 0;

    // Gets the vertex data for this surface
    virtual int  get_vertex_count( ) const = 0;
    virtual void get_vertex_data( brush_vertex_t* pVerts ) = 0;

    // Gets at the material properties for this surface
    virtual i_material* get_material( ) = 0;
};

class c_brush_renderer
{
public:
    // Draws the surface; returns true if decals should be rendered on this surface
    virtual bool render_brush_model_surface( i_client_entity* pBaseEntity, c_brush_surface* pBrushSurface ) = 0;
};


class i_render_view
{
public:
    virtual void      draw_brush_model( i_client_entity* pBaseEntity, model_t* pModel, const vec3_t& vecOrigin, const vec3_t& angView, bool bSort ) = 0;
    virtual void      draw_identity_brush_model( i_world_render_list* pList, model_t* pModel ) = 0;
    virtual void      touch_light( struct dlight_t* dLight ) = 0;
    virtual void      draw_3d_debug_overlays( ) = 0;
    virtual void      set_blend( float flBlend ) = 0;
    virtual float      get_blend( ) = 0;
    virtual void      set_color_modulation( float const* flBlend ) = 0;
    virtual void      get_color_modulation( float* flBlend ) = 0;
    virtual void      scene_begin( ) = 0;
    virtual void      scene_end( ) = 0;
    virtual void      get_visible_fog_volume( const vec3_t& vecEyePoint, visible_fog_volume_info_t* pInfo ) = 0;
    virtual i_world_render_list* create_world_list( ) = 0;
    virtual void      build_world_lists_epilogue( i_world_render_list* pList, world_list_info_t* pInfo, bool bShadowDepth ) = 0;
    virtual void      build_world_lists( i_world_render_list* pList, world_list_info_t* pInfo, int iForceFViewLeaf, const VisOverrideData_t* pVisData = nullptr, bool bShadowDepth = false, float* pReflectionWaterHeight = nullptr ) = 0;
    virtual void      draw_world_lists( i_world_render_list* pList, unsigned long flags, float flWaterZAdjust ) = 0;
    virtual void      get_world_list_indices_info( void* pIndicesInfoOut, i_world_render_list* pList, unsigned long nFlags ) = 0;
    virtual void      draw_top_view( bool bEnable ) = 0;
    virtual void      top_view_no_backface_culling( bool bDisable ) = 0;
    virtual void      top_view_no_vis_check( bool bDisable ) = 0;
    virtual void      top_view_bounds( vec2_t const& vecMins, vec2_t const& vecMaxs ) = 0;
    virtual void      set_top_view_volume_culler( const void* pVolumeCuller ) = 0;
    virtual void      draw_lights( ) = 0;
    virtual void      draw_mask_entities( ) = 0;
    virtual void      draw_translucent_surfaces( i_world_render_list* pList, int* pSortList, int iSortCount, unsigned long fFlags ) = 0;
    virtual void      draw_line_file( ) = 0;
    virtual void      draw_lightmaps( i_world_render_list* pList, int iPageID ) = 0;
    virtual void      view_setup_vis( bool bNoVis, int nOrigins, const vec3_t vecOrigin[ ] ) = 0;
    virtual bool      are_any_leaves_visible( int* pLeafList, int nLeaves ) = 0;
    virtual  void      vgui_paint( ) = 0;
    virtual void      view_draw_fade( uint8_t* pColor, i_material* pMaterial ) = 0;
    virtual void      old_set_projection_matrix( float flFov, float zNear, float zFar ) = 0;
    virtual void    get_light_at_point( vec3_t& vecPosition ) = 0;
    virtual int        get_view_entity( ) = 0;
    virtual bool      is_view_entity( int nEntityIndex ) = 0;
    virtual float      get_field_of_view( ) = 0;
    virtual unsigned char** get_area_bits( ) = 0;
    virtual void      set_fog_volume_state( int nVisibleFogVolume, bool bUseHeightFog ) = 0;
    virtual void      install_brush_surface_renderer( c_brush_renderer* pBrushRenderer ) = 0;
    virtual void      draw_brush_model_shadow( i_client_renderable* pRenderable ) = 0;
    virtual  bool      leaf_contains_translucent_surfaces( i_world_render_list* pList, int nSortIndex, unsigned long fFlags ) = 0;
    virtual bool      does_box_intersect_water_volume( const vec3_t& vecMins, const vec3_t& vecMaxs, int nLeafWaterDataID ) = 0;
    virtual void      set_area_state( unsigned char uAreaBits[ MAX_AREA_STATE_BYTES ], unsigned char uAreaPortalBits[ MAX_AREA_PORTAL_STATE_BYTES ] ) = 0;
    virtual void      v_gui_paint( int nMode ) = 0;
    virtual void      push_3d_view( i_mat_render_context* pRenderContext, const c_view_setup& view, int nFlags, i_texture* pRenderTarget, frustum_t frustumPlanes ) = 0;
    virtual void      push_2d_view( i_mat_render_context* pRenderContext, const c_view_setup& view, int nFlags, i_texture* pRenderTarget, frustum_t frustumPlanes ) = 0;
    virtual void      pop_view( i_mat_render_context* pRenderContext, frustum_t frustumPlanes ) = 0;
    virtual void      set_main_view( const vec3_t& vecOrigin, const vec3_t& ang_view ) = 0;
    virtual void      view_setup_vis_ex( bool bNoVis, int nOrigins, const vec3_t arrOrigin[ ], unsigned int& uReturnFlags ) = 0;
    virtual void      override_view_frustum( frustum_t custom ) = 0;
    virtual void      draw_brush_model_shadow_depth( i_client_entity* pEntity, model_t* pModel, const vec3_t& vecOrigin, const vec3_t& angView, int nDepthMode ) = 0;
    virtual void      update_brush_model_lightmap( model_t* pModel, i_client_renderable* pRenderable ) = 0;
    virtual void      begin_update_lightmaps( ) = 0;
    virtual void      end_update_lightmaps( ) = 0;
    virtual void      old_set_off_center_projection_matrix( float flFOV, float flNearZ, float flFarZ, float flAspectRatio, float flBottom, float flTop, float flLeft, float flRight ) = 0;
    virtual void      old_set_projection_matrix_ortho( float flLeft, float flTop, float flRight, float flBottom, float flNearZ, float flFarZ ) = 0;
    virtual void      push_3d_view( i_mat_render_context* pRenderContext, const c_view_setup& view, int nFlags, i_texture* pRenderTarget, frustum_t frustumPlanes, i_texture* pDepthTexture ) = 0;
    virtual void      get_matrices_for_view( const c_view_setup& view, view_matrix_t* pWorldToView, view_matrix_t* pViewToProjection, view_matrix_t* pWorldToProjection, view_matrix_t* pWorldToPixels ) = 0;
    virtual void      draw_brush_model_ex( i_client_entity* pEntity, model_t* pModel, const vec3_t& vecOrigin, const vec3_t& angView, int nMode ) = 0;
    virtual bool      does_brush_model_need_power_of2_framebuffer( const model_t* pModel ) = 0;
    virtual void      draw_brush_model_array( i_mat_render_context* pContext, int nCount, const void* pInstanceData, int nModelTypeFlags ) = 0;
};