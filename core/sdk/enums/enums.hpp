#pragma once
#include <cstdint>

#define MAX_COORD_FLOAT		16'384.f			// max world coordinates
#define MIN_COORD_FLOAT		-(MAX_COORD_FLOAT)	// min world coordinates
#define MAX_DISTANCE		8'192.f				// max bullet distance, etc
#define MAX_FOV				180.f				// max field of view
#define MAX_DAMAGE			500.f				// max weapons damage
#define MAX_WEAPONS			64					// max number of weapons available

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/shared/shareddefs.h
#pragma region valve_shareddefs
#define TICK_INTERVAL			( csgo::global_vars->interval_per_tick )
#define TIME_TO_TICKS( t )		( static_cast<int>( 0.5f + static_cast<float>( t ) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL * static_cast<float>( t ) )
#define ROUND_TO_TICKS( t )		( TICK_INTERVAL * TIME_TO_TICKS( t ) )
#define TICK_NEVER_THINK		( -1 )

#pragma region valve_textures
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/texture_group_names.h
#define TEXTURE_GROUP_LIGHTMAP						"Lightmaps"
#define TEXTURE_GROUP_WORLD							"World textures"
#define TEXTURE_GROUP_MODEL							"Model textures"
#define TEXTURE_GROUP_VGUI							"VGUI textures"
#define TEXTURE_GROUP_PARTICLE						"Particle textures"
#define TEXTURE_GROUP_DECAL							"Decal textures"
#define TEXTURE_GROUP_SKYBOX						"SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				"ClientEffect textures"
#define TEXTURE_GROUP_OTHER							"Other textures"
#define TEXTURE_GROUP_PRECACHED						"Precached"
#define TEXTURE_GROUP_CUBE_MAP						"CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					"RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED					"Unaccounted textures"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER			"Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			"Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			"Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			"Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					"DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					"ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					"Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				"Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			"RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					"Morph Targets"

#define TEXTURE_SKYBOX_BAGGAGE						"cs_baggage_skybox_"
#define TEXTURE_SKYBOX_TIBET						"cs_tibet"
#define TEXTURE_SKYBOX_EMBASSY						"embassy"
#define TEXTURE_SKYBOX_ITALY						"italy"
#define TEXTURE_SKYBOX_JUNGLE						"jungle"
#define TEXTURE_SKYBOX_NUKEBLANK					"nukeblank"
#define TEXTURE_SKYBOX_OFFICE						"office"
#define TEXTURE_SKYBOX_DAYLIGHT01					"sky_cs15_daylight01_hdr"
#define TEXTURE_SKYBOX_DAYLIGHT02					"sky_cs15_daylight02_hdr"
#define TEXTURE_SKYBOX_DAYLIGHT03					"sky_cs15_daylight03_hdr"
#define TEXTURE_SKYBOX_DAYLIGHT04					"sky_cs15_daylight04_hdr"
#define TEXTURE_SKYBOX_CLOUDY01						"sky_csgo_cloudy01"
#define TEXTURE_SKYBOX_NIGHT02						"sky_csgo_night02"
#define TEXTURE_SKYBOX_NIGHT02B						"sky_csgo_night02b"
#define TEXTURE_SKYBOX_DUST							"sky_dust"
#define TEXTURE_SKYBOX_VENICE						"sky_venice"
#define TEXTURE_SKYBOX_VERTIGO						"vertigo"
#define TEXTURE_SKYBOX_VERTIGO_HDR					"vertigoblue_hdr"
#define TEXTURE_SKYBOX_VIETNAM						"vietnam"
#pragma endregion

#pragma region valve_decals
#define CHAR_TEX_ANTLION		'A'
#define CHAR_TEX_BLOODYFLESH	'B'
#define CHAR_TEX_CONCRETE		'C'
#define CHAR_TEX_DIRT			'D'
#define CHAR_TEX_EGGSHELL		'E'
#define CHAR_TEX_FLESH			'F'
#define CHAR_TEX_GRATE			'G'
#define CHAR_TEX_ALIENFLESH		'H'
#define CHAR_TEX_CLIP			'I'
#define CHAR_TEX_SNOW			'K'
#define CHAR_TEX_PLASTIC		'L'
#define CHAR_TEX_METAL			'M'
#define CHAR_TEX_SAND			'N'
#define CHAR_TEX_FOLIAGE		'O'
#define CHAR_TEX_COMPUTER		'P'
#define CHAR_TEX_REFLECTIVE		'R'
#define CHAR_TEX_SLOSH			'S'
#define CHAR_TEX_TILE			'T'
#define CHAR_TEX_CARDBOARD		'U'
#define CHAR_TEX_VENT			'V'
#define CHAR_TEX_WOOD			'W'
#define CHAR_TEX_GLASS			'Y'
#define CHAR_TEX_WARPSHIELD		'Z'
#pragma endregion

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/tier0/basetypes.h
#pragma region valve_basetypes
inline float bits_to_float( std::uint32_t i )
{
	union Convertor_t
	{
		float fl;
		unsigned long ul;
	} tmp;

	tmp.ul = i;
	return tmp.fl;
}

#define FLOAT32_NAN_BITS		0x7FC00000U
#define FLOAT32_NAN				bits_to_float( FLOAT32_NAN_BITS )
#define VEC_T_NAN				FLOAT32_NAN
#pragma endregion

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/const.h
#pragma region valve_consts
#define MAX_AREA_STATE_BYTES		32
#define MAX_AREA_PORTAL_STATE_BYTES 24

/* baseentity lifestate */
enum e_life_state : int
{
	life_alive = 0,
	life_dying,
	life_dead,
	life_respawnable,
	life_discardbody
};

/* baseentity flags */
enum e_flags : int
{
	fl_onground = (1 << 0),
	fl_ducking = (1 << 1),
	fl_animducking = (1 << 2),
	fl_waterjump = (1 << 3),
	fl_ontrain = (1 << 4),
	fl_inrain = (1 << 5),
	fl_frozen = (1 << 6),
	fl_atcontrols = (1 << 7),
	fl_client = (1 << 8),
	fl_fakeclient = (1 << 9),
	fl_inwater = (1 << 10),
	fl_fly = (1 << 11),
	fl_swim = (1 << 12),
	fl_conveyor = (1 << 13),
	fl_npc = (1 << 14),
	fl_godmode = (1 << 15),
	fl_notarget = (1 << 16),
	fl_aimtarget = (1 << 17),
	fl_partialground = (1 << 18),
	fl_staticprop = (1 << 19),
	fl_graphed = (1 << 20),
	fl_grenade = (1 << 21),
	fl_stepmovement = (1 << 22),
	fl_donttouch = (1 << 23),
	fl_basevelocity = (1 << 24),
	fl_worldbrush = (1 << 25),
	fl_object = (1 << 26),
	fl_killme = (1 << 27),
	fl_onfire = (1 << 28),
	fl_dissolving = (1 << 29),
	fl_transragdoll = (1 << 30),
	fl_unblockable_by_player = (1 << 31)
};

enum e_e_flags : int
{
	efl_killme = (1 << 0),
	efl_dormant = (1 << 1),
	efl_noclip_active = (1 << 2),
	efl_setting_up_bones = (1 << 3),
	efl_keep_on_recreate_entities = (1 << 4),
	efl_dirty_shadowupdate = (1 << 5),
	efl_notify = (1 << 6),
	efl_force_check_transmit = (1 << 7),
	efl_bot_frozen = (1 << 8),
	efl_server_only = (1 << 9),
	efl_no_auto_edict_attach = (1 << 10),
	efl_dirty_abstransform = (1 << 11),
	efl_dirty_absvelocity = (1 << 12),
	efl_dirty_absangvelocity = (1 << 13),
	efl_dirty_surrounding_collision_bounds = (1 << 14),
	efl_dirty_spatial_partition = (1 << 15),
	efl_has_player_child = (1 << 16),
	efl_in_skybox = (1 << 17),
	efl_use_partition_when_not_solid = (1 << 18),
	efl_touching_fluid = (1 << 19),
	efl_is_being_lifted_by_barnacle = (1 << 20),
	efl_no_rotorwash_push = (1 << 21),
	efl_no_think_function = (1 << 22),
	efl_no_game_physics_simulation = (1 << 23),
	efl_check_untouch = (1 << 24),
	efl_dontblocklos = (1 << 25),
	efl_dontwalkon = (1 << 26),
	efl_no_dissolve = (1 << 27),
	efl_no_megaphyscannon_ragdoll = (1 << 28),
	efl_no_water_velocity_change = (1 << 29),
	efl_no_physcannon_interaction = (1 << 30),
	efl_no_damage_forces = (1 << 31)
};

enum e_move_type : int
{
	movetype_none = 0,
	movetype_isometric,
	movetype_walk,
	movetype_step,
	movetype_fly,
	movetype_flygravity,
	movetype_vphysics,
	movetype_push,
	movetype_noclip,
	movetype_ladder,
	movetype_observer,
	movetype_custom,
	movetype_last = movetype_custom,
	movetype_max_bits = 4
};

enum e_solid_type : int
{
	solid_none = 0,	// no solid model
	solid_bsp,		// a bsp tree
	solid_bbox,		// an aabb
	solid_obb,		// an obb (not implemented yet)
	solid_obb_yaw,	// an obb, constrained so that it can only yaw
	solid_custom,	// always call into the entity for tests
	solid_vphysics,	// solid vphysics object, get vcollide from the model and collide with that
	solid_last
};

enum e_solid_flags : unsigned int
{
	fsolid_customraytest = 0x0001,	// ignore solid type + always call into the entity for ray tests
	fsolid_customboxtest = 0x0002,	// ignore solid type + always call into the entity for swept box tests
	fsolid_not_solid = 0x0004,	// are we currently not solid?
	fsolid_trigger = 0x0008,	// this is something may be collideable but fires touch functions
	fsolid_not_standable = 0x0010,	// you can't stand on this
	fsolid_volume_contents = 0x0020,	// contains volumetric contents (like water)
	fsolid_force_world_aligned = 0x0040,	// forces the collision rep to be world-aligned even if it's solid_bsp or solid_vphysics
	fsolid_use_trigger_bounds = 0x0080,	// uses a special trigger bounds separate from the normal obb
	fsolid_root_parent_aligned = 0x0100,	// collisions are defined in root parent's local coordinate space
	fsolid_trigger_touch_debris = 0x0200,	// this trigger will touch debris objects
	fsolid_max_bits = 10
};

enum e_effect_flags : unsigned int
{
	ef_bonemerge = 0x001,	// performs bone merge on client side
	ef_brightlight = 0x002,	// dlight centered at entity origin
	ef_dimlight = 0x004,	// player flashlight
	ef_nointerp = 0x008,	// don't interpolate the next frame
	ef_noshadow = 0x010,	// don't cast no shadow
	ef_nodraw = 0x020,	// don't draw entity
	ef_noreceiveshadow = 0x040,	// don't receive no shadow
	ef_bonemerge_fastcull = 0x080,	// for use with ef_bonemerge. if this is set, then it places this ent's origin at its
	// parent and uses the parent's bbox + the max extents of the aiment.
	// otherwise, it sets up the parent's bones every frame to figure out where to place
	// the aiment, which is inefficient because it'll setup the parent's bones even if
	// the parent is not in the pvs.
	ef_item_blink = 0x100,	// blink an item so that the user notices it.
	ef_parent_animates = 0x200,	// always assume that the parent entity is animating
	ef_marked_for_fast_reflection = 0x400,	// marks an entity for reflection rendering when using $reflectonlymarkedentities material variable
	ef_noshadowdepth = 0x800,	// indicates this entity does not render into any shadow depthmap
	ef_shadowdepth_nocache = 0x1000,	// indicates this entity cannot be cached in shadow depthmap and should render every frame
	ef_noflashlight = 0x2000,
	ef_nocsm = 0x4000,	// indicates this entity does not render into the cascade shadow depthmap
	ef_max_bits = 15
};

enum e_collision_group : int
{
	collision_group_none = 0,
	collision_group_debris,						// collides with nothing but world and static stuff
	collision_group_debris_trigger,				// same as debris, but hits triggers
	collision_group_interactive_debris,			// collides with everything except other interactive debris or debris
	collision_group_interactive,				// collides with everything except interactive debris or debris
	collision_group_player,
	collision_group_breakable_glass,
	collision_group_vehicle,
	collision_group_player_movement,			// for hl2, same as collision_group_player, for / tf2, this filters out other players and cbaseobjects
	collision_group_npc,						// generic npc group
	collision_group_in_vehicle,					// for any entity inside a vehicle
	collision_group_weapon,						// for any weapons that need collision detection
	collision_group_vehicle_clip,				// vehicle clip brush to restrict vehicle movement
	collision_group_projectile,					// projectiles!
	collision_group_door_blocker,				// blocks entities not permitted to get near moving doors
	collision_group_passable_door,				// doors that the player shouldn't collide with
	collision_group_dissolving,					// things that are dissolving are in this group
	collision_group_pushaway,					// nonsolid on client and server, pushaway in player code
	collision_group_npc_actor,					// used so npcs in scripts ignore the player.
	collision_group_npc_scripted,				// used for npcs in scripts that should not collide with each other
	collision_group_pz_clip,
	collision_group_debris_block_projectile,	// only collides with bullets
	last_shared_collision_group
};
#pragma endregion

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/shared/shareddefs.h
#pragma region valve_shareddefs
enum e_multiplayer_physics_mode : int
{
	physics_multiplayer_autodetect = 0,	// use multiplayer physics mode as defined in model prop data
	physics_multiplayer_solid = 1,	// solid, pushes player away
	physics_multiplayer_non_solid = 2,	// nonsolid, but pushed by player
	physics_multiplayer_clientside = 3	// clientside only, nonsolid
};

/* hitgroup standarts */
enum e_hit_group_index : int
{
	hitgroup_generic = 0,
	hitgroup_head,
	hitgroup_chest,
	hitgroup_stomach,
	hitgroup_leftarm,
	hitgroup_rightarm,
	hitgroup_leftleg,
	hitgroup_rightleg,
	hitgroup_neck,
	hitgroup_gear = 10
};

/* settings for m_takedamage */
enum e_damage_type : int
{
	damage_no = 0,
	damage_events_only,
	damage_yes,
	damage_aim
};

/* spectator movement modes */
enum e_observer_mode : int
{
	obs_mode_none = 0,
	obs_mode_deathcam,
	obs_mode_freezecam,
	obs_mode_fixed,
	obs_mode_in_eye,
	obs_mode_chase,
	obs_mode_roaming
};
#pragma endregion

// @note: see "sv_dump_class_info" command
enum class e_class_index : int
{
	caibasenpc = 0,
	cak47,
	cbaseanimating,
	cbaseanimatingoverlay,
	cbaseattributableitem,
	cbasebutton,
	cbasecombatcharacter,
	cbasecombatweapon,
	cbasecsgrenade,
	cbasecsgrenadeprojectile,
	cbasedoor,
	cbaseentity,
	cbaseflex,
	cbasegrenade,
	cbaseparticleentity,
	cbaseplayer,
	cbasepropdoor,
	cbaseteamobjectiveresource,
	cbasetementity,
	cbasetoggle,
	cbasetrigger,
	cbaseviewmodel,
	cbasevphysicstrigger,
	cbaseweaponworldmodel,
	cbeam,
	cbeamspotlight,
	cbonefollower,
	cbrc4target,
	cbreachcharge,
	cbreachchargeprojectile,
	cbreakableprop,
	cbreakablesurface,
	cbumpmine,
	cbumpmineprojectile,
	cc4,
	ccascadelight,
	cchicken,
	ccolorcorrection,
	ccolorcorrectionvolume,
	ccsgamerulesproxy,
	ccsplayer,
	ccsplayerresource,
	ccsragdoll,
	ccsteam,
	cdangerzone,
	cdangerzonecontroller,
	cdeagle,
	cdecoygrenade,
	cdecoyprojectile,
	cdrone,
	cdronegun,
	cdynamiclight,
	cdynamicprop,
	ceconentity,
	ceconwearable,
	cembers,
	centitydissolve,
	centityflame,
	centityfreezing,
	centityparticletrail,
	cenvambientlight,
	cenvdetailcontroller,
	cenvdofcontroller,
	cenvgascanister,
	cenvparticlescript,
	cenvprojectedtexture,
	cenvquadraticbeam,
	cenvscreeneffect,
	cenvscreenoverlay,
	cenvtonemapcontroller,
	cenvwind,
	cfeplayerdecal,
	cfirecrackerblast,
	cfiresmoke,
	cfiretrail,
	cfish,
	cfists,
	cflashbang,
	cfogcontroller,
	cfootstepcontrol,
	cfunc_dust,
	cfunc_lod,
	cfuncareaportalwindow,
	cfuncbrush,
	cfuncconveyor,
	cfuncladder,
	cfuncmonitor,
	cfuncmovelinear,
	cfuncoccluder,
	cfuncreflectiveglass,
	cfuncrotating,
	cfuncsmokevolume,
	cfunctracktrain,
	cgamerulesproxy,
	cgrassburn,
	chandletest,
	chegrenade,
	chostage,
	chostagecarriableprop,
	cincendiarygrenade,
	cinferno,
	cinfoladderdismount,
	cinfomapregion,
	cinfooverlayaccessor,
	citem_healthshot,
	citemcash,
	citemdogtags,
	cknife,
	cknifegg,
	clightglow,
	cmapvetopickcontroller,
	cmaterialmodifycontrol,
	cmelee,
	cmolotovgrenade,
	cmolotovprojectile,
	cmoviedisplay,
	cparadropchopper,
	cparticlefire,
	cparticleperformancemonitor,
	cparticlesystem,
	cphysbox,
	cphysboxmultiplayer,
	cphysicsprop,
	cphysicspropmultiplayer,
	cphysmagnet,
	cphyspropammobox,
	cphysproplootcrate,
	cphyspropradarjammer,
	cphyspropweaponupgrade,
	cplantedc4,
	cplasma,
	cplayerping,
	cplayerresource,
	cpointcamera,
	cpointcommentarynode,
	cpointworldtext,
	cposecontroller,
	cpostprocesscontroller,
	cprecipitation,
	cprecipitationblocker,
	cpredictedviewmodel,
	cprop_hallucination,
	cpropcounter,
	cpropdoorrotating,
	cpropjeep,
	cpropvehicledriveable,
	cragdollmanager,
	cragdollprop,
	cragdollpropattached,
	cropekeyframe,
	cscar17,
	csceneentity,
	csensorgrenade,
	csensorgrenadeprojectile,
	cshadowcontrol,
	cslideshowdisplay,
	csmokegrenade,
	csmokegrenadeprojectile,
	csmokestack,
	csnowball,
	csnowballpile,
	csnowballprojectile,
	cspatialentity,
	cspotlightend,
	csprite,
	cspriteoriented,
	cspritetrail,
	cstatueprop,
	csteamjet,
	csun,
	csunlightshadowcontrol,
	csurvivalspawnchopper,
	ctablet,
	cteam,
	cteamplayroundbasedrulesproxy,
	ctearmorricochet,
	ctebasebeam,
	ctebeamentpoint,
	ctebeaments,
	ctebeamfollow,
	ctebeamlaser,
	ctebeampoints,
	ctebeamring,
	ctebeamringpoint,
	ctebeamspline,
	ctebloodsprite,
	ctebloodstream,
	ctebreakmodel,
	ctebspdecal,
	ctebubbles,
	ctebubbletrail,
	cteclientprojectile,
	ctedecal,
	ctedust,
	ctedynamiclight,
	cteeffectdispatch,
	cteenergysplash,
	cteexplosion,
	ctefirebullets,
	ctefizz,
	ctefootprintdecal,
	ctefoundryhelpers,
	ctegaussexplosion,
	cteglowsprite,
	cteimpact,
	ctekillplayerattachments,
	ctelargefunnel,
	ctemetalsparks,
	ctemuzzleflash,
	cteparticlesystem,
	ctephysicsprop,
	cteplantbomb,
	cteplayeranimevent,
	cteplayerdecal,
	cteprojecteddecal,
	cteradioicon,
	cteshattersurface,
	cteshowline,
	ctesla,
	ctesmoke,
	ctesparks,
	ctesprite,
	ctespritespray,
	ctest_proxytoggle_networkable,
	ctesttraceline,
	cteworlddecal,
	ctriggerplayermovement,
	ctriggersoundoperator,
	cvguiscreen,
	cvotecontroller,
	cwaterbullet,
	cwaterlodcontrol,
	cweaponaug,
	cweaponawp,
	cweaponbaseitem,
	cweaponbizon,
	cweaponcsbase,
	cweaponcsbasegun,
	cweaponcycler,
	cweaponelite,
	cweaponfamas,
	cweaponfiveseven,
	cweapong3sg1,
	cweapongalil,
	cweapongalilar,
	cweaponglock,
	cweaponhkp2000,
	cweaponm249,
	cweaponm3,
	cweaponm4a1,
	cweaponmac10,
	cweaponmag7,
	cweaponmp5navy,
	cweaponmp7,
	cweaponmp9,
	cweaponnegev,
	cweaponnova,
	cweaponp228,
	cweaponp250,
	cweaponp90,
	cweaponsawedoff,
	cweaponscar20,
	cweaponscout,
	cweaponsg550,
	cweaponsg552,
	cweaponsg556,
	cweaponshield,
	cweaponssg08,
	cweapontaser,
	cweapontec9,
	cweapontmp,
	cweaponump45,
	cweaponusp,
	cweaponxm1014,
	cweaponzonerepulsor,
	cworld,
	cworldvguitext,
	dusttrail,
	movieexplosion,
	particlesmokegrenade,
	rockettrail,
	smoketrail,
	sporeexplosion,
	sporetrail,
};

// @credits: from protobufs (cstrike15_usermessages.proto)
enum e_user_messages : int
{
	cs_um_vguimenu = 1,
	cs_um_geiger = 2,
	cs_um_train = 3,
	cs_um_hudtext = 4,
	cs_um_saytext = 5,
	cs_um_saytext2 = 6,
	cs_um_textmsg = 7,
	cs_um_hudmsg = 8,
	cs_um_resethud = 9,
	cs_um_gametitle = 10,
	cs_um_shake = 12,
	cs_um_fade = 13,
	cs_um_rumble = 14,
	cs_um_closecaption = 15,
	cs_um_closecaptiondirect = 16,
	cs_um_sendaudio = 17,
	cs_um_rawaudio = 18,
	cs_um_voicemask = 19,
	cs_um_requeststate = 20,
	cs_um_damage = 21,
	cs_um_radiotext = 22,
	cs_um_hinttext = 23,
	cs_um_keyhinttext = 24,
	cs_um_processspottedentityupdate = 25,
	cs_um_reloadeffect = 26,
	cs_um_adjustmoney = 27,
	cs_um_updateteammoney = 28,
	cs_um_stopspectatormode = 29,
	cs_um_killcam = 30,
	cs_um_desiredtimescale = 31,
	cs_um_currenttimescale = 32,
	cs_um_achievementevent = 33,
	cs_um_matchendconditions = 34,
	cs_um_disconnecttolobby = 35,
	cs_um_playerstatsupdate = 36,
	cs_um_displayinventory = 37,
	cs_um_warmuphasended = 38,
	cs_um_clientinfo = 39,
	cs_um_xrankget = 40,
	cs_um_xrankupd = 41,
	cs_um_callvotefailed = 45,
	cs_um_votestart = 46,
	cs_um_votepass = 47,
	cs_um_votefailed = 48,
	cs_um_votesetup = 49,
	cs_um_serverrankrevealall = 50,
	cs_um_sendlastkillerdamagetoclient = 51,
	cs_um_serverrankupdate = 52,
	cs_um_itempickup = 53,
	cs_um_showmenu = 54,
	cs_um_bartime = 55,
	cs_um_ammodenied = 56,
	cs_um_markachievement = 57,
	cs_um_matchstatsupdate = 58,
	cs_um_itemdrop = 59,
	cs_um_glowpropturnoff = 60,
	cs_um_sendplayeritemdrops = 61,
	cs_um_roundbackupfilenames = 62,
	cs_um_sendplayeritemfound = 63,
	cs_um_reporthit = 64,
	cs_um_xpupdate = 65,
	cs_um_questprogress = 66,
	cs_um_scoreleaderboarddata = 67,
	cs_um_playerdecaldigitalsignature = 68
};

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/model_types.h
enum e_draw_model_flags : unsigned int
{
	studio_none = 0x00000000,
	studio_render = 0x00000001,
	studio_viewxformattachments = 0x00000002,
	studio_drawtranslucentsubmodels = 0x00000004,
	studio_twopass = 0x00000008,
	studio_static_lighting = 0x00000010,
	studio_wireframe = 0x00000020,
	studio_item_blink = 0x00000040,
	studio_noshadows = 0x00000080,
	studio_wireframe_vcollide = 0x00000100,
	studio_nolighting_or_cubemap = 0x00000200,
	studio_skip_flexes = 0x00000400,
	studio_donotmodifystencilstate = 0x00000800,
	studio_skip_decals = 0x10000000,
	studio_shadowtexture = 0x20000000,
	studio_shadowdepthtexture = 0x40000000,
	studio_transparency = 0x80000000
};

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/shaderapi/ishadershadow.h
enum e_shader_stencil_func : int
{
	shader_stencilfunc_never = 0,
	shader_stencilfunc_less,
	shader_stencilfunc_equal,
	shader_stencilfunc_lequal,
	shader_stencilfunc_greater,
	shader_stencilfunc_notequal,
	shader_stencilfunc_gequal,
	shader_stencilfunc_always
};

enum e_shader_stencil_op : int
{
	shader_stencilop_keep = 0,
	shader_stencilop_zero,
	shader_stencilop_set_to_reference,
	shader_stencilop_increment_clamp,
	shader_stencilop_decrement_clamp,
	shader_stencilop_invert,
	shader_stencilop_increment_wrap,
	shader_stencilop_decrement_wrap,
};

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/string_t.h
struct string_t
{
public:
	bool operator!( ) const { return (value == nullptr); }
	bool operator==( const string_t& rhs ) const { return (value == rhs.value); }
	bool operator!=( const string_t& rhs ) const { return (value != rhs.value); }
	bool operator<( const string_t& rhs ) const { return (reinterpret_cast<void*>(const_cast<char*>(value)) < reinterpret_cast<void*>(const_cast<char*>(rhs.value))); }

	const char* c_str( ) const { return (value) ? value : ""; }
protected:
	const char* value;
};

enum e_property_data_interactions : int
{
	propinter_physgun_world_stick,		// "onworldimpact"		"stick"
	propinter_physgun_first_break,		// "onfirstimpact"		"break"
	propinter_physgun_first_paint,		// "onfirstimpact"		"paintsplat"
	propinter_physgun_first_impale,		// "onfirstimpact"		"impale"
	propinter_physgun_launch_spin_none,	// "onlaunch"			"spin_none"
	propinter_physgun_launch_spin_z,	// "onlaunch"			"spin_zaxis"
	propinter_physgun_break_explode,	// "onbreak"			"explode_fire"
	propinter_physgun_break_explode_ice,// "onbreak"			"explode_ice"
	propinter_physgun_damage_none,		// "damage"				"none"
	propinter_fire_flammable,			// "flammable"			"yes"
	propinter_fire_explosive_resist,	// "explosive_resist"	"yes"
	propinter_fire_ignite_halfhealth,	// "ignite"				"halfhealth"
	propinter_physgun_create_flare,		// "onpickup"			"create_flare"
	propinter_physgun_allow_overhead,	// "allow_overhead"		"yes"
	propinter_world_bloodsplat,			// "onworldimpact",		"bloodsplat"
	propinter_physgun_notify_children,	// "onfirstimpact" - cause attached flechettes to explode
	propinter_melee_immune,				// "melee_immune"		"yes"
	propinter_num_interactions			// if we get more than 32 of these, we'll need a different system
};

enum e_multiplayer_break : int
{
	multiplayer_break_default,
	multiplayer_break_serverside,
	multiplayer_break_clientside,
	multiplayer_break_both
};

enum e_hide_hud : int
{
	hide_weapon_selection = (1 << 0),
	hide_flash_light = (1 << 1),
	hide_all = (1 << 2),
	hide_health = (1 << 3),
	hide_player_death = (1 << 4),
	hide_need_suit = (1 << 5),	//Hide when the local player doesn't have the HEV suit
	hide_misc_status = (1 << 6), // Hide miscellaneous status elements (trains, pickup history, death notices, etc)
	hide_chat = (1 << 7),
	hide_crosshair = (1 << 8),
	hide_radar = (1 << 12)
};

class i_breakable_with_prop_data
{
public:
	// Damage modifiers
	virtual void		set_dmg_mod_bullet( float flDmgMod ) = 0;
	virtual void		set_dmg_mod_club( float flDmgMod ) = 0;
	virtual void		set_dmg_mod_explosive( float flDmgMod ) = 0;
	virtual float		get_dmg_mod_bullet( ) = 0;
	virtual float		get_dmg_mod_club( ) = 0;
	virtual float		get_dmg_mod_explosive( ) = 0;
	virtual float		get_dmg_mod_fire( ) = 0;

	// Explosive
	virtual void		set_explosive_radius( float flRadius ) = 0;
	virtual void		set_explosive_damage( float flDamage ) = 0;
	virtual float		get_explosive_radius( ) = 0;
	virtual float		get_explosive_damage( ) = 0;

	// Physics damage tables
	virtual void		set_physics_damage_table( string_t iszTableName ) = 0;
	virtual string_t	get_physics_damage_table( ) = 0;

	// Breakable chunks
	virtual void		set_breakable_model( string_t iszModel ) = 0;
	virtual string_t 	get_breakable_model( ) = 0;
	virtual void		set_breakable_skin( int iSkin ) = 0;
	virtual int			get_breakable_skin( ) = 0;
	virtual void		set_breakable_count( int iCount ) = 0;
	virtual int			get_breakable_count( ) = 0;
	virtual void		set_max_breakable_size( int iSize ) = 0;
	virtual int			get_max_breakable_size( ) = 0;

	// LOS blocking
	virtual void		set_prop_data_blocks_los( bool bBlocksLOS ) = 0;
	virtual void		set_prop_data_is_ai_walkable( bool bBlocksLOS ) = 0;

	// Interactions
	virtual void		set_interaction( e_property_data_interactions Interaction ) = 0;
	virtual bool		has_interaction( e_property_data_interactions Interaction ) = 0;

	// Multiplayer physics mode
	virtual void		set_physics_mode( int iMode ) = 0;
	virtual int			get_physics_mode( ) = 0;

	// Multiplayer breakable spawn behavior
	virtual void		set_multiplayer_break_mode( e_multiplayer_break mode ) = 0;
	virtual e_multiplayer_break get_multiplayer_break_mode( ) const = 0;

	// Used for debugging
	virtual void		set_base_prop_data( string_t iszBase ) = 0;
	virtual string_t	get_base_prop_data( ) = 0;
};

class i_multiplayer_physics
{
public:
	virtual int		get_multiplayer_physics_mode( ) = 0;
	virtual float	get_mass( ) = 0;
	virtual bool	is_asleep( ) = 0;
};
