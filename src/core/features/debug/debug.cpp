#include "dependencies/utilities/csgo.hpp"
#include "core/features/debug/debug.hpp"

void debug::draw_aimbot_targets() {
	
	for(auto &point : points) {
		vec3_t s_point;
		if(!math::world_to_screen(point.origin, s_point))
			continue;

		color col = point.enabled ? color::green() : color::red();

		render::draw_circle(s_point.x, s_point.y, 2, 15, col);
	}

}

void debug::draw_autowall_traces() {

	for(auto &shot : shots) {

		vec3_t s_start, s_end;

		for(auto &trace : shot.traces) {
			if(!(math::world_to_screen(trace.start, s_start) && math::world_to_screen(trace.end, s_end)))
				continue;

			color col = trace.in_wall ? color::red() : color::green();

			render::draw_line(s_start.x, s_start.y, s_end.x, s_end.y, col);

			render::draw_text_string(s_end.x, s_end.y, render::fonts::watermark_font, std::to_string(trace.damage_dropoff), false, color::white());

		}

	}

}

void debug::log::puts(const char *str) {
	strs.push_front(str);
	while(strs.size() > MAX_SIZE)
		strs.pop_back();
}

void debug::log::draw() {

	int w, h;
	interfaces::engine->get_screen_size(w, h);

	int x = w / 2;
	int y = h / 2;

	for(auto &str : strs)
		render::draw_text_string(x, y += 12, render::fonts::watermark_font, str, false, color::red());

}