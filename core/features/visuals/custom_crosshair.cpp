#include "../features.hpp"

void visuals::crosshair::draw_custom_crosshair(int x, int y, int outline, color cross_color) {

	// TODO: Remove comments
	/*
	// Draw diagonal with gap (cod hitmarker) crosshair
	render::draw_line(mid_x - len, mid_y - len, mid_x - gap, mid_y - gap, color::red(255));
	render::draw_line(mid_x + len, mid_y + len, mid_x + gap, mid_y + gap, color::red(255));
	render::draw_line(mid_x + len, mid_y - len, mid_x + gap, mid_y - gap, color::red(255));
	render::draw_line(mid_x - len, mid_y + len, mid_x - gap, mid_y + gap, color::red(255));
	*/
	/*
	// Draw horizontal with gap (1px width)
	render::draw_line(mid_x,	top,		mid_x, mid_y - gap, color::red(255));
	render::draw_line(mid_x,	bottom,		mid_x, mid_y + gap, color::red(255));
	render::draw_line(left,		mid_y,		mid_x - gap, mid_y, color::red(255));
	render::draw_line(right,	mid_y,		mid_x + gap, mid_y, color::red(255));
	*/
	/*
	if (width > 2) {	// With outline
		render::draw_filled_rect(mid_x - width / 2, top, width, len, color::red(255));
		render::draw_rect(mid_x - width / 2, top, width, len, color::black(255));
		render::draw_filled_rect(mid_x - width / 2, mid_y + gap, width, len, color::red(255));
		render::draw_rect(mid_x - width / 2, mid_y + gap, width, len, color::black(255));
		render::draw_filled_rect(left + 1, mid_y - width / 2 + 1, len, width, color::red(255));
		render::draw_rect(left + 1, mid_y - width / 2 + 1, len, width, color::black(255));
		render::draw_filled_rect(mid_x + gap + 1, mid_y - width / 2 + 1, len, width, color::red(255));
		render::draw_rect(mid_x + gap + 1, mid_y - width / 2 + 1, len, width, color::black(255));
	} else {
		render::draw_filled_rect(mid_x - width / 2, top, width, len, color::red(255));
		render::draw_filled_rect(mid_x - width / 2, mid_y + gap, width, len, color::red(255));
		render::draw_filled_rect(left, mid_y - width / 2, len, width, color::red(255));
		render::draw_filled_rect(mid_x + gap, mid_y - width / 2, len, width, color::red(255));
	}
	*/
	/*
	render::draw_filled_rect(mid_x - (width - width / 2), top, width, len, color::red(255));
	render::draw_filled_rect(mid_x - (width - width / 2), mid_y + gap, width, len, color::red(255));
	render::draw_filled_rect(left, mid_y - (width - width / 2), len, width, color::red(255));
	render::draw_filled_rect(mid_x + gap, mid_y - (width - width / 2), len, width, color::red(255));
	*/

	const int gap = (int)variables::crosshair::crosshair_gap;
	//const int width = (int)variables::crosshair::crosshair_w;
	const int len = (int)variables::crosshair::crosshair_len;

	const int top = y - len - gap;
	const int bottom = y + len + gap;
	const int left = x - len - gap;
	const int right = x + len + gap;

	// Draw outline
	if (outline) {
		render::draw_rect(x - 1, top - 1, 3, len + 2, color::black(255));
		render::draw_rect(x - 1, y + gap, 3, len + 2, color::black(255));
		render::draw_rect(left - 1, y - 1, len + 2, 3, color::black(255));
		render::draw_rect(x + gap, y - 1, len + 2, 3, color::black(255));
	}
	// Draw 1px crosshair
	render::draw_line(x, top, x, y - gap, cross_color);
	render::draw_line(x, bottom, x, y + gap, cross_color);
	render::draw_line(left, y, x - gap, y, cross_color);
	render::draw_line(right, y, x + gap, y, cross_color);
}

void visuals::crosshair::custom_crosshair() {
	if (!variables::crosshair_bool) {
		if (variables::crosshair::only_engine_crosshair && !variables::crosshair::using_cs_crosshair) {
			variables::crosshair::using_cs_crosshair = true;
			interfaces::engine->execute_cmd("crosshair 1");
			custom_helpers::state_to_console("Crosshair", "Using cs:go crosshair...");
		}
		return;
	}

	if (!interfaces::engine->is_in_game()) return;
	if (!csgo::local_player) return;

	if (interfaces::engine->is_connected()) {
		if (csgo::local_player->is_scoped()) return;
		int screen_w, screen_h;
		interfaces::engine->get_screen_size(screen_w, screen_h);

		const int mid_x = screen_w / 2;
		const int mid_y = screen_h / 2;

		if (variables::crosshair::only_engine_crosshair && variables::crosshair::using_cs_crosshair) {
			variables::crosshair::using_cs_crosshair = false;
			interfaces::engine->execute_cmd("crosshair 0");
			custom_helpers::state_to_console("Crosshair", "Using engine crosshair...");
		}

		visuals::crosshair::draw_custom_crosshair(mid_x, mid_y, true, color::white(255));
	}
}