#include "event_listener.hpp"

// including features
#include "../skins/skins.h"

void c_event_listener::setup(const std::deque<const char*>& game_events)
{
	if (game_events.empty())
		return;

	for (auto event : game_events)
	{
		add_listener(event);
	}
}

void c_event_listener::release()
{
	csgo::event_manager->remove_listener(this);
}

void c_event_listener::add_listener( const char* name )
{
	csgo::event_manager->add_listener( this, name, false );

	if (!find_listener( name ))
		throw std::runtime_error( std::vformat( "failed to add {} listener", std::make_format_args(name) ) );

	csl::push_color( FOREGROUND_INTENSE_YELLOW );
	csl::print( "[event_listener] added [{}] listener ", name );
	csl::pop_color( );
}

bool c_event_listener::find_listener(const char* name)
{
	return csgo::event_manager->find_listener(this, name);
}

void c_event_listener::fire_game_event(i_game_event* game_event)
{
	if (game_event == nullptr)
		return;

	const fnv1a_t name_hash = fnv1a::hash(game_event->get_name());
	switch (name_hash)
	{
	case fnv1a::hash_const( "player_death" ):
	{
		c_skin::get( ).on_player_hurt( game_event );
		break;
	}
	case fnv1a::hash_const( "player_hurt" ):
	{

		break;
	}
	}
}

c_event_listener event_listener;
