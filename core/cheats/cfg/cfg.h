#pragma once
// used: std::any
#include <any>
// used: std::filesystem
#include <filesystem>
// used: std::deque
#include <deque>
// used: std::vector
#include <vector>
// used: get_game_path
#include "../../utils.h"
//used: fnv1a hashing
#include "../../utils/hash/fnv1a.h"

// custom variable to save
#include "../../sdk/data_types/color.hpp"
#include "../../utils/keybind.h"
#include "extra_cfg_struct.h"

struct variable_object_t
{
	variable_object_t( const char* name, const char* type, std::any&& default_value )
		: name( name ), type( type ), value( std::move( default_value ) )
	{ 
		name_hash = fnv1a::hash( name );
		type_hash = fnv1a::hash( type );
	}

	~variable_object_t( ) = default;

	/* get casted variable value */
	template<typename T>
	T& get( )
	{
		return *static_cast< T* >( std::any_cast< T >( &value ) );
	}

	/* emplace casted variable value */
	template<typename T>
	void set( T set_value )
	{
		value.emplace<T>( set_value );
	}
	
	const char* name = "";
	const char* type = "";

	fnv1a_t name_hash = 0x0;
	fnv1a_t type_hash = 0x0;
	std::any value = { };
};

namespace cfg
{
	/* create directories, create and load default config */
	bool setup( std::string_view default_file_name );
	/* write values in config file */
	bool save( std::string_view file_name );
	/* read values from config file */
	bool load( std::string_view file_name );
	/* remove config file at given index */
	void remove( const std::size_t index );
	/* loop through directory content and push config filenames to vector */
	void refresh( );

	/* constant file path: csgo dir / cunny / cfg */
	const std::filesystem::path path = utils::get_game_path( ) / "cunny\\cfg";
	/* all user config filenames */
	inline std::deque<std::string> file_names = { };
	/* configuration variables */
	inline std::vector<variable_object_t> variables = { };

	std::size_t get_variable_index( const fnv1a_t name_hash );

	// get that config index and cast to the template
	template <typename t>
	t& get( const std::uint32_t index )
	{
		return variables.at( index ).get<t>( );
	}

	/* add new configuration variable to massive, and return index of it */
	template <typename T>
	std::uint32_t add_variable( const char* name_hash, const char* type_hash, const T default_value )
	{
		variables.emplace_back( name_hash, type_hash, std::make_any<T>( default_value ) );
		return variables.size( ) - 1U;
	}
}

#pragma region config_definitions
#define add_var( type, name, default_value ) const std::uint32_t name = cfg::add_variable<type>(#name, #type, default_value);
#define add_var_vector( type, size, name, default_value ) const std::uint32_t name = cfg::add_variable<std::vector<type>>(#name, "std::vector<" #type ">", utils::get_filled_vector<type, size>(default_value));
#define c_invalid_variable (std::size_t)(-1)
#pragma endregion