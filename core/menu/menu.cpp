#include "menu.hpp"

auto do_frame = [&](std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, color bg, color header_text, color header_line, const std::string& name) {
	// Background
	render::draw_filled_rect(x, y, w, h, bg);
	// Header title
	render::draw_filled_rect(x, y, w, 30, header_text);
	render::draw_filled_rect(x, y + 30, w, 2, header_line);
	render::draw_text_string(x + 10, y + 8, render::fonts::watermark_font, name, false, color::white(255));
};

void menu::render() {
	if (!variables::menu::opened) return;

	const int top_margin = 35;
	const int part1_items_num = 4;
	const int part1_items_margin = 30;
	const int part2_con_margin = part1_items_margin - 5;
	const int part2_items_margin = part1_items_margin - 10;
	const int part1_h = 15 * part1_items_num + part1_items_margin;

	do_frame(variables::menu::x, variables::menu::y, variables::menu::w, variables::menu::h,
		color(36, 36, 36, 255), color(25, 25, 25, 255), color(36, 36, 36, 255), "NullHooks Lite");

	menu_framework::group_box(variables::menu::x + 5, variables::menu::y + top_margin, variables::menu::w - 10, part1_h - 15, render::fonts::watermark_font, "Visuals", false); {
		menu_framework::check_box(variables::menu::x + 15, variables::menu::y + part1_items_margin + (15 * 1), variables::menu::x + variables::menu::w - 25,	// 25 = 15 - check w
			render::fonts::watermark_font, "Enable team ESP", variables::showteamesp_bool);
		menu_framework::check_box(variables::menu::x + 15, variables::menu::y + part1_items_margin + (15 * 2), variables::menu::x + variables::menu::w - 25,
			render::fonts::watermark_font, "Box ESP", variables::boxesp_bool);
		menu_framework::check_box(variables::menu::x + 15, variables::menu::y + part1_items_margin + (15 * 3), variables::menu::x + variables::menu::w - 25,
			render::fonts::watermark_font, "Name ESP", variables::nameesp_bool);
		menu_framework::check_box(variables::menu::x + 15, variables::menu::y + part1_items_margin + (15 * 4), variables::menu::x + variables::menu::w - 25,
			render::fonts::watermark_font, "No flash", variables::noflash_bool);
		//menu_framework::check_box(variables::menu::x + 15, variables::menu::y + part1_items_margin + (15 * 4), variables::menu::x + variables::menu::w - 25,
		//	render::fonts::watermark_font, "Display stats?", variables::draw_stats_bool);
	}

	menu_framework::group_box(variables::menu::x + 5, variables::menu::y + part1_h + part2_con_margin , variables::menu::w - 10 , 60 /*TODO: H*/, render::fonts::watermark_font, "Misc", false); {
		menu_framework::check_box(variables::menu::x + 15, variables::menu::y + part1_h + part2_items_margin + (15 * 1), variables::menu::x + variables::menu::w - 25,
			render::fonts::watermark_font, "Bhop", variables::bhop_bool);
		menu_framework::check_box(variables::menu::x + 15, variables::menu::y + part1_h + part2_items_margin + (15 * 2), variables::menu::x + variables::menu::w - 25,
			render::fonts::watermark_font, "Custom crosshair", variables::crosshair_bool);
		menu_framework::check_box(variables::menu::x + 15, variables::menu::y + part1_h + part2_items_margin + (15 * 3), variables::menu::x + variables::menu::w - 25,
			render::fonts::watermark_font, "Spectator list", variables::spectator_list_bool);
	}

	const int button_items_num = 1;
	const int button_items_h = button_items_num * 15 + 15;
	const int buttons_margin_pos = variables::menu::y + variables::menu::h - 5;
	const int buttons_con_pos = buttons_margin_pos - button_items_h;
	menu_framework::group_box(variables::menu::x + 5, buttons_con_pos, variables::menu::w - 10, button_items_h, render::fonts::watermark_font, "Misc", false); {
		custom_menu_framework::button(variables::menu::x + 15, buttons_con_pos + (15 * 1) - 5, variables::menu::x + variables::menu::w - 25 - 20,
			render::fonts::watermark_font, "Disconnect (Test button)", 1);
	}
	
	menu_framework::menu_movement(variables::menu::x, variables::menu::y, variables::menu::w, 30);
}

void menu::toggle() {
	if (GetAsyncKeyState(VK_INSERT) & 1)
		variables::menu::opened = !variables::menu::opened;
}
