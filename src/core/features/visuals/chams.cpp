#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/features/misc/backtrack.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"

#ifdef _DEBUG
#include "core/features/debug/debug.hpp"
#endif // _DEBUG

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

void visuals::draw_chams(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix) {
	if (!csgo::local_player) return;
	if (!(variables::chams::player_chams
		|| variables::chams::localplayer_chams
		|| variables::chams::vm_weapon_chams
		|| variables::chams::vm_arm_chams
		|| variables::chams::vm_sleeve_chams
		|| variables::chams::backtrack_chams)) return;

	const auto mdl = info.model;
	if (!mdl) return;

	#pragma region PLAYER
	if (strstr(mdl->name, "models/player") && (variables::chams::player_chams || variables::chams::localplayer_chams || variables::chams::backtrack_chams)) {
		const char* player_material = (variables::chams::player_chams_mat_id.idx < materials.size()) ? materials.at(variables::chams::player_chams_mat_id.idx) : materials.at(materials.size() - 1);

		player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));
		if (!player || !player->is_alive() || player->dormant()) return;

		if (player->has_gun_game_immunity()) {
			if (variables::chams::draw_chams_on_top) hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, false, color(255, 255, 255, 100), player_material);
		} else {

			#ifdef _DEBUG
			if(player->index() == debug::best_target_idx) {
				override_material(false, false, color::green(), materials[1]);
				hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, debug::best_record.matrix);
				interfaces::model_render->override_material(nullptr);
			}
			#endif // _DEBUG

			// Backtrack chams
			if (variables::misc::backtrack && backtrack::records[player->index()].size() > 0 && variables::chams::backtrack_chams && (player->team() != csgo::local_player->team() || variables::misc::backtrack_team)) {
				if (!variables::chams::player_chams)
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);	// Draw original player before backtrack if normal player chams are disabled. Probably a bad way of doing it

				// TODO: Maybe make the color a fade from player chams color to backtrack chams color
				// TODO: Backtrack color at max opacity glitches a bit with normal chams
				const color chams_col = (player->team() == csgo::local_player->team()) ? variables::colors::bt_chams_friend : variables::colors::bt_chams_enemy;
				for (uint32_t i = 0; i < backtrack::records[player->index()].size(); i++) {
					if (!backtrack::valid_tick(backtrack::records[player->index()][i].simulation_time, 0.2f)
						|| backtrack::records[player->index()][i].matrix == nullptr)
						continue;

					override_material(false, false, chams_col, materials[1]);
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, backtrack::records[player->index()][i].matrix);		// Use backtrack's matrix
				}
			}

			// For thirdperson
			if (player == csgo::local_player && variables::misc::thirdperson && variables::chams::localplayer_chams) {
				if (variables::chams::draw_chams_on_top)
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
				
				const float localplayer_col_a = (csgo::local_player->is_scoped()) ? 30 : variables::colors::chams_localplayer.col.a;
				override_material(false, variables::chams::wireframe_chams, variables::colors::chams_localplayer.col.get_custom_alpha(localplayer_col_a), materials[variables::chams::localplayer_chams_mat_id.idx]);
			// Enemies
			} else if (variables::chams::player_chams && player->team() != csgo::local_player->team()) {
				if (variables::chams::draw_chams_on_top)
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
				if (!variables::chams::only_visible_chams) {
					const color invisible_chams_col = variables::colors::chams_inv_enemy_c.col.get_custom_alpha(variables::colors::chams_vis_enemy_c.col.a);	// So it uses the same alpha as normal col
					override_material(true, variables::chams::wireframe_chams, invisible_chams_col, player_material);						// Not visible - Enemy
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);								// Call original to draw the ignorez one
				}
				override_material(false, variables::chams::wireframe_chams, variables::colors::chams_vis_enemy_c, player_material);			// Visible - Enemy
			// Friends
			} else if (variables::chams::player_chams && variables::player_visuals::showteamesp && player != csgo::local_player) {
				if (variables::chams::draw_chams_on_top)
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
				if (!variables::chams::only_visible_chams) {
					const color invisible_chams_col = variables::colors::chams_inv_friend_c.col.get_custom_alpha(variables::colors::chams_vis_friend_c.col.a);	// So it uses the same alpha as normal col
					override_material(true, variables::chams::wireframe_chams, invisible_chams_col, player_material);						// Not visible - Friendly
					hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);								// Call original to draw the ignorez one
				}
				override_material(false, variables::chams::wireframe_chams, variables::colors::chams_vis_friend_c, player_material);		// Visible - Friendly
			}
		}
	}
	#pragma endregion

	#pragma region VIEWMODEL
	// Sleeve
	if (strstr(mdl->name, "sleeve")) {
		if (variables::chams::vm_sleeve_chams) {
			if (variables::chams::draw_chams_on_top)
				hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, variables::chams::wireframe_chams, variables::colors::chams_sleeve_c, materials.at(variables::chams::sleeve_chams_mat_id.idx));
		}
	// Arms
	} else if (strstr(mdl->name + 17, "arms")) {
		// Remove normal arms if we have a custom model and alive
		if (csgo::local_player->is_alive() && skins::custom_models.find(ARMS) != skins::custom_models.end() && strstr(csgo::local_player->arms_model(), skins::custom_models.at(ARMS).worldmodel.c_str())) {
			override_material(false, variables::chams::wireframe_chams, color{0,0,0,0}, materials.at(1));
		// Normal arms
		} else if (variables::chams::vm_arm_chams) {
			if (variables::chams::draw_chams_on_top)
				hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, variables::chams::wireframe_chams, variables::colors::chams_arms_c, materials.at(variables::chams::arm_chams_mat_id.idx));
		}
	// Viewmodel weapon
	} else if (strstr(mdl->name, "models/weapons/v")) {
		if (variables::chams::vm_weapon_chams && !csgo::local_player->is_scoped()) {
			if (variables::chams::draw_chams_on_top)
				hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, variables::chams::wireframe_chams, variables::colors::chams_weapon_c, materials.at(variables::chams::weapon_chams_mat_id.idx));
		}
	}
	#pragma endregion

	hooks::draw_model_execute::original(interfaces::model_render, 0, ctx, state, info, matrix);
}
