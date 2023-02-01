#pragma once

class iv_panel
{
public:
	void set_key_board_input_enabled( unsigned int panel_index, bool state )
	{
		return utils::call_vfunc<void>( this, 31, panel_index, state );
	}

	void set_mouse_input_enabled( unsigned int panel_index, bool state )
	{
		return utils::call_vfunc<void>( this, 32, panel_index, state );
	}

	const char* get_name( int panel_index )
	{
		return utils::call_vfunc<const char*>( this, 36, panel_index );
	}

	const char* get_name_class( int panel_index )
	{
		return utils::call_vfunc<const char*>( this, 37, panel_index );
	}
};

class ui_panel {
public:
	int get_child_count( )
	{
		return utils::call_vfunc<int>( this, 48 );
	}

	ui_panel* get_child( int n )
	{
		return utils::call_vfunc<ui_panel*>( this, 49, n );
	}

	bool has_class( const char* name )
	{
		return utils::call_vfunc<bool>( this, 139, name );
	}

	void set_has_class( const char* name, bool has_class )
	{
		return utils::call_vfunc<void>( this, 145, name, has_class );
	}

	float get_attribute_float( const char* name, float default_value )
	{
		return utils::call_vfunc<float>( this, 278, name, default_value );
	}

	void set_attribute_float( const char* name, float value )
	{
		return utils::call_vfunc<void>( this, 288, name, value );
	}
};