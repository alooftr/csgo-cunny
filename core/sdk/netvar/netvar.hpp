#pragma once
#include <fstream>
#include <unordered_map>

#include "../../utils/singleton.h"

#include "../data_types/map.hpp"
#include "../data_types/table.hpp"
#include "../../utils/hash/fnv1a.h"

#pragma region netvar_definitions
/* add function to get variable with additional offset from netvar offset */
#define N_ADD_VARIABLE_OFFSET( type, function_name, net_var, additional )								\
	[[nodiscard]] std::add_lvalue_reference_t<type> function_name()										\
	{																										\
		static constexpr fnv1a_t hash = fnv1a::hash_const(net_var);										\
		static std::uintptr_t offset = c_netvar_manager::get().map_props[hash].offset;						\
		return *(std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + offset + additional);	\
	}

/* add function to get netvar variable */
#define N_ADD_VARIABLE( type, function_name, net_var ) N_ADD_VARIABLE_OFFSET( type, function_name, net_var, 0U )

/* add function to get variable pointer with additional offset from netvar offset */
#define N_ADD_PVARIABLE_OFFSET( type, function_name, net_var, additional )								\
	[[nodiscard]] std::add_pointer_t<type> function_name()													\
	{																										\
		static constexpr fnv1a_t hash = fnv1a::hash_const(net_var);										\
		static std::uintptr_t offset = c_netvar_manager::get().map_props[hash].offset;						\
		return (std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + offset + additional);	\
	}

/* add function to get netvar variable pointer */
#define N_ADD_PVARIABLE( type, function_name, net_var ) N_ADD_PVARIABLE_OFFSET( type, function_name, net_var, 0U )

/* add function to get csplayerresource variable for entity by index from netvar offset */
#define N_ADD_RESOURCE_VARIABLE( type, function_name, net_var )														\
	[[nodiscard]] std::add_lvalue_reference_t<type> function_name(int index)											\
	{																													\
		static constexpr fnv1a_t hash = fnv1a::hash_const(net_var);													\
		static std::uintptr_t offset = c_netvar_manager::get().map_props[hash].offset;									\
		return *(std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + offset + index * sizeof(type));	\
	}

/* add function to get datamap variable */
#define N_ADD_DATAFIELD( type, function_name, map, szDataField )											\
	[[nodiscard]] std::add_lvalue_reference_t<type> function_name()										\
	{																										\
		static constexpr fnv1a_t hash = fnv1a::hash_const(szDataField);										\
		static std::uintptr_t offset = c_netvar_manager::get().find_in_data_map(map, hash);					\
		return *(std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + offset);				\
	}

/* add function to get datamap variable pointer */
#define N_ADD_PDATAFIELD( type, function_name, map, szDataField )											\
	[[nodiscard]] std::add_pointer_t<type> function_name()													\
	{																										\
		static constexpr fnv1a_t hash = fnv1a::hash_const(szDataField);										\
		static std::uintptr_t offset = c_netvar_manager::get().find_in_data_map(map, hash);					\
		return (std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + offset);				\
	}

/* add function to get variable by offset */
#define N_ADD_OFFSET( type, function_name, offset )														\
	[[nodiscard]] std::add_lvalue_reference_t<type> function_name()										\
	{																										\
		return *(std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + offset);				\
	}

/* add function to get variable pointer by offset */
#define N_ADD_POFFSET( type, function_name, offset )														\
	[[nodiscard]] std::add_pointer_t<type> function_name()													\
	{																										\
		return (std::add_pointer_t<type>)(reinterpret_cast<std::uintptr_t>(this) + offset);				\
	}
#pragma endregion

class c_recv_prop_hook
{
public:
	c_recv_prop_hook( recv_prop_t* recv_prop, const recv_var_proxy_fn new_proxy_fn ) :
		recv_prop( recv_prop ), original_fn( recv_prop->proxy_fn )
	{
		set_proxy( new_proxy_fn );
	}

	// get
	/* replace with our function */
	void replace( recv_prop_t* recv_prop )
	{
		this->recv_prop = recv_prop;
		this->original_fn = recv_prop->proxy_fn;
	}

	/* restore original function */
	void restore( ) const
	{
		if (this->original_fn != nullptr)
			this->recv_prop->proxy_fn = this->original_fn;
	}

	void set_proxy( const recv_var_proxy_fn new_proxy_fn ) const
	{
		this->recv_prop->proxy_fn = new_proxy_fn;
	}

	recv_var_proxy_fn get_original( ) const
	{
		return this->original_fn;
	}

private:
	// Values
	/* in future that is being modified and replace the original prop */
	recv_prop_t* recv_prop = nullptr;
	/* original proxy function to make able to restore it later */
	recv_var_proxy_fn original_fn = nullptr;
};

class c_netvar_manager : public c_singleton<c_netvar_manager>
{
public:
	struct netvar_object_t
	{
		recv_prop_t* recv_prop = nullptr;
		std::uintptr_t offset = 0U;
	};

	// get
	/* fill map with netvars and also dump it to given file */
	bool initialize( const char* file_name );
	/*
	 * stores the variables of objects in the hierarchy
	 * used to iterate through an object's data descriptions from data map
	 */
	std::uintptr_t find_in_data_map( data_map_t* map, const fnv1a_t field_hash );

	// Values
	/* logging counters */
	int stored_props = 0;
	int stored_tables = 0;
	/* networkable properties map */
	std::unordered_map<fnv1a_t, netvar_object_t> map_props = { };

private:
	/*
	 * recursively stores networked properties info from data tables in our map
	 * and also format our dump and write values to file
	 */
	void store_props( const char* class_name, recv_table_t* recv_table, const std::uintptr_t offset, const int depth );

	// Extra
	std::string get_property_type( const recv_prop_t* recv_prop ) const;

	// Values
	/* output file */
	std::ofstream dump_file = { };
};
