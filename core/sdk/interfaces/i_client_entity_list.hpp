#pragma once
#include "../data_types/utl_vector.hpp"
#include "i_client_entity.hpp"

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/client/cliententitylist.h#L298
class i_client_entity_listener
{
public:
	virtual void on_entity_created( c_base_entity* entity ) { }
	virtual void on_entity_deleted( c_base_entity* entity ) { }
};

class i_client_networkable;
class i_client_unknown;
class i_client_entity;

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/icliententitylist.h
class i_client_entity_list
{
public:
	virtual i_client_networkable* get_client_networkable( int index ) = 0;
	virtual i_client_networkable* get_client_networkable_from_handle( c_base_handle nettworkable ) = 0;
	virtual i_client_unknown* get_client_unknown_from_handle( c_base_handle unknown ) = 0;
	virtual i_client_entity* get_client_entity( int index ) = 0;
	virtual i_client_entity* get_client_entity_from_handle( c_base_handle entity ) = 0;
	virtual int					number_of_entities( bool include_non_networkable ) = 0;
	virtual int					get_highest_entity_index( ) = 0;
	virtual void				set_max_entities( int max ) = 0;
	virtual int					get_max_entities( ) = 0;

	template <class T = i_client_entity>
	inline T* get( const int index )
	{
		return static_cast<T*>(get_client_entity( index ));
	}

	template <class T = i_client_entity>
	inline T* get( const c_base_handle entity )
	{
		return static_cast<T*>(get_client_entity_from_handle( entity ));
	}

	void add_listener_entity( i_client_entity_listener* listener )
	{
		entity_listeners.add_to_tail( listener );
	}

	void remove_listener_entity( i_client_entity_listener* listener )
	{
		entity_listeners.find_and_remove( listener );
	}

private:
	c_utl_vector<i_client_entity_listener*> entity_listeners;
};
