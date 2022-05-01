#include "../features.hpp"

void visuals::playeresp() {
	if (!(variables::boxesp_bool || variables::nameesp_bool)) return;
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

		if (variables::nameesp_bool) {
			player_info_t playerinfo;
			interfaces::engine->get_player_info(iPlayer, &playerinfo);

			if (pCSPlayer->team() == csgo::local_player->team() && variables::showteamesp_bool)
				render::draw_text_string(x + 10, y + h + 2, render::fonts::watermark_font, playerinfo.name, true, color::blue());
			else if (pCSPlayer->team() != csgo::local_player->team())
				render::draw_text_string(x + 10, y + h + 2, render::fonts::watermark_font, playerinfo.name, true, color::red());
		}
	}
}

// ------------------------------

// Remove scout black shit
// Tracers?
// Custom fov?
