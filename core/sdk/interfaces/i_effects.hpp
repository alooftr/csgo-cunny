#pragma once

class i_prediction_system
{
public:
	virtual ~i_prediction_system( ) { }
};

class i_effects : public i_prediction_system
{
public:
	virtual void	beam( const vec3_t& vecStart, const vec3_t& vecEnd, int nModelIndex,
		int nHaloIndex, unsigned char frameStart, unsigned char frameRate,
		float flLife, unsigned char width, unsigned char endWidth, unsigned char fadeLength,
		unsigned char noise, unsigned char red, unsigned char green,
		unsigned char blue, unsigned char brightness, unsigned char speed ) = 0;

	virtual void	smoke( const vec3_t& vecOrigin, int nModelIndex, float flScale, float flFrameRate ) = 0;
	virtual void	sparks( const vec3_t& vecOrigin, int iMagnitude = 1, int nTrailLength = 1, const vec3_t* pvecDirection = nullptr ) = 0;
	virtual void	dust( const vec3_t& vecOrigin, const vec3_t& vecDirection, float flSize, float flSpeed ) = 0;
	virtual void	muzzle_flash( const vec3_t& vecOrigin, const vec3_t& angView, float flScale, int iType ) = 0;
	virtual void	metal_sparks( const vec3_t& vecOrigin, const vec3_t& vecDirection ) = 0;
	virtual void	energy_splash( const vec3_t& vecOrigin, const vec3_t& vecDirection, bool bExplosive = false ) = 0;
	virtual void	ricochet( const vec3_t& vecOrigin, const vec3_t& vecDirection ) = 0;
	virtual float	time( ) = 0;
	virtual bool	is_server( ) = 0;
	virtual void	suppress_effects_sounds( bool bSuppress ) = 0;
};
