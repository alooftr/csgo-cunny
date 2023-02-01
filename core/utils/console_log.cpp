#include "console_log.h"

bool csl::attach_console( const char* title )
{
	if (!AllocConsole( ))
		return false;

	AttachConsole( ATTACH_PARENT_PROCESS );

	if (freopen_s( &stream, "CONOUT$", "w", stdout ) != 0)
		return false;

	// set console window title
	if (!SetConsoleTitle( title ))
		return false;

	csl::print( "setup noctilua" );

	return true;
}

void csl::detach_console( )
{
	fclose( stream );

	FreeConsole( );

	if (const auto hConsoleWnd = GetConsoleWindow( ); hConsoleWnd != nullptr)
		PostMessageW( hConsoleWnd, WM_CLOSE, 0U, 0L );
}
