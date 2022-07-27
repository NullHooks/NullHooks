#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/menu.hpp"

void draw_spec_frame(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, std::int32_t wname_h, std::int32_t wname_margin, color bg, color header_text, color header_line, const std::string& name) {
	// Background
	render::draw_filled_rect(x, y, w, h, bg);
	// Header title
	render::draw_filled_rect(x, y, w, wname_h, header_text);
	render::draw_filled_rect(x, y + wname_h, w, 2, header_line);
	render::draw_text_string(x + 10, y + wname_margin, render::fonts::watermark_font, name, false, color::white(255));
};

void misc::spectator_list() {
	if (!variables::ui::spectators::spectator_list) return;
	if ((!interfaces::engine->is_connected() && !interfaces::engine->is_in_game()) && !variables::ui::menu::opened) return;
	if (!csgo::local_player) {
		// Draw only frame in main menu for example
		variables::ui::spectators::h = 5 + 5 + 25;
		draw_spec_frame(variables::ui::spectators::x, variables::ui::spectators::y, variables::ui::spectators::w, variables::ui::spectators::h, 25, 5,
			color(36, 36, 36, 255), color(25, 25, 25, 255), color(36, 36, 36, 255), "Spectators");
		return;
	}

	int spec_count = 0;			// Will count actual spectators
	std::wstring spec_arr[64 + 1];

	// Get spectator from self (alive) or currently spected
	player_t* spec_player = csgo::local_player->is_alive() ? csgo::local_player : reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(csgo::local_player->observer_target()));
	if (!spec_player) return;


	// Clear usernames array
	for (int i = 0; i <= 64; i++) {
		spec_arr[i] = L"";
	}

	// Get usernames from spectators
	for (int i = 0; i <= 64; i++) {
		auto e = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!e
			|| e->dormant()
			|| e->is_alive()
			|| reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(e->observer_target())) != spec_player
			|| e == csgo::local_player)
			continue;

		player_info_t pinfo;
		interfaces::engine->get_player_info(i, &pinfo);

		auto obs = e->observer_target();
		if (!obs) continue;
		player_t* spec = (player_t*)interfaces::entity_list->get_client_entity_handle(obs);
		if (!spec) continue;

		wchar_t w_player_name[255];
		if (MultiByteToWideChar(CP_UTF8, 0, pinfo.name, -1, w_player_name, 128) < 0) continue;

		if (pinfo.ishltv) continue;	// Compare .name cuz we cant wstring

		if (!csgo::local_player || !spec) continue;
		if (spec->index() == spec_player->index()) {
			spec_arr[spec_count] = w_player_name;
			spec_count++;
		}
	}

	// Only render if there are spectators or the menu is open
	if (spec_arr[0] != L"" || variables::ui::menu::opened) {
		int cur_name_w = variables::ui::spectators::w;
		int cur_name_h;

		const int wname_h = 25;
		variables::ui::spectators::w = 100;
		variables::ui::spectators::h = 5 + (15 * spec_count) + 5 + wname_h;

		draw_spec_frame(variables::ui::spectators::x, variables::ui::spectators::y, cur_name_w, variables::ui::spectators::h, wname_h, 5,
			color(36, 36, 36, 255), color(25, 25, 25, 255), color(36, 36, 36, 255), "Spectators");

		// Print each username
		std::wstring username;
		for (int i = 0; i < spec_count; i++) {
			username = spec_arr[i];
			if (username != L"") {
				interfaces::surface->get_text_size(render::fonts::watermark_font, username.c_str(), cur_name_w, cur_name_h);
				if (cur_name_w > variables::ui::spectators::w - 20)
					variables::ui::spectators::w = 10 + cur_name_w + 10;
				render::draw_text_wchar(variables::ui::spectators::x + 10, (variables::ui::spectators::y + wname_h + 5 + (15 * i)),
					render::fonts::watermark_font, username.c_str(), false, color(255, 255, 255));
			}
		}
	}
}