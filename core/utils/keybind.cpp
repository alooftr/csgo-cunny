#include "keybind.h"

// used hold, waskeypressed
#include "input.h"

bool key_t::is_active( )
{
	switch (this->key_state)
	{
	case key_state_hold:

		active = ipt::is_key_down( this->key );

		break;
	case key_state_toggle:

		if (ipt::is_key_released( this->key ))
			active = !active;
		break;
	case key_state_on:

		active = true;

		break;
	}

	return active;
}
