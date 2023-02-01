#pragma once

#include "../sdk/data_types/vector.hpp"
#include "../sdk/data_types/matrix.hpp"

extern "C" using random_seed_fn = int( __cdecl* )(int seed);
extern "C" using random_float_fn = float( __cdecl* )(float min_val, float max_val);
extern "C" using random_float_exp_fn = float( __cdecl* )(float min_val, float max_val, float exponent);
extern "C" using random_int_fn = int( __cdecl* )(int min_val, int max_val);
extern "C" using random_gaussian_float_fn = float( __cdecl* )(float mean, float std_dev);

namespace math {
	bool setup( );
	void transform_vector( const vec3_t& a, const matrix3x4_t& b, vec3_t& out );
	void angle_vectors( const vec3_t& view, vec3_t* forward, vec3_t* right = nullptr, vec3_t* up = nullptr );
	void vector_angles( const vec3_t& forward, vec3_t& view );
	float normalize_yaw( float yaw );
	inline float normalize_pitch( float pitch )
	{
		if (pitch > 89.0f)
			pitch -= 180.0f;

		else if (pitch < -89.0f)
			pitch += 180.0f;

		return pitch;
	}

	vec3_t vector_transform( const vec3_t& transform, const matrix3x4_t& matrix );
	void	vector_transform( const vec3_t& in, const matrix3x4_t& matrix, vec3_t& out );

	inline vec3_t to_angle( vec3_t vector )
	{
		return vec3_t{ RAD2DEG( atan2( -vector.z, std::hypot( vector.x, vector.y ) ) ),
			   RAD2DEG( atan2( vector.y, vector.x ) ),
			   0.0f };
	};

	inline vec3_t to_vector( vec3_t angle )
	{
		return vec3_t{ std::cos( DEG2RAD( angle.x ) ) * std::cos( DEG2RAD( angle.y ) ),
			   std::cos( DEG2RAD( angle.x ) ) * std::sin( DEG2RAD( angle.y ) ),
			  -std::sin( DEG2RAD( angle.x ) ) };
	};

	vec3_t calc_angle( const vec3_t start, vec3_t end );

	float get_fov( const vec3_t& view_angle, const vec3_t& aim_angle );

	bool world_to_screen( const vec3_t& origin, vec2_t& screen );

	void 	angle_matrix( const vec3_t& ang, matrix3x4_t& mat, const vec3_t& origin = vec3_t( 0.0f, 0.0f, 0.0f ) );
	void	angle_matrix( const vec3_t& ang, const vec3_t& pos, matrix3x4_t& out );

	void    matrix_copy( const matrix3x4_t& in, matrix3x4_t& out );
	void	concat_transforms( const matrix3x4_t& in1, const matrix3x4_t& in2, matrix3x4_t& out );

	vec3_t calculate_relative_angle( const vec3_t& src, const vec3_t& dst, const vec3_t& view_angle );

	inline float simple_spline( float value )
	{
		float squared = value * value;

		return (3 * squared - 2 * squared * value);
	}

	inline float simple_spline_remap_val_clamped( float val, float A, float B, float C, float D )
	{
		if (A == B)
			return val >= B ? D : C;
		float cVal = (val - A) / (B - A);
		cVal = std::clamp( cVal, 0.0f, 1.0f );
		return C + (D - C) * simple_spline( cVal );
	}

	inline float lerp( float percent, float a, float b )
	{
		return a + (b - a) * percent;
	};

	inline vec3_t lerp( float percent, vec3_t a, vec3_t b ) noexcept
	{
		return a + (b - a) * percent;
	}

	inline random_seed_fn random_seed;
	inline random_float_fn random_float;
	inline random_float_exp_fn random_float_exp;
	inline random_int_fn random_int;
	inline random_gaussian_float_fn random_gaussian_float;
}