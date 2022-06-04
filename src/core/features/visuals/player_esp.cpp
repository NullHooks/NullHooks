#include "../features.hpp"

void visuals::playeresp() {
	if (!(variables::player_visuals::boxesp
		|| variables::player_visuals::nameesp
		|| variables::player_visuals::skeletonesp
		|| variables::player_visuals::healthesp
		|| variables::player_visuals::playerinfo
		|| variables::player_visuals::lineesp)) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;

	// Will ignore ESP if the player being spectated
	player_t* local_player_ent = (csgo::local_player->is_alive()) ? csgo::local_player : reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(csgo::local_player->observer_target()));

	/* interfaces::globals->max_clients */
	for (int iPlayer = 0; iPlayer < 64; iPlayer++)
	{
		player_t* pCSPlayer = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(iPlayer));
		if (!pCSPlayer) continue;
		if (pCSPlayer == local_player_ent) continue;
		if (pCSPlayer->dormant()) continue;
		if (!(pCSPlayer->is_alive() && pCSPlayer->health() > 0)) continue;

		vec3_t vecFoot, vecHead;
		vec3_t vecFootScreen, vecHeadScreen;

		auto pStudioModel = interfaces::model_info->get_studio_model(pCSPlayer->model());
		if (!pStudioModel) return;
		static matrix_t pBoneToWorldOut[128];
		if (!pCSPlayer->setup_bones(pBoneToWorldOut, 128, 256, 0)) continue;

		vecHead = pBoneToWorldOut[8].get_origin() + vec3_t{0.f, 0.f, 7.f};
		if (!(math::world_to_screen(vecHead, vecHeadScreen))) continue;

		vecFoot = pCSPlayer->abs_origin() - vec3_t{0.f, 0.f, 1.f};		// Now better thanks to cazz
		if (!(math::world_to_screen(vecFoot, vecFootScreen))) continue;

		const int h = vecFootScreen.y - vecHeadScreen.y;
		const int w = h * 0.35f;
		const int y = vecHeadScreen.y;
		const int x = vecHeadScreen.x - w / 2;

		/* ------------- SKELETON ESP ------------- */
		if (variables::player_visuals::skeletonesp) {
			for (int i = 0; i < pStudioModel->bones_count; i++) {
				studio_bone_t* pBone = pStudioModel->bone(i);
				if (!pBone || !(pBone->flags & 256) || pBone->parent == -1) continue;

				vec3_t vBonePos1;
				if (!math::world_to_screen(vec3_t(pBoneToWorldOut[i][0][3], pBoneToWorldOut[i][1][3], pBoneToWorldOut[i][2][3]), vBonePos1)) continue;

				vec3_t vBonePos2;
				if (!math::world_to_screen(vec3_t(pBoneToWorldOut[pBone->parent][0][3], pBoneToWorldOut[pBone->parent][1][3], pBoneToWorldOut[pBone->parent][2][3]), vBonePos2)) continue;

				if (pCSPlayer->team() == csgo::local_player->team() && variables::player_visuals::showteamesp)
					render::draw_line(vBonePos1.x, vBonePos1.y, vBonePos2.x, vBonePos2.y, variables::colors::friendly_color_soft);
				else if (pCSPlayer->team() != csgo::local_player->team())
					render::draw_line(vBonePos1.x, vBonePos1.y, vBonePos2.x, vBonePos2.y, variables::colors::enemy_color_soft);
			}
		}
		/* ------------- BOX ESP ------------- */
		if (variables::player_visuals::boxesp) {
			if (pCSPlayer->team() == csgo::local_player->team() && variables::player_visuals::showteamesp) {
				// Draw box outline
				render::draw_rect(x - 1, y - 1, w + 2, h + 2, color::black());
				render::draw_rect(x + 1, y + 1, w - 2, h - 2, color::black());
				render::draw_rect(x, y, w, h, variables::colors::friendly_color);	// Drawing with render tools
			} else if (pCSPlayer->team() != csgo::local_player->team()) {
				render::draw_rect(x - 1, y - 1, w + 2, h + 2, color::black());
				render::draw_rect(x + 1, y + 1, w - 2, h - 2, color::black());
				render::draw_rect(x, y, w, h, variables::colors::enemy_color);
			}
		}
		/* ------------- LINE ESP ------------- */
		if (variables::player_visuals::lineesp) {
			int screen_width, screen_height;
			interfaces::engine->get_screen_size(screen_width, screen_height);
			
			// Draw from crosshair
			if (pCSPlayer->team() == csgo::local_player->team() && variables::player_visuals::showteamesp)
				render::draw_line(x + w / 2, y + h, screen_width / 2, screen_height / 2, variables::colors::friendly_color);
			else if (pCSPlayer->team() != csgo::local_player->team())
				render::draw_line(x + w / 2, y + h, screen_width / 2, screen_height / 2, variables::colors::enemy_color);
		}
		/* ------------- NAME ESP ------------- */
		if (variables::player_visuals::nameesp) {
			player_info_t playerinfo;
			interfaces::engine->get_player_info(iPlayer, &playerinfo);
			wchar_t w_player_name[128];
			if (MultiByteToWideChar(CP_UTF8, 0, playerinfo.name, -1, w_player_name, 128) < 0) continue;

			if (pCSPlayer->team() == csgo::local_player->team() && variables::player_visuals::showteamesp)
				render::draw_text_wchar(x + w/2, y + h + 2, render::fonts::watermark_font, w_player_name, true, variables::colors::friendly_color_soft);
			else if (pCSPlayer->team() != csgo::local_player->team())
				render::draw_text_wchar(x + w/2, y + h + 2, render::fonts::watermark_font, w_player_name, true, variables::colors::enemy_color);
		}
		/* ------------- INFO ESP ------------- */
		if (variables::player_visuals::playerinfo) {
			if (pCSPlayer->team() == csgo::local_player->team() && variables::player_visuals::showteamesp) {
				if (pCSPlayer->armor() > 0) {
					int armor_x = (variables::player_visuals::healthesp) ? 6 : 0;
					render::draw_text_string(x - 10 - armor_x, y + 1, render::fonts::watermark_font, "A", false, variables::colors::friendly_color_softer);
				}

				int item_num = 0;
				if (pCSPlayer->is_defusing()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "D", true, color::blue(255));
					item_num++;
				} else if (pCSPlayer->has_defuser()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "D", true, variables::colors::friendly_color_softer);
					item_num++;
				} // TODO: Has c4

				if (pCSPlayer->is_scoped()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "S", true, (pCSPlayer->is_defusing()) ? color::blue(255) : variables::colors::friendly_color_softer);
					item_num++;
				}
				if (pCSPlayer->is_flashed()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "F", true, color(255, 255, 0));
					item_num++;
				}

				auto current_weapon = pCSPlayer->active_weapon();
				if (!current_weapon) continue;

				std::string s_weapon_name = current_weapon->get_weapon_data()->weapon_name_alt;
				
				int y_weapon = (variables::player_visuals::nameesp) ? 12 : 0;
				if (strstr(s_weapon_name.c_str(), "weapon_")) s_weapon_name.erase(s_weapon_name.begin(), s_weapon_name.begin() + 7);	// Remove "weapon_"
				render::draw_text_string(x + w / 2, y + h + 2 + y_weapon, render::fonts::watermark_font, s_weapon_name, true, variables::colors::friendly_color_softer);
			} else if (pCSPlayer->team() != csgo::local_player->team()) {
				if (pCSPlayer->armor() > 0) {
					int armor_x = (variables::player_visuals::healthesp) ? 6 : 0;
					render::draw_text_string(x - 10 - armor_x, y + 1, render::fonts::watermark_font, "A", false, variables::colors::friendly_color_softer);
				}

				int item_num = 0;
				if (pCSPlayer->is_defusing()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "D", true, color::blue(255));
					item_num++;
				} else if (pCSPlayer->has_defuser()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "D", true, variables::colors::friendly_color_softer);
					item_num++;
				} // TODO: Has c4

				if (pCSPlayer->is_scoped()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "S", true, (pCSPlayer->is_defusing()) ? color::blue(255) : variables::colors::friendly_color_softer);
					item_num++;
				}
				if (pCSPlayer->is_flashed()) {
					render::draw_text_string(x + w + 5, y + 1 + 10 * item_num, render::fonts::watermark_font, "F", true, color(255, 255, 0));
					item_num++;
				}

				auto current_weapon = pCSPlayer->active_weapon();
				if (!current_weapon) continue;

				std::string s_weapon_name = current_weapon->get_weapon_data()->weapon_name_alt;
				
				int y_weapon = (variables::player_visuals::nameesp) ? 12 : 0;
				if (strstr(s_weapon_name.c_str(), "weapon_")) s_weapon_name.erase(s_weapon_name.begin(), s_weapon_name.begin() + 7);	// Remove "weapon_"
				render::draw_text_string(x + w / 2, y + h + 2 + y_weapon, render::fonts::watermark_font, s_weapon_name, true, variables::colors::enemy_color_softer);
			}
		}
		/* ------------- HEALTH ESP ------------- */
		if (variables::player_visuals::healthesp) {
			int health = pCSPlayer->health();
			const int health_h = (h * health) / 100;
			const int health_w = 4;
			const int health_y = y + (h - health_h);
			const int health_x = x - 6;
			//render::draw_text_string(10, 20, render::fonts::watermark_font, std::to_string(h), true, color::red());
			if (pCSPlayer->team() == csgo::local_player->team() && variables::player_visuals::showteamesp) {
				render::draw_filled_rect(health_x, y, health_w, h, color::red());
				render::draw_filled_rect(health_x, health_y, health_w, health_h, color::green());
				render::draw_rect(health_x, y, health_w, h, color::black(180));
			} else if (pCSPlayer->team() != csgo::local_player->team()) {
				render::draw_filled_rect(health_x, y, health_w, h, color::red());
				render::draw_filled_rect(health_x, health_y, health_w, health_h, color::green());
				render::draw_rect(health_x, y, health_w, h, color::black(180));
			}
		}
	}
}
