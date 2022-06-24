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
	if (!variables::ui::menu::opened) return;

	// Menu title and all that
	do_frame(variables::ui::menu::x, variables::ui::menu::y, variables::ui::menu::w, variables::ui::menu::h,
		color(36, 36, 36, 255), color(25, 25, 25, 255), color(36, 36, 36, 255), "NullHooks");

	const int top_margin = 30;
	const int tab_height = 24;
	const int top_margin_with_tabs = top_margin + tab_height + 1;	// See first column comment (container_width--)
	const int container_margin = 5;		// Empty space between containers
	const int container_padding = 10;	// Space before and after item list (top and bottom)

	/* ------------------ TABS ------------------ */
	const int tab_number = 4;
	const int tab_size = variables::ui::menu::w / tab_number;

	gui::tab(variables::ui::menu::x + (tab_size * 0), variables::ui::menu::y + top_margin, tab_size, tab_height,
		render::fonts::watermark_font, "Aim", menu::current_tab, 0);
	gui::tab(variables::ui::menu::x + (tab_size * 1), variables::ui::menu::y + top_margin, tab_size, tab_height,
		render::fonts::watermark_font, "Visuals", menu::current_tab, 1);
	gui::tab(variables::ui::menu::x + (tab_size * 2), variables::ui::menu::y + top_margin, tab_size, tab_height,
		render::fonts::watermark_font, "Misc", menu::current_tab, 2);
	gui::tab(variables::ui::menu::x + (tab_size * 3), variables::ui::menu::y + top_margin, tab_size, tab_height,
		render::fonts::watermark_font, "Config", menu::current_tab, 3);

	/* ------------------ CONTAINERS ------------------ */
	const int item_slider_length = 80;
	const int item_checkbox_length = 11;
	int container_left_pos = variables::ui::menu::x + container_margin;	// Not const because we can have more than one col
	int container_width = variables::ui::menu::w - container_margin*2;
	int item_left_pos = container_left_pos + container_padding;
	int item_checkbox_pos = variables::ui::menu::x + container_width - container_margin - item_checkbox_length;
	int item_slider_pos = variables::ui::menu::x + container_width - container_margin - item_slider_length;	// Top left corner of the actual slider

	const int part1_y = variables::ui::menu::y + top_margin_with_tabs + container_margin;
	const int part1_base_item_y = part1_y + container_padding;

	switch (current_tab) {
		case 0: {	// Aim
			const int part1_items_num = 6;
			const int part1_h = (15 * part1_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part1_y, container_width, part1_h, render::fonts::watermark_font, "Interface", false); {
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Enable aimbot", variables::aim::aimbot);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "Also target teammates (for deathmatch)", variables::aim::target_friends);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 2), item_checkbox_pos,
					render::fonts::watermark_font, "Enable noscope aimbot on snipers", variables::aim::aimbot_noscope);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 3), item_checkbox_pos,
					render::fonts::watermark_font, "Enable non-rifle aim punch", variables::aim::non_rifle_aimpunch);
				gui::slider(item_left_pos, part1_base_item_y + (15 * 4), item_slider_pos, item_slider_length,
					render::fonts::watermark_font, "Aimbot fov", variables::aim::aimbot_fov, 0.f, 1.f);
				/*gui::check_box(item_left_pos, part1_base_item_y + (15 * 3), item_checkbox_pos,
					render::fonts::watermark_font, "Draw fov", variables::aim::draw_fov);*/
				gui::slider(item_left_pos, part1_base_item_y + (15 * 5), item_slider_pos, item_slider_length,
					render::fonts::watermark_font, "Aimbot smoothing", variables::aim::aimbot_smoothing, 0.f, 1.f);
			}

			break;
		}
		case 1:	{	// Visuals
			const int columns = 2;
			container_width = (container_width / columns) - (container_margin / columns);
			item_checkbox_pos = variables::ui::menu::x + container_width - container_margin - item_checkbox_length;
			item_slider_pos = variables::ui::menu::x + container_width - container_margin - item_slider_length;

			const int part1_items_num = 7;
			const int part1_h = (15 * part1_items_num) + (container_padding * 2) - 4;	// top and bottom - 4 necesary because of the items mult

			gui::group_box(container_left_pos, part1_y, container_width, part1_h, render::fonts::watermark_font, "Player ESP", false); {
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 0), item_checkbox_pos, render::fonts::watermark_font,
					"Enable team ESP (global)", variables::player_visuals::showteamesp, variables::colors::friendly_color, variables::colors::friendly_color_tog);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 1), item_checkbox_pos, render::fonts::watermark_font,
					"Box ESP", variables::player_visuals::boxesp, variables::colors::enemy_color, variables::colors::enemy_color_tog);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 2), item_checkbox_pos, render::fonts::watermark_font,
					"Skeleton ESP", variables::player_visuals::skeletonesp, variables::colors::enemy_color_soft, variables::colors::enemy_color_soft_tog);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 3), item_checkbox_pos,
					render::fonts::watermark_font, "Name ESP", variables::player_visuals::nameesp);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 4), item_checkbox_pos,
					render::fonts::watermark_font, "Player info", variables::player_visuals::playerinfo);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 5), item_checkbox_pos,
					render::fonts::watermark_font, "Health ESP", variables::player_visuals::healthesp);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 6), item_checkbox_pos,
					render::fonts::watermark_font, "Line ESP", variables::player_visuals::lineesp);
			}

			const int part2_items_num = 3;
			const int part2_y = part1_y + part1_h + container_margin;
			const int part2_base_item_y = part2_y + container_padding;
			const int part2_h = (15 * part2_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part2_y, container_width, part2_h, render::fonts::watermark_font, "Glow", false); {
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Player glow", variables::player_visuals::playerglow);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "Entity glow", variables::entity_visuals::entityglow);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 2), item_checkbox_pos,
					render::fonts::watermark_font, "Chicken pride", variables::misc_visuals::chickenpride);
			}

			const int part3_items_num = 11;
			const int part3_y = part2_y + part2_h + container_margin;
			const int part3_base_item_y = part3_y + container_padding;
			const int part3_h = (15 * part3_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part3_y, container_width, part3_h, render::fonts::watermark_font, "Chams", false); {
				gui::check_box(item_left_pos, part3_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Only visible chams", variables::chams::only_visible_chams);
				gui::check_box(item_left_pos, part3_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "Wireframe chams", variables::chams::wireframe_chams);
				gui::check_box(item_left_pos, part3_base_item_y + (15 * 2), item_checkbox_pos,
					render::fonts::watermark_font, "Draw on top", variables::chams::draw_chams_on_top);
				gui::check_box(item_left_pos, part3_base_item_y + (15 * 3), item_checkbox_pos, render::fonts::watermark_font,
					"Player chams", variables::chams::player_chams, variables::colors::chams_vis_enemy_c, variables::colors::chams_vis_enemy_tog);
				gui::combobox(item_left_pos, part3_base_item_y + (15 * 4), item_checkbox_pos + item_checkbox_length,
					render::fonts::watermark_font, "Player chams material", variables::chams::materials, variables::chams::player_chams_mat_id, variables::chams::player_chams_popup_tog);
				gui::check_box(item_left_pos, part3_base_item_y + (15 * 5), item_checkbox_pos, render::fonts::watermark_font,
					"Viewmodel weapon chams", variables::chams::vm_weapon_chams, variables::colors::chams_weapon_c, variables::colors::chams_weapon_c_tog);
				gui::combobox(item_left_pos, part3_base_item_y + (15 * 6), item_checkbox_pos + item_checkbox_length,
					render::fonts::watermark_font, "Weapon chams material", variables::chams::materials, variables::chams::weapon_chams_mat_id, variables::chams::weapon_chams_popup_tog);
				gui::check_box(item_left_pos, part3_base_item_y + (15 * 7), item_checkbox_pos, render::fonts::watermark_font,
					"Arms chams", variables::chams::vm_arm_chams, variables::colors::chams_arms_c, variables::colors::chams_arms_c_tog);
				gui::combobox(item_left_pos, part3_base_item_y + (15 * 8), item_checkbox_pos + item_checkbox_length,
					render::fonts::watermark_font, "Arms chams material", variables::chams::materials, variables::chams::arm_chams_mat_id, variables::chams::arm_chams_popup_tog);
				gui::check_box(item_left_pos, part3_base_item_y + (15 * 9), item_checkbox_pos, render::fonts::watermark_font,
					"Sleeve chams", variables::chams::vm_sleeve_chams, variables::colors::chams_sleeve_c, variables::colors::chams_sleeve_c_tog);
				gui::combobox(item_left_pos, part3_base_item_y + (15 * 10), item_checkbox_pos + item_checkbox_length,
					render::fonts::watermark_font, "Sleeve chams material", variables::chams::materials, variables::chams::sleeve_chams_mat_id, variables::chams::sleeve_chams_popup_tog);
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
					render::fonts::watermark_font, "Bomb timer", variables::entity_visuals::bombtimer);
				gui::check_box(item_left_pos, part4_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "Nade projectile ESP", variables::entity_visuals::nade_esp);
				gui::check_box(item_left_pos, part4_base_item_y + (15 * 2), item_checkbox_pos,
					render::fonts::watermark_font, "Entity info", variables::entity_visuals::entitytext);
			}

			const int part5_items_num = 5;
			const int part5_y = part4_y + part4_h + container_margin;
			const int part5_base_item_y = part5_y + container_padding;
			const int part5_h = (15 * part5_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part5_y, container_width, part5_h, render::fonts::watermark_font, "Misc", false); {
				gui::check_box(item_left_pos, part5_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Nade prediction", variables::misc_visuals::nade_predict);
				gui::slider(item_left_pos, part5_base_item_y + (15 * 1), item_slider_pos, item_slider_length,
					render::fonts::watermark_font, "No flash", variables::misc_visuals::noflash_alpha, 0.f, 1.f);
				gui::check_box(item_left_pos, part5_base_item_y + (15 * 2), item_checkbox_pos,
					render::fonts::watermark_font, "No sniper scope", variables::misc_visuals::noscope);
				gui::check_box(item_left_pos, part5_base_item_y + (15 * 3), item_checkbox_pos, render::fonts::watermark_font,
					"Custom crosshair", variables::misc_visuals::crosshair, variables::colors::crosshair_c, variables::colors::crosshair_c_tog);
				gui::check_box(item_left_pos, part5_base_item_y + (15 * 4), item_checkbox_pos, render::fonts::watermark_font,
					"Recoil crosshair", variables::misc_visuals::recoil_crosshair, variables::colors::recoil_crosshair_c, variables::colors::recoil_crosshair_c_tog);
			}
			break;
		}
		case 2: {	// Misc
			const int part1_items_num = 4;
			const int part1_h = (15 * part1_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part1_y, container_width, part1_h, render::fonts::watermark_font, "Interface", false); {
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Disable cheat on screenshots", variables::misc::clean_screenshots);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "Show watermark", variables::misc::draw_watermark);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 2), item_checkbox_pos,
					render::fonts::watermark_font, "Show stats", variables::misc::draw_stats);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 3), item_checkbox_pos,
					render::fonts::watermark_font, "Spectator list", variables::ui::spectators::spectator_list);
			}

			const int part2_y = part1_y + part1_h + container_margin;
			const int part2_items_num = 5;
			const int part2_base_item_y = part2_y + container_padding;
			const int part2_h = (15 * part2_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part2_y, container_width, part2_h, render::fonts::watermark_font, "Misc", false); {
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Bhop", variables::misc::bhop);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "Draw speedgraph", variables::misc::draw_speedgraph);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 2), item_checkbox_pos,
					render::fonts::watermark_font, "Enable speedgraph color", variables::misc::use_speedgraph_color);
				gui::slider(item_left_pos, part2_base_item_y + (15 * 3), item_slider_pos, item_slider_length,
					render::fonts::watermark_font, "Custom FOV", variables::misc_visuals::custom_fov_slider, 80.f, 130.f);
				gui::slider(item_left_pos, part2_base_item_y + (15 * 4), item_slider_pos, item_slider_length,
					render::fonts::watermark_font, "Custom viewmodel FOV (Mult.)", variables::misc_visuals::custom_vmfov_slider, 0.5f, 2.f);
			}

			// Buttons (start from bottom)
			const int buttons_con_margin_pos = variables::ui::menu::y + variables::ui::menu::h - container_margin;	// Bottom left corner of container
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
			render::draw_text_string(item_left_pos + 2, part1_base_item_y - 1, render::fonts::watermark_font, "Coming soon...", false, color::white(255));
			break;
		}
	}
	// TODO: If the 2 dragable zones are in top of each other, they both get dragged
	spectator_framework::spec_list_movement(variables::ui::spectators::x, variables::ui::spectators::y, variables::ui::spectators::w, variables::ui::spectators::h);
	gui::menu_movement(variables::ui::menu::x, variables::ui::menu::y, variables::ui::menu::w, 30);
}

// Toggle using IsPressed (GetAsyncKeyState)
void menu::check_toggle() {
	if (input::gobal_input.IsPressed(VK_INSERT))
		variables::ui::menu::opened = !variables::ui::menu::opened;
}