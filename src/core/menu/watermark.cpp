#include "core/menu/menu.hpp"

/*
 * Based on based repos:
 * https://github.com/notbemji/templehook 
 * https://github.com/notbemji/5shekelcheats
 */

void watermark::draw() {
    if (!variables::misc::draw_watermark) return;
    const std::string cheat_name = "NullHooks";
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
        interfaces::surface->draw_text_pos(variables::ui::watermark::x, variables::ui::watermark::y);
        interfaces::surface->get_text_size(render::fonts::watermark_font, converted_name.c_str(), width, height);

        interfaces::surface->set_text_color(watermark_color.r, watermark_color.g, watermark_color.b, watermark_color.a);
        interfaces::surface->draw_render_text(converted_name.c_str(), wcslen(converted_name.c_str()));
        
        interfaces::surface->set_text_color(at_color.r, at_color.g, at_color.b, at_color.a);
        interfaces::surface->draw_render_text(converted_at.c_str(), wcslen(converted_at.c_str()));

        interfaces::surface->set_text_color(watermark_color.r, watermark_color.g, watermark_color.b, watermark_color.a);
        interfaces::surface->draw_render_text(converted_cn.c_str(), wcslen(converted_cn.c_str()));
    } else {
        render::draw_text_string(variables::ui::watermark::x, variables::ui::watermark::y, render::fonts::watermark_font, cheat_name, false, color::red(255));
    }
}

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

    if (cl_updaterate->float_value > 0.001f)
        average_latency += -0.5f / cl_updaterate->float_value;

    const int num = std::abs(average_latency) * 1000.0f;

    return (num < 1000) ? num : -1;
}

// Will draw the actual string based on the values and lengths
void draw_stats_string(std::string ts, color ts_col, std::string fps, color fps_col, std::string ping, color ping_col, bool draw_speed) {
    const int x = variables::ui::watermark::x;
    const int y = variables::ui::watermark::y + 12;
    const unsigned long font = render::fonts::watermark_font;
    const int margin = 40;

    const std::wstring converted_ts = std::wstring(ts.begin(), ts.end());
    const std::wstring converted_fps = std::wstring(fps.begin(), fps.end());

    interfaces::surface->draw_text_font(font);

    int width, height;
    interfaces::surface->draw_text_pos(x, y);
    interfaces::surface->get_text_size(font, converted_ts.c_str(), width, height);
    interfaces::surface->set_text_color(ts_col.r, ts_col.g, ts_col.b, ts_col.a);
    interfaces::surface->draw_render_text(converted_ts.c_str(), wcslen(converted_ts.c_str()));

    interfaces::surface->set_text_color(fps_col.r, fps_col.g, fps_col.b, fps_col.a);
    interfaces::surface->draw_render_text(converted_fps.c_str(), wcslen(converted_fps.c_str()));

    if (draw_speed) {
        const std::string pingtext = " | Ping: ";
        const std::wstring converted_pingtext = std::wstring(pingtext.begin(), pingtext.end());
        const std::wstring converted_ping = std::wstring(ping.begin(), ping.end());

        interfaces::surface->set_text_color(ts_col.r, ts_col.g, ts_col.b, ts_col.a);
        interfaces::surface->draw_render_text(converted_pingtext.c_str(), wcslen(converted_pingtext.c_str()));

        interfaces::surface->set_text_color(ping_col.r, ping_col.g, ping_col.b, ping_col.a);
        interfaces::surface->draw_render_text(converted_ping.c_str(), wcslen(converted_ping.c_str()));
    }
}

// Will get the stats and colors and pass them to draw_stats_string()
void watermark::draw_stats() {
    if (!variables::misc::draw_stats) return;

    // Colors
    const color base_color = color(220,   5,   5, 255);
    const color color_l    = color(255, 150,   0, 255);
    const color color_m    = color(255, 255,   0, 255);
    const color color_h    = color(  0, 255,  10, 255);

    color fps_color = base_color;

    const int fps = get_fps();
    if (fps < 80)      fps_color = color_l;
    else if (fps < 110) fps_color = color_m;
    else                fps_color = color_h;

    const int ping = get_ping();
    if (ping >= 0) {
        color ping_color = base_color;
        if (ping < 50)      ping_color = color_h;
        else if (ping < 75) ping_color = color_m;
        else                ping_color = color_l;

        draw_stats_string(get_timestamp_string() + " | FPS: ", base_color, std::to_string(fps), fps_color, std::to_string(ping), ping_color, true);
    } else {
        draw_stats_string(get_timestamp_string() + " | FPS: ", base_color, std::to_string(fps), fps_color, "", base_color, false);
    }
}