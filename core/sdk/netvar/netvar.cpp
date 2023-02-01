#include <format>

#include "netvar.hpp"
#include "../sdk.hpp"

bool c_netvar_manager::initialize( const char* file_name )
{
	// clear values
	map_props.clear( );
	stored_props = 0;
	stored_tables = 0;

	// format time
	const std::string time = std::vformat( "[{:%d-%m-%Y %X}] ", std::make_format_args( std::chrono::system_clock::now( ) ) );

#ifdef _DEBUG
	// open our dump file to write in (here is not exception handle because dump is not critical)
	dump_file.open( utils::get_game_path( ).append( file_name ), std::ios::out | std::ios::trunc );

	if (dump_file.good( ))
		// write current date, time and info
		dump_file << time << "netvars dump\n\n";
#endif

	for (auto class_pointer = csgo::client->get_all_classes( ); class_pointer != nullptr; class_pointer = class_pointer->next)
	{
		if (class_pointer->recv_table == nullptr)
			continue;

		store_props( class_pointer->network_name, class_pointer->recv_table, 0U, 0 );
	}

#ifdef _DEBUG
	// close dump file
	dump_file.close( );
#endif

	return !map_props.empty( );
}

std::uintptr_t c_netvar_manager::find_in_data_map( data_map_t* map, const fnv1a_t field_hash )
{
	while (map != nullptr)
	{
		for (int i = 0; i < map->data_fields; i++)
		{
			if (map->data_desc[ i ].field_name == nullptr)
				continue;

			if (fnv1a::hash( map->data_desc[ i ].field_name ) == field_hash)
				return map->data_desc[ i ].field_offset[ td_offset_normal ];

			if (map->data_desc[ i ].field_type == field_embedded)
			{
				if (map->data_desc[ i ].type_description != nullptr)
				{
					if (const auto uOffset = find_in_data_map( map->data_desc[ i ].type_description, field_hash ); uOffset != 0U)
						return uOffset;
				}
			}
		}

		map = map->base_map;
	}

	return 0U;
}

void c_netvar_manager::store_props( const char* class_name, recv_table_t* recv_table, const std::uintptr_t offset, const int depth )
{
#ifdef _DEBUG
	std::string szDepth = { };

	for (int i = 0; i < depth; i++)
		szDepth.append( "\t" );

	if (dump_file.good( ))
		dump_file << std::format( "{0}[{1}]\n", szDepth, recv_table->net_table_name );
#endif

	const fnv1a_t class_hash = fnv1a::hash( class_name );
	const fnv1a_t delimiter_hash = fnv1a::hash( "->", class_hash );
	const fnv1a_t base_class_hash = fnv1a::hash( "baseclass", delimiter_hash );

	for (int i = 0; i < recv_table->props_num; ++i)
	{
		const auto current_prop = &recv_table->props_pointer[ i ];

		// base tables filter
		if (current_prop == nullptr || isdigit( current_prop->var_name[ 0 ] ))
			continue;

		// concat to our netvar format just by hash
		const fnv1a_t total_hash = fnv1a::hash( current_prop->var_name, delimiter_hash );

		// skip baseclass
		if (total_hash == base_class_hash)
			continue;

		// has child table
		if (const auto child_table = current_prop->data_table; child_table != nullptr &&
			// has props
			child_table->props_num > 0 &&
			// first char is 'D' ("DT" - "DataTable")
			child_table->net_table_name[ 0 ] == 'D' &&
			// type is data table
			current_prop->recv_type == DPT_DATATABLE)
			// recursively get props in all child tables
			store_props( class_name, child_table, static_cast<std::uintptr_t>(current_prop->offset) + offset, depth + 1 );

		const std::uintptr_t uTotalOffset = static_cast<std::uintptr_t>(current_prop->offset) + offset;

		// check if we have already grabbed property
		if (!map_props[ total_hash ].offset)
		{
#ifdef _DEBUG
			if (dump_file.good( ))
				dump_file << std::format( "{0}\t{1} {2} = 0x{3:04X};\n", szDepth, get_property_type( current_prop ), current_prop->var_name, uTotalOffset );
#endif

			// write values to map entry
			map_props[ total_hash ] = { current_prop, uTotalOffset };

			// count total stored props
			stored_props++;
		}
	}

	// count total stored tables
	stored_tables++;
}

std::string c_netvar_manager::get_property_type( const recv_prop_t* recv_prop ) const
{
	static c_standart_recv_proxies* pStandartRecvProxies = csgo::client->get_standard_recv_proxies( );

	if (recv_prop == nullptr)
		return "";

	recv_var_proxy_fn pProxyFn = recv_prop->proxy_fn;

	switch (recv_prop->recv_type)
	{
	case DPT_INT:
		// @credits: hinnie
		if (pProxyFn == pStandartRecvProxies->int32_to_int8)
			return "byte";
		else if (pProxyFn == pStandartRecvProxies->int32_to_int16)
			return "short";

		return "int";
	case DPT_FLOAT:
		return "float";
	case DPT_VECTOR:
		return "vec3_t";
	case DPT_VECTOR2D:
		return "vec2_t";
	case DPT_STRING:
		return std::vformat( "char[{:d}]", std::make_format_args( recv_prop->string_buffer_size ) );
	case DPT_ARRAY:
		return std::vformat( "array[{:d}]", std::make_format_args( recv_prop->elements ) );
	case DPT_DATATABLE:
		return "void*";
	case DPT_INT64:
		return "std::int64_t";
	default:
		break;
	}

	return "";
}
