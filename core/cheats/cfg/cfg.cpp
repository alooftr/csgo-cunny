#include "cfg.h"
#include "../../../ext/json/json.hpp"

bool cfg::setup( std::string_view default_file_name )
{
	if ( !std::filesystem::is_directory( path ) )
	{
		std::filesystem::remove( path );
		if ( !std::filesystem::create_directories( path ) )
			return false;
	}

	if ( !save( default_file_name ) )
		return false;

	// load default config
	if ( !load( default_file_name ) )
		return false;

	// refresh cfgs list
	refresh( );

	return true;
}

bool cfg::save( std::string_view file_name )
{
	// check for extension if it is not our replace it
	std::filesystem::path file_path( file_name );
	if ( file_path.extension( ) != _( ".cfg" ) )
		file_path.replace_extension( _( ".cfg" ) );

	// get utf-8 full path to config
	const std::string file = std::filesystem::path( path / file_path ).string( );
	nlohmann::json config = { };

	try
	{
		for ( auto& variable : variables )
		{
			nlohmann::json entry = { };

			// save hashes to compare it later
			entry[ _( "name-id" ) ] = variable.name_hash;
			entry[ _( "type-id" ) ] = variable.type_hash;

			// get current variable
			switch ( variable.type_hash )
			{
				case fnv1a::hash_const( "int" ):
				{
					entry[ _( "value" ) ] = variable.get<int>( );
					break;
				}
				case fnv1a::hash_const( "float" ):
				{
					entry[ _( "value" ) ] = variable.get<float>( );
					break;
				}
				case fnv1a::hash_const( "bool" ):
				{
					entry[ _( "value" ) ] = variable.get<bool>( );
					break;
				}
				case fnv1a::hash_const( "std::string" ):
				{
					entry[ _( "value" ) ] = variable.get<std::string>( );
					break;
				}
				case fnv1a::hash_const( "col_t" ):
				{
					// dump it as hex value
					entry[ _( "value" ) ] = variable.get<col_t>( ).to_hex( );
					break;
				}
				case fnv1a::hash_const( "key_t" ):
				{
					const auto& bind = variable.get<key_t>( );

					// store key code and mode as sub-node
					nlohmann::json sub = { };

					// fill node with all key values
					sub.push_back( bind.key );
					sub.push_back( bind.key_state );

					entry[ _( "value" ) ] = sub.dump( );

					break;
				}
				case fnv1a::hash_const( "esp_variable_t" ):
				{
					auto& var = variable.get<esp_variable_t>( );

					// store key code and mode as sub-node
					nlohmann::json sub = { };

					sub.push_back( var.enable ? 1 : 0 );
					sub.push_back( var.position );

					sub.push_back( var.primary_color.to_hex( ) );
					sub.push_back( var.background_color.to_hex( ) );
					sub.push_back( var.outline_color.to_hex( ) );

					sub.push_back( var.glow ? 1 : 0 );
					sub.push_back( var.glow_color.to_hex( ) );

					entry[ _( "value" ) ] = sub.dump( );

					break;
				}
				case fnv1a::hash_const( "std::vector<bool>" ):
				{
					const auto& bools = variable.get<std::vector<bool>>( );

					// store vector values as sub-node
					nlohmann::json sub = { };

					// fill node with all vector values
					for ( const auto&& value : bools )
						sub.push_back( static_cast< bool >( value ) );

					entry[ _( "value" ) ] = sub.dump( );
					break;
				}
				case fnv1a::hash_const( "std::vector<int>" ):
				{
					const auto& ints = variable.get<std::vector<int>>( );

					// store vector values as sub-node
					nlohmann::json sub = { };

					// fill node with all vector values
					for ( const auto& value : ints )
						sub.push_back( value );

					entry[ _( "value" ) ] = sub.dump( );
					break;
				}
				case fnv1a::hash_const( "std::vector<float>" ):
				{
					const auto& floats = variable.get<std::vector<float>>( );

					// store vector values as sub-node
					nlohmann::json sub = { };

					// fill node with all vector values
					for ( const auto& value : floats )
						sub.push_back( value );

					entry[ _( "value" ) ] = sub.dump( );
					break;
				}
				case fnv1a::hash_const( "std::vector<std::string>" ):
				{
					const auto& strings = variable.get<std::vector<std::string>>( );
					// store vector values as sub-node
					nlohmann::json sub = { };

					// fill node with all vector values
					for ( const auto& value : strings )
						sub.push_back( value );

					entry[ _( "value" ) ] = sub.dump( );

					break;
				}
				case fnv1a::hash_const( "std::vector<col_t>" ):
				{
					auto& colors = variable.get<std::vector<col_t>>( );
					// store vector values as sub-node
					nlohmann::json sub = { };

					// fill node with all vector values
					for ( auto& value : colors )
						sub.push_back( value.to_hex( ) );
							
					entry[ _( "value" ) ] = sub.dump( );

					break;
				}
				default:
					break;
			}

			// add current variable to config
			config.push_back( entry );
		}
	}
	catch ( const nlohmann::detail::exception& ex )
	{
		csl::push_color( FOREGROUND_RED );
		csl::print( _( "[error] json save failed: {}" ), ex.what( ) );
		csl::pop_color( );
		return false;
	}

	// open output config file
	std::ofstream out_file( file, std::ios::out | std::ios::trunc );

	if ( !out_file.good( ) )
		return false;

	try
	{
		// write stored variables
		out_file << config.dump( 4 );
		out_file.close( );
	}
	catch ( std::ofstream::failure& ex )
	{
		csl::push_color( FOREGROUND_RED );
		csl::print( _( "[error] failed to save configuration: {}" ), ex.what( ) );
		csl::pop_color( );
		return false;
	}

	return true;
}

bool cfg::load( std::string_view file_name )
{
	// get utf-8 full path to config
	const std::string file = std::filesystem::path( path / file_name ).string( );
	nlohmann::json config = { };

	// open input config file
	std::ifstream in_file( file, std::ios::in );

	if ( !in_file.good( ) )
		return false;

	try
	{
		// parse saved variables
		config = nlohmann::json::parse( in_file, nullptr, false );

		// check is json parse failed
		if ( config.is_discarded( ) )
			return false;

		in_file.close( );
	}
	catch ( std::ifstream::failure& ex )
	{
		csl::push_color( FOREGROUND_RED );
		csl::print( _( "[error] Failed to load configuration: {}" ), ex.what( ) );
		csl::pop_color( );
		return false;
	}

	try
	{
		for ( const auto& variable : config )
		{
			const std::size_t index = get_variable_index( variable[ _( "name-id" ) ].get<fnv1a_t>( ) );

			// check is variable exist
			if ( index == c_invalid_variable )
				continue;

			// get variable
			auto& entry = variables.at( index );

			switch ( variable[ _( "type-id" ) ].get<fnv1a_t>( ) )
			{
				case fnv1a::hash_const( "bool" ):
				{
					entry.set<bool>( variable[ _( "value" ) ].get<bool>( ) );
					break;
				}
				case fnv1a::hash_const( "float" ):
				{
					entry.set<float>( variable[ _( "value" ) ].get<float>( ) );
					break;
				}
				case fnv1a::hash_const( "int" ):
				{
					entry.set<int>( variable[ _( "value" ) ].get<int>( ) );
					break;
				}
				case fnv1a::hash_const( "std::string" ):
				{
					entry.set<std::string>( variable[ _( "value" ) ].get<std::string>( ) );
					break;
				}
				case fnv1a::hash_const( "col_t" ):
				{
					entry.set<col_t>( col_t::from_hex( variable[ _( "value" ) ].get<unsigned long>( ) ) );
					break;
				}
				case fnv1a::hash_const( "key_t" ):
				{
					const nlohmann::json vector = nlohmann::json::parse( variable[ _( "value" ) ].get<std::string>( ) );

					entry.set<key_t>( key_t( vector.at( 0 ), vector.at( 1 ) ) );

					break;
				}
				case fnv1a::hash_const( "esp_variable_t" ):
				{
					const nlohmann::json vector = nlohmann::json::parse( variable[ _( "value" ) ].get<std::string>( ) );

					col_t primary_color = col_t::from_hex( vector.at( 2 ).get<unsigned long>( ) );
					col_t background_color = col_t::from_hex( vector.at( 3 ).get<unsigned long>( ) );
					col_t outline_color = col_t::from_hex( vector.at( 4 ).get<unsigned long>( ) );
					col_t glow_color = col_t::from_hex( vector.at( 6 ).get<unsigned long>( ) );

					entry.set<esp_variable_t>( esp_variable_t( vector.at( 1 ).get<int>( ), primary_color, vector.at( 0 ).get<int>( ) ? true : false, background_color, outline_color, vector.at( 5 ).get<int>( ) ? true : false, glow_color ) );

					break;
				}
				case fnv1a::hash_const( "std::vector<bool>" ):
				{
					const nlohmann::json vector = nlohmann::json::parse( variable[ _( "value" ) ].get<std::string>( ) );
					auto& bools = entry.get<std::vector<bool>>( );

					for ( std::size_t i = 0U; i < vector.size( ); i++ )
					{
						// check is item out of bounds
						if ( i < bools.size( ) )
							bools.at( i ) = vector.at( i ).get<bool>( );
					}

					break;
				}
				case fnv1a::hash_const( "std::vector<int>" ):
				{
					const nlohmann::json vector = nlohmann::json::parse( variable[ _( "value" ) ].get<std::string>( ) );
					auto& ints = entry.get<std::vector<int>>( );

					for ( std::size_t i = 0U; i < vector.size( ); i++ )
					{
						// check is item out of bounds
						if ( i < ints.size( ) )
							ints.at( i ) = vector.at( i ).get<int>( );
					}

					break;
				}
				case fnv1a::hash_const( "std::vector<float>" ):
				{
					const nlohmann::json vector = nlohmann::json::parse( variable[ _( "value" ) ].get<std::string>( ) );
					auto& floats = entry.get<std::vector<float>>( );

					for ( std::size_t i = 0U; i < vector.size( ); i++ )
					{
						// check is item out of bounds
						if ( i < floats.size( ) )
							floats.at( i ) = vector.at( i ).get<float>( );
					}

					break;
				}
				case fnv1a::hash_const( "std::vector<std::string>" ):
				{
					const nlohmann::json vector = nlohmann::json::parse( variable[ _( "value" ) ].get<std::string>( ) );
					auto& strings = entry.get<std::vector<std::string>>( );

					for ( std::size_t i = 0U; i < vector.size( ); i++ )
					{
						// check is item out of bounds
						if ( i < strings.size( ) )
							strings.at( i ) = vector.at( i ).get<std::string>( );
					}

					break;
				}
				case fnv1a::hash_const( "std::vector<col_t>" ):
				{
					const nlohmann::json vector = nlohmann::json::parse( variable[ _( "value" ) ].get<std::string>( ) );
					auto& colors = entry.get<std::vector<col_t>>( );

					for ( std::size_t i = 0U; i < vector.size( ); i++ )
					{
						// check is item out of bounds
						if ( i < colors.size( ) )
							colors.at( i ) = col_t::from_hex( vector.at( i ).get<unsigned long>( ) );
					}

					break;
				}
				default:
					break;
			}
		}
	}
	catch ( const nlohmann::detail::exception& ex )
	{
		csl::push_color( FOREGROUND_RED );
		csl::print( _( "[ error ] json load failed : {}" ), ex.what( ) );
		csl::pop_color( );
		return false;
	}

	return true;
}

void cfg::remove( const std::size_t index )
{
	const std::string& file_name = file_names.at( index );

	// unable delete default config
	if ( file_name.compare( _( "default.cfg" ) ) == 0 )
		return;

	// get utf-8 full path to config
	const std::string file = std::filesystem::path( path / file_name ).string( );

	if ( std::filesystem::remove( file ) )
	{
		file_names.erase( file_names.cbegin( ) + static_cast< std::ptrdiff_t >( index ) );
	}
}

void cfg::refresh( )
{
	file_names.clear( );

	for ( const auto& it : std::filesystem::directory_iterator( path ) )
	{
		if ( it.path( ).filename( ).extension( ) == _( ".cfg" ) )
		{
			file_names.push_back( it.path( ).filename( ).string( ) );
			csl::print( _( "found config: {}" ), it.path( ).filename( ).string( ) );
		}
	}
}

std::size_t cfg::get_variable_index( const fnv1a_t name_hash )
{
	for ( std::size_t i = 0U; i < variables.size( ); i++ )
	{
		if ( variables.at( i ).name_hash == name_hash )
			return i;
	}

	return c_invalid_variable;
}