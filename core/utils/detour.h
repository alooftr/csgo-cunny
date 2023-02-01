#pragma once
// used: format
#include <format>

// used: minhook hooking library
#include "../../ext/minhook/minhook.h"
#include "console_log.h"

/*
 * detour hooking implementation using minhook
 * @credits: https://github.com/TsudaKageyu/minhook
 */

class c_detour_hook
{
public:
	c_detour_hook( ) = default;

	explicit c_detour_hook( void* func, void* detour )
		: base_fn( func ), replace_fn( detour ) { }

	/* first hook setup */
	bool create( void* func, void* detour )
	{
		base_fn = func;

		if (base_fn == nullptr)
			return false;

		replace_fn = detour;

		if (replace_fn == nullptr)
			return false;

		const MH_STATUS status = MH_CreateHook( base_fn, replace_fn, &original_fn );

		if (status != MH_OK)
			throw std::runtime_error( std::format( "failed to create hook function, status: {}\nbase function -> {:#08x}", MH_StatusToString( status ), reinterpret_cast<std::uintptr_t>(base_fn) ) );

		if (!this->replace( ))
			return false;

		csl::print( "hooked base function -> {:#08X}", reinterpret_cast<std::uintptr_t>(base_fn) );

		return true;
	}

	/* replace function to our modified */
	bool replace( )
	{
		// check is hook be created
		if (base_fn == nullptr)
			return false;

		// check is function not already hooked
		if (hooked)
			return false;

		const MH_STATUS status = MH_EnableHook( base_fn );

		if (status != MH_OK)
			throw std::runtime_error( std::format( "failed to enable hook function, status: {}\nbase function -> {:#08X} address", MH_StatusToString( status ), reinterpret_cast<std::uintptr_t>(base_fn) ) );

		// switch hook state
		hooked = true;
		return true;
	}

	/* replace swaped modified function back to original */
	bool restore( )
	{
		// check is function hooked
		if (!hooked)
			return false;

		const MH_STATUS status = MH_DisableHook( base_fn );

		if (status != MH_OK)
			throw std::runtime_error( std::format( "failed to restore hook, status: {}\n base function -> {:#08X} address", MH_StatusToString( status ), reinterpret_cast<std::uintptr_t>(base_fn) ) );

		// switch hook state
		hooked = false;
		return true;
	}

	/* get original function pointer (not a call!) */
	template <typename Fn>
	Fn get_original( )
	{
		return static_cast<Fn>(original_fn);
	}

	/* returns hook state */
	inline bool is_hooked( ) const
	{
		return hooked;
	}

private:
	// Values
	/* hook state */
	bool hooked = false;
	/* function base address */
	void* base_fn = nullptr;
	/* in future that is being modified and replace original */
	void* replace_fn = nullptr;
	/* saved function to get available restore hook later */
	void* original_fn = nullptr;
};