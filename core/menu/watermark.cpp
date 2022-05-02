#include "menu.hpp"

/*
 * Based on based repos:
 * https://github.com/notbemji/templehook 
 * https://github.com/notbemji/5shekelcheats
 */

void watermark::draw() {
    const std::string cheat_name = "NullHooks-Beta";
    if (csgo::local_player && interfaces::engine->is_connected()) {
        player_info_t player_info;
        interfaces::engine->get_player_info(interfaces::engine->get_local_player(), &player_info);
        const std::string name(player_info.name);
        const std::string at = "@";

        const color at_color = color(180, 5, 5, 255);
        const color watermark_color = color::red(255);
        const std::wstring converted_name = std::wstring(name.begin(), name.end());
        const std::wstring converted_at = std::wstring(at.begin(), at.end());
        const std::wstring converted_cn = std::wstring(cheat_name.begin(), cheat_name.end());

        int width, height;
        interfaces::surface->draw_text_font(render::fonts::watermark_font);
        interfaces::surface->draw_text_pos(variables::watermark::x, variables::watermark::y);
        interfaces::surface->get_text_size(render::fonts::watermark_font, converted_name.c_str(), width, height);

        interfaces::surface->set_text_color(watermark_color.r, watermark_color.g, watermark_color.b, watermark_color.a);
        interfaces::surface->draw_render_text(converted_name.c_str(), wcslen(converted_name.c_str()));
        
        interfaces::surface->set_text_color(at_color.r, at_color.g, at_color.b, at_color.a);
        interfaces::surface->draw_render_text(converted_at.c_str(), wcslen(converted_at.c_str()));

        interfaces::surface->set_text_color(watermark_color.r, watermark_color.g, watermark_color.b, watermark_color.a);
        interfaces::surface->draw_render_text(converted_cn.c_str(), wcslen(converted_cn.c_str()));
    } else {
        render::draw_text_string(variables::watermark::x, variables::watermark::y, render::fonts::watermark_font, cheat_name, false, color::red(255));
    }
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

    const std::wstring converted_ts = std::wstring(ts.begin(), ts.end());
    const std::wstring converted_fps = std::wstring(fps.begin(), fps.end());

    interfaces::surface->draw_text_font(font);

    int width, height;
    interfaces::surface->draw_text_pos(x, y);
    interfaces::surface->get_text_size(font, converted_ts.c_str(), width, height);
    interfaces::surface->set_text_color(tscolor.r, tscolor.g, tscolor.b, tscolor.a);
    interfaces::surface->draw_render_text(converted_ts.c_str(), wcslen(converted_ts.c_str()));

    interfaces::surface->set_text_color(fpscolor.r, fpscolor.g, fpscolor.b, fpscolor.a);
    interfaces::surface->draw_render_text(converted_fps.c_str(), wcslen(converted_fps.c_str()));

    if (draw_speed) {
        const std::string speedtext = " | Speed: ";
        const std::wstring converted_speedtext = std::wstring(speedtext.begin(), speedtext.end());
        const std::wstring converted_speed = std::wstring(speed.begin(), speed.end());

        interfaces::surface->set_text_color(tscolor.r, tscolor.g, tscolor.b, tscolor.a);
        interfaces::surface->draw_render_text(converted_speedtext.c_str(), wcslen(converted_speedtext.c_str()));

        interfaces::surface->set_text_color(speedcolor.r, speedcolor.g, speedcolor.b, speedcolor.a);
        interfaces::surface->draw_render_text(converted_speed.c_str(), wcslen(converted_speed.c_str()));
    }
}