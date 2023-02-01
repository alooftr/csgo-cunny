#pragma once
#include <corecrt_malloc.h>

template <class T, class N = int>
class c_utl_memory
{
public:
	T& operator[]( N i )
	{
		return memory[ i ];
	}

	const T& operator[]( N i ) const
	{
		return memory[ i ];
	}

	T* base( )
	{
		return memory;
	}

	[[nodiscard]] int num_allocated( ) const
	{
		return i_allocation_count;
	}

	[[nodiscard]] bool is_externally_allocated( ) const
	{
		return grow_size < 0;
	}

	void grow( const int iCount = 1 )
	{
		if (is_externally_allocated( ))
			return;

		int allocation_requested = i_allocation_count + iCount;
		int new_allocation_count = 0;

		if (grow_size)
			i_allocation_count = ((1 + ((allocation_requested - 1) / grow_size)) * grow_size);
		else
		{
			if (!i_allocation_count)
				i_allocation_count = (31 + sizeof( T )) / sizeof( T );

			while (i_allocation_count < allocation_requested)
				i_allocation_count <<= 1;
		}

		if (static_cast<int>(static_cast<N>(new_allocation_count)) < allocation_requested)
		{
			if (static_cast<int>(static_cast<N>(new_allocation_count)) == 0 && static_cast<int>(static_cast<N>(new_allocation_count - 1)) >= allocation_requested)
				--new_allocation_count;
			else
			{
				if (static_cast<int>(static_cast<N>(allocation_requested)) != allocation_requested)
					return;

				while (static_cast<int>(static_cast<N>(new_allocation_count)) < allocation_requested)
					new_allocation_count = (new_allocation_count + allocation_requested) / 2;
			}
		}

		i_allocation_count = new_allocation_count;

		if (memory != nullptr)
			memory = static_cast<T*>(realloc( memory, i_allocation_count * sizeof( T ) ));
		else
			memory = static_cast<T*>(malloc( i_allocation_count * sizeof( T ) ));
	}

protected:
	T* memory;
	int i_allocation_count;
	int grow_size;
};
