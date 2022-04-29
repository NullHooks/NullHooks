#include "../features.hpp"

//TODO: Reduce window size depending on number of spectators
void misc::spectators()
{
	if (!variables::spectator_list_bool) return;

	if (interfaces::engine->is_connected()) {

		int wa, ha;
		interfaces::engine->get_screen_size(wa, ha);

		const int pos_x = 10;
		const int pos_y = ha / 2 + 20;

		const int arr_len = 64;				// For the spectator array
		int spec_count = 0;					// Will count actual spectators
		const char *spec_arr[arr_len + 1];

		// Clear usernames array
		for (int i = 0; i <= arr_len; i++) {
			spec_arr[i] = "";
		}

		// Get usernames from spectators
		for (int i = 0; i <= arr_len; i++) {
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
				char buf[255];
				sprintf(buf, "%s", pinfo.name);		// TODO

				if (strstr(pinfo.name, "GOTV")) continue;

				if (spec->index() == csgo::local_player->index()) {
					spec_arr[spec_count] = buf;
					spec_count++;
				}
			}
		}

		const int win_w = 100;
		const int win_h = 5 + (15 * spec_count) + 5;

		// Only render if there are spectators
		if (spec_arr[0] != "") {
			// Bakground
			interfaces::surface->set_drawing_color(34, 34, 37, 200);
			interfaces::surface->draw_filled_rectangle(pos_x, pos_y - 1, win_w, win_h);
			// Rectangle
			interfaces::surface->set_drawing_color(150, 22, 22);
			interfaces::surface->draw_outlined_rect(pos_x, pos_y - 1, win_w, win_h);
			// Title
			render::draw_text_string(pos_x, pos_y - 15, render::fonts::watermark_font, "Spectators", false, color(230, 0, 0));

			// Print each username
			for (int i = 0; i < spec_count; i++) {
				const char* username = spec_arr[i];
				if (username != "")
					render::draw_text_string(pos_x + 5, (pos_y + 5 + (15 * i)), render::fonts::watermark_font, username, false, color(255, 255, 255));
			}
		}
	}
}