#include "../features.hpp"

void misc::spectators()
{
	if (!variables::spectator_list_bool) return;

	if (interfaces::engine->is_connected()) {

		int wa, ha;
		interfaces::engine->get_screen_size(wa, ha);

		int pos_x = 10;
		int pos_y = ha / 2 + 20;

		int loop = 0;

		// Bakground
		interfaces::surface->set_drawing_color(34, 34, 37, 200);
		interfaces::surface->draw_filled_rectangle(pos_x, pos_y - 1, 100, 70);
		// Rectangle
		interfaces::surface->set_drawing_color(150, 22, 22);
		interfaces::surface->draw_outlined_rect(pos_x, pos_y - 1, 100, 70);
		// Title
		render::draw_text_string(pos_x, pos_y - 15, render::fonts::watermark_font, "Spectators", false, color(230, 0, 0));

		for (int i = 0; i <= 64; i++) {
			player_t* e = (player_t*)interfaces::entity_list->get_client_entity(i);
			player_info_t pinfo;

			if (e && e != csgo::local_player && !e->dormant()) {
				interfaces::engine->get_player_info(i, &pinfo);
				auto obs = e->observer_target();
				if (!obs) continue;
				player_t* spec = (player_t*)interfaces::entity_list->get_client_entity_handle(obs);
				if (spec == nullptr) continue;
				player_info_t spec_info;
				interfaces::engine->get_player_info(i, &spec_info);
				char buf[255]; sprintf_s(buf, "%s", pinfo.name);

				if (strstr(pinfo.name, "GOTV")) continue;

				if (spec->index() == csgo::local_player->index()) {
					render::draw_text_string(pos_x + 5, (pos_y + (14 * loop)), render::fonts::watermark_font, buf, false, color(255, 255, 255));
					loop++;
				}
			}
		}
		char loop_str[24];
	}
}