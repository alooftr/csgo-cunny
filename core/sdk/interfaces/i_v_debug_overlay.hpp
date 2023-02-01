#pragma once

class overlay_text_t;
class iv_debug_overlay
{
public:
	virtual void			add_entity_text_overlay( int iEntityIndex, int iLineOffset, float flDuration, int r, int g, int b, int a, const char* fmt, ... ) = 0;
	virtual void			add_box_overlay( const vec3_t& vecOrigin, const vec3_t& vecAbsMin, const vec3_t& vecAbsMax, const vec3_t& angOrientation, int r, int g, int b, int a, float flDuration ) = 0;
	virtual void			add_sphere_overlay( const vec3_t& vecOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration ) = 0;
	virtual void			add_triangle_overlay( const vec3_t& p1, const vec3_t& p2, const vec3_t& p3, int r, int g, int b, int a, bool bNoDepthTest, float flDuration ) = 0;
	virtual void			add_line_overlay( const vec3_t& vecOrigin, const vec3_t& vecDest, int r, int g, int b, bool bNoDepthTest, float flDuration ) = 0;
	virtual void			add_text_overlay( const vec3_t& vecOrigin, float flDuration, const char* fmt, ... ) = 0;
	virtual void			add_text_overlay( const vec3_t& vecOrigin, int iLineOffset, float flDuration, const char* fmt, ... ) = 0;
	virtual void			add_screen_text_overlay( float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char* szText ) = 0;
	virtual void			add_swept_box_overlay( const vec3_t& vecStart, const vec3_t& vecEnd, const vec3_t& vecMin, const vec3_t& vecMax, const vec3_t& angles, int r, int g, int b, int a, float flDuration ) = 0;
	virtual void			add_grid_overlay( const vec3_t& vecOrigin ) = 0;
	virtual void			add_coord_frame_overlay( const matrix3x4_t& matFrame, float flScale, int vColorTable[ 3 ][ 3 ] = nullptr ) = 0;
	virtual int				screen_position( const vec3_t& vecPoint, vec3_t& vecScreen ) = 0;
	virtual int				screen_position( float flXPos, float flYPos, vec3_t& vecScreen ) = 0;
	virtual overlay_text_t* get_first( ) = 0;
	virtual overlay_text_t* get_next( overlay_text_t* pCurrent ) = 0;
	virtual void			clear_dead_overlays( ) = 0;
	virtual void			clear_all_overlays( ) = 0;
	virtual void			add_text_overlay_rgb( const vec3_t& vecOrigin, int iLineOffset, float flDuration, float r, float g, float b, float a, const char* fmt, ... ) = 0;
	virtual void			add_text_overlay_rgb( const vec3_t& vecOrigin, int iLineOffset, float flDuration, int r, int g, int b, int a, const char* fmt, ... ) = 0;
	virtual void			add_line_overlay_alpha( const vec3_t& vecOrigin, const vec3_t& dest, int r, int g, int b, int a, bool bNoDepthTest, float flDuration ) = 0;
	virtual void			add_box_overlay2( const vec3_t& vecOrigin, const vec3_t& vecAbsMin, const vec3_t& vecAbsMax, const vec3_t& angOrientation, const col_t& faceColor, const col_t& edgeColor, float flDuration ) = 0;
	virtual void			add_line_overlay( const vec3_t& vecOrigin, const vec3_t& vecDest, int r, int g, int b, int a, float flThickness, float flDuration ) = 0;
	virtual void			purge_text_overlays( ) = 0;
	virtual void			add_capsule_overlay( const vec3_t& vecAbsMin, const vec3_t& vecAbsMax, const float& flRadius, int r, int g, int b, int a, float flDuration ) = 0;
	virtual void			draw_pill( vec3_t& vecAbsMin, vec3_t& vecAbsMax, float flRadius, int r, int g, int b, int a, float flDuration ) = 0;
};
