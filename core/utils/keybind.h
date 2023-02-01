#pragma once

enum e_key_state_type : unsigned
{
	key_state_hold = 0,
	key_state_toggle,
	key_state_on
};

class key_t
{
public:
	key_t( int key = 0, int key_state = key_state_on ) : key{ key }, key_state{ key_state } { }
	
	bool is_active( );

	int key = 0;
	int key_state = key_state_on;

private:
	bool active = false;
};