#pragma once

class i_file_system
{
public:
	int read( void* output, int size, void* handle )
	{
		return utils::call_vfunc<int>( this, 0, output, size, handle );
	}

	void* open( const char* fileName, const char* options, const char* pathID )
	{
		return utils::call_vfunc<void*>( this, 2, fileName, options, pathID );
	}

	void close( void* handle )
	{
		return utils::call_vfunc<void>( this, 3, handle );
	}

	unsigned int size( void* handle )
	{
		return utils::call_vfunc<unsigned int>( this, 7, handle );
	}
};
