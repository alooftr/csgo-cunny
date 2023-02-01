#include "key_values.hpp"

#include "../sdk.hpp"


c_key_values::c_key_values( const char* szKeyName, void* pUnknown1, h_key_symbol hCaseInsensitiveKeyName )
{
	using CKeyValuesConstructorFn = void( __thiscall* )(void*, const char*, void*, h_key_symbol);
	static CKeyValuesConstructorFn oConstructor = reinterpret_cast<CKeyValuesConstructorFn>(utils::find_pattern( "client.dll", "55 8B EC 56 8B F1 33 C0 8B 4D 0C 81" )); // @xref: client.dll -> "OldParticleSystem_Destroy"
	oConstructor( this, szKeyName, pUnknown1, hCaseInsensitiveKeyName );
}

c_key_values::~c_key_values( )
{
	using CKeyValuesDestructorFn = void( __thiscall* )(void*, int);
	static CKeyValuesDestructorFn oDestructor = reinterpret_cast<CKeyValuesDestructorFn>(utils::find_pattern( "client.dll", "56 8B F1 E8 ? ? ? ? 8B 4E 14" ));
	oDestructor( this, 1 );
}

void* c_key_values::operator new(std::size_t nAllocSize)
{
	// manually allocate memory, because game constructor doesn't call it automatically
	return csgo::key_values_system->alloc_key_values_memory( nAllocSize );
}

void c_key_values::operator delete(void* pMemory)
{
	// do nothing, because game destructor will automatically free memory
	// I::KeyValuesSystem->FreeKeyValuesMemory(pMemory);
	(void)pMemory;
}

const char* c_key_values::get_name( )
{
	return csgo::key_values_system->get_string_for_symbol( this->uKeyNameCaseSensitive1 | (this->uKeyNameCaseSensitive2 << 8) );
}

c_key_values* c_key_values::from_string( const char* szName, const char* szValue )
{
	static auto oFromString = utils::relative_to_absolute<std::uintptr_t>( utils::find_pattern( "client.dll", "E8 ? ? ? ? 83 C4 04 89 45 D8" ) + 0x1 ); // @xref: "#empty#", "#int#"
	c_key_values* pKeyValues = nullptr;

	if (oFromString == 0U)
		return nullptr;

	__asm
	{
		push 0
		mov edx, szValue
		mov ecx, szName
		call oFromString
		add esp, 4
		mov pKeyValues, eax
	}

	return pKeyValues;
}

void c_key_values::load_from_buffer( char const* szResourceName, const char* szBuffer, void* pFileSystem, const char* szPathID, get_symbol_proc_fn pfnEvaluateSymbolProc )
{
	using LoadFromBufferFn = void( __thiscall* )(void*, const char*, const char*, void*, const char*, void*, void*);
	static auto oLoadFromBuffer = reinterpret_cast<LoadFromBufferFn>(utils::find_pattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89" )); // @xref: "KeyValues::LoadFromBuffer(%s%s%s): Begin"
	assert( oLoadFromBuffer != nullptr );

	oLoadFromBuffer( this, szResourceName, szBuffer, pFileSystem, szPathID, pfnEvaluateSymbolProc, nullptr );
}

bool c_key_values::load_from_file( void* pFileSystem, const char* szResourceName, const char* szPathID, get_symbol_proc_fn pfnEvaluateSymbolProc )
{
	using LoadFromFileFn = bool( __thiscall* )(void*, void*, const char*, const char*, void*);
	static auto oLoadFromFile = reinterpret_cast<LoadFromFileFn>(utils::find_pattern( "client.dll", "55 8B EC 83 E4 F8 83 EC 14 53 56 8B 75 08 57 FF" )); // @xref: "rb"
	assert( oLoadFromFile != nullptr );

	return oLoadFromFile( this, pFileSystem, szResourceName, szPathID, pfnEvaluateSymbolProc );
}

c_key_values* c_key_values::find_key( const char* szKeyName, const bool bCreate )
{
	using FindKeyFn = c_key_values * (__thiscall*)(void*, const char*, bool);
	static auto oFindKey = reinterpret_cast<FindKeyFn>(utils::find_pattern( "client.dll", "55 8B EC 83 EC 1C 53 8B D9 85 DB" ));
	assert( oFindKey != nullptr );

	return oFindKey( this, szKeyName, bCreate );
}

int c_key_values::get_int( const char* szKeyName, const int iDefaultValue )
{
	c_key_values* pSubKey = this->find_key( szKeyName, false );

	if (pSubKey == nullptr)
		return iDefaultValue;

	switch (pSubKey->iDataType)
	{
	case type_string:
		return std::atoi( pSubKey->szValue );
	case type_wstring:
		return _wtoi( pSubKey->wszValue );
	case type_float:
		return static_cast<int>(pSubKey->flValue);
	case type_uint64:
		// can't convert, since it would lose data
		assert( false );
		return 0;
	default:
		break;
	}

	return pSubKey->iValue;
}

float c_key_values::get_float( const char* szKeyName, const float flDefaultValue )
{
	c_key_values* pSubKey = this->find_key( szKeyName, false );

	if (pSubKey == nullptr)
		return flDefaultValue;

	switch (pSubKey->iDataType)
	{
	case type_string:
		return static_cast<float>(std::atof( pSubKey->szValue ));
	case type_wstring:
		return std::wcstof( pSubKey->wszValue, nullptr );
	case type_float:
		return pSubKey->flValue;
	case type_int:
		return static_cast<float>(pSubKey->iValue);
	case type_uint64:
		return static_cast<float>(*reinterpret_cast<std::uint64_t*>(pSubKey->szValue));
	case type_ptr:
	default:
		return 0.0f;
	}
}

const char* c_key_values::get_string( const char* szKeyName, const char* szDefaultValue )
{
	using GetStringFn = const char* (__thiscall*)(void*, const char*, const char*);
	static auto oGetString = reinterpret_cast<GetStringFn>(utils::find_pattern( "client.dll", "55 8B EC 83 E4 C0 81 EC ? ? ? ? 53 8B 5D 08" ));
	assert( oGetString != nullptr );

	return oGetString( this, szKeyName, szDefaultValue );
}

void c_key_values::set_string( const char* szKeyName, const char* szStringValue )
{
	c_key_values* pSubKey = find_key( szKeyName, true );

	if (pSubKey == nullptr)
		return;

	using SetStringFn = void( __thiscall* )(void*, const char*);
	static auto oSetString = reinterpret_cast<SetStringFn>(utils::find_pattern( "client.dll", "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01" ));
	assert( oSetString != nullptr );

	oSetString( pSubKey, szStringValue );
}

void c_key_values::set_int( const char* szKeyName, const int iValue )
{
	c_key_values* pSubKey = find_key( szKeyName, true );

	if (pSubKey == nullptr)
		return;

	pSubKey->iValue = iValue;
	pSubKey->iDataType = type_int;
}

void c_key_values::set_uint64( const char* szKeyName, const int nLowValue, const int nHighValue )
{
	c_key_values* pSubKey = find_key( szKeyName, true );

	if (pSubKey == nullptr)
		return;

	// delete the old value
	delete[ ] pSubKey->szValue;

	// make sure we're not storing the WSTRING - as we're converting over to STRING
	delete[ ] pSubKey->wszValue;
	pSubKey->wszValue = nullptr;

	pSubKey->szValue = new char[ sizeof( std::uint64_t ) ];
	*reinterpret_cast<std::uint64_t*>(pSubKey->szValue) = static_cast<std::uint64_t>(nHighValue) << 32ULL | nLowValue;
	pSubKey->iDataType = type_uint64;
}
