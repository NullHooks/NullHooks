#include "menu.hpp"

auto do_frame = [&](std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, color bg, color header_text, color header_line, const std::string& name) {
		// Background	
		render::draw_filled_rect(x, y, w, h, bg);
		// Header title
		render::draw_filled_rect(x, y, w, 30, header_text);
		render::draw_filled_rect(x, y + 30, w, 2, header_line);
		render::draw_text_string(x + 10, y + 8, render::fonts::watermark_font, name, false, color::white());
};

void menu::render() {
	if (!variables::menu::opened)
		return;

	do_frame(variables::menu::x, variables::menu::y, variables::menu::w, variables::menu::h, color(36, 36, 36, 255), color(25, 25, 25, 255), color(36, 36, 36, 255), "NullHooks Lite");

	menu_framework::group_box(variables::menu::x + 5, variables::menu::y + 35, 290, 70, render::fonts::watermark_font, "Visuals", false); {
		menu_framework::check_box(variables::menu::x + 15, variables::menu::y + 45, variables::menu::x + 275, render::fonts::watermark_font, "Team glow", variables::showteamesp);
		menu_framework::check_box(variables::menu::x + 15, variables::menu::y + 60, variables::menu::x + 275, render::fonts::watermark_font, "Box ESP", variables::boxesp);
		menu_framework::check_box(variables::menu::x + 15, variables::menu::y + 85, variables::menu::x + 275, render::fonts::watermark_font, "No flash", variables::noflash_bool);
	}

	menu_framework::group_box(variables::menu::x + 5, variables::menu::y + 110, 290, 60, render::fonts::watermark_font, "Misc", false); {
		menu_framework::check_box(variables::menu::x + 15, variables::menu::y + 120, variables::menu::x + 275, render::fonts::watermark_font, "Bhop", variables::bhop_bool);
		menu_framework::check_box(variables::menu::x + 15, variables::menu::y + 135, variables::menu::x + 275, render::fonts::watermark_font, "Dot crosshair", variables::crosshair_bool);
		menu_framework::check_box(variables::menu::x + 15, variables::menu::y + 150, variables::menu::x + 275, render::fonts::watermark_font, "Spectator list", variables::spectator_list_bool);
	}
	
	menu_framework::menu_movement(variables::menu::x, variables::menu::y, variables::menu::w, 30);
}

void menu::toggle() {
	if (GetAsyncKeyState(VK_INSERT) & 1)
		variables::menu::opened = !variables::menu::opened;
}
