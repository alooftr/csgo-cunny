#pragma once

// used: vector
#include "../data_types/vector.hpp"
// used: irefcouted
#include "irefcount.hpp"
// used: texture
#include "i_texture.hpp"
// used: material
#include "i_material.hpp"

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/mathlib/lightdesc.h
enum e_light_type : int
{
	material_light_disable = 0,
	material_light_point,
	material_light_directional,
	material_light_spot
};

enum e_light_type_optimization_flags
{
	lighttype_optimizationflags_has_attenuation0 = 1,
	lighttype_optimizationflags_has_attenuation1 = 2,
	lighttype_optimizationflags_has_attenuation2 = 4,
	lighttype_optimizationflags_derived_values_calced = 8
};

struct light_desc_t
{
	void init_directional( const vec3_t& direction, const vec3_t& color )
	{
		this->type = material_light_directional;
		this->color = color;
		this->direction = direction;
		this->range = 0.0f;
		this->attenuation0 = 1.0f;
		this->attenuation1 = 0.f;
		this->attenuation2 = 0.f;

		flags = lighttype_optimizationflags_derived_values_calced;

		if (attenuation0)
			flags |= lighttype_optimizationflags_has_attenuation0;
		if (attenuation1)
			flags |= lighttype_optimizationflags_has_attenuation1;
		if (attenuation2)
			flags |= lighttype_optimizationflags_has_attenuation2;

		if (type == material_light_spot)
		{
			theta_dot = std::cosf( theta );
			phi_dot = std::cosf( phi );

			if (const float spread = theta_dot - phi_dot; spread > 1.0e-10f)
				// note - this quantity is very sensitive to round off error. the sse reciprocal approximation won't cut it here.
				one_over_theta_dot_minus_phi_dot = 1.0f / spread;
			else
				// hard falloff instead of divide by zero
				one_over_theta_dot_minus_phi_dot = 1.0f;
		}
		else if (type == material_light_directional)
		{
			// set position to be real far away in the right direction
			position = direction;
			position *= 2.0e6;
		}

		range_squared = range * range;
	}

	e_light_type type;
	vec3_t		color;
	vec3_t		position;
	vec3_t		direction;
	float		range;
	float		falloff;
	float		attenuation0;
	float		attenuation1;
	float		attenuation2;
	float		theta;
	float		phi;
	float		theta_dot;
	float		phi_dot;
	float		one_over_theta_dot_minus_phi_dot;
	std::uint32_t flags;
protected:
	float		range_squared;
};

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/client/glow_outline_effect.cpp
enum e_stencil_operation : int
{
	stenciloperation_keep = 1,
	stenciloperation_zero = 2,
	stenciloperation_replace = 3,
	stenciloperation_incrsat = 4,
	stenciloperation_decrsat = 5,
	stenciloperation_invert = 6,
	stenciloperation_incr = 7,
	stenciloperation_decr = 8,
	stenciloperation_force_dword = 0x7fffffff
};

enum e_stencil_comparison_function
{
	stencilcomparisonfunction_never = 1,
	stencilcomparisonfunction_less = 2,
	stencilcomparisonfunction_equal = 3,
	stencilcomparisonfunction_lessequal = 4,
	stencilcomparisonfunction_greater = 5,
	stencilcomparisonfunction_notequal = 6,
	stencilcomparisonfunction_greaterequal = 7,
	stencilcomparisonfunction_always = 8,
	stencilcomparisonfunction_force_dword = 0xFFFFFFFF
};

struct shader_stencil_state_t
{
	shader_stencil_state_t( )
	{
		enable = false;
		pass_operation = fail_operation = z_fail_operation = stenciloperation_keep;
		compare_function = stencilcomparisonfunction_always;
		reference_value = 0;
		test_mask = write_mask = 0xFFFFFFFF;
	}

	bool						enable;
	e_stencil_operation fail_operation;
	e_stencil_operation z_fail_operation;
	e_stencil_operation pass_operation;
	e_stencil_comparison_function compare_function;
	int							reference_value;
	std::uint32_t				test_mask;
	std::uint32_t				write_mask;
};

class i_client_renderable;
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/materialsystem/imaterialsystem.h
class i_mat_render_context : public i_ref_counted
{
public:
	void begin_render( )
	{
		utils::call_vfunc<void>( this, 2 );
	}

	void end_render( )
	{
		utils::call_vfunc<void>( this, 3 );
	}

	void bind_local_cubemap( i_texture* texture )
	{
		utils::call_vfunc<void>( this, 5, texture );
	}

	void set_render_target( i_texture* texture )
	{
		utils::call_vfunc<void>( this, 6, texture );
	}

	i_texture* get_render_target( )
	{
		return utils::call_vfunc<i_texture*>( this, 7 );
	}

	void clear_buffers( bool clear_color, bool clear_depth, bool clear_stencil = false )
	{
		utils::call_vfunc<void>( this, 12, clear_color, clear_depth, clear_stencil );
	}

	void set_lights( int count, const light_desc_t* lights )
	{
		utils::call_vfunc<void>( this, 17, count, lights );
	}

	void set_ambient_light_cube( vec4_t cube[ 6 ] )
	{
		utils::call_vfunc<void>( this, 18, cube );
	}

	void viewport( int x, int y, int w, int h )
	{
		utils::call_vfunc<void>( this, 40, x, y, w, h );
	}

	void get_viewport( int& x, int& y, int& w, int& h )
	{
		utils::call_vfunc<void>( this, 41, std::ref( x ), std::ref( y ), std::ref( w ), std::ref( h ) );
	}

	void clear_color3ub( unsigned char r, unsigned char g, unsigned char b )
	{
		utils::call_vfunc<void>( this, 78, r, g, b );
	}

	void clear_color4ub( unsigned char r, unsigned char g, unsigned char b, unsigned char a )
	{
		utils::call_vfunc<void>( this, 79, r, g, b, a );
	}

	void draw_screen_space_rectangle( i_material* material, int dest_x, int dest_y, int w, int h, float texture_x0, float texture_y0, float texture_x1, float texture_y1, int texture_width, int texture_height, i_client_renderable* client_renderable = nullptr, int x_dice = 1, int y_dice = 1 )
	{
		utils::call_vfunc<void>( this, 114, material, dest_x, dest_y, w, h, texture_x0, texture_y0, texture_x1, texture_y1, texture_width, texture_height, client_renderable, x_dice, y_dice );
	}

	void push_render_target_and_viewport( )
	{
		utils::call_vfunc<void>( this, 119 );
	}

	void pop_render_target_and_viewport( )
	{
		utils::call_vfunc<void>( this, 120 );
	}

	void set_int_rendering_parameter( int parm_number, int value )
	{
		utils::call_vfunc<void>( this, 126, parm_number, value );
	}

	void set_lighting_origin(/*vec3_t vecLightingOrigin*/float x, float y, float z )
	{
		utils::call_vfunc<void>( this, 158, x, y, z );
	}

	void set_scissor_rect( const int left, const int top, const int right, const int bottom, const bool enable_scissor = true )
	{
		utils::call_vfunc<void>( this, 159, left, top, right, bottom, enable_scissor );
	}

	void pop_scissor_rect( )
	{
		utils::call_vfunc<void>( this, 160 );
	}
};
