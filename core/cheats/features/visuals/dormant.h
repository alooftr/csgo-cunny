#pragma once
#include "../../../sdk/sdk.hpp"
#include "../../../utils/singleton.h"

class c_dormant_esp : public c_singleton <c_dormant_esp>
{
	friend class c_singleton<c_dormant_esp>;
public:
	void start( c_base_entity* local_player )
	{
		cur_sound_list.remove_all( );
		csgo::engine_sound->get_active_sounds( cur_sound_list );

		if (!cur_sound_list.count( ))
			return;

		for (auto i = 0; i < cur_sound_list.count( ); i++)
		{
			auto& sound = cur_sound_list[ i ];

			if (sound.sound_source < 1 || sound.sound_source > 64)
				continue;

			if (sound.origin->is_zero( ))
				continue;

			if (!valid_sound( sound ))
				continue;

			auto player = csgo::entity_list->get<c_base_entity>( sound.sound_source );

			if (!player || player->get_client_class( )->class_id != e_class_index::ccsplayer || !player->is_alive( ))
				continue;

			if (!local_player->is_enemy( player ))
				continue;

			setup_adjust( player, sound );
			sound_players[ sound.sound_source ].write( sound );
		}

		sound_buffer = cur_sound_list;
	}

	bool adjust_sound( c_base_entity* player )
	{
		auto i = player->get_index( );
		auto sound_player = sound_players[ i ];

		auto expired = false;

		if (fabs( csgo::global_vars->real_time - sound_player.receive_time ) > 10.0f)
			expired = true;

		*player->is_spotted( ) = true;
		player->get_flags( ) = sound_player.flags;
		player->set_abs_origin( sound_player.origin );

		return !expired;
	}

	void setup_adjust( c_base_entity* player, sound_info_t& sound )
	{
		vec3_t src3D = *sound.origin + vec3_t( 0.0f, 0.0f, 1.0f );
		vec3_t dst3D = src3D - vec3_t( 0.0f, 0.0f, 100.0f );

		trace_t tr;
		ray_t ray{ src3D ,dst3D };
		c_trace_filter filter{ player };

		csgo::engine_trace->trace_ray( ray, MASK_PLAYERSOLID, &filter, &tr );

		if (tr.all_solid)
			sound_players[ sound.sound_source ].receive_time = -1;

		*sound.origin = tr.fraction <= 0.97f?tr.end:*sound.origin;

		sound_players[ sound.sound_source ].flags = player->get_flags( );
		sound_players[ sound.sound_source ].flags |= (tr.fraction < 0.50f?fl_ducking:0) | (tr.fraction < 1.0f?fl_onground:0);
		sound_players[ sound.sound_source ].flags &= (tr.fraction >= 0.50f?~fl_ducking:0) | (tr.fraction >= 1.0f?~fl_onground:0);
	}

	bool valid_sound( sound_info_t& sound )
	{
		for (auto i = 0; i < sound_buffer.count( ); i++)
			if (sound_buffer[ i ].guid == sound.guid)
				return false;

		return true;
	}

	struct sound_player_t
	{
		void reset( bool store_data = false, const vec3_t& pos = vec3_t( 0, 0, 0 ), int flags = 0 )
		{
			if (store_data)
			{
				receive_time = csgo::global_vars->real_time;
				origin = pos;
				flags = flags;
			}
			else
			{
				receive_time = 0.0f;
				origin = vec3_t( );
				flags = 0;
			}
		}

		void write( sound_info_t& sound )
		{
			receive_time = csgo::global_vars->real_time;
			origin = *sound.origin;
		}

		float receive_time = 0.0f;
		vec3_t origin = vec3_t( 0, 0, 0 );
		int flags = 0;
	} sound_players[ 65 ];

	c_utl_vector <sound_info_t> sound_buffer;
	c_utl_vector <sound_info_t> cur_sound_list;
};
