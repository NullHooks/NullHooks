#include "menu.hpp"

// https://github.com/notbemji/templehook based on based repo
void watermark::draw() {
    render::draw_text_string(variables::watermark::x, variables::watermark::y, render::fonts::watermark_font, "NullHooks-Beta", false, color::red(255));
}

void watermark::draw_stats() {
    // Colors
    const color base_color = color(220, 5,   5,  255);
    const color color_l    = color(255, 150, 0,  255);
    const color color_m    = color(255, 255, 0,  255);
    const color color_h    = color(0,   255, 10, 255);

    color fps_color = base_color;

    const int fps = helpers::get_fps();
    if (fps < 100) fps_color = color_l;
    else if (fps < 150) fps_color = color_m;
    else fps_color = color_h;

    if (csgo::local_player) {
        const int speed = (int)std::ceil(csgo::local_player->velocity().length_2d());

        color speed_color = base_color;
        if (speed < 95) speed_color = color_l;
        else if (speed < 135) speed_color = color_m;
        else speed_color = color_h;

        watermark::draw_stats_string(helpers::get_timestamp_string() + " | FPS: ", base_color, std::to_string(fps), fps_color, std::to_string(speed), speed_color, true);
    } else {
        watermark::draw_stats_string(helpers::get_timestamp_string() + " | FPS: ", base_color, std::to_string(fps), fps_color, "", base_color, false);
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
    std::string final_str = std::string(dt);
    final_str.pop_back();   // Remove last char because timestamp is too long (tab?)
    return final_str;
}

void watermark::draw_stats_string(std::string ts, color tscolor, std::string fps, color fpscolor, std::string speed, color speedcolor, bool draw_speed) {
    const int x = variables::watermark::x;
    const int y = variables::watermark::y + 12;
    const unsigned long font = render::fonts::watermark_font;
    const int margin = 40;

    const auto converted_ts = std::wstring(ts.begin(), ts.end());
    const auto converted_fps = std::wstring(fps.begin(), fps.end());

    interfaces::surface->draw_text_font(font);

    int ts_width, ts_height;
    interfaces::surface->get_text_size(font, converted_ts.c_str(), ts_width, ts_height);
    interfaces::surface->set_text_color(tscolor.r, tscolor.g, tscolor.b, tscolor.a);
    interfaces::surface->draw_text_pos(x, y);
    interfaces::surface->draw_render_text(converted_ts.c_str(), wcslen(converted_ts.c_str()));

    int fps_width, fps_height;
    interfaces::surface->get_text_size(font, converted_fps.c_str(), fps_width, fps_height);
    interfaces::surface->set_text_color(fpscolor.r, fpscolor.g, fpscolor.b, fpscolor.a);
    //interfaces::surface->draw_text_pos(x + ts_width + margin, y);
    interfaces::surface->draw_render_text(converted_fps.c_str(), wcslen(converted_fps.c_str()));

    if (draw_speed) {
        const std::string speedtext = " | Speed: ";
        const auto converted_speedtext = std::wstring(speedtext.begin(), speedtext.end());
        const auto converted_speed = std::wstring(speed.begin(), speed.end());

        int speedtext_width, speedtext_height;
        interfaces::surface->get_text_size(font, converted_speedtext.c_str(), speedtext_width, speedtext_height);
        interfaces::surface->set_text_color(tscolor.r, tscolor.g, tscolor.b, tscolor.a);
        //interfaces::surface->draw_text_pos(x + fps_width + margin, y);
        interfaces::surface->draw_render_text(converted_speedtext.c_str(), wcslen(converted_speedtext.c_str()));

        interfaces::surface->set_text_color(speedcolor.r, speedcolor.g, speedcolor.b, speedcolor.a);
        //interfaces::surface->draw_text_pos(x + speedtext_width + margin, y);
        interfaces::surface->draw_render_text(converted_speed.c_str(), wcslen(converted_speed.c_str()));
    }
}