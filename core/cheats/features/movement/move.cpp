#include <random>

#include "../../../utils/math.h"

#include "move.hpp"

void c_movement::pre_prediction( c_user_cmd* cmd, c_base_entity* lp_ptr )
{
	local_player = lp_ptr;

	if (!local_player->is_alive( ))
		return;

	old_flags = local_player->get_flags( );

	if (cfg_get( bool, vars.misc_auto_bhop ))
		this->auto_bhop( cmd );

	if (cfg_get( bool, vars.misc_fast_stop ))
		this->fast_stop( cmd );

	if (cfg_get( bool, vars.misc_reveal_rank ) && cmd->buttons & in_score)
		csgo::client->dispatch_user_message( 50, 0U, 0, nullptr );

	if (cfg_get( bool, vars.misc_auto_strafe ) && cfg_get( key_t, vars.misc_auto_strafe_key ).is_active( ))
		this->auto_strafe( cmd );
}

void c_movement::pos_prediction( c_user_cmd* cmd )
{
	if (cfg_get( bool, vars.misc_infinite_duck ))
		this->infinite_duck( cmd );
}

void c_movement::auto_bhop( c_user_cmd* cmd )
{
	static auto sv_autobunnyhopping = csgo::convar->find_var( "sv_autobunnyhopping" );

	if (sv_autobunnyhopping->get_bool( ))
		return;

	if (const auto mt = local_player->get_move_type( ); mt == movetype_ladder || mt == movetype_noclip || mt == movetype_observer)
		return;

	std::random_device randomDevice;
	std::mt19937 generate( randomDevice( ) );
	std::uniform_int_distribution<> chance( 0, 100 );

	if (chance( generate ) > cfg_get(int, vars.auto_bhop_chance ))
		return;

	static bool last_jumped = false, should_fake = false;

	if (!last_jumped && should_fake)
	{
		should_fake = false;
		cmd->buttons |= in_jump;
	}
	else if (cmd->buttons & in_jump)
	{
		if (local_player->get_flags( ) & fl_onground || local_player->get_flags( ) & fl_partialground)
		{
			last_jumped = true;
			should_fake = true;
		}
		else
		{
			cmd->buttons &= ~in_jump;
			last_jumped = false;
		}
	}
	else
	{
		last_jumped = false;
		should_fake = false;
	}
}

void c_movement::fast_stop( c_user_cmd* cmd )
{
	if (!local_player || !local_player->is_alive( ))
		return;

	if (const auto mt = local_player->get_move_type( ); mt == movetype_ladder || mt == movetype_noclip)
		return;

	if (!(local_player->get_flags( ) & fl_onground) || cmd->buttons & in_jump)
		return;

	if (cmd->buttons & (in_moveleft | in_moveright | in_forward | in_back))
		return;

	const auto velocity = local_player->get_velocity( );
	const auto speed = velocity.length_2d( );
	if (speed < 15.0f)
		return;

	vec3_t direction = math::to_angle( velocity );
	direction.y = cmd->view_angle.y - direction.y;

	const auto negated_direction = math::to_vector( direction ) * -speed;

	cmd->forward_move = negated_direction.x;
	cmd->side_move = negated_direction.y;
}

void c_movement::infinite_duck( c_user_cmd* cmd )
{
	if (cfg_get( bool, vars.misc_anti_untrusted ))
		return;

	if (!local_player || !local_player->is_alive( ) || local_player->get_flags( ) & fl_frozen)
		return;

	cmd->buttons |= in_bullrush;
}

#define check_if_non_valid_number(x) (fpclassify(x) == FP_INFINITE || fpclassify(x) == FP_NAN || fpclassify(x) == FP_SUBNORMAL)
void c_movement::auto_strafe( c_user_cmd* cmd )
{

	if (!local_player || !local_player->is_alive( ))
		return;

	if (const auto mt = local_player->get_move_type( ); mt == movetype_ladder || mt == movetype_noclip)
		return;

	if (local_player->get_flags( ) & fl_onground)
		return;

	if (local_player->get_velocity( ).length_2d( ) <= 5.f)
		return;


	static c_convar* cl_sidespeed = csgo::convar->find_var( "cl_sidespeed" );
	if (cl_sidespeed == nullptr)
		return;

	static auto old_yaw = 0.0f;


	auto get_velocity_degree = [ ]( float velocity )
	{
		auto tmp = RAD2DEG( atan( 30.0f / velocity ) );

		if (check_if_non_valid_number( tmp ) || tmp > 90.0f)
			return 90.0f;

		else if (tmp < 0.0f)
			return 0.0f;
		else
			return tmp;
	};


	auto velocity = local_player->get_velocity( );
	velocity.z = 0.0f;

	auto forward_move = cmd->forward_move;
	auto side_move = cmd->side_move;

	if (velocity.length_2d( ) < 5.0f && !forward_move && !side_move)
		return;

	static auto flip = false;
	flip = !flip;

	auto turn_direction_modifier = flip ? 1.0f : -1.0f;
	auto view_angles = cmd->view_angle;

	if (forward_move || side_move)
	{
		cmd->forward_move = 0.0f;
		cmd->side_move = 0.0f;

		auto turn_angle = atan2( -side_move, forward_move );
		view_angles.y += turn_angle * M_RADPI;
	}
	else if (forward_move) //-V550
		cmd->forward_move = 0.0f;

	auto strafe_angle = RAD2DEG( atan( 15.0f / velocity.length_2d( ) ) );

	if (strafe_angle > 90.0f)
		strafe_angle = 90.0f;
	else if (strafe_angle < 0.0f)
		strafe_angle = 0.0f;

	auto temp = vec3_t( 0.0f, view_angles.y - old_yaw, 0.0f );
	temp.y = math::normalize_yaw( temp.y );

	auto yaw_delta = temp.y;
	old_yaw = view_angles.y;

	auto abs_yaw_delta = fabs( yaw_delta );

	if (abs_yaw_delta <= strafe_angle || abs_yaw_delta >= 30.0f)
	{
		vec3_t velocity_angles;
		math::vector_angles( velocity, velocity_angles );

		temp = vec3_t( 0.0f, view_angles.y - velocity_angles.y, 0.0f );
		temp.y = math::normalize_yaw( temp.y );

		auto velocity_angle_yaw_delta = temp.y;
		auto velocity_degree = get_velocity_degree( velocity.length_2d( ) ) * (cfg_get( int, vars.misc_auto_strafe_smoothness ) / 100);

		if (velocity_angle_yaw_delta <= velocity_degree || velocity.length_2d( ) <= 15.0f)
		{
			if (-velocity_degree <= velocity_angle_yaw_delta || velocity.length_2d( ) <= 15.0f)
			{
				view_angles.y += strafe_angle * turn_direction_modifier;
				cmd->side_move = cl_sidespeed->get_float( ) * turn_direction_modifier;
			}
			else
			{
				view_angles.y = velocity_angles.y - velocity_degree;
				cmd->side_move = cl_sidespeed->get_float( );
			}
		}
		else
		{
			view_angles.y = velocity_angles.y + velocity_degree;
			cmd->side_move = -cl_sidespeed->get_float( );
		}
	}
	else if (yaw_delta > 0.0f)
		cmd->side_move = -cl_sidespeed->get_float( );
	else if (yaw_delta < 0.0f)
		cmd->side_move = cl_sidespeed->get_float( );

	auto move = vec3_t( cmd->forward_move, cmd->side_move, 0.0f );
	auto speed = move.length( );

	vec3_t angles_move;
	math::vector_angles( move, angles_move );

	auto normalized_x = fmod( cmd->view_angle.x + 180.0f, 360.0f ) - 180.0f;
	auto normalized_y = fmod( cmd->view_angle.y + 180.0f, 360.0f ) - 180.0f;

	auto yaw = DEG2RAD( normalized_y - view_angles.y + angles_move.y );

	if (normalized_x >= 90.0f || normalized_x <= -90.0f || cmd->view_angle.x >= 90.0f && cmd->view_angle.x <= 200.0f || cmd->view_angle.x <= -90.0f && cmd->view_angle.x <= 200.0f) //-V648
		cmd->forward_move = -cos( yaw ) * speed;
	else
		cmd->forward_move = cos( yaw ) * speed;

	cmd->side_move = sin( yaw ) * speed;
}

void c_movement::movement_correction( c_user_cmd* cmd, vec3_t old_angles, vec3_t wish_angle, c_base_entity* local_player )
{
	if (old_angles.x != wish_angle.x || old_angles.y != wish_angle.y || old_angles.z != wish_angle.z)
	{
		vec3_t wish_forward, wish_right, wish_up, cmd_forward, cmd_right, cmd_up;

		auto view_point = old_angles;
		auto movedata = vec3_t( cmd->forward_move, cmd->side_move, cmd->up_move );
		view_point.normalize( );

		if (!(local_player->get_flags( ) & fl_onground) && view_point.z != 0.f)
			movedata.y = 0.f;

		math::angle_vectors( wish_angle, &wish_forward, &wish_right, &wish_up );
		math::angle_vectors( view_point, &cmd_forward, &cmd_right, &cmd_up );

		auto v8 = sqrt( wish_forward.x * wish_forward.x + wish_forward.y * wish_forward.y ), v10 = sqrt( wish_right.x * wish_right.x + wish_right.y * wish_right.y ), v12 = sqrt( wish_up.z * wish_up.z );

		vec3_t wish_forward_norm( 1.0f / v8 * wish_forward.x, 1.0f / v8 * wish_forward.y, 0.f ),
			wish_right_norm( 1.0f / v10 * wish_right.x, 1.0f / v10 * wish_right.y, 0.f ),
			wish_up_norm( 0.f, 0.f, 1.0f / v12 * wish_up.z );

		auto v14 = sqrt( cmd_forward.x * cmd_forward.x + cmd_forward.y * cmd_forward.y ), v16 = sqrt( cmd_right.x * cmd_right.x + cmd_right.y * cmd_right.y ), v18 = sqrt( cmd_up.z * cmd_up.z );

		vec3_t cmd_forward_norm( 1.0f / v14 * cmd_forward.x, 1.0f / v14 * cmd_forward.y, 1.0f / v14 * 0.0f ),
			cmd_right_norm( 1.0f / v16 * cmd_right.x, 1.0f / v16 * cmd_right.y, 1.0f / v16 * 0.0f ),
			cmd_up_norm( 0.f, 0.f, 1.0f / v18 * cmd_up.z );

		auto v22 = wish_forward_norm.x * movedata.x, v26 = wish_forward_norm.y * movedata.x, v28 = wish_forward_norm.z * movedata.x, v24 = wish_right_norm.x * movedata.y, v23 = wish_right_norm.y * movedata.y, v25 = wish_right_norm.z * movedata.y, v30 = wish_up_norm.x * movedata.z, v27 = wish_up_norm.z * movedata.z, v29 = wish_up_norm.y * movedata.z;

		vec3_t correct_movement;
		correct_movement.x = cmd_forward_norm.x * v24 + cmd_forward_norm.y * v23 + cmd_forward_norm.z * v25 + (cmd_forward_norm.x * v22 + cmd_forward_norm.y * v26 + cmd_forward_norm.z * v28) + (cmd_forward_norm.y * v30 + cmd_forward_norm.x * v29 + cmd_forward_norm.z * v27);
		correct_movement.y = cmd_right_norm.x * v24 + cmd_right_norm.y * v23 + cmd_right_norm.z * v25 + (cmd_right_norm.x * v22 + cmd_right_norm.y * v26 + cmd_right_norm.z * v28) + (cmd_right_norm.x * v29 + cmd_right_norm.y * v30 + cmd_right_norm.z * v27);
		correct_movement.z = cmd_up_norm.x * v23 + cmd_up_norm.y * v24 + cmd_up_norm.z * v25 + (cmd_up_norm.x * v26 + cmd_up_norm.y * v22 + cmd_up_norm.z * v28) + (cmd_up_norm.x * v30 + cmd_up_norm.y * v29 + cmd_up_norm.z * v27);

		static c_convar* cl_forwardspeed = csgo::convar->find_var( "cl_forwardspeed" );

		if (cl_forwardspeed == nullptr)
			return;

		static c_convar* cl_sidespeed = csgo::convar->find_var( "cl_sidespeed" );

		if (cl_sidespeed == nullptr)
			return;

		static c_convar* cl_upspeed = csgo::convar->find_var( "cl_upspeed" );

		if (cl_upspeed == nullptr)
			return;
		// get max speed limits by convars
		const float max_forward_speed = cl_forwardspeed->get_float( );
		const float max_side_speed = cl_sidespeed->get_float( );
		const float max_up_speed = cl_upspeed->get_float( );

		correct_movement.x = std::clamp( correct_movement.x, -max_forward_speed, max_forward_speed );
		correct_movement.y = std::clamp( correct_movement.y, -max_side_speed, max_side_speed );
		correct_movement.z = std::clamp( correct_movement.z, -max_up_speed, max_up_speed );

		cmd->forward_move = correct_movement.x;
		cmd->side_move = correct_movement.y;
		cmd->up_move = correct_movement.z;

		cmd->buttons &= ~(in_moveright | in_moveleft | in_back | in_forward);
		if (cmd->side_move != 0.0) {
			if (cmd->side_move <= 0.0)
				cmd->buttons |= in_moveleft;
			else
				cmd->buttons |= in_moveright;
		}

		if (cmd->forward_move != 0.0) {
			if (cmd->forward_move <= 0.0)
				cmd->buttons |= in_back;
			else
				cmd->buttons |= in_forward;
		}
	}
}
