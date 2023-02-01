#pragma once

#include "../../../sdk/sdk.hpp"

class c_preview_model
{
public:
	void on_paint( );
	bool setup( );
	i_texture* get_texture( ) { return this->texture; }
	c_merged_mdl* get_model( ) { return this->model; }
private:
	i_texture* texture = nullptr;
	i_texture* cube_map = nullptr;
	c_merged_mdl* model = nullptr;

	c_view_setup view_setup = {};
};

inline c_preview_model* preview_model = new c_preview_model( );