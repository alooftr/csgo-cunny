#include "math.h"
#include "../sdk/sdk.hpp"
#include "../utils.h"

bool math::setup( )
{
	const auto vstdlib = GetModuleHandleA( "vstdlib.dll" );

	if (vstdlib == nullptr)
		return false;

	random_seed = reinterpret_cast<random_seed_fn>(GetProcAddress( vstdlib, "RandomSeed" ));
	if (random_seed == nullptr)
		return false;

	random_float = reinterpret_cast<random_float_fn>(GetProcAddress( vstdlib, "RandomFloat" ));
	if (random_float == nullptr)
		return false;

	random_float_exp = reinterpret_cast<random_float_exp_fn>(GetProcAddress( vstdlib, "RandomFloatExp" ));
	if (random_float_exp == nullptr)
		return false;

	random_int = reinterpret_cast<random_int_fn>(GetProcAddress( vstdlib, "RandomInt" ));
	if (random_int == nullptr)
		return false;

	random_gaussian_float = reinterpret_cast<random_gaussian_float_fn>(GetProcAddress( vstdlib, "RandomGaussianFloat" ));
	if (random_gaussian_float == nullptr)
		return false;

	return true;
}

void math::transform_vector( const vec3_t& a, const matrix3x4_t& b, vec3_t& out )
{
	out.x = a.dot_product( b.data[ 0 ] ) + b.data[ 0 ][ 3 ];
	out.y = a.dot_product( b.data[ 1 ] ) + b.data[ 1 ][ 3 ];
	out.z = a.dot_product( b.data[ 2 ] ) + b.data[ 2 ][ 3 ];
}

void math::angle_vectors( const vec3_t& view, vec3_t* forward, vec3_t* right, vec3_t* up )
{
	float sp, sy, sr, cp, cy, cr;

	DirectX::XMScalarSinCos( &sp, &cp, DEG2RAD( view.x ) );
	DirectX::XMScalarSinCos( &sy, &cy, DEG2RAD( view.y ) );
	DirectX::XMScalarSinCos( &sr, &cr, DEG2RAD( view.z ) );

	if (forward != nullptr)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right != nullptr)
	{
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if (up != nullptr)
	{
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}

void math::vector_angles( const vec3_t& forward, vec3_t& view )
{
	float pitch, yaw;

	if (forward.x == 0.f && forward.y == 0.f)
	{
		pitch = (forward.z > 0.f) ? 270.f : 90.f;
		yaw = 0.f;
	}
	else
	{
		pitch = std::atan2f( -forward.z, forward.length_2d( ) ) * 180.f / M_PI;

		if (pitch < 0.f)
			pitch += 360.f;

		yaw = std::atan2f( forward.y, forward.x ) * 180.f / M_PI;

		if (yaw < 0.f)
			yaw += 360.f;
	}

	view.x = pitch;
	view.y = yaw;
	view.z = 0.f;
}

float math::normalize_yaw( float yaw )
{
	if (!std::isfinite( yaw ))
		return 0.0f;

	if (yaw >= -180.f && yaw <= 180.f)
		return yaw;

	const float rot = std::round( std::abs( yaw / 360.f ) );

	yaw = (yaw < 0.f) ? yaw + (360.f * rot) : yaw - (360.f * rot);
	return yaw;
}

vec3_t math::vector_transform( const vec3_t& transform, const matrix3x4_t& matrix )
{
	return vec3_t( transform.dot_product( matrix[ 0 ] ) + matrix[ 0 ][ 3 ],
		transform.dot_product( matrix[ 1 ] ) + matrix[ 1 ][ 3 ],
		transform.dot_product( matrix[ 2 ] ) + matrix[ 2 ][ 3 ] );
}

void math::vector_transform( const vec3_t& in, const matrix3x4_t& matrix, vec3_t& out )
{
	out =
	{
		in.dot_product( vec3_t( matrix[ 0 ][ 0 ], matrix[ 0 ][ 1 ], matrix[ 0 ][ 2 ] ) ) + matrix[ 0 ][ 3 ],
		in.dot_product( vec3_t( matrix[ 1 ][ 0 ], matrix[ 1 ][ 1 ], matrix[ 1 ][ 2 ] ) ) + matrix[ 1 ][ 3 ],
		in.dot_product( vec3_t( matrix[ 2 ][ 0 ], matrix[ 2 ][ 1 ], matrix[ 2 ][ 2 ] ) ) + matrix[ 2 ][ 3 ]
	};
}

vec3_t math::calc_angle( const vec3_t start, vec3_t end )
{
	vec3_t delta = vec3_t(
		(start.x - end.x),
		(start.y - end.y),
		(start.z - end.z) ),
		angle;

	double hyp = sqrt( delta.x * delta.x + delta.y * delta.y );

	angle.x = float( atanf( float( delta.z / hyp ) ) * M_RADPI );
	angle.y = float( atanf( float( delta.y / delta.x ) ) * M_RADPI );
	angle.z = 0.0f;

	if (delta.x >= 0.0) {
		angle.y += 180.0f;
	}

	return angle;
}

float math::get_fov( const vec3_t& view_angle, const vec3_t& aim_angle )
{
	auto AngleVector = [ ]( vec3_t ang ) -> vec3_t
	{
		return vec3_t(
			cos( DEG2RAD( ang.x ) ) * cos( DEG2RAD( ang.y ) ),
			cos( DEG2RAD( ang.x ) ) * sin( DEG2RAD( ang.y ) ),
			-sin( DEG2RAD( ang.x ) ) );
	};

	vec3_t vAim = AngleVector( view_angle ),
		vAng = AngleVector( aim_angle );

	return RAD2DEG( acos( vAim.dot_product( vAng ) / vAim.length_sqr( ) ) );
}

bool math::world_to_screen( const vec3_t& origin, vec2_t& screen )
{
	const view_matrix_t& matrix = csgo::engine->world_to_screen_matrix( );
	const float width = matrix[ 3 ][ 0 ] * origin.x + matrix[ 3 ][ 1 ] * origin.y + matrix[ 3 ][ 2 ] * origin.z + matrix[ 3 ][ 3 ];

	if (width < 0.001f)
		return false;

	// compute the scene coordinates of a point in 3d
	const float inverse = 1.0f / width;
	screen.x = (matrix[ 0 ][ 0 ] * origin.x + matrix[ 0 ][ 1 ] * origin.y + matrix[ 0 ][ 2 ] * origin.z + matrix[ 0 ][ 3 ]) * inverse;
	screen.y = (matrix[ 1 ][ 0 ] * origin.x + matrix[ 1 ][ 1 ] * origin.y + matrix[ 1 ][ 2 ] * origin.z + matrix[ 1 ][ 3 ]) * inverse;

	// screen transform
	// get the screen position in pixels of given point
	const ImVec2 screen_size = ImGui::GetIO( ).DisplaySize;
	screen.x = (screen_size.x * 0.5f) + (screen.x * screen_size.x) * 0.5f;
	screen.y = (screen_size.y * 0.5f) - (screen.y * screen_size.y) * 0.5f;

	return true;
}

void math::angle_matrix( const vec3_t& angView, matrix3x4_t& matOutput, const vec3_t& vecOrigin )
{
	float sp, sy, sr, cp, cy, cr;

	DirectX::XMScalarSinCos( &sp, &cp, DEG2RAD( angView.x ) );
	DirectX::XMScalarSinCos( &sy, &cy, DEG2RAD( angView.y ) );
	DirectX::XMScalarSinCos( &sr, &cr, DEG2RAD( angView.z ) );

	matOutput.set_forward( vec3_t( cp * cy, cp * sy, -sp ) );

	const float crcy = cr * cy;
	const float crsy = cr * sy;
	const float srcy = sr * cy;
	const float srsy = sr * sy;

	matOutput.set_left( vec3_t( sp * srcy - crsy, sp * srsy + crcy, sr * cp ) );
	matOutput.set_up( vec3_t( sp * crcy + srsy, sp * crsy - srcy, cr * cp ) );
	matOutput.set_origin( vecOrigin );
}

void math::angle_matrix( const vec3_t& ang, const vec3_t& pos, matrix3x4_t& out )
{
	angle_matrix( ang, out, vec3_t( 0.0f, 0.0f, 0.0f ) );
	out.set_origin( pos );
}

void math::matrix_copy( const matrix3x4_t& in, matrix3x4_t& out ) {
	memcpy( out.base( ), in.base( ), sizeof( float ) * 3 * 4 );
}

void math::concat_transforms( const matrix3x4_t& in1, const matrix3x4_t& in2, matrix3x4_t& out )
{
	if (&in1 == &out)
	{
		matrix3x4_t in1b;
		matrix_copy( in1, in1b );
		concat_transforms( in1b, in2, out );
		return;
	}
	if (&in2 == &out)
	{
		matrix3x4_t in2b;
		matrix_copy( in2, in2b );
		concat_transforms( in1, in2b, out );
		return;
	}

	out[ 0 ][ 0 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 0 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 0 ] +
		in1[ 0 ][ 2 ] * in2[ 2 ][ 0 ];
	out[ 0 ][ 1 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 1 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 1 ] +
		in1[ 0 ][ 2 ] * in2[ 2 ][ 1 ];
	out[ 0 ][ 2 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 2 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 2 ] +
		in1[ 0 ][ 2 ] * in2[ 2 ][ 2 ];
	out[ 0 ][ 3 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 3 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 3 ] +
		in1[ 0 ][ 2 ] * in2[ 2 ][ 3 ] + in1[ 0 ][ 3 ];
	out[ 1 ][ 0 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 0 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 0 ] +
		in1[ 1 ][ 2 ] * in2[ 2 ][ 0 ];
	out[ 1 ][ 1 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 1 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 1 ] +
		in1[ 1 ][ 2 ] * in2[ 2 ][ 1 ];
	out[ 1 ][ 2 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 2 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 2 ] +
		in1[ 1 ][ 2 ] * in2[ 2 ][ 2 ];
	out[ 1 ][ 3 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 3 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 3 ] +
		in1[ 1 ][ 2 ] * in2[ 2 ][ 3 ] + in1[ 1 ][ 3 ];
	out[ 2 ][ 0 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 0 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 0 ] +
		in1[ 2 ][ 2 ] * in2[ 2 ][ 0 ];
	out[ 2 ][ 1 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 1 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 1 ] +
		in1[ 2 ][ 2 ] * in2[ 2 ][ 1 ];
	out[ 2 ][ 2 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 2 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 2 ] +
		in1[ 2 ][ 2 ] * in2[ 2 ][ 2 ];
	out[ 2 ][ 3 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 3 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 3 ] +
		in1[ 2 ][ 2 ] * in2[ 2 ][ 3 ] + in1[ 2 ][ 3 ];
}
