#include "i_engine_trace.hpp"
#include "../sdk.hpp"

void i_engine_trace::trace_line( const vec3_t& src, const vec3_t& dst, int mask, i_handle_entity* ent, int collision_group, trace_t* trace )
{
    static auto trace_filter_simple = utils::find_pattern( "client.dll", "55 8B EC 83 E4 F0 83 EC 7C 56 52" ) + 0x3D;
    assert( trace_filter_simple != 0 );

    std::uintptr_t filter[ 4 ] = {
    *reinterpret_cast<std::uintptr_t*>(trace_filter_simple),
    reinterpret_cast<std::uintptr_t>(ent),
    static_cast<std::uintptr_t>(collision_group),
    0
    };

    this->trace_ray( { src, dst }, mask, reinterpret_cast<c_trace_filter*>(filter), trace );
}

void i_engine_trace::trace_hull( const vec3_t& src, const vec3_t& dst, const vec3_t& mins, const vec3_t& maxs, int mask, i_handle_entity* ent, int collision_group, trace_t* trace )
{
    static auto trace_filter_simple = utils::find_pattern( "client.dll", "55 8B EC 83 E4 F0 83 EC 7C 56 52" ) + 0x3D;
    assert( trace_filter_simple != 0 );

    std::uintptr_t filter[ 4 ] = {
    *reinterpret_cast<std::uintptr_t*>(trace_filter_simple),
    reinterpret_cast<std::uintptr_t>(ent),
    static_cast<std::uintptr_t>(collision_group),
    0
    };

    this->trace_ray( { src, dst, mins, maxs }, mask, reinterpret_cast<c_trace_filter*>(filter), trace );
}
