#include "visuals.h"

// thread safe drawing
#include "../../draw/draw.h"
// math funcitons
#include "../../../utils/math.h"
// dormant esp
#include "dormant.h"

#define get_player_config(enemy, team, local) ctx.entity->is_enemy( local_player ) ? cfg_get( esp_variable_t, enemy ) : ctx.entity->get_index( ) == local_player->get_index( ) ? cfg_get( esp_variable_t, local ) : cfg_get( esp_variable_t, team )

void c_visuals::run( c_base_entity* local_player )
{
	if ( !csgo::engine->is_connected( ) || !csgo::engine->is_in_game( ) || !local_player )
		return;

	float server_time = TICKS_TO_TIME( local_player->get_tick_base( ) );
	const ImVec2 screen_size = ImGui::GetIO( ).DisplaySize;

	// grab entities
	std::vector<std::pair<c_base_entity*, float>> order = { };
	for ( int i = 1; i < csgo::entity_list->get_max_entities( ); ++i )
	{
		auto entity = csgo::entity_list->get<c_base_entity>( i );

		if ( !entity )
			continue;

		order.emplace_back( std::make_pair( entity, ( entity->get_render_origin( ) - globals::camera ).length( ) ) );

		//if (!entity->is_alive( ) && entity->is_player( ))
		//	this->gather_spectator_data( local_player, entity );
		//else
		//	this->spectator_data.at( entity->get_index( ) ).remove( );
	}

	// sort it for render queues ( mostly looks better )
	std::sort( order.begin( ), order.end( ), [ ]( const auto& a, const auto& b )
		{
			return a.second > b.second;
		}
	);

	static auto hud_radar = reinterpret_cast< ccsgo_hud_radar* >( utils::find_hud_element( "CCSGO_HudRadar" ) - 0x14 );
	c_dormant_esp::get( ).start( local_player );

	for ( const auto& [entity, cam_distance] : order )
	{
		// master switch
		if ( !cfg_get( bool, vars.vis_esp_enable ) )
			continue;

		c_base_client* client_class = entity->get_client_class( );

		if ( client_class == nullptr )
			continue;

		// setup our context for later use
		context_t ctx{ entity };

		const e_class_index index = client_class->class_id;
		switch ( index )
		{
			case e_class_index::ccsplayer:
			{
				// config check
				if ( !entity->is_alive( ) )
					break;

				// get the bounding box
				if ( !calc_bound_box( entity, &ctx.bbox ) )
					break;

				this->process_dormancy( &ctx, hud_radar );

				this->player_esp( ctx, local_player, cam_distance );

				break;
			}
		}
	}
}

bool c_visuals::calc_bound_box( c_base_entity* ent, bbox_t* bbox_in ) const
{
	const i_collideable* collideable = ent->get_collideable( );

	if ( collideable == nullptr )
		return false;

	// get mins/maxs
	const vec3_t obb_mins = collideable->obb_mins( );
	const vec3_t obb_maxs = collideable->obb_maxs( );

	/*
	 * build AABB points
	 *
	 * points navigation:
	 * [N] [back/front][left/right][bottom/top]
	 *	0 - blb
	 *	1 - brb
	 *	2 - frb
	 *	3 - flb
	 *	4 - frt
	 *	5 - brt
	 *	6 - blt
	 *	7 - flt
	 */

	std::array<vec3_t, 8U> points =
	{
		vec3_t( obb_mins.x, obb_mins.y, obb_mins.z ),
		vec3_t( obb_mins.x, obb_maxs.y, obb_mins.z ),
		vec3_t( obb_maxs.x, obb_maxs.y, obb_mins.z ),
		vec3_t( obb_maxs.x, obb_mins.y, obb_mins.z ),
		vec3_t( obb_maxs.x, obb_maxs.y, obb_maxs.z ),
		vec3_t( obb_mins.x, obb_maxs.y, obb_maxs.z ),
		vec3_t( obb_mins.x, obb_mins.y, obb_maxs.z ),
		vec3_t( obb_maxs.x, obb_mins.y, obb_maxs.z )
	};

	if ( points.data( ) == nullptr )
		return false;

	// get transformation matrix
	const matrix3x4_t& transformed = ent->get_coordinate_frame( );

	float left = FLT_MAX;
	float top = FLT_MAX;
	float right = -FLT_MAX;
	float bottom = -FLT_MAX;

	// get screen points position
	std::array<vec2_t, 8U> screen = { };
	for ( std::size_t i = 0U; i < 8U; i++ )
	{
		if ( !math::world_to_screen( math::vector_transform( points.at( i ), transformed ), screen.at( i ) ) )
			return false;

		/*
		 * transformed points navigation:
		 * [N] [back/front][left/right][bottom/top]
		 *	0 - frb
		 *	1 - flb
		 *	2 - blb
		 *	3 - brb
		 *	4 - frt
		 *	5 - blt
		 *	6 - brt
		 *	7 - flt
		 */

		left = std::min( left, screen.at( i ).x );
		top = std::min( top, screen.at( i ).y );
		right = std::max( right, screen.at( i ).x );
		bottom = std::max( bottom, screen.at( i ).y );
	}

	// set calculated box
	bbox_in->left = left;
	bbox_in->top = top;
	bbox_in->right = right;
	bbox_in->bottom = bottom;
	bbox_in->width = right - left;
	bbox_in->height = bottom - top;

	return true;
}

void c_visuals::process_dormancy( context_t* ctx, ccsgo_hud_radar* hud_radar )
{
	bool valid_dormant = false;
	auto backup_flags = ctx->entity->get_flags( );
	auto backup_origin = ctx->entity->get_abs_origin( );

	if ( ctx->entity->is_dormant( ) )
		valid_dormant = c_dormant_esp::get( ).adjust_sound( ctx->entity );
	else
	{
		ctx->health = ctx->entity->get_health( );
		c_dormant_esp::get( ).sound_players[ ctx->entity->get_index( ) ].reset( true, ctx->entity->get_abs_origin( ), ctx->entity->get_flags( ) );
	}

	if ( hud_radar && ctx->entity->is_dormant( ) && ctx->entity->is_spotted( ) )
		ctx->health = hud_radar->radar_info[ ctx->entity->get_index( ) ].health;

	if ( !ctx->health )
	{
		if ( ctx->entity->is_dormant( ) )
		{
			ctx->entity->get_flags( ) = backup_flags;
			ctx->entity->set_abs_origin( backup_origin );
		}

		return;
	}

	auto fast = 2.5f * csgo::global_vars->frame_time;
	auto slow = 0.25f * csgo::global_vars->frame_time;

	if ( ctx->entity->is_dormant( ) )
	{
		auto origin = ctx->entity->get_abs_origin( );

		if ( origin.is_zero( ) )
			ctx->dormant_alpha = 0.0f;
		else if ( !valid_dormant && ctx->dormant_alpha > 0.0f )
			ctx->dormant_alpha -= slow;
		else if ( valid_dormant && ctx->dormant_alpha < 1.0f )
			ctx->dormant_alpha += fast;
	}
	else if ( ctx->dormant_alpha < 1.0f )
		ctx->dormant_alpha += fast;

	ctx->dormant_alpha = std::clamp( ctx->dormant_alpha, 0.0f, 1.0f );
}

void c_visuals::player_esp( context_t& ctx, c_base_entity* local_player, float cam_dist )
{
	// capture player info for later use
	player_info_t info;
	if ( !csgo::engine->get_player_info( ctx.entity->get_index( ), &info ) )
		return;

	// capture our weapon pointer for later use
	auto wpn = ctx.entity->get_active_weapon( );
	if ( wpn == nullptr )
		return;

	auto wpn_data = wpn->get_weapon_data( );
	if ( wpn_data == nullptr )
		return;

	// bounding box
	if ( const auto bounding_box_cfg = get_player_config( vars.vis_esp_enemy_box, vars.vis_esp_teammate_box, vars.vis_esp_local_box ); bounding_box_cfg.enable )
		this->player_bounding_box( ctx, &bounding_box_cfg );

	/* note:
	// we draw our bar first
	// so it will be pad before we draw our text
	*/

	// hp bar
	if ( const auto hp_bar_cfg = get_player_config( vars.vis_esp_enemy_hp_bar, vars.vis_esp_teammate_hp_bar, vars.vis_esp_local_hp_bar ); hp_bar_cfg.enable )
	{
		static int prev_hp[ 65 ];
		constexpr float anim_speed = 255 / 4.0f;

		if ( prev_hp[ ctx.entity->get_index( ) ] > ctx.entity->get_health( ) )
			prev_hp[ ctx.entity->get_index( ) ] -= anim_speed * csgo::global_vars->frame_time;
		else
			prev_hp[ ctx.entity->get_index( ) ] = ctx.entity->get_health( );

		const float factor = static_cast< float >( prev_hp[ ctx.entity->get_index( ) ] ) / static_cast< float >( ctx.entity->get_max_health( ) );

		this->draw_bar_factor( ctx, &hp_bar_cfg, factor );
	}
	
	// @TODO: ammo bar
	
	// name
	if ( const auto name_cfg = get_player_config( vars.vis_esp_enemy_name, vars.vis_esp_teammate_name, vars.vis_esp_local_name ); name_cfg.enable )
	{
		auto str = ctx.entity->get_player_name( );

		if ( str.empty( ) )
			return;

		if ( str.length( ) > 15 )
			str = str.substr( 0, 15 ) + "...";

		this->draw_text( ctx, &name_cfg, str, imgui_fonts::arial );
	}

	// weapon icon
	if ( const auto icon_cfg = get_player_config( vars.vis_esp_enemy_weapon_icon, vars.vis_esp_teammate_weapon_icon, vars.vis_esp_local_weapon_icon ); icon_cfg.enable )
	{
		auto icon = reinterpret_cast< const char* >( utils::get_weapon_icon( wpn->get_item_definition_index( ) ) );

		this->draw_text( ctx, &icon_cfg, icon, imgui_fonts::weapon_icons );
	}

	/* player flags ( infos ) */
	// is a bot
	if ( const auto fl_bot_cfg = get_player_config( vars.vis_esp_enemy_flags_bot, vars.vis_esp_teammate_flags_bot, vars.vis_esp_local_flags_bot ); fl_bot_cfg.enable )
	{
		if ( info.fake_player )
			this->draw_text( ctx, &fl_bot_cfg, "BOT", imgui_fonts::verdana );
	}

	// is armored
	if ( const auto fl_armor_cfg = get_player_config( vars.vis_esp_enemy_flags_armor, vars.vis_esp_teammate_flags_armor, vars.vis_esp_local_flags_armor ); fl_armor_cfg.enable )
	{
		std::string armor_text = "";
		if ( ctx.entity->has_helmet( ) && ctx.entity->get_armor( ) > 0 )
			armor_text = "HK";
		else if ( !ctx.entity->has_helmet( ) && ctx.entity->get_armor( ) > 0 )
			armor_text = "K";
		else
			armor_text = "";

		if ( !armor_text.empty( ) )
			this->draw_text( ctx, &fl_armor_cfg, armor_text, imgui_fonts::verdana );
	}

	// has c4
	if ( const auto fl_c4_cfg = get_player_config( vars.vis_esp_enemy_flags_c4, vars.vis_esp_teammate_flags_c4, vars.vis_esp_local_flags_c4 ); fl_c4_cfg.enable )
	{
		if ( ctx.entity->has_c4( ) )
			this->draw_text( ctx, &fl_c4_cfg, "C4", imgui_fonts::verdana );
	}

	// scoping / zooming
	if ( const auto fl_zoom_cfg = get_player_config( vars.vis_esp_enemy_flags_zoom, vars.vis_esp_teammate_flags_zoom, vars.vis_esp_local_flags_zoom ); fl_zoom_cfg.enable )
	{
		if ( ctx.entity->is_scoped( ) )
			this->draw_text( ctx, &fl_zoom_cfg, "ZOOM", imgui_fonts::verdana );
	}

	// reloading
	if ( const auto fl_reload_cfg = get_player_config( vars.vis_esp_enemy_flags_reload, vars.vis_esp_teammate_flags_reload, vars.vis_esp_local_flags_reload ); fl_reload_cfg.enable )
	{
		if ( ctx.entity->is_reloading( ) )
			this->draw_text( ctx, &fl_reload_cfg, "RELOAD", imgui_fonts::verdana );
	}

	// flashed
	if ( const auto fl_flashed_cfg = get_player_config( vars.vis_esp_enemy_flags_flashed, vars.vis_esp_teammate_flags_flashed, vars.vis_esp_local_flags_flashed ); fl_flashed_cfg.enable )
	{
		if ( ctx.entity->get_flash_duration( ) > 0.0f )
			this->draw_text( ctx, &fl_flashed_cfg, "BLIND", imgui_fonts::verdana );
	}
}

void c_visuals::player_bounding_box( context_t& ctx, const esp_variable_t* config )
{
	col_t col = config->primary_color.set<col_a>( ctx.entity->is_dormant( ) ? 130 : ctx.dormant_alpha * 255 );
	col_t outline_col = config->outline_color.set<col_a>( ctx.entity->is_dormant( ) ? 130 : ctx.dormant_alpha * 255 );

	draw::add_rect( ImVec2( ctx.bbox.left, ctx.bbox.top ), ImVec2( ctx.bbox.right, ctx.bbox.bottom ), col, draw_rect_outline | draw_rect_border, outline_col );
}

void c_visuals::draw_text( context_t& ctx, const esp_variable_t* config, std::string str, ImFont* font )
{
	const auto name_size = draw::get_text_size( font, str.c_str( ) );

	const col_t col = config->primary_color.set<col_a>( ctx.entity->is_dormant( ) ? 130 : ctx.dormant_alpha * 255 );
	const col_t outline_col = config->outline_color.set<col_a>( ctx.entity->is_dormant( ) ? 130 : ctx.dormant_alpha * 255 );

	switch ( config->position )
	{
		case esp_position_top:
		{
			draw::add_text( font, ImVec2( ctx.bbox.left + ctx.bbox.width * 0.5f - name_size.x * 0.5f, ctx.bbox.top - 2 - name_size.y - ctx.padding.at( pad_top ).y ), str, col, draw_text_dropshadow, outline_col );

			// pad at the y vector only
			ctx.padding.at( pad_top ) += ImVec2( 0, name_size.y );
			break;
		}
		case esp_position_left:
		{
			draw::add_text( font, ImVec2( ctx.bbox.left - 2 - ctx.padding.at( pad_left ).x - name_size.x, ctx.bbox.top + ctx.padding.at( pad_left ).y ), str, col, draw_text_dropshadow, outline_col );

			// pad for the
			ctx.padding.at( pad_left ) += ImVec2( 0.f, name_size.y );
			break;
		}
		case esp_position_right:
		{
			draw::add_text( font, ImVec2( ctx.bbox.right + 2 + ctx.padding.at( pad_right ).x, ctx.bbox.top + ctx.padding.at( pad_right ).y ), str, col, draw_text_dropshadow, outline_col );
			
			ctx.padding.at( pad_right ) += ImVec2( 0.f, name_size.y );
			break;
		}
		case esp_position_bot:
		{
			draw::add_text( font, ImVec2( ctx.bbox.left + ctx.bbox.width * 0.5f - name_size.x * 0.5f, ctx.bbox.bottom + 2 + ctx.padding.at( pad_bottom ).y ), str, col, draw_text_dropshadow, outline_col );

			ctx.padding.at( pad_bottom ) += ImVec2( 0.f, name_size.y );
			break;
		}
	}
}

void c_visuals::draw_bar_factor( context_t& ctx, const esp_variable_t* config, float factor )
{ 
	const col_t col = config->primary_color.set<col_a>( ctx.entity->is_dormant( ) ? 130 : ctx.dormant_alpha * 255 );
	const col_t outline_col = config->outline_color.set<col_a>( ctx.entity->is_dormant( ) ? 130 : ctx.dormant_alpha * 255 );
	const col_t bg_col = config->background_color.set<col_a>( ctx.entity->is_dormant( ) ? 25 : ctx.dormant_alpha * 75 );
	const col_t glow_col = config->glow_color.set<col_a>( ctx.entity->is_dormant( ) ? 130 : ctx.dormant_alpha * 255 );

	switch ( config->position )
	{
		case esp_position_top:
		{
			if ( config->glow )
				draw::add_shadow_rect( ImVec2( ctx.bbox.left, ctx.bbox.top - 3 - ctx.padding.at( pad_top ).y ), ImVec2( ctx.bbox.left + ctx.bbox.width, ctx.bbox.top - 5 - ctx.padding.at( pad_top ).y ), glow_col );

			draw::add_rect( ImVec2( ctx.bbox.left, ctx.bbox.top - 3 - ctx.padding.at( pad_top ).y ), ImVec2( ctx.bbox.left + ctx.bbox.width, ctx.bbox.top - 5 - ctx.padding.at( pad_top ).y ), bg_col, draw_rect_filled | draw_rect_outline | draw_rect_border, outline_col );

			draw::add_rect( ImVec2( ctx.bbox.left, ctx.bbox.top - 3 - ctx.padding.at( pad_top ).y ), ImVec2( ctx.bbox.left + ctx.bbox.width * factor, ctx.bbox.top - 5 - ctx.padding.at( pad_top ).y ), col, draw_rect_filled );

			ctx.padding.at( pad_top ) += ImVec2( 0.f, 6.f );

			break;
		}
		case esp_position_left:
		{
			if ( config->glow )
				draw::add_shadow_rect( ImVec2( ctx.bbox.left - 5 - ctx.padding.at( pad_left ).x, ctx.bbox.bottom - ctx.bbox.height ), ImVec2( ctx.bbox.left - 3 - ctx.padding.at( pad_left ).x, ctx.bbox.bottom ), glow_col );

			draw::add_rect( ImVec2( ctx.bbox.left - 5 - ctx.padding.at( pad_left ).x, ctx.bbox.bottom - ctx.bbox.height ), ImVec2( ctx.bbox.left - 3 - ctx.padding.at( pad_left ).x, ctx.bbox.bottom ), bg_col, draw_rect_filled | draw_rect_outline | draw_rect_border, outline_col );

			draw::add_rect( ImVec2( ctx.bbox.left - 5 - ctx.padding.at( pad_left ).x, ctx.bbox.bottom - ( ctx.bbox.height * factor ) ), ImVec2( ctx.bbox.left - 3 - ctx.padding.at( pad_left ).x, ctx.bbox.bottom ), col, draw_rect_filled );

			ctx.padding.at( pad_left ) += ImVec2( 6.f, 0.f );

			break;
		}
		case esp_position_right:
		{
			if ( config->glow )
				draw::add_shadow_rect( ImVec2( ctx.bbox.right + 3 + ctx.padding.at( pad_right ).x, ctx.bbox.bottom - ctx.bbox.height ), ImVec2( ctx.bbox.right + 5 + ctx.padding.at( pad_right ).x, ctx.bbox.bottom ), glow_col );

			draw::add_rect( ImVec2( ctx.bbox.right + 3 + ctx.padding.at( pad_right ).x, ctx.bbox.bottom - ctx.bbox.height ), ImVec2( ctx.bbox.right + 5 + ctx.padding.at( pad_right ).x, ctx.bbox.bottom ), bg_col, draw_rect_filled | draw_rect_outline | draw_rect_border, outline_col );

			draw::add_rect( ImVec2( ctx.bbox.right + 3 + ctx.padding.at( pad_right ).x, ctx.bbox.bottom - ( ctx.bbox.height * factor ) ), ImVec2( ctx.bbox.right + 5 + ctx.padding.at( pad_right ).x, ctx.bbox.bottom ), col, draw_rect_filled );

			ctx.padding.at( pad_right ) += ImVec2( 6.f, 0.f );

			break;
		}
		case esp_position_bot:
		{
			if ( config->glow )
				draw::add_shadow_rect( ImVec2( ctx.bbox.left, ctx.bbox.bottom + 3 + ctx.padding.at( pad_bottom ).y ), ImVec2( ctx.bbox.left + ctx.bbox.width, ctx.bbox.bottom + 5 + ctx.padding.at( pad_bottom ).y ), glow_col );

			draw::add_rect( ImVec2( ctx.bbox.left, ctx.bbox.bottom + 3 + ctx.padding.at( pad_bottom ).y ), ImVec2( ctx.bbox.left + ctx.bbox.width, ctx.bbox.bottom + 5 + ctx.padding.at( pad_bottom ).y ), bg_col, draw_rect_filled | draw_rect_outline | draw_rect_border, outline_col );

			draw::add_rect( ImVec2( ctx.bbox.left, ctx.bbox.bottom + 3 + ctx.padding.at( pad_bottom ).y ), ImVec2( ctx.bbox.left + ctx.bbox.width * factor, ctx.bbox.bottom + 5 + ctx.padding.at( pad_bottom ).y ), col, draw_rect_filled );

			ctx.padding.at( pad_bottom ) += ImVec2( 0.f, 6.f );

			break;
		}
	}
}
