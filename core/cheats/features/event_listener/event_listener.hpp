#pragma once
#include <deque>
#include "../../../sdk/sdk.hpp"

class c_event_listener : public i_game_event_listener2
{
public:
	void setup(const std::deque<const char*>& game_events);
	void release();
	void add_listener(const char* name);
	bool find_listener(const char* name);

	virtual void fire_game_event(i_game_event* game_events) override;
	virtual int get_event_debug_id() override
	{
		return EVENT_DEBUG_ID_INIT;
	}
};

extern c_event_listener event_listener;
