#include "core/menu/menu.hpp"

/*
 * Based on based repos:
 * https://github.com/notbemji/templehook
 * https://github.com/notbemji/5shekelcheats
 */

cursor_coords w_cursor;
cursor_coords w_cursor_corrected;
const int margin = 5;

void watermark::draw() {
	bool watermark = variables::misc::draw_watermark;
	const bool draw_time = variables::misc::watermark_stats.is_enabled(0);
	const bool draw_fps = variables::misc::watermark_stats.is_enabled(1);
	const bool draw_ping = variables::misc::watermark_stats.is_enabled(2);
	if (watermark || draw_time || draw_fps || draw_ping) {
		variables::ui::watermark::h = 0;
		if (watermark)							variables::ui::watermark::h += 15;
		if (draw_time || draw_fps || draw_ping)	variables::ui::watermark::h += 15;
		variables::ui::watermark::h += margin * 2;
	} else {
		return;
	}

	variables::ui::watermark::w = 0;		// Reset before drawing, will be set there
	draw_watermark(variables::ui::watermark::x + margin, variables::ui::watermark::y + margin);
	draw_stats();

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

#pragma region WATERMARK
void watermark::draw_watermark(int x, int y) {
	if (!variables::misc::draw_watermark) return;
	static const std::string cheat_name = "NullHooks";
	if (csgo::local_player && interfaces::engine->is_connected()) {
		player_info_t player_info;
		interfaces::engine->get_player_info(interfaces::engine->get_local_player(), &player_info);
		const std::string name(player_info.name);
		const std::string at = (name == "") ? "" : "@";

		const color at_color = color(180, 5, 5, 255);
		const color watermark_color = color::red(255);
		const std::wstring converted_name = std::wstring(name.begin(), name.end());
		const std::wstring converted_at = std::wstring(at.begin(), at.end());
		const std::wstring converted_cn = std::wstring(cheat_name.begin(), cheat_name.end());

		int width, height;
		interfaces::surface->draw_text_font(render::fonts::watermark_font);
		interfaces::surface->draw_text_pos(x, y);
		interfaces::surface->get_text_size(render::fonts::watermark_font, converted_name.c_str(), width, height);

		interfaces::surface->set_text_color(watermark_color.r, watermark_color.g, watermark_color.b, watermark_color.a);
		interfaces::surface->draw_render_text(converted_name.c_str(), wcslen(converted_name.c_str()));

		interfaces::surface->set_text_color(at_color.r, at_color.g, at_color.b, at_color.a);
		interfaces::surface->draw_render_text(converted_at.c_str(), wcslen(converted_at.c_str()));

		interfaces::surface->set_text_color(watermark_color.r, watermark_color.g, watermark_color.b, watermark_color.a);
		interfaces::surface->draw_render_text(converted_cn.c_str(), wcslen(converted_cn.c_str()));

		variables::ui::watermark::w = render::get_text_size(render::fonts::watermark_font, cheat_name + at + name).x + margin * 2;
	} else {
		render::draw_text_string(x, y, render::fonts::watermark_font, cheat_name, false, color::red(255));
		variables::ui::watermark::w = render::get_text_size(render::fonts::watermark_font, cheat_name).x + margin * 2;
	}
}
#pragma endregion

#pragma region STATS
std::string get_timestamp_string() noexcept {
	auto now = std::time(nullptr);
	auto dt = std::ctime(&now);
	std::string final_str = std::string(dt);
	final_str.pop_back();   // Remove last char because timestamp is too long (has a tab?)
	return final_str;
}

int get_fps() noexcept {
	static float frame_rate = 0;
	frame_rate = 0.9f * frame_rate + (1.f - 0.9f) * interfaces::globals->absolute_frametime;
	return int(1.f / frame_rate);
}

// Will return -1 if the ping is invalid
int get_ping() noexcept {
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

// Will draw the actual string based on the values and lengths
void draw_stats_string(std::string base, color base_col, std::string fps, color fps_col, bool draw_fps, std::string ping, color ping_col, bool draw_ping) {
	static int text_w_buff = 0;
	const int x = variables::ui::watermark::x + margin;
	const int y = variables::ui::watermark::y + 20;
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
		if (variables::misc::watermark_stats.is_enabled(0) || variables::misc::watermark_stats.is_enabled(1))
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

	if (text_w_buff > variables::ui::watermark::w + margin * 2) variables::ui::watermark::w = text_w_buff + margin * 2;
}

// Will get the stats and colors and pass them to draw_stats_string()
void watermark::draw_stats() {
	const bool draw_time = variables::misc::watermark_stats.is_enabled(0);
	const bool draw_fps = variables::misc::watermark_stats.is_enabled(1);
	const bool draw_ping = variables::misc::watermark_stats.is_enabled(2);
	if (!draw_time && !draw_fps && !draw_ping) return;

	std::string base_str = "";
	if (draw_time) {
		base_str = get_timestamp_string();  // Get the base string with the timestamp
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
#pragma endregion