#pragma once
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/dt_common.h
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/dt_recv.h
#include <cstdint>

enum e_send_prop_type : int
{
	DPT_INT = 0,
	DPT_FLOAT,
	DPT_VECTOR,
	DPT_VECTOR2D,
	DPT_STRING,
	DPT_ARRAY,
	DPT_DATATABLE,
	DPT_INT64,
	DPT_SENDPROPTYPEMAX
};

struct recv_prop_t;
struct data_variant_t
{
	union
	{
		float	Float;
		long	Int;
		char* String;
		void* Data;
		float	Vector[ 3 ];
		int64_t Int64;
	};

	e_send_prop_type iType;
};

class c_recv_proxy_data
{
public:
	const recv_prop_t* recv_prop;	// the property it's receiving
	data_variant_t		value;		// the value given to you to store
	int					element;	// which array element you're getting
	int					object_id;	// the object being referred to
};

using recv_var_proxy_fn = void( __cdecl* )(const c_recv_proxy_data*, void*, void*);
using array_length_proxy_fn = void( __cdecl* )(void*, int, int);
using data_table_proxy_fn = void( __cdecl* )(const recv_prop_t*, void**, void*, int);

class c_standart_recv_proxies
{
public:
	recv_var_proxy_fn int32_to_int8;
	recv_var_proxy_fn int32_to_int16;
	recv_var_proxy_fn int32_to_int32;
	recv_var_proxy_fn int64_to_int64;
	recv_var_proxy_fn float_to_float;
	recv_var_proxy_fn vector_to_vector;
};

// receive data table
struct recv_table_t
{
	recv_prop_t* props_pointer;
	int				props_num;
	void* decoder;
	char* net_table_name;
	bool			initialized;
	bool			in_main_list;
};

// receive prop comes from the receive data table
struct recv_prop_t
{
	char* var_name;
	e_send_prop_type		recv_type;
	int						flags;
	int						string_buffer_size;
	bool					inside_array;
	const void* extra_data;
	recv_prop_t* array_prop;
	array_length_proxy_fn* array_length_proxy_fn;
	recv_var_proxy_fn		proxy_fn;
	data_table_proxy_fn* data_table_proxy_fn;
	recv_table_t* data_table;
	int						offset;
	int						element_stride;
	int						elements;
	const char* parent_array_prop_name;
};
