#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

// Works more or less but only if you are not IN the smoke :/

void visuals::misc::nosmoke(client_frame_stage_t frame_stage) {
	if (frame_stage != FRAME_RENDER_START && frame_stage != FRAME_RENDER_END) return;

	constexpr std::array smokeMaterials{
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade"
	};

	for (const auto mat : smokeMaterials) {
		const auto material = interfaces::material_system->find_material(mat);
		material->set_material_var_flag(material_var_no_draw, frame_stage == FRAME_RENDER_START && false);		// No smoke
		material->set_material_var_flag(material_var_wireframe, frame_stage == FRAME_RENDER_START && variables::misc_visuals::wireframe_smoke);		// Wireframe smoke
	}
}