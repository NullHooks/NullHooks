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

void debug::draw_angles() {
	if(!csgo::local_player) return;
	vec3_t engine_angles;
	vec3_t origin = csgo::local_player->abs_origin();
	interfaces::engine->get_view_angles(engine_angles);
	float eye = csgo::local_player->eye_angles().y;
	float eng = engine_angles.y;
	float lby = csgo::local_player->lower_body_yaw();
	float abs = csgo::local_player->abs_angles().y;

	auto draw_angle = [origin](float angle, std::string label, color col) {
		vec3_t a = { 0.0f, angle, 0.0f };
		vec3_t v = math::angle_vector(a);
		v *= 100.0f;
		vec3_t s_start, s_end;
		if(!(math::world_to_screen(origin, s_start) && math::world_to_screen(v + origin, s_end)))
			return;
		render::draw_line(s_start.x, s_start.y, s_end.x, s_end.y, col);
		render::draw_text_string(s_end.x, s_end.y, render::fonts::watermark_font, label, false, col);
	};

	draw_angle(sv_yaw, "SV",  helpers::colors::hsv2color(int_hsv(42,  255, 255), 255));
	draw_angle(cl_yaw, "CL",  helpers::colors::hsv2color(int_hsv(84,  255, 255), 255));
	draw_angle(abs,    "ABS", helpers::colors::hsv2color(int_hsv(126, 255, 255), 255));
	draw_angle(eye,    "EYE", helpers::colors::hsv2color(int_hsv(168, 255, 255), 255));
	draw_angle(eng,    "ENG", helpers::colors::hsv2color(int_hsv(210, 255, 255), 255));
	draw_angle(lby,    "LBY", helpers::colors::hsv2color(int_hsv(255, 255, 255), 255));
}

void debug::log::puts(std::string str) {
	strs.push_front({str, interfaces::globals->cur_time});

	while(strs.size() > MAX_SIZE)
		strs.pop_back();
}

void debug::log::draw() {

	int w, h;
	interfaces::engine->get_screen_size(w, h);

	int x = w / 2;
	int y = h / 2;

	for(auto &str : strs) {
		color col = color::white();
		col.a = ((MAX_TIME - (interfaces::globals->cur_time - str.time)) / MAX_TIME) * 255.0f;
		render::draw_text_string(x, y += 12, render::fonts::watermark_font, str.label, false, col);
	}

	while(!strs.empty() && interfaces::globals->cur_time - strs.back().time > MAX_TIME)
		strs.pop_back();

}