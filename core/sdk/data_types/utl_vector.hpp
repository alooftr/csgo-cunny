#pragma once
#include "utl_memory.hpp"

template <class T>
void destruct( T* memory )
{
	memory->~T( );
}

template <class T>
T* construct( T* memory )
{
	return new(memory) T;
}

template< class T >
T* copy( T* memory, T const& src )
{
	return new(memory) T( src );
}

template <class T, class A = c_utl_memory<T>>
class c_utl_vector
{
	using c_allocator = A;
public:
	auto begin( ) noexcept
	{
		return memory.base( );
	}

	auto end( ) noexcept
	{
		return memory.base( ) + vector_size;
	}

	auto begin( ) const noexcept
	{
		return memory.base( );
	}

	auto end( ) const noexcept
	{
		return memory.base( ) + vector_size;
	}

	T& operator[]( int i )
	{
		return memory[ i ];
	}

	const T& operator[]( int i ) const
	{
		return memory[ i ];
	}

	T& element( int i )
	{
		return memory[ i ];
	}

	const T& element( int i ) const
	{
		return memory[ i ];
	}

	T* base( )
	{
		return memory.base( );
	}

	int count( ) const
	{
		return vector_size;
	}

	int& size( )
	{
		return vector_size;
	}

	void grow_vector( int nCount = 1 )
	{
		if (vector_size + nCount > memory.num_allocated( ))
			memory.grow( vector_size + nCount - memory.num_allocated( ) );

		vector_size += nCount;
	}

	void shift_elements_right( const int nElement, const int nShift = 1 )
	{
		const int nToMove = vector_size - nElement - nShift;

		if (nToMove > 0 && nShift > 0)
			memmove( &element( nElement + nShift ), &element( nElement ), nToMove * sizeof( T ) );
	}

	void shift_elements_left( const int nElement, const int nShift = 1 )
	{
		const int nToMove = vector_size - nElement - nShift;

		if (nToMove > 0 && nShift > 0)
			memmove( &element( nElement ), &element( nElement + nShift ), nToMove * sizeof( T ) );
	}

	int insert_before( const int nElement )
	{
		// can insert at the end
		grow_vector( );
		shift_elements_right( nElement );
		construct( &element( nElement ) );
		return nElement;
	}

	int insert_before( int nElement, const T& src )
	{
		// reallocate if can't insert something that's in the list
		// can insert at the end
		grow_vector( );
		shift_elements_right( nElement );
		copy( &element( nElement ), src );
		return nElement;
	}

	int add_to_tail( )
	{
		return insert_before( vector_size );
	}

	int add_to_tail( const T& src )
	{
		return insert_before( vector_size, src );
	}

	int find( const T& src ) const
	{
		for (int i = 0; i < count( ); ++i)
		{
			if (element( i ) == src)
				return i;
		}

		return -1;
	}

	void remove( const int nElement )
	{
		destruct( &element( nElement ) );
		shift_elements_left( nElement );
		--vector_size;
	}

	void remove_all( )
	{
		for (int i = vector_size; --i >= 0;)
			destruct( &element( i ) );

		vector_size = 0;
	}

	bool find_and_remove( const T& src )
	{
		if (const int nElement = find( src ); nElement != -1)
		{
			remove( nElement );
			return true;
		}

		return false;
	}

protected:
	c_allocator memory;
	int vector_size;
	T* elements;
};