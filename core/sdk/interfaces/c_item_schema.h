#pragma once

#include <cstddef>

#define CONCAT(a, b) a##b
#define PAD_NAME(n) CONCAT(pad, n)

#define PAD(size) \
private: \
    std::byte PAD_NAME(__COUNTER__) [size]; \
public:

template <typename T>
struct utl_memory
{
	T& operator[]( int i ) const noexcept
	{
		return memory[ i ];
	};

	T* memory;
	int allocationCount;
	int growSize;
};

template <typename T>
class utl_vector
{
public:
	constexpr T& operator[]( int i ) noexcept
	{
		return memory[ i ];
	}
	constexpr const T& operator[]( int i ) const noexcept
	{
		return memory[ i ];
	}

	T* memory;
	int allocationCount;
	int growSize;
	int size;
	T* elements;
};

template <typename key, typename value>
struct node
{
	int left;
	int right;
	int parent;
	int type;
	key key;
	value value;
};

template <typename key, typename value>
struct utl_map
{
	auto begin( ) const noexcept
	{
		return memory.memory;
	}
	auto end( ) const noexcept
	{
		return memory.memory + numElements;
	}

	int find( key key ) const noexcept
	{
		auto curr = root;

		while (curr != -1)
		{
			const auto el = memory[ curr ];

			if (el.key < key)
				curr = el.right;
			else if (el.key > key)
				curr = el.left;
			else
				break;
		}
		return curr;
	}

	void* lessFunc;
	utl_memory<node<key, value>> memory;
	int root;
	int numElements;
	int firstFree;
	int lastAlloc;
	node<key, value>* elements;
};

struct String_t
{
	utl_memory<char> buffer;
	int length;

	const char* data( ) const noexcept
	{
		return buffer.memory;
	}
};

struct paint_kit_t
{
	int id;
	String_t name;
	String_t description;
	String_t itemName;
	String_t sameNameFamilyAggregate;
	String_t pattern;
	String_t normal;
	String_t logoMaterial;
	bool baseDiffuseOverride;
	int rarity;
	int style;
	int color1;
	int color2;
	int color3;
	int color4;
	int logoColor1;
	int logoColor2;
	int logoColor3;
	int logoColor4;
	float flWearDefault;
	float flWearRemapMin;
	float flWearRemapMax;
	char nFixedSeed;
	char uchPhongExponent;
	char uchPhongAlbedoBoost;
	char uchPhongIntensity;
	float flPatternScale;
	float flPatternOffsetXStart;
	float flPatternOffsetXEnd;
	float flPatternOffsetYStart;
	float flPatternOffsetYEnd;
	float flPatternRotateStart;
	float flPatternRotateEnd;
	float flLogoScale;
	float flLogoOffsetX;
	float flLogoOffsetY;
	float flLogoRotation;
	int bIgnoreWeaponSizeScale;
	int nViewModelExponentOverrideSize;
	int bOnlyFirstMaterial;
	float pearlescent;
	int sVmtPath[ 4 ];
	int kvVmtOverrides;
};

struct sticker_kit
{
	int id;
	int rarity;
	String_t name;
	String_t description;
	String_t itemName;
	PAD( 2 * sizeof( String_t ) )
		String_t inventoryImage;
};

class econ_item_definition
{
public:
	e_item_definition_index get_weapon_index( )
	{
		return utils::call_vfunc<e_item_definition_index>( this, 0 );
	}

	const char* get_item_base_name( )
	{
		return utils::call_vfunc<const char*>( this, 2 );
	}

	const char* get_item_type_name( )
	{
		return utils::call_vfunc<const char*>( this, 3 );
	}

	const char* get_player_display_model( )
	{
		return utils::call_vfunc<const char*>( this, 6 );
	}

	const char* get_world_display_model( )
	{
		return utils::call_vfunc<const char*>( this, 7 );
	}

	std::uint8_t get_rarity( )
	{
		return utils::call_vfunc< std::uint8_t>( this, 12 );
	}

	int get_capabilities( ) noexcept
	{
		return *reinterpret_cast<int*>(this + 0x148);
	}

	bool is_paintable( ) noexcept
	{
		return get_capabilities( ) & 1; // ITEM_CAP_PAINTABLE
	}

	const char* get_definition_name( ) noexcept
	{
		return *reinterpret_cast<const char**>(this + 0x1DC);
	}
};

struct item_list_entry
{
	int itemDef;
	int paintKit;
	PAD( 20 )

		auto GetWeaponId( ) const noexcept
	{
		return static_cast<e_item_definition_index>(itemDef);
	}
};

class econ_loot_list_definition
{
public:
	const char* get_name( )
	{
		return utils::call_vfunc<const char*>( this, 0 );
	}

	const utl_vector<item_list_entry>& GetLootListContents( )
	{
		return utils::call_vfunc<const utl_vector<item_list_entry>&>( this, 1 );
	}
};

class econ_item_set_definition
{
public:
	const char* get_loc_key( )
	{
		return utils::call_vfunc<const char*>( this, 1 );
	}

	int get_item_count( )
	{
		return utils::call_vfunc<int>( this, 4 );
	}

	e_item_definition_index get_item_definition( int index )
	{
		return utils::call_vfunc<e_item_definition_index>( this, 5, index );
	}

	int get_item_paint_kit( int index )
	{
		return utils::call_vfunc<int>( this, 6, index );
	}
};

struct econ_item_quality_definition
{
	int value;
	const char* name;
	unsigned weight;
	bool explicitMatchesOnly;
	bool canSupportSet;
	const char* hexColor;
};

struct alternate_icon_data
{
	String_t simpleName;
	String_t largeSimpleName;
	String_t iconURLSmall;
	String_t iconURLLarge;
	PAD( 28 )
};

class item_schema
{
public:
	PAD( 0x88 )
		utl_map<int, econ_item_quality_definition> qualities;
	PAD( 0x48 )
		utl_map<int, econ_item_definition*> itemsSorted;
	PAD( 0x104 )
		utl_map<std::uint64_t, alternate_icon_data> alternateIcons;
	PAD( 0x48 )
		utl_map<int, paint_kit_t*> paintKits;
	utl_map<int, sticker_kit*> stickerKits;

	econ_item_definition* get_item_definition_interface( e_item_definition_index id )
	{
		return utils::call_vfunc<econ_item_definition*>( this, 4, id );
	}
};

class c_item_system
{
public:
	item_schema* get_item_schema( )
	{
		return utils::call_vfunc<item_schema*>( this, 0 );
	}
};