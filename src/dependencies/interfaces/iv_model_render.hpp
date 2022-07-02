#pragma once
#include "source-sdk/structs/materials.hpp"

struct draw_model_state_t {
public:
	studio_hdr_t		*studio_hdr;
	studiohwdata_t		*studio_hw_data;
	i_client_renderable	*renderable;
	const matrix3x4_t	*model_to_world;
	studio_decal_handle_t	decals;
	int			drawflags;
	int			lod;
};

class iv_model_render {
public:
	//virtual void override_material(i_material*);
	//virtual bool is_forced();
	void override_material(i_material *material) {
		using original_fn = void( __thiscall*)(iv_model_render*, i_material*, int, int);
		return (*(original_fn**)this)[1](this, material, 0, 0 );
	}
	bool is_forced() {
		using original_fn = bool(__thiscall*)(iv_model_render*);
		return (*(original_fn**)this)[2](this);
	}
};
