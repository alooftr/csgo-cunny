#pragma once
#include "vector.hpp"

using matrix3x3_t = float[ 3 ][ 3 ];
struct matrix3x4_t
{
	matrix3x4_t( ) = default;

	constexpr matrix3x4_t(
		const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23 )
	{
		data[ 0 ][ 0 ] = m00; data[ 0 ][ 1 ] = m01; data[ 0 ][ 2 ] = m02; data[ 0 ][ 3 ] = m03;
		data[ 1 ][ 0 ] = m10; data[ 1 ][ 1 ] = m11; data[ 1 ][ 2 ] = m12; data[ 1 ][ 3 ] = m13;
		data[ 2 ][ 0 ] = m20; data[ 2 ][ 1 ] = m21; data[ 2 ][ 2 ] = m22; data[ 2 ][ 3 ] = m23;
	}

	constexpr matrix3x4_t( const vec3_t& x, const vec3_t& y, const vec3_t& z, const vec3_t& origin )
	{
		init( x, y, z, origin );
	}

	constexpr void init( const vec3_t& forward, const vec3_t& left, const vec3_t& up, const vec3_t& origin )
	{
		set_forward( forward );
		set_left( left );
		set_up( up );
		set_origin( origin );
	}

	constexpr void set_forward( const vec3_t& forward )
	{
		this->data[ 0 ][ 0 ] = forward.x;
		this->data[ 1 ][ 0 ] = forward.y;
		this->data[ 2 ][ 0 ] = forward.z;
	}

	constexpr void set_left( const vec3_t& left )
	{
		this->data[ 0 ][ 1 ] = left.x;
		this->data[ 1 ][ 1 ] = left.y;
		this->data[ 2 ][ 1 ] = left.z;
	}

	constexpr void set_up( const vec3_t& up )
	{
		this->data[ 0 ][ 2 ] = up.x;
		this->data[ 1 ][ 2 ] = up.y;
		this->data[ 2 ][ 2 ] = up.z;
	}

	constexpr void set_origin( const vec3_t& origin )
	{
		this->data[ 0 ][ 3 ] = origin.x;
		this->data[ 1 ][ 3 ] = origin.y;
		this->data[ 2 ][ 3 ] = origin.z;
	}

	constexpr vec3_t get_origin( ) const
	{
		return vec3_t( this->data[ 0 ][ 3 ], this->data[ 1 ][ 3 ], this->data[ 2 ][ 3 ] );
	}

	constexpr void invalidate( )
	{
		for (auto& arrSubData : data)
		{
			for (auto& flData : arrSubData)
				flData = std::numeric_limits<float>::infinity( );
		}
	}

	float* operator[]( const int index )
	{
		return data[ index ];
	}

	const float* operator[]( const int index ) const
	{
		return data[ index ];
	}

	[[nodiscard]] constexpr vec3_t at( const int index ) const
	{
		return vec3_t( data[ 0 ][ index ], data[ 1 ][ index ], data[ 2 ][ index ] );
	}

	float* base( )
	{
		return &data[ 0 ][ 0 ];
	}

	[[nodiscard]] const float* base( ) const
	{
		return &data[ 0 ][ 0 ];
	}

	float data[ 3 ][ 4 ] = { };
};

__declspec(align(16)) class matrix3x4a_t : public matrix3x4_t
{
public:
	matrix3x4a_t& operator=( const matrix3x4_t& source )
	{
		std::copy_n( source.base( ), sizeof( float ) * 3U * 4U, this->base( ) );
		return *this;
	}
};

struct view_matrix_t
{
	view_matrix_t( ) = default;

	constexpr view_matrix_t(
		const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23,
		const float m30, const float m31, const float m32, const float m33 )
	{
		data[ 0 ][ 0 ] = m00; data[ 0 ][ 1 ] = m01; data[ 0 ][ 2 ] = m02; data[ 0 ][ 3 ] = m03;
		data[ 1 ][ 0 ] = m10; data[ 1 ][ 1 ] = m11; data[ 1 ][ 2 ] = m12; data[ 1 ][ 3 ] = m13;
		data[ 2 ][ 0 ] = m20; data[ 2 ][ 1 ] = m21; data[ 2 ][ 2 ] = m22; data[ 2 ][ 3 ] = m23;
		data[ 3 ][ 0 ] = m30; data[ 3 ][ 1 ] = m31; data[ 3 ][ 2 ] = m32; data[ 3 ][ 3 ] = m33;
	}

	constexpr view_matrix_t( const matrix3x4_t& from, const vec4_t& additional_column = { } )
	{
		data[ 0 ][ 0 ] = from.data[ 0 ][ 0 ]; data[ 0 ][ 1 ] = from.data[ 0 ][ 1 ]; data[ 0 ][ 2 ] = from.data[ 0 ][ 2 ]; data[ 0 ][ 3 ] = from.data[ 0 ][ 3 ];
		data[ 1 ][ 0 ] = from.data[ 1 ][ 0 ]; data[ 1 ][ 1 ] = from.data[ 1 ][ 1 ]; data[ 1 ][ 2 ] = from.data[ 1 ][ 2 ]; data[ 1 ][ 3 ] = from.data[ 1 ][ 3 ];
		data[ 2 ][ 0 ] = from.data[ 2 ][ 0 ]; data[ 2 ][ 1 ] = from.data[ 2 ][ 1 ]; data[ 2 ][ 2 ] = from.data[ 2 ][ 2 ]; data[ 2 ][ 3 ] = from.data[ 2 ][ 3 ];
		data[ 3 ][ 0 ] = additional_column.x; data[ 3 ][ 1 ] = additional_column.y; data[ 3 ][ 2 ] = additional_column.z; data[ 3 ][ 3 ] = additional_column.w;
	}

	float* operator[]( const int index )
	{
		return data[ index ];
	}

	const float* operator[]( const int index ) const
	{
		return data[ index ];
	}

	[[nodiscard]] constexpr vec4_t at( const int index ) const
	{
		return vec4_t( data[ 0 ][ index ], data[ 1 ][ index ], data[ 2 ][ index ], data[ 3 ][ index ] );
	}

	constexpr view_matrix_t& operator+=( const view_matrix_t& add )
	{
		for (int i = 0; i < 4; i++)
		{
			for (int n = 0; n < 4; n++)
				this->data[ i ][ n ] += add[ i ][ n ];
		}

		return *this;
	}

	constexpr view_matrix_t& operator-=( const view_matrix_t& subtract )
	{
		for (int i = 0; i < 4; i++)
		{
			for (int n = 0; n < 4; n++)
				this->data[ i ][ n ] -= subtract[ i ][ n ];
		}

		return *this;
	}

	view_matrix_t operator*( const view_matrix_t& multiply ) const
	{
		return view_matrix_t(
			data[ 0 ][ 0 ] * multiply.data[ 0 ][ 0 ] + data[ 0 ][ 1 ] * multiply.data[ 1 ][ 0 ] + data[ 0 ][ 2 ] * multiply.data[ 2 ][ 0 ] + data[ 0 ][ 3 ] * multiply.data[ 3 ][ 0 ],
			data[ 0 ][ 0 ] * multiply.data[ 0 ][ 1 ] + data[ 0 ][ 1 ] * multiply.data[ 1 ][ 1 ] + data[ 0 ][ 2 ] * multiply.data[ 2 ][ 1 ] + data[ 0 ][ 3 ] * multiply.data[ 3 ][ 1 ],
			data[ 0 ][ 0 ] * multiply.data[ 0 ][ 2 ] + data[ 0 ][ 1 ] * multiply.data[ 1 ][ 2 ] + data[ 0 ][ 2 ] * multiply.data[ 2 ][ 2 ] + data[ 0 ][ 3 ] * multiply.data[ 3 ][ 2 ],
			data[ 0 ][ 0 ] * multiply.data[ 0 ][ 3 ] + data[ 0 ][ 1 ] * multiply.data[ 1 ][ 3 ] + data[ 0 ][ 2 ] * multiply.data[ 2 ][ 3 ] + data[ 0 ][ 3 ] * multiply.data[ 3 ][ 3 ],

			data[ 1 ][ 0 ] * multiply.data[ 0 ][ 0 ] + data[ 1 ][ 1 ] * multiply.data[ 1 ][ 0 ] + data[ 1 ][ 2 ] * multiply.data[ 2 ][ 0 ] + data[ 1 ][ 3 ] * multiply.data[ 3 ][ 0 ],
			data[ 1 ][ 0 ] * multiply.data[ 0 ][ 1 ] + data[ 1 ][ 1 ] * multiply.data[ 1 ][ 1 ] + data[ 1 ][ 2 ] * multiply.data[ 2 ][ 1 ] + data[ 1 ][ 3 ] * multiply.data[ 3 ][ 1 ],
			data[ 1 ][ 0 ] * multiply.data[ 0 ][ 2 ] + data[ 1 ][ 1 ] * multiply.data[ 1 ][ 2 ] + data[ 1 ][ 2 ] * multiply.data[ 2 ][ 2 ] + data[ 1 ][ 3 ] * multiply.data[ 3 ][ 2 ],
			data[ 1 ][ 0 ] * multiply.data[ 0 ][ 3 ] + data[ 1 ][ 1 ] * multiply.data[ 1 ][ 3 ] + data[ 1 ][ 2 ] * multiply.data[ 2 ][ 3 ] + data[ 1 ][ 3 ] * multiply.data[ 3 ][ 3 ],

			data[ 2 ][ 0 ] * multiply.data[ 0 ][ 0 ] + data[ 2 ][ 1 ] * multiply.data[ 1 ][ 0 ] + data[ 2 ][ 2 ] * multiply.data[ 2 ][ 0 ] + data[ 2 ][ 3 ] * multiply.data[ 3 ][ 0 ],
			data[ 2 ][ 0 ] * multiply.data[ 0 ][ 1 ] + data[ 2 ][ 1 ] * multiply.data[ 1 ][ 1 ] + data[ 2 ][ 2 ] * multiply.data[ 2 ][ 1 ] + data[ 2 ][ 3 ] * multiply.data[ 3 ][ 1 ],
			data[ 2 ][ 0 ] * multiply.data[ 0 ][ 2 ] + data[ 2 ][ 1 ] * multiply.data[ 1 ][ 2 ] + data[ 2 ][ 2 ] * multiply.data[ 2 ][ 2 ] + data[ 2 ][ 3 ] * multiply.data[ 3 ][ 2 ],
			data[ 2 ][ 0 ] * multiply.data[ 0 ][ 3 ] + data[ 2 ][ 1 ] * multiply.data[ 1 ][ 3 ] + data[ 2 ][ 2 ] * multiply.data[ 2 ][ 3 ] + data[ 2 ][ 3 ] * multiply.data[ 3 ][ 3 ],

			data[ 3 ][ 0 ] * multiply.data[ 0 ][ 0 ] + data[ 3 ][ 1 ] * multiply.data[ 1 ][ 0 ] + data[ 3 ][ 2 ] * multiply.data[ 2 ][ 0 ] + data[ 3 ][ 3 ] * multiply.data[ 3 ][ 0 ],
			data[ 3 ][ 0 ] * multiply.data[ 0 ][ 1 ] + data[ 3 ][ 1 ] * multiply.data[ 1 ][ 1 ] + data[ 3 ][ 2 ] * multiply.data[ 2 ][ 1 ] + data[ 3 ][ 3 ] * multiply.data[ 3 ][ 1 ],
			data[ 3 ][ 0 ] * multiply.data[ 0 ][ 2 ] + data[ 3 ][ 1 ] * multiply.data[ 1 ][ 2 ] + data[ 3 ][ 2 ] * multiply.data[ 2 ][ 2 ] + data[ 3 ][ 3 ] * multiply.data[ 3 ][ 2 ],
			data[ 3 ][ 0 ] * multiply.data[ 0 ][ 3 ] + data[ 3 ][ 1 ] * multiply.data[ 1 ][ 3 ] + data[ 3 ][ 2 ] * multiply.data[ 2 ][ 3 ] + data[ 3 ][ 3 ] * multiply.data[ 3 ][ 3 ] );
	}

	constexpr void identity( )
	{
		for (int i = 0; i < 4; i++)
		{
			for (int n = 0; n < 4; n++)
				this->data[ i ][ n ] = i == n ? 1.0f : 0.0f;
		}
	}

	const matrix3x4_t& as3x4( ) const
	{
		return *reinterpret_cast<const matrix3x4_t*>(this);
	}

	matrix3x4_t& as3x4( )
	{
		return *reinterpret_cast<matrix3x4_t*>(this);
	}

	float data[ 4 ][ 4 ] = { };
};

