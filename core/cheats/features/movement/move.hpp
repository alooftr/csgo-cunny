#pragma once
#include "../../cfg/var.h"
#include "../../../sdk/sdk.hpp"
#include "../../../utils/singleton.h"

class c_movement : public c_singleton<c_movement>
{
public:
	void pre_prediction( c_user_cmd* cmd, c_base_entity* local_player );
	void pos_prediction( c_user_cmd* cmd );

	void movement_correction( c_user_cmd* cmd, vec3_t old_angles, vec3_t wish_angle, c_base_entity* local_player );
private:
	c_base_entity* local_player = nullptr;
	int old_flags = 0;

	// main functions
	void auto_bhop( c_user_cmd* cmd );
	void fast_stop( c_user_cmd* cmd );
	void infinite_duck( c_user_cmd* cmd );
	void auto_strafe( c_user_cmd* cmd );
};