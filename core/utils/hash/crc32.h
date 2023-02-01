#pragma once

// used: std::uint32_t
#include <cstdint>

using crc32_t = std::uint32_t;

namespace crc32
{
	void	init( crc32_t* pulCRC );
	void	process_buffer( crc32_t* pulCRC, const void* p, int len );
	void	final( crc32_t* pulCRC );
	crc32_t	get_table_entry( unsigned int nSlot );

	inline crc32_t process_single_buffer( const void* p, int len )
	{
		crc32_t crc;
		crc32::init( &crc );
		crc32::process_buffer( &crc, p, len );
		crc32::final( &crc );
		return crc;
	}
}