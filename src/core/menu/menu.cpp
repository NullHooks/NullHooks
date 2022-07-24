#include "core/features/features.hpp"
#include "core/menu/menu.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"
#include "core/config/config.hpp"

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

	constexpr int top_margin           = 30;
	constexpr int tab_height           = 24;
	constexpr int top_margin_with_tabs = top_margin + tab_height + 1;	// See first column comment (container_width--)
	constexpr int container_margin     = 5;								// Empty space between containers
	constexpr int container_padding    = 10;							// Space before and after item list (top and bottom)

	/* ------------------ TABS ------------------ */
	constexpr int tab_number = 4;
	const int tab_size   = variables::ui::menu::w / tab_number;

	gui::tab(variables::ui::menu::x + (tab_size * 0), variables::ui::menu::y + top_margin, tab_size, tab_height,
		render::fonts::watermark_font, "Aim", menu::current_tab, 0);
	gui::tab(variables::ui::menu::x + (tab_size * 1), variables::ui::menu::y + top_margin, tab_size, tab_height,
		render::fonts::watermark_font, "Visuals", menu::current_tab, 1);
	gui::tab(variables::ui::menu::x + (tab_size * 2), variables::ui::menu::y + top_margin, tab_size, tab_height,
		render::fonts::watermark_font, "Misc", menu::current_tab, 2);
	gui::tab(variables::ui::menu::x + (tab_size * 3), variables::ui::menu::y + top_margin, tab_size, tab_height,
		render::fonts::watermark_font, "Config", menu::current_tab, 3);

	/* ------------------ CONTAINERS ------------------ */
	constexpr int item_slider_length   = 80;
	constexpr int item_checkbox_length = 11;
	int container_left_pos         = variables::ui::menu::x + container_margin;	// Not const because we can have more than one col
	int container_width            = variables::ui::menu::w - container_margin*2;
	int item_left_pos              = container_left_pos + container_padding;
	int item_checkbox_pos          = variables::ui::menu::x + container_width - container_margin - item_checkbox_length;
	int item_slider_pos            = variables::ui::menu::x + container_width - container_margin - item_slider_length;	// Top left corner of the actual slider
	int item_combo_pos             = item_checkbox_pos + item_checkbox_length;
	int item_hotkey_w              = container_width - container_padding*2;

	const int part1_y           = variables::ui::menu::y + top_margin_with_tabs + container_margin;
	const int part1_base_item_y = part1_y + container_padding;

	switch (current_tab) {
		case 0: {	// Aim
			const int columns = 2;
			container_width = (container_width / columns) - (container_margin / columns);
			item_checkbox_pos = variables::ui::menu::x + container_width - container_margin - item_checkbox_length;
			item_slider_pos = variables::ui::menu::x + container_width - container_margin - item_slider_length;
			item_combo_pos = item_checkbox_pos + item_checkbox_length;
			item_hotkey_w = container_width - container_padding * 2;

			const int part1_items_num = 3;
			const int part1_h         = (15 * part1_items_num) + (container_padding * 2) - 4;
			
			gui::group_box(container_left_pos, part1_y, container_width, part1_h, render::fonts::watermark_font, "General", false); {
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Also target teammates", variables::aim::target_friends);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "Enable noscope aimbot on snipers", variables::aim::aimbot_noscope);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 2), item_checkbox_pos,
					render::fonts::watermark_font, "Enable non-rifle aim punch", variables::aim::non_rifle_aimpunch);
			}

			const int part2_items_num   = 3;
			const int part2_y           = part1_y + part1_h + container_margin;
			const int part2_base_item_y = part2_y + container_padding;
			const int part2_h           = (15 * part2_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part2_y, container_width, part2_h, render::fonts::watermark_font, "Triggerbot", false); {
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Enable triggerbot", variables::aim::triggerbot);
				gui::hotkey(item_left_pos, part2_base_item_y + (15 * 1), item_hotkey_w,
					render::fonts::watermark_font, "Triggerbot key", variables::aim::triggerbot_key);
				gui::slider(item_left_pos, part2_base_item_y + (15 * 2), item_slider_pos, item_slider_length,
					render::fonts::watermark_font, "Triggerbot delay", variables::aim::triggerbot_delay, 0.f, 30.f);
			}

			const int part3_items_num   = 11;
			const int part3_y           = part2_y + part2_h + container_margin;
			const int part3_base_item_y = part3_y + container_padding;
			const int part3_h           = (15 * part3_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part3_y, container_width, part3_h, render::fonts::watermark_font, "Aimbot", false); {
				gui::check_box(item_left_pos, part3_base_item_y + (15 * 0), item_checkbox_pos, render::fonts::watermark_font,
					"Enable aimbot", variables::aim::aimbot);
				gui::check_box(item_left_pos, part3_base_item_y + (15 * 1), item_checkbox_pos, render::fonts::watermark_font,
					"Autofire", variables::aim::autofire);
				gui::hotkey(item_left_pos, part3_base_item_y + (15 * 2), item_hotkey_w, render::fonts::watermark_font,
					"Only on key", variables::aim::aimbot_key);
				gui::check_box(item_left_pos, part3_base_item_y + (15 * 3), item_checkbox_pos, render::fonts::watermark_font,
					"Silent", variables::aim::silent);
				gui::combobox(item_left_pos, part3_base_item_y + (15 * 4), item_combo_pos, render::fonts::watermark_font,
					"Autowall", variables::aim::autowall_settings, variables::aim::autowall);
				gui::multicombobox(item_left_pos, part3_base_item_y + (15 * 5), item_combo_pos, render::fonts::watermark_font,
					"Aimbot hitboxes", variables::aim::hitboxes);
				gui::check_box(item_left_pos, part3_base_item_y + (15 * 6), item_checkbox_pos, render::fonts::watermark_font,
					"Bodyaim if lethal", variables::aim::bodyaim_if_lethal);
				gui::slider(item_left_pos, part3_base_item_y + (15 * 7), item_slider_pos, item_slider_length, render::fonts::watermark_font,
					"Minimum damage", variables::aim::min_damage, 0.f, 100.f);
				gui::slider(item_left_pos, part3_base_item_y + (15 * 8), item_slider_pos, item_slider_length, render::fonts::watermark_font,
					"Aimbot fov", variables::aim::aimbot_fov, 0.f, 180.f);
				gui::check_box(item_left_pos, part3_base_item_y + (15 * 9), item_checkbox_pos, render::fonts::watermark_font,
					"Draw fov", variables::aim::draw_fov, variables::colors::aimbot_fov_c);
				gui::slider(item_left_pos, part3_base_item_y + (15 * 10), item_slider_pos, item_slider_length, render::fonts::watermark_font,
					"Aimbot smoothing", variables::aim::aimbot_smoothing, 0.f, 1.f);
			}

			int column_number = 1;
			container_width--;	// Not the best way to do it, but the margin on the right was always smaller because of (5/2=2)
			container_left_pos = container_left_pos + (container_width * column_number) + container_margin;
			item_left_pos = item_left_pos + (container_width * column_number) + container_margin;
			item_checkbox_pos = item_checkbox_pos + (container_width * column_number) + container_margin;
			item_slider_pos = item_slider_pos + (container_width * column_number) + container_margin;
			item_combo_pos = item_checkbox_pos + item_checkbox_length;

			const int part1c2_items_num = 3;			// part1c2 => part 1 from column 2
			const int part1c2_y = part1_y;	// Needs to reset y pos on new col
			const int part1c2_base_item_y = part1_base_item_y;
			const int part1c2_h = (15 * part1c2_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part1c2_y, container_width, part1c2_h, render::fonts::watermark_font, "Antiaim", false); {
				gui::check_box(item_left_pos, part1c2_base_item_y + (15 * 0), item_checkbox_pos, render::fonts::watermark_font,
					"AntiAim", variables::antiaim::antiaim);
				gui::slider(item_left_pos, part1c2_base_item_y + (15 * 1), item_slider_pos, item_slider_length, render::fonts::watermark_font,
					"Pitch (x)", variables::antiaim::pitch, static_cast<float>(-89), static_cast<float>(89));
				gui::slider(item_left_pos, part1c2_base_item_y + (15 * 2), item_slider_pos, item_slider_length, render::fonts::watermark_font,
					"Yaw (y)", variables::antiaim::yaw, static_cast<float>(-180), static_cast<float>(180));
			}

			break;
		}
		case 1:	{	// Visuals
			const int columns = 2;
			container_width   = (container_width / columns) - (container_margin / columns);
			item_checkbox_pos = variables::ui::menu::x + container_width - container_margin - item_checkbox_length;
			item_slider_pos   = variables::ui::menu::x + container_width - container_margin - item_slider_length;
			item_combo_pos    = item_checkbox_pos + item_checkbox_length;
			item_hotkey_w     = container_width - container_padding * 2;

			const int part1_items_num = 7;
			const int part1_h         = (15 * part1_items_num) + (container_padding * 2) - 4;	// top and bottom - 4 necesary because of the items mult

			gui::group_box(container_left_pos, part1_y, container_width, part1_h, render::fonts::watermark_font, "Player ESP", false); {
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 0), item_checkbox_pos, render::fonts::watermark_font,
					"Enable team ESP (global)", variables::player_visuals::showteamesp, variables::colors::friendly_color);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 1), item_checkbox_pos, render::fonts::watermark_font,
					"Box ESP", variables::player_visuals::boxesp, variables::colors::enemy_color);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 2), item_checkbox_pos, render::fonts::watermark_font,
					"Skeleton ESP", variables::player_visuals::skeletonesp, variables::colors::enemy_color_soft);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 3), item_checkbox_pos, render::fonts::watermark_font,
					"Name ESP", variables::player_visuals::nameesp);
				gui::multicombobox(item_left_pos, part1_base_item_y + (15 * 4), item_combo_pos, render::fonts::watermark_font,
					"Player info", variables::player_visuals::playerinfo);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 5), item_checkbox_pos, render::fonts::watermark_font,
					"Health ESP", variables::player_visuals::healthesp);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 6), item_checkbox_pos, render::fonts::watermark_font,
					"Line ESP", variables::player_visuals::lineesp);
			}

			const int part2_items_num   = 13;
			const int part2_y           = part1_y + part1_h + container_margin;
			const int part2_base_item_y = part2_y + container_padding;
			const int part2_h           = (15 * part2_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part2_y, container_width, part2_h, render::fonts::watermark_font, "Chams", false); {
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 0), item_checkbox_pos, render::fonts::watermark_font,
					"Only visible chams", variables::chams::only_visible_chams);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 1), item_checkbox_pos, render::fonts::watermark_font,
					"Wireframe chams", variables::chams::wireframe_chams);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 2), item_checkbox_pos, render::fonts::watermark_font,
					"Draw on top", variables::chams::draw_chams_on_top);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 3), item_checkbox_pos, render::fonts::watermark_font,
					"Player chams", variables::chams::player_chams, variables::colors::chams_vis_friend_c, variables::colors::chams_vis_enemy_c);
				gui::combobox(item_left_pos, part2_base_item_y + (15 * 4), item_combo_pos, render::fonts::watermark_font,
					"Player chams material", variables::chams::materials, variables::chams::player_chams_mat_id);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 5), item_checkbox_pos, render::fonts::watermark_font,
					"Local player chams", variables::chams::localplayer_chams, variables::colors::chams_localplayer);
				gui::combobox(item_left_pos, part2_base_item_y + (15 * 6), item_combo_pos, render::fonts::watermark_font,
					"Local chams material", variables::chams::materials, variables::chams::localplayer_chams_mat_id);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 7), item_checkbox_pos, render::fonts::watermark_font,
					"Viewmodel weapon chams", variables::chams::vm_weapon_chams, variables::colors::chams_weapon_c);
				gui::combobox(item_left_pos, part2_base_item_y + (15 * 8), item_combo_pos, render::fonts::watermark_font,
					"Weapon chams material", variables::chams::materials, variables::chams::weapon_chams_mat_id);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 9), item_checkbox_pos, render::fonts::watermark_font,
					"Arms chams", variables::chams::vm_arm_chams, variables::colors::chams_arms_c);
				gui::combobox(item_left_pos, part2_base_item_y + (15 * 10), item_combo_pos, render::fonts::watermark_font,
					"Arms chams material", variables::chams::materials, variables::chams::arm_chams_mat_id);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 11), item_checkbox_pos, render::fonts::watermark_font,
					"Sleeve chams", variables::chams::vm_sleeve_chams, variables::colors::chams_sleeve_c);
				gui::combobox(item_left_pos, part2_base_item_y + (15 * 12), item_combo_pos, render::fonts::watermark_font,
					"Sleeve chams material", variables::chams::materials, variables::chams::sleeve_chams_mat_id);
			}

			/* ----- Visuals - Second column ----- */

			int column_number  = 1;
			container_width--;	// Not the best way to do it, but the margin on the right was always smaller because of (5/2=2)
			container_left_pos = container_left_pos + (container_width * column_number) + container_margin;
			item_left_pos      = item_left_pos + (container_width * column_number) + container_margin;
			item_checkbox_pos  = item_checkbox_pos + (container_width * column_number) + container_margin;
			item_slider_pos    = item_slider_pos + (container_width * column_number) + container_margin;
			item_combo_pos     = item_checkbox_pos + item_checkbox_length;

			const int part1c2_items_num   = 3;			// part1c2 => part 1 from column 2
			const int part1c2_y           = part1_y;	// Needs to reset y pos on new col
			const int part1c2_base_item_y = part1_base_item_y;
			const int part1c2_h           = (15 * part1c2_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part1c2_y, container_width, part1c2_h, render::fonts::watermark_font, "Glow", false); {
				gui::check_box(item_left_pos, part1c2_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Player glow", variables::player_visuals::playerglow);
				gui::check_box(item_left_pos, part1c2_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "Entity glow", variables::entity_visuals::entityglow);
				gui::check_box(item_left_pos, part1c2_base_item_y + (15 * 2), item_checkbox_pos,
					render::fonts::watermark_font, "Chicken pride", variables::misc_visuals::chickenpride);
			}

			const int part2c2_items_num   = 3;
			const int part2c2_y           = part1c2_y + part1c2_h + container_margin;
			const int part2c2_base_item_y = part2c2_y + container_padding;
			const int part2c2_h           = (15 * part2c2_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part2c2_y, container_width, part2c2_h, render::fonts::watermark_font, "Other ESP", false); {
				gui::check_box(item_left_pos, part2c2_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Bomb timer", variables::entity_visuals::bombtimer);
				gui::check_box(item_left_pos, part2c2_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "Nade projectile ESP", variables::entity_visuals::nade_esp);
				gui::check_box(item_left_pos, part2c2_base_item_y + (15 * 2), item_checkbox_pos,
					render::fonts::watermark_font, "Entity info", variables::entity_visuals::entitytext);
			}

			const int part3c2_items_num   = 8;
			const int part3c2_y           = part2c2_y + part2c2_h + container_margin;
			const int part3c2_base_item_y = part3c2_y + container_padding;
			const int part3c2_h           = (15 * part3c2_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part3c2_y, container_width, part3c2_h, render::fonts::watermark_font, "Misc", false); {
				gui::slider(item_left_pos, part3c2_base_item_y + (15 * 0), item_slider_pos, item_slider_length, render::fonts::watermark_font,
					"No flash", variables::misc_visuals::noflash_alpha, 0.f, 1.f);
				gui::check_box(item_left_pos, part3c2_base_item_y + (15 * 1), item_checkbox_pos, render::fonts::watermark_font,
					"Wireframe smoke", variables::misc_visuals::wireframe_smoke);
				gui::check_box(item_left_pos, part3c2_base_item_y + (15 * 2), item_checkbox_pos, render::fonts::watermark_font,
					"Nade prediction", variables::misc_visuals::nade_predict);
				gui::check_box(item_left_pos, part3c2_base_item_y + (15 * 3), item_checkbox_pos, render::fonts::watermark_font,
					"No sniper scope", variables::misc_visuals::noscope);
				gui::multicombobox(item_left_pos, part3c2_base_item_y + (15 * 4), item_combo_pos, render::fonts::watermark_font,
					"Bullet tracers", variables::misc_visuals::bulletracer_draw_target);
				gui::multicombobox(item_left_pos, part3c2_base_item_y + (15 * 5), item_combo_pos, render::fonts::watermark_font,
					"Bullet tracer teams", variables::misc_visuals::bulletracer_team_target);
				gui::check_box(item_left_pos, part3c2_base_item_y + (15 * 6), item_checkbox_pos, render::fonts::watermark_font,
					"Custom crosshair", variables::misc_visuals::crosshair, variables::colors::crosshair_c);
				gui::check_box(item_left_pos, part3c2_base_item_y + (15 * 7), item_checkbox_pos, render::fonts::watermark_font,
					"Recoil crosshair", variables::misc_visuals::recoil_crosshair, variables::colors::recoil_crosshair_c);
			}

			break;
		}
		case 2: {	// Misc
			const int columns              = 2;
			const int container_width_o    = container_width;			// Original used for buttons
			const int container_left_pos_o = container_left_pos;		// Original used for buttons
			const int item_left_pos_o      = item_left_pos;				// Original used for buttons
			const int item_checkbox_pos_o  = item_checkbox_pos;			// Original used for buttons
			const int item_slider_pos_o    = item_slider_pos;			// Original used for buttons
			const int item_hotkey_w_o      = item_hotkey_w;				// Original used for buttons
			container_width                = (container_width / columns) - (container_margin / columns);
			item_checkbox_pos              = variables::ui::menu::x + container_width - container_margin - item_checkbox_length;
			item_slider_pos                = variables::ui::menu::x + container_width - container_margin - item_slider_length;
			item_combo_pos                 = item_checkbox_pos + item_checkbox_length;
			item_hotkey_w                  = container_width - container_padding * 2;

			const int part1_items_num = 3;
			const int part1_h         = (15 * part1_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part1_y, container_width, part1_h, render::fonts::watermark_font, "Network", false); {
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Backtrack", variables::misc::backtrack);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "Also teammates", variables::misc::backtrack_team);
				gui::check_box(item_left_pos, part1_base_item_y + (15 * 2), item_checkbox_pos,
					render::fonts::watermark_font, "Backtrack chams", variables::chams::backtrack_chams, variables::colors::bt_chams_friend, variables::colors::bt_chams_enemy);
			}

			const int part2_y = part1_y + part1_h + container_margin;
			const int part2_items_num = 11;
			const int part2_base_item_y = part2_y + container_padding;
			const int part2_h = (15 * part2_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part2_y, container_width, part2_h, render::fonts::watermark_font, "Movement", false); {
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 0), item_checkbox_pos, render::fonts::watermark_font,
					"Infinite duck (WARNING: Untrusted)", variables::misc::infinite_duck);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 1), item_checkbox_pos, render::fonts::watermark_font,
					"Bhop", variables::misc::bhop);
				gui::combobox(item_left_pos, part2_base_item_y + (15 * 2), item_combo_pos, render::fonts::watermark_font,
					"Autostrafe", variables::misc::autostrafe_options, variables::misc::autostrafe_target);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 3), item_checkbox_pos, render::fonts::watermark_font,
					"Enable edgejump", variables::misc::edgejump);
				gui::hotkey(item_left_pos, part2_base_item_y + (15 * 4), item_hotkey_w, render::fonts::watermark_font,
					"Edgejump key", variables::misc::ej_key);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 5), item_checkbox_pos, render::fonts::watermark_font,
					"Edgebug", variables::misc::edgebug);
				gui::hotkey(item_left_pos, part2_base_item_y + (15 * 6), item_hotkey_w, render::fonts::watermark_font,
					"Edgebug key", variables::misc::eb_key);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 7), item_checkbox_pos, render::fonts::watermark_font,
					"Jumpbug", variables::misc::jumpbug);
				gui::hotkey(item_left_pos, part2_base_item_y + (15 * 8), item_hotkey_w, render::fonts::watermark_font,
					"Jumpbug key", variables::misc::jb_key);
				gui::check_box(item_left_pos, part2_base_item_y + (15 * 9), item_checkbox_pos, render::fonts::watermark_font,
					"Slowwalk", variables::misc::slowwalk);
				gui::hotkey(item_left_pos, part2_base_item_y + (15 * 10), item_hotkey_w, render::fonts::watermark_font,
					"Slowwalk key", variables::misc::slowwalk_key);
			}

			const int part3_y = part2_y + part2_h + container_margin;
			const int part3_items_num = 3;
			const int part3_base_item_y = part3_y + container_padding;
			const int part3_h = (15 * part3_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part3_y, container_width, part3_h, render::fonts::watermark_font, "Movement", false); {
				gui::multicombobox(item_left_pos, part3_base_item_y + (15 * 0), item_combo_pos, render::fonts::watermark_font,
					"Speedgraph options", variables::misc::speedgraph_target);
				gui::slider(item_left_pos, part3_base_item_y + (15 * 1), item_slider_pos, item_slider_length, render::fonts::watermark_font,
					"Speedgraph height", variables::misc::speedgraph_h, 0.f, 100.f);
				gui::slider(item_left_pos, part3_base_item_y + (15 * 2), item_slider_pos, item_slider_length, render::fonts::watermark_font,
					"Speedgraph pos", variables::misc::speedgraph_pos, 0.f, 100.f);
			}
			
			/* ----- Misc - Second column ----- */

			int column_number  = 1;
			container_width--;	// Not the best way to do it, but the margin on the right was always smaller because of (5/2=2)
			container_left_pos = container_left_pos + (container_width * column_number) + container_margin;
			item_left_pos      = item_left_pos + (container_width * column_number) + container_margin;
			item_checkbox_pos  = item_checkbox_pos + (container_width * column_number) + container_margin;
			item_slider_pos    = item_slider_pos + (container_width * column_number) + container_margin;
			item_combo_pos     = item_checkbox_pos + item_checkbox_length;

			const int part1c2_items_num   = 3;			// part1c2 => part 1 from column 2
			const int part1c2_y           = part1_y;	// Needs to reset y pos on new col
			const int part1c2_base_item_y = part1_base_item_y;
			const int part1c2_h           = (15 * part1c2_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part1c2_y, container_width, part1c2_h, render::fonts::watermark_font, "Movement", false); {
				gui::check_box(item_left_pos, part1c2_base_item_y + (15 * 0), item_checkbox_pos, render::fonts::watermark_font,
					"Thirdperson", variables::misc::thirdperson);
				gui::hotkey(item_left_pos, part1c2_base_item_y + (15 * 1), item_hotkey_w, render::fonts::watermark_font,
					"Thirdperson toggle key", variables::misc::thirdperson_key);
				gui::slider(item_left_pos, part1c2_base_item_y + (15 * 2), item_slider_pos, item_slider_length, render::fonts::watermark_font,
					"Thirdperson distance", variables::misc::thirdperson_dist, 50.f, 200.f);
			}

			const int part2c2_items_num = 4;
			const int part2c2_y = part1c2_y + part1c2_h + container_margin;
			const int part2c2_base_item_y = part2c2_y + container_padding;
			const int part2c2_h = (15 * part2c2_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part2c2_y, container_width, part2c2_h, render::fonts::watermark_font, "Interface", false); {
				gui::check_box(item_left_pos, part2c2_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Hide cheat on screenshots", variables::misc::clean_screenshots);
				gui::check_box(item_left_pos, part2c2_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "Show watermark", variables::misc::draw_watermark);
				gui::check_box(item_left_pos, part2c2_base_item_y + (15 * 2), item_checkbox_pos,
					render::fonts::watermark_font, "Show stats", variables::misc::draw_stats);
				gui::check_box(item_left_pos, part2c2_base_item_y + (15 * 3), item_checkbox_pos,
					render::fonts::watermark_font, "Spectator list", variables::ui::spectators::spectator_list);
			}

			const int part3c2_items_num   = 2;
			const int part3c2_y           = part2c2_y + part2c2_h + container_margin;
			const int part3c2_base_item_y = part3c2_y + container_padding;
			const int part3c2_h           = (15 * part3c2_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part3c2_y, container_width, part3c2_h, render::fonts::watermark_font, "Fov", false); {
				gui::slider(item_left_pos, part3c2_base_item_y + (15 * 0), item_slider_pos, item_slider_length,
					render::fonts::watermark_font, "Custom FOV", variables::misc_visuals::custom_fov_slider, 80.f, 130.f);
				gui::slider(item_left_pos, part3c2_base_item_y + (15 * 1), item_slider_pos, item_slider_length,
					render::fonts::watermark_font, "Custom viewmodel FOV", variables::misc_visuals::custom_vmfov_slider, 0.5f, 2.f);
			}

			const int part4c2_items_num   = 5;
			const int part4c2_y           = part3c2_y + part3c2_h + container_margin;
			const int part4c2_base_item_y = part4c2_y + container_padding;
			const int part4c2_h           = (15 * part4c2_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part4c2_y, container_width, part4c2_h, render::fonts::watermark_font, "Fov", false); {
				gui::check_box(item_left_pos, part4c2_base_item_y + (15 * 0), item_checkbox_pos,
					render::fonts::watermark_font, "Enable motion blur", variables::motion_blur.enabled);
				gui::check_box(item_left_pos, part4c2_base_item_y + (15 * 1), item_checkbox_pos,
					render::fonts::watermark_font, "Forward motion blur", variables::motion_blur.forwardEnabled);
				gui::slider(item_left_pos, part4c2_base_item_y + (15 * 2), item_slider_pos, item_slider_length,
					render::fonts::watermark_font, "Strenght", variables::motion_blur.strength, 0.f, 15.f);
				gui::slider(item_left_pos, part4c2_base_item_y + (15 * 3), item_slider_pos, item_slider_length,
					render::fonts::watermark_font, "Falling intensity", variables::motion_blur.fallingIntensity, 0.f, 10.f);
				gui::slider(item_left_pos, part4c2_base_item_y + (15 * 4), item_slider_pos, item_slider_length,
					render::fonts::watermark_font, "Rotation intensity", variables::motion_blur.rotationIntensity, 0.f, 10.f);
			}

			/* ----- Misc - Buttons ----- */

			const int buttons_con_margin_pos = variables::ui::menu::y + variables::ui::menu::h - container_margin;	// Bottom left corner of container
			const int button_items_num       = 1;
			const int button_items_h         = (button_items_num * 15) + (container_padding * 2) - 4;
			const int buttons_con_y          = buttons_con_margin_pos - button_items_h;		// Get the top left corner based on the margin pos and the height (start from bottom)
			const int buttons_items_base_y   = buttons_con_y + container_padding;			// Same as other containers

			gui::group_box(container_left_pos_o, buttons_con_y, container_width_o, button_items_h, render::fonts::watermark_font, "Buttons", false); {
				gui::button(item_left_pos_o, buttons_items_base_y + (15 * 0), item_checkbox_pos_o - 20, render::fonts::watermark_font,		// Bigger "checkbox" as button. TODO: Pass size and pos
					"Exec autoexec", button_functions::exec_autoexec);
			}
			break;
		}
		case 3: {	// Config
			const int columns = 2;
			container_width = (container_width / columns) - (container_margin / columns);
			item_checkbox_pos = variables::ui::menu::x + container_width - container_margin - item_checkbox_length;
			item_slider_pos = variables::ui::menu::x + container_width - container_margin - item_slider_length;
			item_combo_pos = item_checkbox_pos + item_checkbox_length;
			item_hotkey_w = container_width - container_padding * 2;

			const int part1_items_num = 4;
			const int part1_h = (15 * part1_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part1_y, container_width, part1_h, render::fonts::watermark_font, "Skins", false); {
				gui::button(item_left_pos, part1_base_item_y + (15 * 0), item_checkbox_pos - 20, render::fonts::watermark_font,
					"Refresh configs", config::refresh_list);
				gui::textbox(item_left_pos - 1, part1_base_item_y + (15 * 1), item_hotkey_w, render::fonts::watermark_font,
					"Config name...", config::new_config_name, config::create_new_config);
				gui::button(item_left_pos, part1_base_item_y + (15 * 2), item_checkbox_pos - 20, render::fonts::watermark_font,
					"Load selected config", config::load_selected_config);
				gui::button(item_left_pos, part1_base_item_y + (15 * 3), item_checkbox_pos - 20, render::fonts::watermark_font,
					"Save selected config", config::save_selected_config);
			}

			const int part2_y = part1_y + part1_h + container_margin;
			const int part2_items_num = 2;
			const int part2_base_item_y = part2_y + container_padding;
			const int part2_h = (15 * part2_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part2_y, container_width, part2_h, render::fonts::watermark_font, "Skins", false); {
				gui::button(item_left_pos, part2_base_item_y + (15 * 0), item_checkbox_pos - 20, render::fonts::watermark_font,
					"Load skin config", skins::read_skins);
				gui::button(item_left_pos, part2_base_item_y + (15 * 1), item_checkbox_pos - 20, render::fonts::watermark_font,
					"Update game skins (Full update)", button_functions::full_update);
			}

			/* ----- Config - Second column ----- */

			int column_number = 1;
			container_width--;	// Not the best way to do it, but the margin on the right was always smaller because of (5/2=2)
			container_left_pos = container_left_pos + (container_width * column_number) + container_margin;
			item_left_pos = item_left_pos + (container_width * column_number) + container_margin;
			item_checkbox_pos = item_checkbox_pos + (container_width * column_number) + container_margin;
			item_slider_pos = item_slider_pos + (container_width * column_number) + container_margin;
			item_combo_pos = item_checkbox_pos + item_checkbox_length;

			const int part1c2_items_num = config::max_configs;		// In this case max config number
			const int part1c2_y = part1_y;
			const int part1c2_base_item_y = part1_base_item_y;
			const int part1c2_h = (15 * part1c2_items_num) + (container_padding * 2) - 4;

			gui::group_box(container_left_pos, part1c2_y, container_width, part1c2_h, render::fonts::watermark_font, "Movement", false); {
				gui::config_selection(container_left_pos, part1c2_base_item_y, container_width, render::fonts::watermark_font,
					config::config_names);
			}

			break;
		}
	}

	spectator_framework::spec_list_movement(variables::ui::spectators::x, variables::ui::spectators::y, variables::ui::spectators::w, variables::ui::spectators::h);
	gui::menu_movement(variables::ui::menu::x, variables::ui::menu::y, variables::ui::menu::w, 30);
}

// Toggle using IsPressed (GetAsyncKeyState)
void menu::check_toggle() {
	if (input::gobal_input.IsPressed(VK_INSERT))
		variables::ui::menu::opened = !variables::ui::menu::opened;
}

// Makes the window positions relative based on screen size
void menu::init_windows() {
	int screen_w, screen_h;
	interfaces::engine->get_screen_size(screen_w, screen_h);

	variables::ui::menu::y = screen_h * 0.2;
	variables::ui::menu::x = screen_w * 0.2;
	variables::ui::spectators::y = screen_h * 0.5;		// For smaller screens
}