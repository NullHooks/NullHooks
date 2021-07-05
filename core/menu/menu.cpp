#include "menu.hpp"

auto do_frame = [&](std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, color bg, color header_text, color header_line, const std::string& name) {
		render::draw_filled_rect(x, y, w, h, bg);
		render::draw_filled_rect(x, y, w, 30, header_text);
		render::draw_filled_rect(x, y + 30, w, 2, header_line);
		render::draw_text_string(x + 10, y + 8, render::fonts::watermark_font, name, false, color::white());
};

void menu::render() {
	if (!variables::menu::opened)
		return;

	do_frame(variables::menu::x, variables::menu::y, variables::menu::w, variables::menu::h, color(36, 36, 36, 255), color(25, 25, 25, 255), color(36, 36, 36, 255), "NullHooks Tester");
	
	menu_framework::group_box(variables::menu::x + 5, variables::menu::y + 35, 100, 260, render::fonts::watermark_font, "tabs", false); {
		menu_framework::tab(variables::menu::x + 5, variables::menu::y + (260 / 2) - 35, 100, 30, render::fonts::watermark_font, "Aimbot", menu::current_tab, 0, false);
		menu_framework::tab(variables::menu::x + 5, variables::menu::y + (260 / 2), 100, 30, render::fonts::watermark_font, "Visuals", menu::current_tab, 1, false);
		menu_framework::tab(variables::menu::x + 5, variables::menu::y + (260 / 2) + 35, 100, 30, render::fonts::watermark_font, "Misc", menu::current_tab, 2, false);
	}

	switch (current_tab) {
	case 0:
		menu_framework::group_box(variables::menu::x + 110, variables::menu::y + 35, 285, 260, render::fonts::watermark_font, "Aimbot", false); {
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 45, variables::menu::x + 375, render::fonts::watermark_font, "Legit aimbot", variables::aimbot_bool);
			menu_framework::slider(variables::menu::x + 120, variables::menu::y + 60, 125, render::fonts::watermark_font, "Aimbot FOV", variables::aimbot_fov, 0.f, 10.f);
			menu_framework::slider(variables::menu::x + 120, variables::menu::y + 75, 125, render::fonts::watermark_font, "Aimbot smoothing", variables::aimbot_smoothing, 1.f, 5.f);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 90, variables::menu::x + 375, render::fonts::watermark_font, "Only visible", variables::aimbot_isvisiblecheck);
		}
		break;
	case 1:
		menu_framework::group_box(variables::menu::x + 110, variables::menu::y + 35, 285, 260, render::fonts::watermark_font, "Visuals", false); {
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 45, variables::menu::x + 375, render::fonts::watermark_font, "Team glow", variables::showteamesp);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 75, variables::menu::x + 375, render::fonts::watermark_font, "Glow ESP", variables::glowesp);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 90, variables::menu::x + 375, render::fonts::watermark_font, "Box ESP", variables::boxesp);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 105, variables::menu::x + 375, render::fonts::watermark_font, "Name ESP", variables::nameesp);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 120, variables::menu::x + 375, render::fonts::watermark_font, "Health ESP", variables::healthesp);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 135, variables::menu::x + 375, render::fonts::watermark_font, "Line ESP", variables::snaplineesp);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 150, variables::menu::x + 375, render::fonts::watermark_font, "C4 ESP", variables::drawc4);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 180, variables::menu::x + 375, render::fonts::watermark_font, "No flash", variables::noflash_bool);
		}
		break;
	case 2:
		menu_framework::group_box(variables::menu::x + 110, variables::menu::y + 35, 285, 260, render::fonts::watermark_font, "Misc", false); {
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 45, variables::menu::x + 375, render::fonts::watermark_font, "Bhop", variables::bhop_bool);
			menu_framework::check_box(variables::menu::x + 120, variables::menu::y + 60, variables::menu::x + 375, render::fonts::watermark_font, "Spectator list", variables::spectator_list_bool);
		}
		break;
	}

	menu_framework::menu_movement(variables::menu::x, variables::menu::y, variables::menu::w, 30);
}

void menu::toggle() {
	if (GetAsyncKeyState(VK_INSERT) & 1)
		variables::menu::opened = !variables::menu::opened;
}
