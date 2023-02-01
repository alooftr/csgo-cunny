#include "preview_model.h"

#include "../../../utils/math.h"

int width = 250;
int height = 350;

std::array < vec4_t, 6 > white_array =
{
	vec4_t( 0.4f, 0.4f, 0.4f, 1.0f ),
	vec4_t( 0.4f, 0.4f, 0.4f, 1.0f ),
	vec4_t( 0.4f, 0.4f, 0.4f, 1.0f ),
	vec4_t( 0.4f, 0.4f, 0.4f, 1.0f ),
	vec4_t( 0.4f, 0.4f, 0.4f, 1.0f ),
	vec4_t( 0.4f, 0.4f, 0.4f, 1.0f ),
};

std::array < float_t, 3 > color_modulation =
{
	1.0f,
	1.0f,
	1.0f
};

void c_preview_model::on_paint( )
{
	model->root_mdl.time += csgo::global_vars->frame_time / 2.0f;

	view_setup.x = 0;
	view_setup.y = 0;
	view_setup.width = width;
	view_setup.height = height;
	view_setup.ortho = false;
	view_setup.fov = 70.f;
	view_setup.origin = vec3_t( -65.0f, 2.0f, 50 );
	view_setup.view = vec3_t( 0.f, 0.f, 0.f );
	view_setup.near_z = 7.0f;
	view_setup.far_z = 1000.f;
	view_setup.do_bloom_and_tone_mapping = true;

	c_mat_render_context_ptr render_context( csgo::material_system );

	render_context->push_render_target_and_viewport( );
	render_context->set_render_target( texture );

	render_context->bind_local_cubemap( cube_map );
	render_context->set_lighting_origin( view_setup.origin.x, view_setup.origin.y, view_setup.origin.z );
	render_context->set_int_rendering_parameter( 10, 0 );

	frustum_t dummy_frustum;
	csgo::render_view->push_3d_view( render_context, view_setup, view_clear_color | view_clear_depth | view_clear_stencil, texture, dummy_frustum );

	render_context->clear_color4ub( false, false, false, false );
	render_context->clear_buffers( true, true, true );
	render_context->set_ambient_light_cube( white_array.data( ) );

	csgo::studio_render->set_ambient_light_colors( white_array.data( ) );
	csgo::studio_render->set_local_lights( 0, nullptr );

	matrix3x4_t player_view = { };
	math::angle_matrix( vec3_t( 0.f, -180.f, 0.f ), player_view, vec3_t( 0, 0, 0 ) );

	csgo::model_render->suppress_engine_lighting( true );
	
	model->draw( player_view );

	csgo::model_render->suppress_engine_lighting( false );

	csgo::render_view->pop_view( render_context, dummy_frustum );
	render_context->bind_local_cubemap( nullptr );

	render_context->pop_render_target_and_viewport( );
	render_context->release( );
}

bool c_preview_model::setup( )
{
	csgo::material_system->begin_render_target_allocation( );
	{
		texture = csgo::material_system->create_named_render_target_texture_ex(
			"preview",
			width, height,
			rt_size_no_change,
			csgo::material_system->get_back_buffer_format( ),
			material_rt_depth_shared,
			textureflags_clamps | textureflags_clampt,
			createrendertargetflags_hdr
		);
	}
	csgo::material_system->finish_render_target_allocation( );

	if (!texture)
		return false;

	cube_map = csgo::material_system->find_texture( "editor/cubemap.hdr", TEXTURE_GROUP_CUBE_MAP );
	if (!cube_map)
		return false;

	model = static_cast<c_merged_mdl*>(csgo::mem_alloc->alloc( 0x75C ));
	auto create_model = reinterpret_cast< void( __thiscall* )( void* ) >( utils::find_pattern( "client.dll", "53 8B D9 56 57 8D 4B 04 C7 03 ? ? ? ? E8 ? ? ? ? 6A" ) );
	create_model( model );

	model->set_mdl( "models/player/custom_player/uiplayer/animset_uiplayer.mdl" );
	model->set_merge_mdl( "models/player/custom_player/legacy/tm_balkan_variantg.mdl" );
	model->set_sequence( 119, false );
	model->setup_bones_for_attachment_queries( );
	model->set_merge_mdl( "models/weapons/w_rif_ak47.mdl" );

	if (!model )
		return false;

	return true;
}