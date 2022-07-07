#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/features/misc/backtrack.hpp"

 std::vector<const char*> materials = {
	"vgui/screens/transparent",														// "Transparent"
	"debug/debugambientcube",														// "Textured"
	"debug/debugdrawflat",															// "Flat"
	"models/inventory_items/cologne_prediction/cologne_prediction_glass",			// "Ghost"
	"models/inventory_items/trophy_majors/gold",									// "Gold 1"
	"models/inventory_items/wildfire_gold/wildfire_gold_detail",					// "Gold 2"
	"models/inventory_items/trophy_majors/crystal_blue",							// "Amethyst"
	"models/inventory_items/trophy_majors/crystal_clear",							// "Silver"
	"models/player/ct_fbi/ct_fbi_glass",											// "Steel"
	"models/gibs/glass/glass",														// "Dark steel"
	"models/inventory_items/trophy_majors/gloss",									// "Plastic"
	"dev/glow_rim3d",																// "Red glow"
	"models/inventory_items/dreamhack_trophies/dreamhack_star_blur",				// "Effect 1"
	"models/inventory_items/dreamhack_trophies/dreamhack_pickem_glow_gold",			// "Effect 2"
	"models/inventory_items/dogtags/dogtags_lightray",								// "Effect 3"
	"models/inventory_items/dogtags/dogtags_outline",								// "Animated blink"
	"models/inventory_items/music_kit/darude_01/mp3_detail"							// "Animated stripes"
};

void override_material(bool ignorez, bool wireframe, const color& rgba, const char* mat_name) {
	auto material = interfaces::material_system->find_material(mat_name, TEXTURE_GROUP_MODEL);
	
	interfaces::render_view->set_blend(1.f);
	material->set_material_var_flag(material_var_ignorez, ignorez);
	material->set_material_var_flag(material_var_wireframe, wireframe);
	material->alpha_modulate(rgba.a / 255.f);
	material->color_modulate(rgba.r / 255.f, rgba.g / 255.f, rgba.b / 255.f);
	interfaces::model_render->override_material(material);
}

void visuals::chams::draw_chams(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix) {
	if (!csgo::local_player) return;
	if (!(variables::chams::player_chams
		|| variables::chams::localplayer_chams
		|| variables::chams::vm_weapon_chams
		|| variables::chams::vm_arm_chams
		|| variables::chams::vm_sleeve_chams)) return;

	const auto mdl = info.model;
	if (!mdl) return;

	// Players
	if (strstr(mdl->name, "models/player") && (variables::chams::player_chams || variables::chams::localplayer_chams)) {
		const char* player_material = (variables::chams::player_chams_mat_id.idx < materials.size()) ? materials.at(variables::chams::player_chams_mat_id.idx) : materials.at(materials.size() - 1);

		player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));
		if (!player || !player->is_alive() || player->dormant()) return;

		if (player->has_gun_game_immunity()) {
			if (variables::chams::draw_chams_on_top) hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, false, color(255, 255, 255, 100), player_material);
			hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
		} else {
			// Backtrack chams
			if (variables::misc::backtrack && backtrack::records[player->index()].size() > 0 && (player->team() != csgo::local_player->team() || variables::misc::backtrack_team)) {
				for (uint32_t i = 0; i < backtrack::records[player->index()].size(); i++) {
					if (!backtrack::valid_tick(backtrack::records[player->index()][i].simulation_time, 0.2f)
						|| backtrack::records[player->index()][i].matrix == nullptr)
						continue;

					override_material(false, false, color(255 - (i * (255 / backtrack::records[player->index()].size())), i * (255 / backtrack::records[player->index()].size()), 255, 30), materials[1]);
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, backtrack::records[player->index()][i].matrix);
				}
			}

			// For thirdperson
			if (player == csgo::local_player && variables::misc::thirdperson && variables::chams::localplayer_chams) {
				const float localplayer_col_a = (csgo::local_player->is_scoped()) ? 30 : variables::colors::chams_localplayer.col.a;
				override_material(false, variables::chams::wireframe_chams, variables::colors::chams_localplayer.col.get_custom_alpha(localplayer_col_a), materials[variables::chams::localplayer_chams_mat_id.idx]);
				hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
				return;
			// Enemies
			} else if (variables::chams::player_chams && player->team() != csgo::local_player->team()) {
				if (variables::chams::draw_chams_on_top) hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
				if (!variables::chams::only_visible_chams) {
					override_material(true, variables::chams::wireframe_chams, variables::colors::chams_inv_enemy_c, player_material);		// Not visible - Enemy
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
				}
				override_material(false, variables::chams::wireframe_chams, variables::colors::chams_vis_enemy_c, player_material);			// Visible - Enemy
				hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			// Friends
			} else if (variables::chams::player_chams && variables::player_visuals::showteamesp && player != csgo::local_player) {
				if (variables::chams::draw_chams_on_top) hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
				if (!variables::chams::only_visible_chams) {
					override_material(true, variables::chams::wireframe_chams, variables::colors::chams_inv_friend_c, player_material);		// Not visible - Friendly
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
				}
				override_material(false, variables::chams::wireframe_chams, variables::colors::chams_vis_friend_c, player_material);		// Visible - Friendly
				hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			}
		}
	}

	// Viewmodel
	if (strstr(mdl->name, "sleeve")) {
		if (variables::chams::vm_sleeve_chams) {
			if (variables::chams::draw_chams_on_top) hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, variables::chams::wireframe_chams, variables::colors::chams_sleeve_c, materials.at(variables::chams::sleeve_chams_mat_id.idx));
			hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
		}
	} else if (strstr(mdl->name + 17, "arms")) {		// Also replaces some player model's arms (worldmodel arms)
		if (variables::chams::vm_arm_chams) {
			if (variables::chams::draw_chams_on_top) hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, variables::chams::wireframe_chams, variables::colors::chams_arms_c, materials.at(variables::chams::arm_chams_mat_id.idx));
			hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
		}
	} else if (strstr(mdl->name, "models/weapons/v")) {
		if (variables::chams::vm_weapon_chams && !csgo::local_player->is_scoped()) {
			if (variables::chams::draw_chams_on_top) hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, variables::chams::wireframe_chams, variables::colors::chams_weapon_c, materials.at(variables::chams::weapon_chams_mat_id.idx));
			hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
		}
	}
}
