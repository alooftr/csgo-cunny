#pragma once

class i_localize
{
public:
	wchar_t* find( const char* token )
	{
		return utils::call_vfunc<wchar_t*>( this, 11, token );
	}

	const wchar_t* find_safe( const char* token )
	{
		return utils::call_vfunc<wchar_t*>( this, 12, token );
	}

	int convert_unicode_to_ansi( const wchar_t* unicode, char* ansi, int ansi_buffer_size ) {
		return utils::call_vfunc<int>( this, 16, unicode, ansi, ansi_buffer_size );
	}

	std::string convert_unicode_to_ansi( const wchar_t* unicode ) noexcept
	{
		char buffer[ 4096 ];
		convert_unicode_to_ansi( unicode, buffer, sizeof( buffer ) );
		return buffer;
	}
};