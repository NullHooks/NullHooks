#include "core/menu/menu.hpp"

void draw_stats_string(std::string base, color base_col, std::string fps, color fps_col, bool draw_fps, std::string ping, color ping_col, bool draw_ping);
int get_fps();
int get_ping();

/*
 * Based on based repos:
 * https://github.com/notbemji/templehook
 * https://github.com/notbemji/5shekelcheats
 */

cursor_coords w_cursor;
cursor_coords w_cursor_corrected;
const int margin = 5;

void watermark::draw() {
	variables::ui::watermark::w = 0;		// Reset before drawing, will be set in get_str_and_draw (in draw_stats_string() actually)
	get_str_and_draw();

	movement(variables::ui::watermark::x, variables::ui::watermark::y, variables::ui::watermark::w, variables::ui::watermark::h);
}

void watermark::movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h) {
	if (gui::user_dragging_menu) return;					// Avoid overlapping
	if (spectator_framework::user_dragging_spec) return;	// Avoid overlapping

	interfaces::surface->surface_get_cursor_pos(w_cursor.x, w_cursor.y);

	if (input::global_input.IsHeld(VK_LBUTTON) && (w_cursor.x > x && w_cursor.x < x + w && w_cursor.y > y && w_cursor.y < y + h)) {
		user_dragging_menu = true;

		if (!should_move_menu) {
			w_cursor_corrected.x = w_cursor.x - x;
			w_cursor_corrected.y = w_cursor.y - y;
			should_move_menu = true;
		}
	}

	if (user_dragging_menu) {
		x = w_cursor.x - w_cursor_corrected.x;
		y = w_cursor.y - w_cursor_corrected.y;
	}

	if (!input::global_input.IsHeld(VK_LBUTTON)) {
		user_dragging_menu = false;
		should_move_menu = false;
	}
}

// Will get the stats and colors and pass them to draw_stats_string()
void watermark::get_str_and_draw() {
	const bool draw_user = variables::misc::watermark.is_enabled(0);
	const bool draw_cheat = variables::misc::watermark.is_enabled(1);
	const bool draw_fps = variables::misc::watermark.is_enabled(2);
	const bool draw_ping = variables::misc::watermark.is_enabled(3);
	if (!draw_user && !draw_cheat && !draw_fps && !draw_ping) return;

	std::string player_name = "";
	if (interfaces::engine->is_in_game()) {
		player_info_t player_info;
		interfaces::engine->get_player_info(interfaces::engine->get_local_player(), &player_info);
		player_name = std::string(player_info.name);		// First add player name
	}

	std::string base_str = "";
	if (draw_user && player_name != "") {
		base_str = player_name;		// First add player name

		if (draw_cheat)						// We also want to draw cheat name, so append
			base_str += " | NullHooks";

		if (draw_fps)                       // We also want to draw fps, so append
			base_str += " | FPS: ";
	} else if (draw_cheat) {
		base_str = "NullHooks";				// No player name so base str is cheat name

		if (draw_fps)                       // We also want to draw fps, so append
			base_str += " | FPS: ";
	} else if (draw_fps) {                  // We don't want to draw time but we want to draw fps, set 'FPS: ' as base
		base_str = "FPS: ";
	}

	// Colors
	static const color base_color = color(220, 5, 5, 255);
	static const color color_l = color(255, 150, 0, 255);
	static const color color_m = color(255, 255, 0, 255);
	static const color color_h = color(0, 255, 10, 255);

	color fps_color = base_color;
	const int fps = get_fps();
	if (fps < 80)       fps_color = color_l;
	else if (fps < 110) fps_color = color_m;
	else                fps_color = color_h;

	const int ping = get_ping();
	if (ping >= 0) {
		color ping_color = base_color;
		if (ping < 50)      ping_color = color_h;
		else if (ping < 75) ping_color = color_m;
		else                ping_color = color_l;

		draw_stats_string(base_str, base_color, std::to_string(fps), fps_color, draw_fps, std::to_string(ping), ping_color, draw_ping);     // We have valid ping, pass the multicombobox bool as arg
	} else {
		draw_stats_string(base_str, base_color, std::to_string(fps), fps_color, draw_fps, "", base_color, false);                           // We don't have a valid ping, we don't care about the combobox value
	}
}

// Will draw the actual string based on the values and lengths
void draw_stats_string(std::string base, color base_col, std::string fps, color fps_col, bool draw_fps, std::string ping, color ping_col, bool draw_ping) {
	static int text_w_buff = margin * 2;
	const int x = variables::ui::watermark::x;
	const int y = variables::ui::watermark::y;
	const unsigned long font = render::fonts::watermark_font;

	const std::wstring converted_base = std::wstring(base.begin(), base.end());
	const std::wstring converted_fps = std::wstring(fps.begin(), fps.end());

	interfaces::surface->draw_text_font(font);

	int width, height;
	interfaces::surface->draw_text_pos(x, y);
	interfaces::surface->get_text_size(font, converted_base.c_str(), width, height);

	interfaces::surface->set_text_color(base_col.r, base_col.g, base_col.b, base_col.a);
	interfaces::surface->draw_render_text(converted_base.c_str(), wcslen(converted_base.c_str()));

	text_w_buff = render::get_text_size(render::fonts::watermark_font, base).x;

	if (draw_fps) {
		interfaces::surface->set_text_color(fps_col.r, fps_col.g, fps_col.b, fps_col.a);
		interfaces::surface->draw_render_text(converted_fps.c_str(), wcslen(converted_fps.c_str()));

		text_w_buff += render::get_text_size(render::fonts::watermark_font, fps).x;
	}

	if (draw_ping) {
		std::string pingtext = "";
		if (variables::misc::watermark.is_enabled(0) || variables::misc::watermark.is_enabled(1) || variables::misc::watermark.is_enabled(2))
			pingtext = " | Ping: ";     // Add ' | ' if it's not the only item we want to draw
		else
			pingtext = "Ping: ";

		const std::wstring converted_pingtext = std::wstring(pingtext.begin(), pingtext.end());
		const std::wstring converted_ping = std::wstring(ping.begin(), ping.end());

		interfaces::surface->set_text_color(base_col.r, base_col.g, base_col.b, base_col.a);
		interfaces::surface->draw_render_text(converted_pingtext.c_str(), wcslen(converted_pingtext.c_str()));

		interfaces::surface->set_text_color(ping_col.r, ping_col.g, ping_col.b, ping_col.a);
		interfaces::surface->draw_render_text(converted_ping.c_str(), wcslen(converted_ping.c_str()));

		text_w_buff += render::get_text_size(render::fonts::watermark_font, pingtext + ping).x;
	}

	variables::ui::watermark::w = text_w_buff;		// Set the watermark w
}

int get_fps() {
	static float frame_rate = 0;
	frame_rate = 0.9f * frame_rate + (1.f - 0.9f) * interfaces::globals->absolute_frametime;
	return int(1.f / frame_rate);
}

// Will return -1 if the ping is invalid
int get_ping() {
	if (!(csgo::local_player && interfaces::engine->is_connected())) return -1;

	const auto net_channel_info = interfaces::engine->get_net_channel_info();
	if (!net_channel_info) return -1;

	auto average_latency = net_channel_info->get_average_latency(FLOW_OUTGOING);
	static auto cl_updaterate = interfaces::console->get_convar("cl_updaterate");

	if (cl_updaterate->get_float() > 0.001f)
		average_latency += -0.5f / cl_updaterate->get_float();

	const int num = std::abs(average_latency) * 1000.0f;

	return (num < 1000) ? num : -1;
}