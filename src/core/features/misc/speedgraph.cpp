#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

const int speed_graph_width = 420;                          // Width in px, each px will be a speed value

struct speedinfo_t {

	int   m_tickcount;
	int   m_buttons;
	int   m_movetype;
	int   m_flags;
	float m_speed;

	bool onground() { return m_flags   & fl_onground; }
	bool jump_cmd() { return m_buttons & in_jump;     }

	bool jump() {
		return onground() && jump_cmd();
	}

	bool to_screen(int &x, int &y) {

		int w, h;
		interfaces::surface->get_screen_size(w, h);

		int tickdelta = m_tickcount - interfaces::globals->tick_count;

		x = w / 2 + speed_graph_width / 2 + tickdelta;
		y = h * 0.9 - m_speed * 0.3;

		return true;
	}

	color get_color() {
		float_hsv hue = { m_speed / 600.0f, 1.0f, 1.0f };
		return custom_helpers::hsv2color(hue, 255);
	}

	speedinfo_t(player_t *player, c_usercmd *cmd) {
		m_buttons   = cmd->buttons;
		m_flags     = player->flags();
		m_movetype  = player->move_type();
		m_speed     = player->velocity().length_2d();
		m_tickcount = interfaces::globals->tick_count;
	}

};

std::deque<speedinfo_t> g_speeds;

void draw_speed_str(int x, int y, int speed, color col) {
	render::draw_text_string(x, y, render::fonts::watermark_font_m, std::to_string(speed), true, col);

	int last_jumped     = -1;
	int old_last_jumped = -1;

	for(auto &speed : g_speeds) {

		if(speed.jump()) {

			if(last_jumped < 0)
				last_jumped = speed.m_speed;

			else if(old_last_jumped < 0) {
				old_last_jumped = speed.m_speed;
				break;
			}

		}

	}

	if (last_jumped > 0) {
		y += 17;
		const color jmpspeed_col = (last_jumped > old_last_jumped) ? color(0, 240, 0) : color(230, 10, 10);
		const color p_col = color::white();

		const std::string jmpspeed_txt = std::to_string(last_jumped);
		const auto converted_jmpspeed = std::wstring(jmpspeed_txt.begin(), jmpspeed_txt.end());
		const std::string p1 = "(";
		const auto c_p1 = std::wstring(p1.begin(), p1.end());
		const std::string p2 = ")";
		const auto c_p2 = std::wstring(p2.begin(), p2.end());
		const std::string full_jmpspeed_txt = p1 + std::to_string(last_jumped) + p2;		// Used to get the len
		const auto converted_full_jmpspeed = std::wstring(full_jmpspeed_txt.begin(), full_jmpspeed_txt.end());

		int width, height;
		interfaces::surface->get_text_size(render::fonts::watermark_font_m, converted_full_jmpspeed.c_str(), width, height);

		interfaces::surface->draw_text_font(render::fonts::watermark_font_m);
		interfaces::surface->draw_text_pos(x - (width / 2), y);

		interfaces::surface->set_text_color(p_col.r, p_col.g, p_col.b, p_col.a);
		interfaces::surface->draw_render_text(c_p1.c_str(), wcslen(c_p1.c_str()));
		interfaces::surface->set_text_color(jmpspeed_col.r, jmpspeed_col.g, jmpspeed_col.b, jmpspeed_col.a);
		interfaces::surface->draw_render_text(converted_jmpspeed.c_str(), wcslen(converted_jmpspeed.c_str()));
		interfaces::surface->set_text_color(p_col.r, p_col.g, p_col.b, p_col.a);
		interfaces::surface->draw_render_text(c_p2.c_str(), wcslen(c_p2.c_str()));
	}
}

void misc::speedgraph::update(c_usercmd* cmd) {
	if (csgo::local_player->move_type() == movetype_observer) return; // Don't update speed if noclip
	g_speeds.push_front(speedinfo_t(csgo::local_player, cmd));
}

void misc::speedgraph::draw() {
	if (!variables::misc::draw_speedgraph) return;
	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected()) return;
	if (!csgo::local_player) return;
	if (!csgo::local_player->is_alive()) return;
	if (csgo::local_player->move_type() == movetype_noclip || csgo::local_player->move_type() == movetype_observer) return;	// Noclip
	if (csgo::local_player->is_scoped()) return;		// We don't want to draw speed graph when scoped

	// Render graph
	int screen_w, screen_h;
	interfaces::surface->get_screen_size(screen_w, screen_h);

	if (!g_speeds.empty())
	for (int n = 0; n < g_speeds.size() - 1; n++) {   // -1 to skip last item
		auto cur_speed  = g_speeds.at(n);
		auto next_speed = g_speeds.at(n + 1);          // Needed to draw line to next value

		color line_col = color::white();
		if (variables::misc::use_speedgraph_color)
			line_col = next_speed.get_color();

		int x1, y1, x2, y2;

		cur_speed.to_screen(x1, y1);
		next_speed.to_screen(x2, y2);

		if(cur_speed.jump())
			render::draw_text_string(x1, y1, render::fonts::watermark_font_m, std::to_string(static_cast<int>(cur_speed.m_speed)), true, cur_speed.get_color());

		render::draw_line(x1, y1, x2, y2, line_col);
	}

	while (g_speeds.size() > speed_graph_width)
		g_speeds.pop_back();

	// Speed text
	auto speed = g_speeds.front();
	draw_speed_str(screen_w / 2, screen_h * 0.9 + 20, speed.m_speed, speed.get_color());
}
