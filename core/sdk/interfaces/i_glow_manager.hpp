#pragma once
#include "../../utils.h"

#include "../data_types/utl_vector.hpp"
#include "../data_types/color.hpp"
#include "../data_types/vector.hpp"

#define END_OF_FREE_LIST -1
#define ENTRY_IN_USE -2

enum e_glow_render_style : int
{
	glow_render_style_default = 0,
	glow_render_style_rimglow3d,
	glow_render_style_edge_highlight,
	glow_render_style_edge_highlight_pulse,
	glow_render_style_count
};

class c_base_entity;
class i_glow_object_manager
{
public:
	int add_glow_box( vec3_t vecOrigin, vec3_t angOrientation, vec3_t mins, vec3_t maxs, col_t colColor, float flLifetime )
	{
		using add_glow_box_fn = int( __thiscall* )(void*, vec3_t, vec3_t, vec3_t, vec3_t, col_t, float);
		static auto o_add_glow_box = reinterpret_cast<add_glow_box_fn>(utils::find_pattern( "client.dll", "55 8B EC 53 56 8D" ));

		assert( o_add_glow_box != nullptr );

		return o_add_glow_box( this, vecOrigin, angOrientation, mins, maxs, colColor, flLifetime );
	}

	struct glow_object_t
	{
		void set( const col_t& colGlow, bool bFullBloom = false, const int nRenderStyle = glow_render_style_default ) // @note: styles not used cuz other styles doesnt have ignorez flag and needed to rebuild glow
		{
			this->arrColor = colGlow.base_alpha( );
			this->flBloomAmount = 1.0f;
			this->bRenderWhenOccluded = true;
			this->bRenderWhenUnoccluded = false;
			this->nRenderStyle = nRenderStyle;
			this->bFullBloomRender = bFullBloom;
		}

		inline bool is_empty( ) const
		{
			return nNextFreeSlot != ENTRY_IN_USE;
		}

		int						nNextFreeSlot;					// 0x00
		c_base_entity* pEntity;						// 0x04
		std::array<float, 4U>	arrColor;						// 0x08
		bool					bAlphaCappedByRenderAlpha;		// 0x18
		std::byte				pad0[ 0x3 ];						// 0x19 - pack 1 bool as 4 bytes
		float					flAlphaFunctionOfMaxVelocity;	// 0x1C
		float					flBloomAmount;					// 0x20
		float					flPulseOverdrive;				// 0x24
		bool					bRenderWhenOccluded;			// 0x28
		bool					bRenderWhenUnoccluded;			// 0x29
		bool					bFullBloomRender;				// 0x2A
		std::byte				pad1[ 0x1 ];						// 0x2B  - pack 3 bool as 4 bytes
		int						iFullBloomStencilTestValue;		// 0x2C
		int						nRenderStyle;					// 0x30
		int						nSplitScreenSlot;				// 0x34
	}; // Size: 0x38

	struct glow_box_object_t
	{
		vec3_t	vecPosition;
		vec3_t	angOrientation;
		vec3_t	vecMins;
		vec3_t	vecMaxs;
		float	flBirthTimeIndex;
		float	flTerminationTimeIndex;
		col_t	colColor;
	};

	c_utl_vector<glow_object_t> vecGlowObjectDefinitions;
	int nFirstFreeSlot;
	c_utl_vector<glow_box_object_t> vecGlowBoxDefinitions;
};