#pragma once

class i_weapon_system
{
public:
	ccs_weapon_data* get_weapon_data( short nItemDefinitionIndex )
	{
		return utils::call_vfunc<ccs_weapon_data*>( this, 2, nItemDefinitionIndex );
	}
};
