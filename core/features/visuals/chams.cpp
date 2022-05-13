#include "../features.hpp"

void override_material(bool ignorez, bool wireframe, const color& rgba) {
	/*
	 * debug/debugambientcube
	 * debug/debugdrawflat
	 * 
	 * models/player/ct_fbi/ct_fbi_glass - platinum
     * models/inventory_items/cologne_prediction/cologne_prediction_glass - glass
     * models/inventory_items/trophy_majors/crystal_clear - crystal
     * models/inventory_items/trophy_majors/gold - gold
     * models/gibs/glass/glass - dark chrome
     * models/inventory_items/trophy_majors/gloss - plastic/glass
     * vgui/achievements/glow - glow
	 * 
	 * models/inventory_items/wildfire_gold/wildfire_gold_detail
     * models/inventory_items/trophy_majors/crystal_blue
	 * models/inventory_items/trophy_majors/velvet
	 * 
	 * models/inventory_items/dogtags/dogtags_outline
	 * models/props_interiors/tvebtest
	 */
	auto material = interfaces::material_system->find_material("debug/debugambientcube", TEXTURE_GROUP_MODEL);
	material->set_material_var_flag(material_var_ignorez, ignorez);
	material->set_material_var_flag(material_var_wireframe, wireframe);
	material->alpha_modulate(rgba.a / 255.f);
	material->color_modulate(rgba.r / 255.f, rgba.g / 255.f, rgba.b / 255.f);
	interfaces::model_render->override_material(material);
}

void visuals::chams::draw_chams(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix) {
	if (!csgo::local_player) return;
	if (!variables::enable_chams_bool) return;

	const auto mdl = info.model;
	if (!mdl) return;

	bool is_player = strstr(mdl->name, "models/player") != nullptr;
	if (!is_player) return;

	player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));
	if (!player || !player->is_alive() || player->dormant()) return;

	if (player->has_gun_game_immunity()) {
		override_material(false, false, color(255, 255, 255, 100));
		hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
	} else {
		if (player->index() == csgo::local_player->index()) {
			/*
			// Remove until thirdperson is implemented
			override_material(false, false, color(255, 100, 255, csgo::local_player->is_scoped() ? 30 : 255));
			hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			*/
			return;
		}

		if (player->team() != csgo::local_player->team()) {
			/*
			// Backtrack chams
			if (variables::backtrack_chams && records[player->index()].size() > 0) {
				for (uint32_t i = 0; i < records[player->index()].size(); i++) {
					if (!backtrack.valid_tick(records[player->index()][i].simulation_time, 0.2f) || records[player->index()][i].matrix == nullptr)
						continue;
					override_material(false, false, color(255 - (i * (255 / records[player->index()].size())), i * (255 / records[player->index()].size()), 255, 30));
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, records[player->index()][i].matrix);
				}
			}
			*/
			if (!variables::only_visible_chams_bool) {
				override_material(true, false, color(150, 15, 15));		// Not visible - Enemy
				hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			}
			override_material(false, false, color(230, 20, 70));		// Visible - Enemy
			hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
		} else if (variables::showteamesp_bool) {
			if (!variables::only_visible_chams_bool) {
				override_material(true, false, color(0, 75, 255));		// Not visible - Friendly
				hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			}
			override_material(false, false, color(0, 150, 255));		// Visible - Friendly
			hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
		}
	}
}