#include "menu.hpp"
#include "../features/features.hpp"

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

	// Menu title and all that
	do_frame(variables::menu::x, variables::menu::y, variables::menu::w, variables::menu::h,
		color(36, 36, 36, 255), color(25, 25, 25, 255), color(36, 36, 36, 255), "NullHooks");

	const int top_margin = 30;
	const int tab_height = 24;
	const int top_margin_with_tabs = top_margin + tab_height;
	const int container_margin = 5;		// Empty space between containers
	const int container_padding = 10;	// Space before and after item list (top and bottom)

	/* ------------------ TABS ------------------ */
	const int tab_number = 4;
	const int tab_size = variables::menu::w / tab_number;

	gui::tab(variables::menu::x + (tab_size * 0), variables::menu::y + top_margin, tab_size, tab_height,
		render::fonts::watermark_font, "Aim", menu::current_tab, 0);
	gui::tab(variables::menu::x + (tab_size * 1), variables::menu::y + top_margin, tab_size, tab_height,
		render::fonts::watermark_font, "Visuals", menu::current_tab, 1);
	gui::tab(variables::menu::x + (tab_size * 2), variables::menu::y + top_margin, tab_size, tab_height,
		render::fonts::watermark_font, "Misc", menu::current_tab, 2);
	gui::tab(variables::menu::x + (tab_size * 3), variables::menu::y + top_margin, tab_size, tab_height,
		render::fonts::watermark_font, "Config", menu::current_tab, 3);

	/* ------------------ CONTAINERS ------------------ */
	const int item_slider_length = 80;
	const int item_checkbox_length = 10;
	int container_left_pos = variables::menu::x + container_margin;	// Not const because we can have more than one col
	int container_width = variables::menu::w - container_margin*2;
	int item_left_pos = container_left_pos + container_padding;
	int item_checkbox_pos = variables::menu::x + container_width - container_margin - item_checkbox_length;
	int item_slider_pos = variables::menu::x + container_width - container_margin - item_slider_length;	// top left corner of the actual slider

	const int part1_y = variables::menu::y + top_margin_with_tabs + container_margin;
	const int part1_base_item_y = part1_y + container_padding;	// container pos + margin

	switch (current_tab) {
		case 0:		// Aim
			break;
		case 1:	{	// Visuals
			const int columns = 2;
			// We need (margin / cols * 2) to remove 25%
			container_width = container_width / columns - (container_margin / columns);
			item_checkbox_pos = variables::menu::x + container_width - container_margin - item_checkbox_length;

			const int part1_items_num = 7;
			const int part1_h = (15 * part1_items_num) + (container_padding * 2) - 4;	// top and bottom - 4 necesary because of the items mult

			gui::group_box(container_left_pos, part1_y, container_width, part1_h, render::fonts::watermark_font, "Player ESP", false); {
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Enable team ESP", variables::showteamesp_bool);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "Box ESP", variables::boxesp_bool);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 2), item_checkbox_pos,
					render::fonts::watermark_font, "Skeleton ESP", variables::skeletonesp_bool);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 3), item_checkbox_pos,
					render::fonts::watermark_font, "Name ESP", variables::nameesp_bool);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 4), item_checkbox_pos,
					render::fonts::watermark_font, "Player info", variables::playerinfo_bool);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 5), item_checkbox_pos,
					render::fonts::watermark_font, "Health ESP", variables::healthesp_bool);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 6), item_checkbox_pos,
					render::fonts::watermark_font, "Line ESP", variables::lineesp_bool);
			}

			const int part2_items_num = 3;
			const int part2_y = part1_y + part1_h + container_margin;
			const int part2_base_item_y = part2_y + container_padding;
			const int part2_h = (15 * part2_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part2_y, container_width, part2_h, render::fonts::watermark_font, "Glow", false); {
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Player glow", variables::playerglow_bool);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "Entity glow", variables::entityglow_bool);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 2), item_checkbox_pos,
					render::fonts::watermark_font, "Chicken pride", variables::chickenpride_bool);
			}

			const int part3_items_num = 2;
			const int part3_y = part2_y + part2_h + container_margin;
			const int part3_base_item_y = part3_y + container_padding;
			const int part3_h = (15 * part3_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part3_y, container_width, part3_h, render::fonts::watermark_font, "Chams", false); {
				gui::check_box(item_left_pos, part3_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Enable chams", variables::enable_chams_bool);
				gui::check_box(item_left_pos, part3_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "Only if visible", variables::only_visible_chams_bool);
			}

			/* ----- Visuals - Second column ----- */

			int column_number = 1;
			container_width--;	// Not the best way to do it, but the margin on the right was always smaller because of (5/2=2)
			container_left_pos = container_left_pos + (container_width * column_number) + container_margin;
			item_left_pos = item_left_pos + (container_width * column_number) + container_margin;
			item_checkbox_pos = item_checkbox_pos + (container_width * column_number) + container_margin;
			item_slider_pos = item_slider_pos + (container_width * column_number) + container_margin;

			const int part4_items_num = 3;
			const int part4_y = part1_y;	// Needs to reset y pos on new col
			const int part4_base_item_y = part1_base_item_y;
			const int part4_h = (15 * part4_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part4_y, container_width, part4_h, render::fonts::watermark_font, "Other ESP", false); {
				gui::check_box(item_left_pos, part4_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Bomb timer", variables::bombtimer_bool);
				gui::check_box(item_left_pos, part4_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "Nade projectile ESP", variables::nade_esp_bool);
				gui::check_box(item_left_pos, part4_base_item_y + (15 * 2), item_checkbox_pos,
					render::fonts::watermark_font, "Entity info", variables::entitytext_bool);
			}

			const int part5_items_num = 5;
			const int part5_y = part4_y + part4_h + container_margin;
			const int part5_base_item_y = part5_y + container_padding;
			const int part5_h = (15 * part5_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part5_y, container_width, part5_h, render::fonts::watermark_font, "Misc", false); {
				gui::check_box(item_left_pos, part5_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Nade prediction", variables::nade_predict_bool);
				gui::check_box(item_left_pos, part5_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "No flash", variables::noflash_bool);
				gui::check_box(item_left_pos, part5_base_item_y + (15 * 2), item_checkbox_pos,
					render::fonts::watermark_font, "No sniper scope", variables::noscope_bool);
				gui::check_box(item_left_pos, part5_base_item_y + (15 * 3), item_checkbox_pos,
					render::fonts::watermark_font, "Custom crosshair", variables::crosshair_bool);
				gui::check_box(item_left_pos, part5_base_item_y + (15 * 4), item_checkbox_pos,
					render::fonts::watermark_font, "Recoil crosshair", variables::recoil_crosshair_bool);
			}
			break;
		}
		case 2: {	// Misc
			const int part1_items_num = 4;
			const int part1_h = (15 * part1_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part1_y, container_width, part1_h, render::fonts::watermark_font, "Interface", false); {
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Disable cheat on screenshots", variables::clean_screenshots_bool);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "Show watermark", variables::draw_watermark);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 2), item_checkbox_pos,
					render::fonts::watermark_font, "Show stats", variables::draw_stats);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 3), item_checkbox_pos,
					render::fonts::watermark_font, "Spectator list", variables::spectators::spectator_list_bool);
			}

			const int part2_y = part1_y + part1_h + container_margin;
			const int part2_items_num = 3;
			const int part2_base_item_y = part2_y + container_padding;
			const int part2_h = (15 * part2_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part2_y, container_width, part2_h, render::fonts::watermark_font, "Misc", false); {
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Bhop", variables::bhop_bool);
				gui::slider(item_left_pos,    part2_base_item_y + (15 * 1), item_slider_pos, item_slider_length,
					render::fonts::watermark_font, "Custom FOV", variables::custom_fov_slider, 80.f, 130.f);
				gui::slider(item_left_pos,    part2_base_item_y + (15 * 2), item_slider_pos, item_slider_length,
					render::fonts::watermark_font, "Custom viewmodel FOV (Mult.)", variables::custom_vmfov_slider, 0.5f, 2.f);
			}

			// Buttons (start from bottom)
			const int buttons_con_margin_pos = variables::menu::y + variables::menu::h - container_margin;	// Bottom left corner of container
			const int button_items_num = 1;
			const int button_items_h = (button_items_num * 15) + (container_padding * 2) - 4;
			const int buttons_con_y = buttons_con_margin_pos - button_items_h;		// Get the top left corner based on the margin pos and the height
			const int buttons_items_base_y = buttons_con_y + container_padding;		// Same as other containers

			gui::group_box(container_left_pos, buttons_con_y, container_width, button_items_h, render::fonts::watermark_font, "Buttons", false); {
				gui::button(item_left_pos, buttons_items_base_y + (15 * 0), item_checkbox_pos - 20,	// Bigger "checkbox" as button. TODO: Pass size and pos
					render::fonts::watermark_font, "Autoexec (callback test)", commands_features::exec_autoexec);
			}
			break;
		}
		case 3: {	// Config
			break;
		}
	}
	// TODO: If the 2 dragable zones are in top of each other, they both get dragged
	spectator_framework::spec_list_movement(variables::spectators::x, variables::spectators::y, variables::spectators::w, variables::spectators::h);
	gui::menu_movement(variables::menu::x, variables::menu::y, variables::menu::w, 30);
}

void menu::toggle() {
	if (GetAsyncKeyState(VK_INSERT) & 1)
		variables::menu::opened = !variables::menu::opened;
}
