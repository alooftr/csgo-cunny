#pragma once

class i_input_system
{
public:
	void get_cursor_position( int* x, int* y )
	{
		utils::call_vfunc<void>( this, 56, x, y );
	}
};
