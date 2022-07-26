#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

// For removal of the inside smoke see src/core/hooks/functions/render_smoke_overlay.cpp

void enable_wiresmoke()  {
	constexpr std::array disable_mats {
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		"particle/vistasmokev1/vistasmokev1_fire"
	};

	for (const auto mat : disable_mats) {
		const auto material = interfaces::material_system->find_material(mat);
		material->set_material_var_flag(material_var_no_draw, true);	// Don't draw
	}

	const auto material = interfaces::material_system->find_material("particle/vistasmokev1/vistasmokev1_smokegrenade");
	material->set_material_var_flag(material_var_no_draw, false);		// Draw
	material->set_material_var_flag(material_var_wireframe, true);		// Wireframe smoke
}

void disable_wiresmoke() {
	constexpr std::array mats {
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade"
	};

	for (const auto mat : mats) {
		const auto material = interfaces::material_system->find_material(mat);
		material->set_material_var_flag(material_var_no_draw, false);		// Draw
		material->set_material_var_flag(material_var_wireframe, false);		// Disable wireframe
	}
}

// Used in frame_stage_notify - FRAME_RENDER_START
void visuals::nosmoke(client_frame_stage_t frame_stage) {
	if (frame_stage != FRAME_RENDER_START && frame_stage != FRAME_RENDER_END) return;

	if (variables::misc_visuals::wireframe_smoke)
		enable_wiresmoke();
	else if (!variables::misc_visuals::wireframe_smoke)
		disable_wiresmoke();
}