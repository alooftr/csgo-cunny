#pragma once

#define INVALID_KEY_SYMBOL (-1)
using h_key_symbol = int;

class i_key_values_system
{
public:
	virtual void register_sizeof_key_values( int iSize ) = 0;
private:
	virtual void function0( ) = 0;
public:
	virtual void* alloc_key_values_memory( int iSize ) = 0;
	virtual void free_key_values_memory( void* pMemory ) = 0;
	virtual h_key_symbol get_symbol_for_string( const char* szName, bool bCreate = true ) = 0;
	virtual const char* get_string_for_symbol( h_key_symbol hSymbol ) = 0;
	virtual void add_key_values_to_memory_leak_list( void* pMemory, h_key_symbol hSymbolName ) = 0;
	virtual void remove_key_values_from_memory_leak_list( void* pMemory ) = 0;
	virtual void set_key_values_expression_symbol( const char* szName, bool bValue ) = 0;
	virtual bool get_key_values_expression_symbol( const char* szName ) = 0;
	virtual h_key_symbol get_symbol_for_string_case_sensitive( h_key_symbol& hCaseInsensitiveSymbol, const char* szName, bool bCreate = true ) = 0;
};

using key_values_system_fn = i_key_values_system * (__cdecl*)();