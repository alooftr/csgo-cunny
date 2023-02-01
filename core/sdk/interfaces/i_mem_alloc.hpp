#pragma once
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/tier0/memalloc.h

using mem_alloc_fail_handler_t = std::size_t( __cdecl* )(std::size_t);

// @note: not full class
class i_mem_alloc
{
public:
	// Release versions
	virtual void* alloc( std::size_t nSize ) = 0;
	virtual void* realloc( void* pMemory, std::size_t nSize ) = 0;
	virtual void  free( void* pMemory ) = 0;
	virtual void* expand( void* pMemory, std::size_t nSize ) = 0; // no longer supported

	// Debug versions
	virtual void* alloc( std::size_t nSize, const char* szFileName, int nLine ) = 0;
	virtual void* realloc( void* pMemory, std::size_t nSize, const char* szFileName, int nLine ) = 0;
	virtual void  free( void* pMemory, const char* szFileName, int nLine ) = 0;
	virtual void* expand( void* pMemory, std::size_t nSize, const char* szFileName, int nLine ) = 0; // no longer supported

	// Returns size of a particular allocation
	virtual std::size_t get_size( void* pMemory ) = 0;
};
