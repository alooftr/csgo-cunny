#pragma once

class i_client_networkable;
using create_client_class_fn = i_client_networkable * (__cdecl*)(int, int);
using create_event_fn = i_client_networkable * (__cdecl*)();

class c_standart_recv_proxies;
struct recv_table_t;
enum class e_class_index;

class c_base_client
{
public:
	create_client_class_fn		create_fn;
	create_event_fn			create_event_fn;
	char* network_name;
	recv_table_t* recv_table;
	c_base_client* next;
	e_class_index	class_id;
};

using create_interface_fn = void* (__cdecl*)(const char*, int*);
using instantiate_interface_fn = void* (__cdecl*)();
using demo_custom_data_callback_fn = void( __cdecl* )(unsigned char*, std::size_t);

class i_base_client_dll
{
public:
	c_base_client* get_all_classes( )
	{
		return utils::call_vfunc<c_base_client*>( this, 8 );
	}

	bool dispatch_user_message( int message_type, unsigned int flags, int size, const void* message_data )
	{
		return utils::call_vfunc<bool>( this, 38, message_type, flags, size, message_data );
	}

	c_standart_recv_proxies* get_standard_recv_proxies( )
	{
		return utils::call_vfunc<c_standart_recv_proxies*>( this, 48 );
	}
};