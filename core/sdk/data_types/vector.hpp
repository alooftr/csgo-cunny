#pragma once

// used: isfinite, fmodf, sqrtf
#include <cmath>
// used: numeric_limits
#include <limits>
#include <algorithm>
#include <DirectXMath.h>

#include "../../../ext/imgui/imgui.h"

#define M_METRE2INCH( x )	( ( x ) / 0.0254f )
#define M_INCH2METRE( x )	( ( x ) * 0.0254f )
#define M_METRE2FOOT( x )	( ( x ) * 3.28f )
#define M_FOOT2METRE( x )	( ( x ) / 3.28f )
#define RAD2DEG(x) DirectX::XMConvertToDegrees( x ) 
#define DEG2RAD(x) DirectX::XMConvertToRadians( x )
#define M_PI 3.14159265358979323846f
#define PI_F ( ( float )( M_PI ) ) 
#define M_RADPI 57.295779513082f

class vec2_t
{
public:
	constexpr vec2_t( float x = 0.f, float y = 0.f ) :
		x( x ), y( y ) { }

	[[nodiscard]] bool is_zero( ) const
	{
		return (std::fpclassify( this->x ) == FP_ZERO &&
			std::fpclassify( this->y ) == FP_ZERO);
	}

	constexpr vec2_t& operator=( const vec2_t& base )
	{
		this->x = base.x; this->y = base.y;
		return *this;
	}

	constexpr vec2_t& operator+=( const vec2_t& base )
	{
		this->x += base.x; this->y += base.y;
		return *this;
	}

	constexpr vec2_t& operator-=( const vec2_t& base )
	{
		this->x -= base.x; this->y -= base.y;
		return *this;
	}

	constexpr vec2_t& operator*=( const vec2_t& base )
	{
		this->x *= base.x; this->y *= base.y;
		return *this;
	}

	constexpr vec2_t& operator/=( const vec2_t& base )
	{
		this->x /= base.x; this->y /= base.y;
		return *this;
	}

	constexpr vec2_t& operator+=( const float add )
	{
		this->x += add; this->y += add;
		return *this;
	}

	constexpr vec2_t& operator-=( const float subtract )
	{
		this->x -= subtract; this->y -= subtract;
		return *this;
	}

	constexpr vec2_t& operator*=( const float multiply )
	{
		this->x *= multiply; this->y *= multiply;
		return *this;
	}

	constexpr vec2_t& operator/=( const float divide )
	{
		this->x /= divide; this->y /= divide;
		return *this;
	}

	vec2_t operator+( const vec2_t& add ) const
	{
		return vec2_t( this->x + add.x, this->y + add.y );
	}

	vec2_t operator-( const vec2_t& subtract ) const
	{
		return vec2_t( this->x - subtract.x, this->y - subtract.y );
	}

	vec2_t operator*( const vec2_t& multiply ) const
	{
		return vec2_t( this->x * multiply.x, this->y * multiply.y );
	}

	vec2_t operator/( const vec2_t& divide ) const
	{
		return vec2_t( this->x / divide.x, this->y / divide.y );
	}

	vec2_t operator+( const float add ) const
	{
		return vec2_t( this->x + add, this->y + add );
	}

	vec2_t operator-( const float subtract ) const
	{
		return vec2_t( this->x - subtract, this->y - subtract );
	}

	vec2_t operator*( const float multiply ) const
	{
		return vec2_t( this->x * multiply, this->y * multiply );
	}

	vec2_t operator/( const float divide ) const
	{
		return vec2_t( this->x / divide, this->y / divide );
	}

	[[nodiscard]] bool is_equal( const vec2_t& vecEqual, const float flErrorMargin = std::numeric_limits<float>::epsilon( ) ) const
	{
		return (std::fabsf( this->x - vecEqual.x ) < flErrorMargin &&
			std::fabsf( this->y - vecEqual.y ) < flErrorMargin);
	}

	bool operator==( const vec2_t& base ) const
	{
		return this->is_equal( base );
	}

	bool operator!=( const vec2_t& base ) const
	{
		return !this->is_equal( base );
	}

	[[nodiscard]] bool is_valid( ) const
	{
		return std::isfinite( this->x ) && std::isfinite( this->y );
	}

	ImVec2 get_im_vec2( ) const
	{
		return ImVec2( x, y );
	}
public:
	float x, y;
};

class vec3_t
{
public:
	constexpr vec3_t( float x = 0.f, float y = 0.f, float z = 0.f ) :
		x( x ), y( y ), z( z ) { }

	constexpr vec3_t( const float* vec3_t ) :
		x( vec3_t[ 0 ] ), y( vec3_t[ 1 ] ), z( vec3_t[ 2 ] ) { }

	constexpr vec3_t( const vec2_t& base_2d ) :
		x( base_2d.x ), y( base_2d.y ), z( 0.0f ) { }

	[[nodiscard]] bool is_valid( ) const
	{
		return std::isfinite( this->x ) && std::isfinite( this->y ) && std::isfinite( this->z );
	}

	constexpr void invalidate( )
	{
		this->x = this->y = this->z = std::numeric_limits<float>::infinity( );
	}

	[[nodiscard]] float* data( )
	{
		return reinterpret_cast<float*>(this);
	}

	[[nodiscard]] const float* data( ) const
	{
		return reinterpret_cast<float*>(const_cast<vec3_t*>(this));
	}

	float& operator[]( const std::size_t i )
	{
		return this->data( )[ i ];
	}

	const float& operator[]( const std::size_t i ) const
	{
		return this->data( )[ i ];
	}

	bool operator==( const vec3_t& base ) const
	{
		return this->is_equal( base );
	}

	bool operator!=( const vec3_t& base ) const
	{
		return !this->is_equal( base );
	}

	constexpr vec3_t& operator=( const vec3_t& base )
	{
		this->x = base.x; this->y = base.y; this->z = base.z;
		return *this;
	}

	constexpr vec3_t& operator=( const vec2_t& base_2d )
	{
		this->x = base_2d.x; this->y = base_2d.y; this->z = 0.0f;
		return *this;
	}

	constexpr vec3_t& operator+=( const vec3_t& base )
	{
		this->x += base.x; this->y += base.y; this->z += base.z;
		return *this;
	}

	constexpr vec3_t& operator-=( const vec3_t& base )
	{
		this->x -= base.x; this->y -= base.y; this->z -= base.z;
		return *this;
	}

	constexpr vec3_t& operator*=( const vec3_t& base )
	{
		this->x *= base.x; this->y *= base.y; this->z *= base.z;
		return *this;
	}

	constexpr vec3_t& operator/=( const vec3_t& base )
	{
		this->x /= base.x; this->y /= base.y; this->z /= base.z;
		return *this;
	}

	constexpr vec3_t& operator+=( const float add )
	{
		this->x += add; this->y += add; this->z += add;
		return *this;
	}

	constexpr vec3_t& operator-=( const float subtract )
	{
		this->x -= subtract; this->y -= subtract; this->z -= subtract;
		return *this;
	}

	constexpr vec3_t& operator*=( const float multiply )
	{
		this->x *= multiply; this->y *= multiply; this->z *= multiply;
		return *this;
	}

	constexpr vec3_t& operator/=( const float divide )
	{
		this->x /= divide; this->y /= divide; this->z /= divide;
		return *this;
	}

	vec3_t operator+( const vec3_t& add ) const
	{
		return vec3_t( this->x + add.x, this->y + add.y, this->z + add.z );
	}

	vec3_t operator-( const vec3_t& subtract ) const
	{
		return vec3_t( this->x - subtract.x, this->y - subtract.y, this->z - subtract.z );
	}

	vec3_t operator*( const vec3_t& multiply ) const
	{
		return vec3_t( this->x * multiply.x, this->y * multiply.y, this->z * multiply.z );
	}

	vec3_t operator/( const vec3_t& divide ) const
	{
		return vec3_t( this->x / divide.x, this->y / divide.y, this->z / divide.z );
	}

	vec3_t operator+( const float add ) const
	{
		return vec3_t( this->x + add, this->y + add, this->z + add );
	}

	vec3_t operator-( const float subtract ) const
	{
		return vec3_t( this->x - subtract, this->y - subtract, this->z - subtract );
	}

	vec3_t operator*( const float multiply ) const
	{
		return vec3_t( this->x * multiply, this->y * multiply, this->z * multiply );
	}

	vec3_t operator/( const float divide ) const
	{
		return vec3_t( this->x / divide, this->y / divide, this->z / divide );
	}

	[[nodiscard]] bool is_equal( const vec3_t& vecEqual, const float flErrorMargin = std::numeric_limits<float>::epsilon( ) ) const
	{
		return (std::fabsf( this->x - vecEqual.x ) < flErrorMargin &&
			std::fabsf( this->y - vecEqual.y ) < flErrorMargin &&
			std::fabsf( this->z - vecEqual.z ) < flErrorMargin);
	}

	[[nodiscard]] bool is_zero( ) const
	{
		return (std::fpclassify( this->x ) == FP_ZERO &&
			std::fpclassify( this->y ) == FP_ZERO &&
			std::fpclassify( this->z ) == FP_ZERO);
	}

	[[nodiscard]] vec2_t to_vector2_d( ) const
	{
		return vec2_t( this->x, this->y );
	}

	[[nodiscard]] float length( ) const
	{
		return std::sqrtf( this->length_sqr( ) );
	}

	[[nodiscard]] constexpr float length_sqr( ) const
	{
		return dot_product( *this );
	}

	[[nodiscard]] float length_2d( ) const
	{
		return std::sqrtf( this->length2_d_sqr( ) );
	}

	[[nodiscard]] constexpr float length2_d_sqr( ) const
	{
		return (this->x * this->x + this->y * this->y);
	}

	[[nodiscard]] float dist_to( const vec3_t& end ) const
	{
		return (*this - end).length( );
	}

	[[nodiscard]] constexpr float dist_to_sqr( const vec3_t& end ) const
	{
		return (*this - end).length_sqr( );
	}

	[[nodiscard]] vec3_t normalized( ) const
	{
		vec3_t vecOut = *this;
		vecOut.normalize_in_place( );
		return vecOut;
	}

	[[nodiscard]] static vec3_t from_angle_2d( float angle )
	{
		return vec3_t( std::cos( DEG2RAD( angle ) ), std::sin( DEG2RAD( angle ) ), 0.0f );
	}

	float normalize_in_place( )
	{
		const float length = this->length( );
		const float radius = 1.0f / (length + std::numeric_limits<float>::epsilon( ));

		this->x *= radius;
		this->y *= radius;
		this->z *= radius;

		return length;
	}

	[[nodiscard]] constexpr float dot_product( const vec3_t& dot ) const
	{
		return (this->x * dot.x + this->y * dot.y + this->z * dot.z);
	}

	[[nodiscard]] constexpr vec3_t cross_product( const vec3_t& cross ) const
	{
		return vec3_t( this->y * cross.z - this->z * cross.y, this->z * cross.x - this->x * cross.z, this->x * cross.y - this->y * cross.x );
	}

	vec3_t clamp( )
	{
		this->x = std::clamp( this->x, -89.f, 89.f );
		this->y = std::clamp( this->y, -180.f, 180.f );
		this->z = std::clamp( this->z, -50.f, 50.f );
		return *this;
	}

	vec3_t normalize( )
	{
		this->x = std::isfinite( this->x ) ? std::remainderf( this->x, 360.f ) : 0.f;
		this->y = std::isfinite( this->y ) ? std::remainderf( this->y, 360.f ) : 0.f;
		this->z = std::clamp( this->z, -50.f, 50.f );
		return *this;
	}

	__forceinline float float_normalize( ) {
		float len = length( );

		(*this) /= (length( ) + std::numeric_limits< float >::epsilon( ));

		return len;
	}

	vec3_t to_angle( )
	{
		return vec3_t(
			RAD2DEG( std::atan2( -this->z, std::hypot( this->x, this->y ) ) ),
			RAD2DEG( std::atan2( this->y, this->x ) ), 
			0.f
		);
	}

	static vec3_t from_angle( const vec3_t& angle )
	{
		return vec3_t( 
			std::cos(DEG2RAD(angle.x)) * std::cos(DEG2RAD(angle.y)), 
			std::cos( DEG2RAD( angle.x ) ) * std::sin( DEG2RAD( angle.y ) ),
			std::sin( DEG2RAD( angle.x ) )
			);
	}


public:
	float x, y, z;
};

class vec4_t
{
public:
	constexpr vec4_t( float x = 0.f, float y = 0.f, float z = 0.f, float w = 0.f ) :
		x( x ), y( y ), z( z ), w( w ) { }

public:
	float x, y, z, w;
};

class __declspec(align(16)) vector_aligned : public vec3_t
{
public:
	vector_aligned( ) = default;

	explicit vector_aligned( const vec3_t& base )
	{
		this->x = base.x; this->y = base.y; this->z = base.z; this->w = 0.f;
	}

	constexpr vector_aligned& operator=( const vec3_t& base )
	{
		this->x = base.x; this->y = base.y; this->z = base.z; this->w = 0.f;
		return *this;
	}

public:
	float w;
};
