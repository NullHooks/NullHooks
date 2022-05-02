#include "../features.hpp"

void visuals::playeresp() {
	if (!(variables::boxesp_bool || variables::nameesp_bool || variables::skeletonesp_bool)) return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;

	for (int iPlayer = 0; iPlayer < interfaces::globals->max_clients; iPlayer++)
	{
		auto pCSPlayer = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(iPlayer));
		if (!pCSPlayer) continue;
		if (pCSPlayer == csgo::local_player) continue;
		if (pCSPlayer->dormant()) continue;
		if (!(pCSPlayer->is_alive() && pCSPlayer->health() > 0)) continue;

		vec3_t vecFoot, vecScreen, vecHeadScreen;
		vecFoot = pCSPlayer->origin();
		if (!(math::world_to_screen(vecFoot, vecScreen))) continue;

		vecFoot.z += 72.f;
		if (!(math::world_to_screen(vecFoot, vecHeadScreen))) continue;

		const int h = vecScreen.y - vecHeadScreen.y;
		const int w = (h / 5) * 2;
		const int y = vecHeadScreen.y;
		const int x = vecHeadScreen.x - w / 2;

		/* ------------- SKELETON ESP ------------- */
		if (variables::skeletonesp_bool) {
			auto pStudioModel = interfaces::model_info->get_studio_model(pCSPlayer->model());
			if (!pStudioModel) return;

			static matrix_t pBoneToWorldOut[128];
			if (pCSPlayer->setup_bones(pBoneToWorldOut, 128, 256, 0))
			{
				for (int i = 0; i < pStudioModel->bones_count; i++)
				{
					studio_bone_t* pBone = pStudioModel->bone(i);
					if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
						continue;

					vec3_t vBonePos1;
					if (!math::world_to_screen(vec3_t(pBoneToWorldOut[i][0][3], pBoneToWorldOut[i][1][3], pBoneToWorldOut[i][2][3]), vBonePos1))
						continue;

					vec3_t vBonePos2;
					if (!math::world_to_screen(vec3_t(pBoneToWorldOut[pBone->parent][0][3], pBoneToWorldOut[pBone->parent][1][3], pBoneToWorldOut[pBone->parent][2][3]), vBonePos2))
						continue;

					if (pCSPlayer->team() == csgo::local_player->team() && variables::showteamesp_bool)
						render::draw_line(vBonePos1.x, vBonePos1.y, vBonePos2.x, vBonePos2.y, color(85, 235, 255, 255));
					else if (pCSPlayer->team() != csgo::local_player->team())
						render::draw_line(vBonePos1.x, vBonePos1.y, vBonePos2.x, vBonePos2.y, color(255, 82, 82, 255));
				}
			}
		}

		/* ------------- BOX ESP ------------- */
		if (variables::boxesp_bool) {
			if (pCSPlayer->team() == csgo::local_player->team() && variables::showteamesp_bool)
				render::draw_rect(x, y, w, h, color::blue()); // Drawing with render tools
				/*
				//interfaces::surface->set_drawing_color(0, 0, 255, 255);  second way of drawing it with the surface interface, we set color, blue here
				//interfaces::surface->draw_outlined_rect(x, y, w, h);  then we draw
				*/
			else if (pCSPlayer->team() != csgo::local_player->team())
				render::draw_rect(x, y, w, h, color::red());
		}

		/* ------------- NAME ESP ------------- */
		if (variables::nameesp_bool) {
			player_info_t playerinfo;
			interfaces::engine->get_player_info(iPlayer, &playerinfo);

			if (pCSPlayer->team() == csgo::local_player->team() && variables::showteamesp_bool)
				render::draw_text_string(x + w/2, y + h + 2, render::fonts::watermark_font, playerinfo.name, true, color::blue());
			else if (pCSPlayer->team() != csgo::local_player->team())
				render::draw_text_string(x + w/2, y + h + 2, render::fonts::watermark_font, playerinfo.name, true, color::red());
		}
	}
}

// ------------------------------

// Remove scout black shit
// Tracers?
// Custom fov?
