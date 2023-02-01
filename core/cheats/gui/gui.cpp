#include "gui.h"
// input system
#include "../../sdk/sdk.hpp"
// skin maps
#include "../features/skins/skins.h"
// spectator data
#include "../features/visuals/visuals.h"
// preview model
#include "../features/preview_model/preview_model.h"
// math w2s function
#include "../../utils/math.h"

template<std::size_t s>
void tabs::render( const char* tab_name, const std::array<c_tab, s> tabs, int* current_tab )
{
	if ( tabs.empty( ) )
		return;

	static ImVec2 a = {};
	ImGui::Columns( 2, "tab_columns", false );
	ImGui::SetColumnWidth( 0, 71.f );
	{
		ImGui::BeginChild( tab_name, ImGui::GetContentRegionAvail( ) );
		{
			if ( static auto image = utils::get_steam_avatar( ); image != nullptr )
				ImGui::Image( image, ImVec2( 55, 55 ) );

			ImGui::Separator( );

			for ( std::size_t i = 0U; i < tabs.size( ); i++ )
			{
				if ( ImGui::TabSelector( tabs.at( i ).name, *current_tab == i ) )
					*current_tab = i;
			}

		}
		ImGui::EndChild( );
	}
	ImGui::NextColumn( );
	{
		// render inner tab
		if ( tabs.at( *current_tab ).render_function != nullptr )
			tabs.at( *current_tab ).render_function( );
	}
}

template<std::size_t s>
void tabs::render_sub_tab( const char* subtab_name, const std::array<c_tab, s> tabs, int* current_tab )
{
	if ( tabs.empty( ) )
		return;

	ImGui::BeginChild( subtab_name, ImVec2( ImGui::GetContentRegionAvail( ).x, ImGui::GetContentRegionAvail( ).y / 10 ) );
	{
		for ( std::size_t i = 0U; i < tabs.size( ); i++ )
		{
			if ( ImGui::SubTabSelector( tabs.at( i ).name, *current_tab == i ) )
				*current_tab = i;
			ImGui::SameLine( 0, 20 );
		}

		if ( windows::current_tab == 2 )
		{
			ImGui::SameLine( 0, 20 );
			ImGui::SetCursorPos( ImGui::GetCursorPos( ) + ImVec2( 0.f, 12.f ) );
			ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( ImGui::GetStyle( ).FramePadding.x, -1 ) );
			ImGui::Checkbox( "master switch", &cfg_get( bool, vars.vis_esp_enable ) );
			ImGui::PopStyleVar( );
		}

		if ( windows::current_tab == 3 && *current_tab == 2 )
		{
			ImGui::SameLine( 0, 20 );
			ImGui::SetCursorPos( ImGui::GetCursorPos( ) + ImVec2( 0.f, 12.f ) );
			ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( ImGui::GetStyle( ).FramePadding.x, -1 ) );
			ImGui::Checkbox( "master switch", &cfg_get( bool, vars.skin_enable ) );
			ImGui::PopStyleVar( );
		}
	}
	ImGui::EndChild( );

	ImGui::BeginChild( tabs.at( *current_tab ).name, ImGui::GetContentRegionAvail( ) );
	{
		ImGui::Spacing( );

		ImGuiStyle& style = ImGui::GetStyle( );
		ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( style.FramePadding.x, -1 ) );
		{
			if ( tabs.at( *current_tab ).render_function != nullptr )
				tabs.at( *current_tab ).render_function( );
		}
		ImGui::PopStyleVar( );
	}
	ImGui::EndChild( );
}

template<std::size_t s>
void tabs::render_inner_sub_tab( const char* inner_subtab_name, const std::array<c_tab, s> tabs, int* current_tab )
{
	if ( tabs.empty( ) )
		return;

	if ( ImGui::BeginTabBar( inner_subtab_name, ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | ImGuiTabBarFlags_NoTooltip ) )
	{
		for ( std::size_t i = 0U; i < tabs.size( ); i++ )
		{
			// add tab
			if ( ImGui::BeginTabItem( tabs.at( i ).name ) )
			{
				// set current tab index
				*current_tab = i;
				ImGui::EndTabItem( );
			}
		}

		// render inner tab
		if ( tabs.at( *current_tab ).render_function != nullptr )
			tabs.at( *current_tab ).render_function( );

		ImGui::EndTabBar( );
	}
}

void tabs::rage_bot( )
{ }

void tabs::anti_aim( )
{ }

void tabs::players( )
{
	auto& style = ImGui::GetStyle( );


	static std::array<c_tab, 3U> const player_inner_tabs =
	{
		c_tab{ "enemy", [ &style ]
		{
			ImGui::BeginDisabled( !cfg_get( bool, vars.vis_esp_enable ) );
			ImGui::Columns( 2, "enemy", false );
			{
				cfg_get( esp_variable_t, vars.vis_esp_enemy_box ).draw_menu_item( "box", false, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_enemy_name ).draw_menu_item( "name", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_enemy_hp_bar ).draw_menu_item( "hp bar", true, true, true );
				cfg_get( esp_variable_t, vars.vis_esp_enemy_weapon_icon ).draw_menu_item( "weapon icon", true, false, false );

				ImGui::TextUnformatted( "players's info:" );
				cfg_get( esp_variable_t, vars.vis_esp_enemy_flags_bot ).draw_menu_item( "is a bot", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_enemy_flags_armor ).draw_menu_item( "is amored", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_enemy_flags_c4 ).draw_menu_item( "has c4", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_enemy_flags_zoom ).draw_menu_item( "is scoped", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_enemy_flags_reload ).draw_menu_item( "is reloading", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_enemy_flags_flashed ).draw_menu_item( "is flashed", true, false, false );

			}
			ImGui::NextColumn( );
			{

			}
			ImGui::Columns( 1 );
			ImGui::EndDisabled( );
		}},
		c_tab{ "teammate", [ &style ]
		{
			ImGui::BeginDisabled( !cfg_get( bool, vars.vis_esp_enable ) );
			ImGui::Columns( 2, "teammate", false );
			{
				cfg_get( esp_variable_t, vars.vis_esp_teammate_box ).draw_menu_item( "box", false, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_teammate_name ).draw_menu_item( "name", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_teammate_hp_bar ).draw_menu_item( "hp bar", true, true, true );
				cfg_get( esp_variable_t, vars.vis_esp_teammate_weapon_icon ).draw_menu_item( "weapon icon", true, false, false );
				
				ImGui::TextUnformatted( "players's info:" );
				cfg_get( esp_variable_t, vars.vis_esp_teammate_flags_bot ).draw_menu_item( "is a bot", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_teammate_flags_armor ).draw_menu_item( "is amored", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_teammate_flags_c4 ).draw_menu_item( "has c4", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_teammate_flags_zoom ).draw_menu_item( "is scoped", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_teammate_flags_reload ).draw_menu_item( "is reloading", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_teammate_flags_flashed ).draw_menu_item( "is flashed", true, false, false );
			}
			ImGui::NextColumn( );
			{

			}
			ImGui::Columns( 1 );
			ImGui::EndDisabled( );
		}},
		c_tab{ "local", [ &style ]
		{
			ImGui::BeginDisabled( !cfg_get( bool, vars.vis_esp_enable ) );
			ImGui::Columns( 2, "local", false );
			{
				cfg_get( esp_variable_t, vars.vis_esp_local_box ).draw_menu_item( "box", false, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_local_name ).draw_menu_item( "name", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_local_hp_bar ).draw_menu_item( "hp bar", true, true, true );
				cfg_get( esp_variable_t, vars.vis_esp_local_weapon_icon ).draw_menu_item( "weapon icon", true, false, false );

				ImGui::TextUnformatted( "players's info:" );
				cfg_get( esp_variable_t, vars.vis_esp_local_flags_bot ).draw_menu_item( "is a bot", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_local_flags_armor ).draw_menu_item( "is amored", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_local_flags_c4 ).draw_menu_item( "has c4", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_local_flags_zoom ).draw_menu_item( "is scoped", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_local_flags_reload ).draw_menu_item( "is reloading", true, false, false );
				cfg_get( esp_variable_t, vars.vis_esp_local_flags_flashed ).draw_menu_item( "is flashed", true, false, false );

			}
			ImGui::NextColumn( );
			{

			}
			ImGui::Columns( 1 );
			ImGui::EndDisabled( );
		}}
	};

	render_sub_tab< player_inner_tabs.size( ) >( "player_sub_tabs", player_inner_tabs, &current_player_sub_tabs );
}

short get_current_def_index( )
{
	if ( !globals::local_player || !globals::local_player->is_alive( ) )
		return weapon_deagle;

	auto wpn = globals::local_player->get_active_weapon( );
	if ( !wpn )
		return weapon_deagle;

	auto data = wpn->get_weapon_data( );
	if ( !data )
		return weapon_deagle;

	// handle all kind of knife since we will modifying it later ( if we did )
	if ( data->is_knife( ) )
		return weapon_knife;

	// else just return the definition index
	auto index = static_cast< e_item_definition_index >( wpn->get_item_definition_index( ) );

	if ( index == weapon_none )
		return weapon_deagle;

	return index;
}

void tabs::visuals( )
{
	auto& style = ImGui::GetStyle( );
	static int current_visuals_sub_tab = 0;
	static std::array<c_tab, 3U> const tabs =
	{
		c_tab{ "effects", [ &style ]
		{
			 ImGui::Columns( 2, "others", false );
			{
			}
			ImGui::NextColumn( );
			{

			}
			ImGui::Columns( 1 );
		}},
		c_tab{ "others", [ &style ]
		{
			 ImGui::Columns( 2, "others", false );
			{
			}
			ImGui::NextColumn( );
			{

			}
			ImGui::Columns( 1 );
		}},
		c_tab{ "skins", [ &style ]
		{
			ImGui::BeginDisabled( !cfg_get( bool, vars.skin_enable ) );
			ImGui::Columns( 2, "skins", false );
			{
				static std::array<c_tab, 2U> inner_subtab =
				{
					c_tab{ "attributes",[ &style ]
					{
						const short cfg_index = c_skin::get( ).get_config_index( get_current_def_index( ) );

						if ( ImGui::SliderFloat( "wear##weapon", cfg_get( std::vector<float>, vars.skin_selected_wear ), cfg_index, 0.0f, 1.0f, "%.5f" ) )
							c_skin::get( ).set_update_state( true );

						if ( ImGui::SliderInt( "seed##weapon", cfg_get( std::vector<int>, vars.skin_selected_seed ), cfg_index, 0, 1000, "%d" ) )
							c_skin::get( ).set_update_state( true );

						if ( ImGui::SliderInt( "stat track##weapon", cfg_get( std::vector<int>, vars.skin_selected_stat_track ), cfg_index, 0, 10000, "%d" ) )
							c_skin::get( ).set_update_state( true );

						if (ImGui::Button( "update skin", ImVec2( ImGui::GetContentRegionAvail( ).x, 0 ) ))
							c_skin::get( ).set_update_state( true );

					}},
					c_tab{ "models",[ &style ]
					{
						if ( ImGui::BeginCombo( "knife models", c_skin::get( ).knife_object.at( cfg_get( int, vars.skin_knife_model ) ).name ) )
						{
							for ( size_t i = 0; i < c_skin::get( ).knife_object.size( ); i++ )
							{
								if ( ImGui::Selectable( c_skin::get( ).knife_object.at( i ).name ) )
								{
									cfg_get( int, vars.skin_knife_model ) = i;
									c_skin::get( ).set_update_state( true );
								}
							}

							ImGui::EndCombo( );
						}

						if ( ImGui::BeginCombo( "glove models", c_skin::get( ).glove_object.at( cfg_get( int, vars.skin_glove_model ) ).name ) )
						{
							for ( size_t i = 0; i < c_skin::get( ).glove_object.size( ); i++ )
							{
								if ( ImGui::Selectable( c_skin::get( ).glove_object.at( i ).name ) )
								{
									cfg_get( int, vars.skin_glove_model ) = i;
									c_skin::get( ).set_update_state( true );
								}
							}

							ImGui::EndCombo( );
						}
					}}
				};

				static int current_inner_subtab = 0;
				render_inner_sub_tab<inner_subtab.size( )>( "inner_skin_subtab", inner_subtab, &current_inner_subtab );
			}
			ImGui::NextColumn( );
			{
				static std::array<c_tab, 2U> inner_subtab =
				{
					c_tab{ "general",  [ &style ]
					{
						const short cfg_index = c_skin::get( ).get_config_index( get_current_def_index( ) );

						static ImGuiTextFilter filter;
						filter.Draw( "##filter_skin", ImGui::GetContentRegionAvail( ).x );

						if ( ImGui::BeginListBox( "##skin_list", ImGui::GetContentRegionAvail( ) ) )
						{
							for ( size_t i = 0; i < parser::skin_kits.size( ); i++ )
							{
								ImGui::PushStyleColor( ImGuiCol_Text, c_skin::get( ).get_rarity_color( parser::skin_kits.at( i ).rarity ) );

								std::string name = parser::skin_kits.at( i ).name;

								if ( filter.PassFilter( name.c_str( ) ) )
								{
									std::string label = parser::skin_kits.at( i ).name + "##" + std::to_string( i );

									if ( ImGui::Selectable( label.c_str( ), i == cfg_get_vector( int, vars.skin_selected_paint_kit, cfg_index ) ) )
									{
										cfg_get_vector( int, vars.skin_selected_paint_kit, cfg_index ) = i;
										c_skin::get( ).set_update_state( true );
									}

									if ( ImGui::IsItemHovered( ) )
									{
										if ( const auto icon = c_skin::get( ).get_icon_texture( parser::skin_kits[ i ].icon ); icon != nullptr )
										{
											ImGui::BeginTooltip( );
											ImGui::Image( icon, { 200.0f, 150.0f } );
											ImGui::EndTooltip( );
										}
									}
								}
								ImGui::PopStyleColor( );
							}

							ImGui::EndListBox( );
						}
					}},
					c_tab{ "gloves",  [ &style ]
					{
						static ImGuiTextFilter filter;
						filter.Draw( "##filter_skin", ImGui::GetContentRegionAvail( ).x );

						if ( ImGui::BeginListBox( "##glove_skin_list", ImGui::GetContentRegionAvail( ) ) )
						{
							for ( size_t i = 0; i < parser::glove_kits.size( ); i++ )
							{
								ImGui::PushStyleColor( ImGuiCol_Text, c_skin::get( ).get_rarity_color( parser::glove_kits.at( i ).rarity ) );

								std::string name = parser::glove_kits.at( i ).name;

								if ( filter.PassFilter( name.c_str( ) ) )
								{
									std::string label = name + "##" + std::to_string( i );

									if ( ImGui::Selectable( label.c_str( ), i == cfg_get( int, vars.skin_glove_paint_kit ) ) )
									{
										cfg_get( int, vars.skin_glove_paint_kit ) = i;
										c_skin::get( ).set_update_state( true );
									}

									if ( ImGui::IsItemHovered( ) )
									{
										if ( const auto icon = c_skin::get( ).get_icon_texture( parser::glove_kits[ i ].icon ); icon != nullptr )
										{
											ImGui::BeginTooltip( );
											ImGui::Image( icon, { 200.0f, 150.0f } );
											ImGui::EndTooltip( );
										}
									}
								}

								ImGui::PopStyleColor( );
							}

							ImGui::EndListBox( );
						}
					}},
				};

				static int current_inner_subtab = 0;
				render_inner_sub_tab<inner_subtab.size( )>( "inner_skin_list_subtab", inner_subtab, &current_inner_subtab );
			}
			ImGui::Columns( 1 );
			ImGui::EndDisabled( );
		}}
	};

	render_sub_tab<tabs.size( )>( "visuals_sub_tab", tabs, &current_visuals_sub_tab );
}

void tabs::miscellaneous( )
{
	auto& style = ImGui::GetStyle( );
	static int misc_sub_tab = 0;
	static std::array<c_tab, 4U> const tabs =
	{
		c_tab{"general", [ &style ]
		{
			ImGui::Checkbox( "reveal rank", &cfg_get( bool, vars.misc_reveal_rank ) );
			ImGui::Checkbox( "anti-untrusted", &cfg_get( bool, vars.misc_anti_untrusted ) );
			if ( ImGui::Button( "unlock hidden cvars", ImVec2( ImGui::GetContentRegionAvail( ).x, 0 ) ) )
			{
				// only unlock it once lol
				static bool did_unlock = false;
				if ( !did_unlock )
				{
					auto it = **reinterpret_cast< c_convar*** >( reinterpret_cast< std::uintptr_t >( csgo::convar ) + 0x34 );
					for ( auto c = it->next; c != nullptr; c = c->next )
					{
						if ( c->flags & fcvar_developmentonly )
							c->flags &= ~fcvar_developmentonly;

						if ( c->flags & fcvar_hidden )
							c->flags &= ~fcvar_hidden;
					}

					did_unlock = true;
				}
			}
		}},
		c_tab{"movement", [ &style ]
		{
			ImGui::Checkbox( "auto-bhop", &cfg_get( bool, vars.misc_auto_bhop ) );
			if ( cfg_get( bool, vars.misc_auto_bhop ) )
				ImGui::SliderInt( "perfect-hop chances", &cfg_get( int, vars.auto_bhop_chance ), 0, 100, "%d%%" );

			ImGui::Checkbox( "infinite duck", &cfg_get( bool, vars.misc_infinite_duck ) );

			ImGui::Checkbox( "fast stop", &cfg_get( bool, vars.misc_fast_stop ) );
			ImGui::Checkbox( "auto-strafe", &cfg_get( bool, vars.misc_auto_strafe ) );
			if ( cfg_get( bool, vars.misc_auto_strafe ) )
			{
				ImGui::HotKey( "key##auto-strafe", &cfg_get( key_t, vars.misc_auto_strafe_key ) );
				ImGui::SliderInt( "smoothness", &cfg_get( int, vars.misc_auto_strafe_smoothness ), 0, 100 );
			}
		}},
		c_tab{"configs", [ &style ]
		{
			static std::string current_file = "none";

			ImGui::Columns( 2, "settings_column", false );
			{
				if ( ImGui::Button( "open folder", ImVec2( -1, 21 ) ) )
					ShellExecuteW( 0, L"open", cfg::path.c_str( ), NULL, NULL, SW_NORMAL );

				ImGui::PushItemWidth( ImGui::GetContentRegionAvail( ).x );

				ImGui::ListBox( "##config_list", &selected_config, [ ]( int index )
					{
						// return current displaying configuration name
						return cfg::file_names.at( index ).c_str( );
					}, cfg::file_names.size( ), 5
				);

				current_file = !cfg::file_names.empty( ) ? cfg::file_names.at( selected_config ) : "";

				ImGui::PopItemWidth( );
			}
			ImGui::NextColumn( );
			{
				ImGui::PushItemWidth( ImGui::GetContentRegionAvail( ).x );

				if ( ImGui::InputTextWithHint( "##config_file", "create new...", &config_file, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank ) )
				{
					if ( !cfg::save( config_file ) )
					{
						csl::push_color( FOREGROUND_RED );
						csl::print( "[error] failed to create \"{}\" config", config_file );
						csl::pop_color( );
					}

					config_file.clear( );
					cfg::refresh( );
				}
				if ( ImGui::IsItemHovered( ) )
					ImGui::SetTooltip( "press enter to create new configuration" );

				if ( ImGui::Button( "save", ImVec2( -1, 21 ) ) )
				{
					if ( !cfg::save( current_file ) )
					{
						csl::push_color( FOREGROUND_RED );
						csl::print( "[error] failed to save \"{}\" config", current_file );
						csl::pop_color( );
					}
				}

				if ( ImGui::Button( "load", ImVec2( -1, 21 ) ) )
				{
					if ( !cfg::load( current_file ) )
					{
						csl::push_color( FOREGROUND_RED );
						csl::print( "[error] failed to load \"{}\" config", current_file );
						csl::pop_color( );
					}

					c_skin::get( ).set_update_state( true );
					tabs::update_accent( style, cfg_get( col_t, vars.menu_accent ) );
				}

				if ( ImGui::Button( "remove", ImVec2( -1, 21 ) ) )
					ImGui::OpenPopup( "confirmation" );

				if ( ImGui::Button( "refresh", ImVec2( -1, 21 ) ) )
					cfg::refresh( );

				ImGui::PopItemWidth( );

				if ( ImGui::BeginPopupModal( "confirmation", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize ) )
				{
					ImGui::Text( "are you sure you want to remove \"%s\" ?", current_file.c_str( ) );
					ImGui::Spacing( );

					if ( ImGui::Button( "no", ImVec2( ImGui::GetContentRegionAvail( ).x / 2, 21 ) ) )
						ImGui::CloseCurrentPopup( );

					ImGui::SameLine( );

					if ( ImGui::Button( "yes", ImVec2( ImGui::GetContentRegionAvail( ).x, 21 ) ) )
					{
						cfg::remove( selected_config );

						// reset current configuration
						selected_config = 0;

						ImGui::CloseCurrentPopup( );
					}

					ImGui::EndPopup( );
				}
			}
		}},
		c_tab{ "menu settings", [ &style ]
		{
			ImGui::HotKey( "menu key", &cfg_get( key_t, vars.menu_open_key ) );

			if ( ImGui::ColorPicker( "menu accent", &cfg_get( col_t, vars.menu_accent ) ) )
			{
				update_accent( style, cfg_get( col_t, vars.menu_accent ) );
			}

			ImGui::Checkbox( "draw shadow", &cfg_get( bool, vars.menu_draw_shadow ) );
			ImGui::SameLine( ImGui::CalcItemWidth( ) - ImGui::GetCursorPos( ).x );
			ImGui::ColorPicker( "##draw_shadow_clr", &cfg_get( col_t, vars.menu_draw_shadow_clr ), true );

			if ( cfg_get( bool, vars.menu_draw_shadow ) )
			{
				ImGui::SliderFloat( "shadow thickness", &cfg_get( float, vars.menu_draw_shadow_thickness ), 12.0f, 128.0f, "%.3fpx" );
			}

			ImGui::Checkbox( "draw blur", &cfg_get( bool, vars.menu_draw_blur ) );
			if ( cfg_get( bool, vars.menu_draw_blur ) )
			{
				ImGui::SliderInt( "blur alpha", &cfg_get( int, vars.menu_draw_blur_alpha ), 0, 255 );
			}
		}}
	};

	render_sub_tab<tabs.size( )>( "misc_sub_tab", tabs, &misc_sub_tab );
}

void tabs::lua( )
{ }

void tabs::update_accent( ImGuiStyle& style, col_t& accent )
{
	auto colors = style.Colors;

	const int alpha = 160;
	const int hovered_alpha = 180;
	const int active_alpha = 200;

	//colors[ ImGuiCol_FrameBg ] = accent.set<col_a>( alpha ).get_vec4( );
	colors[ ImGuiCol_FrameBgHovered ] = accent.set<col_a>( hovered_alpha ).get_vec4( );
	colors[ ImGuiCol_FrameBgActive ] = accent.set<col_a>( active_alpha ).get_vec4( );

	colors[ ImGuiCol_TitleBg ] = accent.set<col_a>( alpha ).get_vec4( );
	colors[ ImGuiCol_TitleBgActive ] = accent.set<col_a>( hovered_alpha ).get_vec4( );
	colors[ ImGuiCol_TitleBgCollapsed ] = accent.set<col_a>( active_alpha ).get_vec4( );

	colors[ ImGuiCol_ScrollbarBg ] = accent.set<col_a>( alpha ).get_vec4( );
	colors[ ImGuiCol_ScrollbarGrab ] = accent.set<col_a>( hovered_alpha ).get_vec4( );
	colors[ ImGuiCol_ScrollbarGrabHovered ] = accent.set<col_a>( hovered_alpha ).get_vec4( );
	colors[ ImGuiCol_ScrollbarGrabActive ] = accent.set<col_a>( active_alpha ).get_vec4( );

	colors[ ImGuiCol_CheckMark ] = accent.set<col_a>( active_alpha ).get_vec4( );

	colors[ ImGuiCol_SliderGrab ] = accent.set<col_a>( alpha ).get_vec4( );
	colors[ ImGuiCol_SliderGrabActive ] = accent.set<col_a>( active_alpha ).get_vec4( );

	colors[ ImGuiCol_Button ] = accent.set<col_a>( alpha ).get_vec4( );
	colors[ ImGuiCol_ButtonHovered ] = accent.set<col_a>( hovered_alpha ).get_vec4( );
	colors[ ImGuiCol_ButtonActive ] = accent.set<col_a>( active_alpha ).get_vec4( );

	colors[ ImGuiCol_Header ] = accent.set<col_a>( alpha ).get_vec4( );
	colors[ ImGuiCol_HeaderHovered ] = accent.set<col_a>( hovered_alpha ).get_vec4( );
	colors[ ImGuiCol_HeaderActive ] = accent.set<col_a>( active_alpha ).get_vec4( );

	colors[ ImGuiCol_Separator ] = accent.set<col_a>( alpha ).get_vec4( );
	colors[ ImGuiCol_SeparatorHovered ] = accent.set<col_a>( hovered_alpha ).get_vec4( );
	colors[ ImGuiCol_SeparatorActive ] = accent.set<col_a>( active_alpha ).get_vec4( );

	colors[ ImGuiCol_Separator ] = accent.set<col_a>( alpha ).get_vec4( );
	colors[ ImGuiCol_SeparatorHovered ] = accent.set<col_a>( hovered_alpha ).get_vec4( );
	colors[ ImGuiCol_SeparatorActive ] = accent.set<col_a>( active_alpha ).get_vec4( );

	colors[ ImGuiCol_ResizeGrip ] = accent.set<col_a>( alpha ).get_vec4( );
	colors[ ImGuiCol_ResizeGripHovered ] = accent.set<col_a>( hovered_alpha ).get_vec4( );
	colors[ ImGuiCol_ResizeGripActive ] = accent.set<col_a>( active_alpha ).get_vec4( );
}

void windows::menu( )
{
	auto& style = ImGui::GetStyle( );
	auto& io = ImGui::GetIO( );
	io.MouseDrawCursor = is_menu_opened;

	if ( !is_menu_opened )
		return;

	const ImVec2 screen_size = io.DisplaySize;

	int x, y;
	csgo::input_system->get_cursor_position( &x, &y );
	io.MousePos = ImVec2( x, y );
	globals::mouse_pos = vec2_t( x, y );

	ImGui::SetNextWindowPos( ImVec2( screen_size.x * 0.5f, screen_size.y * 0.5f ), ImGuiCond_Once, ImVec2( 0.5f, 0.5f ) );
	ImGui::SetNextWindowSize( ImVec2( 500, 400 ), ImGuiCond_Once );
	//ImGui::SetNextWindowSizeConstraints( ImVec2( 450, 325 ), io.DisplaySize / 2 );
	ImGui::PushFont( imgui_fonts::whitney );
	ImGui::Begin( "main_window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse );
	{
		menu_pos = ImGui::GetWindowPos( );
		menu_size = ImGui::GetWindowSize( );

		// draw background blur
		if ( cfg_get( bool, vars.menu_draw_blur ) )
		{
			draw::add_blur( ImGui::GetWindowDrawList( ), cfg_get( int, vars.menu_draw_blur_alpha ), menu_pos, menu_pos + menu_size );
		}

		// draw background shadow
		if ( cfg_get( bool, vars.menu_draw_shadow ) )
		{

			auto draw_list = ImGui::GetBackgroundDrawList( );
			draw_list->AddShadowRect( menu_pos, menu_pos + menu_size, cfg_get( col_t, vars.menu_draw_shadow_clr ).get_u32( ), cfg_get( float, vars.menu_draw_shadow_thickness ), ImVec2( ), ImDrawFlags_RoundCornersAll, style.WindowRounding );
		}

		// add tabs
		static std::array<c_tab, 6U> const tabs =
		{
			c_tab{ "a", &tabs::rage_bot },
			c_tab{ "b", &tabs::anti_aim },
			c_tab{ "e", &tabs::players },
			c_tab{ "c", &tabs::visuals },
			c_tab{ "d", &tabs::miscellaneous },
			c_tab{ "f", &tabs::lua }
		};

		tabs::render<tabs.size( )>( "main_tabs", tabs, &current_tab );
	}
	ImGui::End( );
	ImGui::PopFont( );

	if (current_tab == 2 )
	{
		ImGui::SetNextWindowPos( ImVec2( menu_pos.x + menu_size.x + style.FramePadding.x, menu_pos.y ), ImGuiCond_Always );
		ImGui::SetNextWindowSize( ImVec2( menu_size.x / 2.2f, menu_size.y ), ImGuiCond_Always );
		ImGui::Begin( "preview ( wip )", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove );
		{
			auto draw = ImGui::GetWindowDrawList( );

			// draw background blur
			if ( cfg_get( bool, vars.menu_draw_blur ) )
			{
				draw::add_blur( draw, cfg_get( int, vars.menu_draw_blur_alpha ), ImGui::GetWindowPos( ), ImGui::GetWindowPos( ) + ImGui::GetWindowSize( ) );
			}

			// draw background shadow
			if ( cfg_get( bool, vars.menu_draw_shadow ) )
			{

				auto bg_draw = ImGui::GetBackgroundDrawList( );
				bg_draw->AddShadowRect( ImGui::GetWindowPos( ), ImGui::GetWindowPos( ) + ImGui::GetWindowSize( ), cfg_get( col_t, vars.menu_draw_shadow_clr ).get_u32( ), cfg_get( float, vars.menu_draw_shadow_thickness ), ImVec2( ), ImDrawFlags_RoundCornersAll, style.WindowRounding );
			}

			if ( const auto texture = preview_model->get_texture( ) )
			{
				auto window_pos = ImGui::GetWindowPos( );
				draw->AddImage( texture->texture_handles[ 0 ]->raw_texture,
					ImVec2( window_pos.x - 50.f, window_pos.y - 25.f),
					ImVec2(window_pos.x + texture->get_actual_width( ), window_pos.y + texture->get_actual_height( )),
					ImVec2( 0, 0 ), ImVec2( 1, 1 ),
					ImColor( 1.0f, 1.0f, 1.0f, 1.0f )
				);
			}

			esp_preview( draw );
		}
		ImGui::End( );
	}
}

void windows::esp_preview( ImDrawList* draw )
{
	switch ( tabs::current_player_sub_tabs )
	{
		case 0:
		{
			cfg_get( esp_variable_t, vars.vis_esp_enemy_box ).draw_preview( "box" );
			break;
		}
	}
}
