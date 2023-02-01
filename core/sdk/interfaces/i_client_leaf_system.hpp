#pragma once

enum renderable_model_type_t
{
	renderable_model_unknown_type = -1,
	renderable_model_entity = 0,
	renderable_model_studiomdl,
	renderable_model_static_prop,
	renderable_model_brush,
};

class i_client_leaf_system_engine
{
public:
	virtual void create_renderable_handle( i_client_renderable* renderable, bool render_with_view_models, e_translucency_type type, renderable_model_type_t model_type, UINT32 splitscreen_enabled = 0xFFFFFFFF ) = 0; // = RENDERABLE_MODEL_UNKNOWN_TYPE ) = 0;

};