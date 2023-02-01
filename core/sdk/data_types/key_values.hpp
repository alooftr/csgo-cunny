#pragma once
#include <cstddef>
#include <cstdint>

#include "../interfaces/i_key_values_system.hpp"

using get_symbol_proc_fn = bool( __cdecl* )(const char*);

class c_key_values
{
public:
	enum e_key_type : int
	{
		type_none = 0,
		type_string,
		type_int,
		type_float,
		type_ptr,
		type_wstring,
		type_color,
		type_uint64,
		type_compiled_int_byte,
		type_compiled_int_0,
		type_compiled_int_1,
		type_numtypes
	};

	c_key_values( const char* szKeyName, void* pUnknown1 = nullptr, h_key_symbol hCaseInsensitiveKeyName = INVALID_KEY_SYMBOL );
	~c_key_values( );

	void* operator new(std::size_t nAllocSize);
	void operator delete(void* pMemory);

	const char* get_name( );

	static c_key_values* from_string( const char* szName, const char* szValue );
	void load_from_buffer( char const* szResourceName, const char* szBuffer, void* pFileSystem = nullptr, const char* szPathID = nullptr, get_symbol_proc_fn pfnEvaluateSymbolProc = nullptr );
	bool load_from_file( void* pFileSystem, const char* szResourceName, const char* szPathID = nullptr, get_symbol_proc_fn pfnEvaluateSymbolProc = nullptr );

	c_key_values* find_key( const char* szKeyName, const bool bCreate );

	int get_int( const char* szKeyName, const int iDefaultValue );
	float get_float( const char* szKeyName, const float flDefaultValue );
	const char* get_string( const char* szKeyName, const char* szDefaultValue );

	void set_string( const char* szKeyName, const char* szStringValue );
	void set_int( const char* szKeyName, const int iValue );
	void set_uint64( const char* szKeyName, const int nLowValue, const int nHighValue );

	inline void set_bool( const char* szKeyName, const bool bValue )
	{
		set_int( szKeyName, bValue ? 1 : 0 );
	}

private:
	std::uint32_t uKeyName : 24; // 0x00
	std::uint32_t uKeyNameCaseSensitive1 : 8; // 0x3 // byte, explicitly specify bits due to packing
	char* szValue; // 0x04
	wchar_t* wszValue; // 0x08

	union
	{
		int iValue;
		float flValue;
		void* pValue;
		unsigned char arrColor[ 4 ];
	}; // 0x0C

	std::int8_t iDataType; // 0x10
	bool bHasEscapeSequences; // 0x11
	std::uint16_t uKeyNameCaseSensitive2; // 0x12
	void* pUnknown14; // 0x14 // seems like IKeyValuesSystem*, but why do they need it here? also calling smth on destructor and cleans up
	bool bHasCaseInsensitiveKeySymbol; // 0x18
	c_key_values* pPeer; // 0x1C
	c_key_values* pSub; // 0x20
	c_key_values* pChain; // 0x24
	get_symbol_proc_fn pExpressionGetSymbolProc; // 0x28
};
static_assert(sizeof( c_key_values ) == 0x2C);
