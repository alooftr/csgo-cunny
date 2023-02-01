#pragma once
#include <unordered_map>

#include "parser.h"

#include "../../../sdk/sdk.hpp"

#include "../../../../ext/imgui/imgui.h"
#include "../../../../ext/imgui/dx9/imgui_impl_dx9.h"

struct model_object_t
{
	model_object_t( const char* name, const char* model_name, const char* kill_icon = nullptr, short id = weapon_none )
		: name( name ), model_name( model_name ), kill_icon( kill_icon ), id( id )
	{}

	model_object_t( const char* name, const char* model_name, short id = weapon_none )
		: name( name ), model_name( model_name ), id( id )
	{}
	
	const char* name = nullptr;
	const char* model_name = nullptr;
	const char* kill_icon = nullptr;
	short id = weapon_none;
};

class c_texture
{
	ImTextureID texture = nullptr;
public:
	c_texture( ) = default;
	~c_texture( )
	{
		clear( );
	}
	c_texture( const c_texture& ) = delete;
	c_texture& operator=( const c_texture& ) = delete;
	c_texture( c_texture&& other ) noexcept : texture{ other.texture }
	{
		other.texture = nullptr;
	}
	c_texture& operator=( c_texture&& other ) noexcept
	{
		clear( ); texture = other.texture; other.texture = nullptr; return *this;
	}

	void create( int width, int height, const std::uint8_t* data ) noexcept
	{
		texture = ImGui_CreateTextureRGBA( width, height, data );
	}
	void clear( ) noexcept
	{
		if (texture)
			ImGui_DestroyTexture( texture );
		texture = nullptr;
	}
	ImTextureID get( ) noexcept
	{
		return texture;
	}
};

class c_skin : public c_singleton< c_skin>
{
public:
	// to setup animation fix
	c_skin( );

	std::vector<model_object_t> knife_object =
	{
		{ "default", nullptr, nullptr },
		{ "knife (counter-terrorists)", "models/weapons/v_knife_default_ct.mdl", "knife_default_ct", weapon_knife },
		{ "knife (terrorists)", "models/weapons/v_knife_default_t.mdl", "knife_t", weapon_knife_t },
		{ "bayonet", "models/weapons/v_knife_bayonet.mdl", "bayonet", weapon_knife_bayonet },
		{ "flip", "models/weapons/v_knife_flip.mdl", "knife_flip", weapon_knife_flip },
		{ "gut", "models/weapons/v_knife_gut.mdl", "knife_gut", weapon_knife_gut },
		{ "karambit", "models/weapons/v_knife_karam.mdl", "knife_karambit", weapon_knife_karambit },
		{ "m9 bayonet", "models/weapons/v_knife_m9_bay.mdl", "knife_m9_bayonet", weapon_knife_m9_bayonet },
		{ "huntsman", "models/weapons/v_knife_tactical.mdl", "knife_tactical", weapon_knife_tactical },
		{ "falchion", "models/weapons/v_knife_falchion_advanced.mdl", "knife_falchion", weapon_knife_falchion } ,
		{ "bowie", "models/weapons/v_knife_survival_bowie.mdl", "knife_survival_bowie", weapon_knife_survival_bowie },
		{ "butterfly", "models/weapons/v_knife_butterfly.mdl", "knife_butterfly", weapon_knife_butterfly },
		{ "shadow daggers", "models/weapons/v_knife_push.mdl", "knife_push", weapon_knife_push },
		{ "paracord", "models/weapons/v_knife_cord.mdl", "knife_cord", weapon_knife_cord },
		{ "survival", "models/weapons/v_knife_canis.mdl", "knife_canis", weapon_knife_canis },
		{ "ursus", "models/weapons/v_knife_ursus.mdl", "knife_ursus", weapon_knife_ursus },
		{ "navaja", "models/weapons/v_knife_gypsy_jackknife.mdl", "knife_gypsy_jackknife", weapon_knife_gypsy_jackknife },
		{ "nomad", "models/weapons/v_knife_outdoor.mdl", "knife_outdoor", weapon_knife_outdoor },
		{ "stiletto", "models/weapons/v_knife_stiletto.mdl", "knife_stiletto", weapon_knife_stiletto },
		{ "talon", "models/weapons/v_knife_widowmaker.mdl", "knife_widowmaker", weapon_knife_widowmaker },
		{ "skeleton", "models/weapons/v_knife_skeleton.mdl", "knife_skeleton", weapon_knife_skeleton },
	};

	std::vector<model_object_t> glove_object =
	{
		{"default", nullptr, nullptr },
		{"bloodhound", "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl", glove_studded_bloodhound},
		{"sporty", "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl", glove_sporty},
		{"slick", "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl", glove_slick},
		{"handwraps", "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl", glove_leather_handwraps},
		{"motorcycle", "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl", glove_motorcycle},
		{"specialist", "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl", glove_specialist},
		{"hydra", "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl", glove_studded_hydra},
		{"brokenfang", "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_brokenfang.mdl", glove_studded_brokenfang},
	};

	ImTextureID get_icon_texture( const std::string& path );
	void clear_item_icon_textures( );
	int get_config_index( short index );

	// call in frame_net_update_postdataupdate_end
	void run( c_base_entity* local_player );
	void glove_changer( c_base_entity* local_player );
	void knife_changer( c_base_entity* local_player, c_base_combat_weapon* wpn );

	void set_update_state( bool state )
	{
		this->update_skin = state;
	}

	void force_update_skin( c_base_entity* local_player );

	void on_player_hurt( i_game_event* game_event );

	ImU32 get_rarity_color( int rarity )
	{
		// array for it
		constexpr auto rarity_colors = std::to_array<ImU32>( {
			IM_COL32( 175, 175, 175, 255 ),
			IM_COL32( 176, 195, 217, 255 ),
			IM_COL32( 94, 152, 217, 255 ),
			IM_COL32( 75, 105, 255, 255 ),
			IM_COL32( 136,  71, 255, 255 ),
			IM_COL32( 211,  44, 230, 255 ),
			IM_COL32( 235,  75,  75, 255 ),
			IM_COL32( 228, 174,  57, 255 )
		} );

		return rarity_colors[ static_cast<std::size_t>(rarity) < rarity_colors.size( )?rarity:0 ];
	};

	std::unordered_map<std::string, int(*)(int)> animation_fix_map;

private:
	std::unordered_map<std::string, c_texture> icon_textures;
	
	bool update_skin = false;
};