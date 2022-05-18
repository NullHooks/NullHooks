#include "../features.hpp"

const char* materials[13] = {	// Probaly not the best way
	"vgui/screens/transparent",
	"debug/debugambientcube",
	"debug/debugdrawflat",
	"models/player/ct_fbi/ct_fbi_glass",
	"models/inventory_items/cologne_prediction/cologne_prediction_glass",
	"models/inventory_items/trophy_majors/crystal_clear",
	"models/inventory_items/trophy_majors/gold",
	"models/gibs/glass/glass",
	"models/inventory_items/trophy_majors/gloss",
	"models/inventory_items/wildfire_gold/wildfire_gold_detail",
	"models/inventory_items/trophy_majors/crystal_blue",
	"models/inventory_items/trophy_majors/velvet",
	"models/inventory_items/dogtags/dogtags_outline"
	//"models/props_interiors/tvebtest"	// Kinda broky
};

void override_material(bool ignorez, bool wireframe, const color& rgba, const char* mat_name) {
	auto material = interfaces::material_system->find_material(mat_name, TEXTURE_GROUP_MODEL);
	material->set_material_var_flag(material_var_ignorez, ignorez);
	material->set_material_var_flag(material_var_wireframe, wireframe);
	material->alpha_modulate(rgba.a / 255.f);
	material->color_modulate(rgba.r / 255.f, rgba.g / 255.f, rgba.b / 255.f);
	interfaces::model_render->override_material(material);
}

void visuals::chams::draw_chams(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix) {
	if (!csgo::local_player) return;
	if (!(variables::player_chams_bool
		|| variables::vm_weapon_chams_bool
		|| variables::vm_arm_chams_bool
		|| variables::vm_sleeve_chams_bool)) return;

	const auto mdl = info.model;
	if (!mdl) return;

	// Players
	if (strstr(mdl->name, "models/player") && variables::player_chams_bool) {
		player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));
		if (!player || !player->is_alive() || player->dormant()) return;

		if (player->has_gun_game_immunity()) {
			override_material(false, false, color(255, 255, 255, 100), materials[variables::player_chams_mat_id]);
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
					override_material(true, false, variables::colors::chams_inv_enemy_c, materials[variables::player_chams_mat_id]);		// Not visible - Enemy
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
				}
				override_material(false, false, variables::colors::chams_vis_enemy_c, materials[variables::player_chams_mat_id]);			// Visible - Enemy
				hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			} else if (variables::showteamesp_bool) {
				if (!variables::only_visible_chams_bool) {
					override_material(true, false, variables::colors::chams_inv_friend_c, materials[variables::player_chams_mat_id]);		// Not visible - Friendly
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
				}
				override_material(false, false, variables::colors::chams_vis_friend_c, materials[variables::player_chams_mat_id]);			// Visible - Friendly
				hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			}
		}
	}

	// Viewmodel
	if (strstr(mdl->name, "sleeve")) {
		if (variables::vm_sleeve_chams_bool) {
			override_material(false, false, variables::colors::chams_sleeve_c, materials[variables::sleeve_chams_mat_id]);
			hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
		}
	} else if (strstr(mdl->name + 17, "arms")) {
		if (variables::vm_arm_chams_bool) {
			override_material(false, false, variables::colors::chams_arms_c, materials[variables::arm_chams_mat_id]);
			hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
		}
	} else if (strstr(mdl->name, "models/weapons/v")) {
		if (variables::vm_weapon_chams_bool && !csgo::local_player->is_scoped()) {
			override_material(false, false, variables::colors::chams_weapon_c, materials[variables::weapon_chams_mat_id]);
			hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
		}
	}
}
