#include "menu.hpp"

// https://github.com/notbemji/templehook based on based repo
void watermark::draw() {
    render::draw_text_string(variables::watermark::x, variables::watermark::y, render::fonts::watermark_font, "NullHooks Beta", false, color::red(255));
}

void watermark::draw_stats() {
    // Positions
    const int ypos = variables::watermark::y + 12;
    const int xpos = variables::watermark::x;
    const int xpos_fps = xpos + 115;

    // Colors
    const color base_color = color(220, 5,   5,  255);
    const color color_l    = color(255, 150, 0,  255);
    const color color_m    = color(255, 255, 0,  255);
    const color color_h    = color(0,   255, 10, 255);

	//render::draw_filled_rect(5, 5, 8 * watermark_text.length(), 8, color::navy());
	render::draw_text_string(xpos, ypos, render::fonts::watermark_font, helpers::get_timestamp_string(), false, base_color);

    const int fps = helpers::get_fps();
    if (fps < 100) render::draw_text_string(xpos_fps, ypos, render::fonts::watermark_font, "| FPS: " + std::to_string(fps), false, color_l);
    else if (fps < 150) render::draw_text_string(xpos_fps, ypos, render::fonts::watermark_font, "| FPS: " + std::to_string(fps), false, color_m);
    else render::draw_text_string(xpos_fps, ypos, render::fonts::watermark_font, "| FPS: " + std::to_string(fps), false, color_h);

    if (csgo::local_player) {
        const int xpos_speed = xpos_fps + 60;
        const int speed = (int)std::ceil(csgo::local_player->velocity().length_2d());

        if (speed < 95) render::draw_text_string(xpos_speed, ypos, render::fonts::watermark_font, "| Speed: " + std::to_string(speed), false, color_l);
        else if (speed < 135) render::draw_text_string(xpos_speed, ypos, render::fonts::watermark_font, "| Speed: " + std::to_string(speed), false, color_m);
        else render::draw_text_string(xpos_speed, ypos, render::fonts::watermark_font, "| Speed: " + std::to_string(speed), false, color_h);
    }
}

int watermark::helpers::get_fps() noexcept {
    static float frame_rate = 0;
    frame_rate = 0.9f * frame_rate + (1.f - 0.9f) * interfaces::globals->absolute_frametime;
    return int(1.f / frame_rate);
}

std::string watermark::helpers::get_timestamp_string() noexcept {
    auto now = std::time(nullptr);
    auto dt = std::ctime(&now);
    return std::string(dt);
}