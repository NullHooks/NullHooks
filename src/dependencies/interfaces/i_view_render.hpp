#pragma once

#include "dependencies/interfaces/i_material_system.hpp"
#include "source-sdk/classes/view_setup.h"
#include "source-sdk/classes/c_usercmd.hpp"

// https://github.com/rollraw/qo0-base/blob/master/base/sdk/interfaces/iviewrender.h

struct view_plane_t {
	view_plane_t() = default;

	view_plane_t(const vec3_t& vecNormal, const float flDistance) :
		vecNormal(vecNormal), flDistance(flDistance) { }

	vec3_t vecNormal;
	float flDistance;
};

struct vrect_t;
class i_view_render {
public:
	virtual void                Init() = 0;
	virtual void                LevelInit() = 0;
	virtual void                LevelShutdown() = 0;
	virtual void                Shutdown() = 0;
	virtual void                OnRenderStart() = 0;
};