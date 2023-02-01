#pragma once

#include <cstdint>
#include <string>

using fnv1a_t = std::uint32_t;

/*
 * 32-BIT FNV1A HASH
 * @credits: underscorediscovery
 */
namespace fnv1a
{
	/* @section: fnv1a constants */
	constexpr std::uint32_t basis = 0x811C9DC5;
	constexpr std::uint32_t prime = 0x1000193;

	/* @section: get */
	/// @param string : string to generate hash of
	/// @param value : key of hash generation
	/// @returns : calculated at compile-time FNV1A hash of given string
	consteval fnv1a_t hash_const( const char* string, const fnv1a_t value = basis ) noexcept
	{
		return (string[ 0 ] == '\0') ? value : hash_const( &string[ 1 ], (value ^ static_cast<fnv1a_t>(string[ 0 ])) * prime );
	}

	/// @param string : string to generate hash of
	/// @param value : key of hash generation
	/// @returns : calculated at run-time FNV1A hash of given string
	inline fnv1a_t hash( const char* string, fnv1a_t value = basis )
	{
		for (std::size_t i = 0U; i < strlen( string ); ++i)
		{
			value ^= string[ i ];
			value *= prime;
		}

		return value;
	}
}