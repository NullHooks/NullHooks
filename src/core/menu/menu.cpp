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

	#pragma region TABS
	/* ------------------ TABS ------------------ */
	constexpr int tab_number = 4;
	const int tab_size   = variables::ui::menu::w / tab_number;

	gui::tab(variables::ui::menu::x + (tab_size * 0), variables::ui::menu::y + gui::vars::top_margin, tab_size, gui::vars::tab_height,
		render::fonts::watermark_font, "Aim", menu::current_tab, 0);
	gui::tab(variables::ui::menu::x + (tab_size * 1), variables::ui::menu::y + gui::vars::top_margin, tab_size, gui::vars::tab_height,
		render::fonts::watermark_font, "Visuals", menu::current_tab, 1);
	gui::tab(variables::ui::menu::x + (tab_size * 2), variables::ui::menu::y + gui::vars::top_margin, tab_size, gui::vars::tab_height,
		render::fonts::watermark_font, "Misc", menu::current_tab, 2);
	gui::tab(variables::ui::menu::x + (tab_size * 3), variables::ui::menu::y + gui::vars::top_margin, tab_size, gui::vars::tab_height,
		render::fonts::watermark_font, "Config", menu::current_tab, 3);
	#pragma endregion

	#pragma region CONTAINERS
	/* ------------------ CONTAINERS ------------------ */

	gui::update_positions();		// Get the updated variables with current menu pos and all that

	switch (current_tab) {
		// Aim
		case 0: {
			gui::init_tab();

			gui::add_groupbox("General", 3); {
				gui::add_checkbox	("Also target teammates",				variables::aim::target_friends);
				gui::add_checkbox	("Enable noscope aimbot on snipers",	variables::aim::aimbot_noscope);
				gui::add_checkbox	("Enable non-rifle aim punch",			variables::aim::non_rifle_aimpunch);
			}

			gui::add_groupbox("Triggerbot", 3); {
				gui::add_checkbox	("Enable triggerbot",	variables::aim::triggerbot);
				gui::add_hotkey		("Triggerbot key",		variables::aim::triggerbot_key);
				gui::add_slider		("Triggerbot delay",	variables::aim::triggerbot_delay, 0.f, 30.f);
			}

			gui::add_groupbox("Aimbot", 11); {
				gui::add_checkbox		("Enable aimbot",		variables::aim::aimbot);
				gui::add_checkbox		("Autofire",			variables::aim::autofire);
				gui::add_hotkey			("Only on key",			variables::aim::aimbot_key);
				gui::add_checkbox		("Silent",				variables::aim::silent);
				gui::add_combobox		("Autowall",			variables::aim::autowall_settings, variables::aim::autowall);
				gui::add_multicombobox	("Aimbot hitboxes",		variables::aim::hitboxes);
				gui::add_checkbox		("Bodyaim if lethal",	variables::aim::bodyaim_if_lethal);
				gui::add_slider			("Minimum damage",		variables::aim::min_damage, 0.f, 100.f);
				gui::add_slider			("Aimbot fov",			variables::aim::aimbot_fov, 0.f, 180.f);
				gui::add_checkbox		("Draw fov",			variables::aim::draw_fov, variables::colors::aimbot_fov_c);
				gui::add_slider			("Aimbot smoothing",	variables::aim::aimbot_smoothing, 0.f, 1.f);
			}

			/* -------- Aim - Second column -------- */
			gui::add_column();

			gui::add_groupbox("Antiaim", 7); {
				gui::add_checkbox	("AntiAim",						variables::antiaim::antiaim);
				gui::add_slider		("Pitch (x)",					variables::antiaim::pitch, -89.f, 89.f);
				gui::add_slider		("Yaw (y)",						variables::antiaim::yaw, -180.f, 180.f);
				gui::add_checkbox	("Spinbot",						variables::antiaim::spinbot);
				gui::add_slider		("Spinbot speed",				variables::antiaim::spinbot_speed, 0.f, 100.f);
				gui::add_checkbox	("Peek antiaim (Manual)",		variables::antiaim::peek_aa);						// Probably needs name change
				gui::add_hotkey		("Peek antiaim toggle",			variables::antiaim::peek_aa_toggle_key);
			}
			break;
		}
		// Visuals
		case 1:	{
			gui::init_tab();

			gui::add_groupbox("Player ESP", 7); {
				gui::add_checkbox		("Enable team ESP (global)",	variables::player_visuals::showteamesp, variables::colors::friendly_color);
				gui::add_checkbox		("Box ESP",						variables::player_visuals::boxesp, variables::colors::enemy_color);
				gui::add_checkbox		("Skeleton ESP",				variables::player_visuals::skeletonesp, variables::colors::enemy_color_soft);
				gui::add_checkbox		("Name ESP",					variables::player_visuals::nameesp);
				gui::add_multicombobox	("Player info",					variables::player_visuals::playerinfo);
				gui::add_checkbox		("Health ESP",					variables::player_visuals::healthesp);
				gui::add_checkbox		("Line ESP",					variables::player_visuals::lineesp);
			}

			gui::add_groupbox("Chams", 13); {
				gui::add_checkbox("Only visible chams",			variables::chams::only_visible_chams);
				gui::add_checkbox("Wireframe chams",			variables::chams::wireframe_chams);
				gui::add_checkbox("Draw on top",				variables::chams::draw_chams_on_top);
				gui::add_checkbox("Player chams",				variables::chams::player_chams, variables::colors::chams_vis_friend_c, variables::colors::chams_vis_enemy_c);
				gui::add_combobox("Player chams material",		variables::chams::materials, variables::chams::player_chams_mat_id);
				gui::add_checkbox("Local player chams",			variables::chams::localplayer_chams, variables::colors::chams_localplayer);
				gui::add_combobox("Local chams material",		variables::chams::materials, variables::chams::localplayer_chams_mat_id);
				gui::add_checkbox("Viewmodel weapon chams",		variables::chams::vm_weapon_chams, variables::colors::chams_weapon_c);
				gui::add_combobox("Weapon chams material",		variables::chams::materials, variables::chams::weapon_chams_mat_id);
				gui::add_checkbox("Arms chams",					variables::chams::vm_arm_chams, variables::colors::chams_arms_c);
				gui::add_combobox("Arms chams material",		variables::chams::materials, variables::chams::arm_chams_mat_id);
				gui::add_checkbox("Sleeve chams",				variables::chams::vm_sleeve_chams, variables::colors::chams_sleeve_c);
				gui::add_combobox("Sleeve chams material",		variables::chams::materials, variables::chams::sleeve_chams_mat_id);
			}

			/* ----- Visuals - Second column ----- */
			gui::add_column();

			gui::add_groupbox("Glow", 3); {
				gui::add_checkbox("Player glow",		variables::player_visuals::playerglow, variables::colors::friendly_glow_c, variables::colors::enemy_glow_c);
				gui::add_checkbox("Entity glow",		variables::entity_visuals::entityglow, variables::colors::entity_glow_c);
				gui::add_checkbox("Chicken pride",		variables::misc_visuals::chickenpride);
			}

			gui::add_groupbox("Other ESP", 3); {
				gui::add_checkbox("Bomb timer",				variables::entity_visuals::bombtimer);
				gui::add_checkbox("Nade projectile ESP",	variables::entity_visuals::nade_esp);
				gui::add_checkbox("Entity info",			variables::entity_visuals::entitytext);
			}

			gui::add_groupbox("Misc", 9); {
				gui::add_slider			("No flash",				variables::misc_visuals::noflash_alpha, 0.f, 1.f);
				gui::add_checkbox		("Wireframe smoke",			variables::misc_visuals::wireframe_smoke);
				gui::add_checkbox		("Nade prediction",			variables::misc_visuals::nade_predict);
				gui::add_checkbox		("No sniper scope",			variables::misc_visuals::noscope);
				gui::add_checkbox		("Worldcolor",				variables::misc_visuals::worldcolor, variables::colors::worldcolor_c);
				gui::add_multicombobox	("Bullet tracers",			variables::misc_visuals::bulletracer_draw_target);
				gui::add_multicombobox	("Bullet tracer teams",		variables::misc_visuals::bulletracer_team_target);
				gui::add_checkbox		("Custom crosshair",		variables::misc_visuals::crosshair, variables::colors::crosshair_c);
				gui::add_checkbox		("Recoil crosshair",		variables::misc_visuals::recoil_crosshair, variables::colors::recoil_crosshair_c);
			}
			break;
		}
		case 2: {	// Misc
			gui::init_tab();

			gui::add_groupbox("Network", 4); {
				gui::add_checkbox	("Backtrack",			variables::misc::backtrack);
				gui::add_slider		("Backtrack ms",			variables::misc::backtrack_ticks, 0.f, 400.f);
				gui::add_checkbox	("Also teammates",		variables::misc::backtrack_team, variables::colors::bt_chams_friend_fade, variables::colors::bt_chams_friend);
				gui::add_checkbox	("Backtrack chams",		variables::chams::backtrack_chams, variables::colors::bt_chams_enemy_fade, variables::colors::bt_chams_enemy);
			}

			gui::add_groupbox("Movement", 11); {
				gui::add_checkbox	("Infinite duck (WARNING: Untrusted)",	variables::misc::infinite_duck);
				gui::add_checkbox	("Bhop",								variables::misc::bhop);
				gui::add_combobox	("Autostrafe",							variables::misc::autostrafe_options, variables::misc::autostrafe_target);
				gui::add_checkbox	("Enable edgejump",						variables::misc::edgejump);
				gui::add_hotkey		("Edgejump key",						variables::misc::ej_key);
				gui::add_checkbox	("Edgebug",								variables::misc::edgebug);
				gui::add_hotkey		("Edgebug key",							variables::misc::eb_key);
				gui::add_checkbox	("Jumpbug",								variables::misc::jumpbug);
				gui::add_hotkey		("Jumpbug key",							variables::misc::jb_key);
				gui::add_checkbox	("Slowwalk",							variables::misc::slowwalk);
				gui::add_hotkey		("Slowwalk key",						variables::misc::slowwalk_key);
			}

			gui::add_groupbox("Movement", 3); {
				gui::add_multicombobox	("Speedgraph options",	variables::misc::speedgraph_target);
				gui::add_slider			("Speedgraph height",	variables::misc::speedgraph_h, 0.f, 100.f);
				gui::add_slider			("Speedgraph pos",		variables::misc::speedgraph_pos, 0.f, 100.f);
			}
			
			/* ----- Misc - Second column ----- */
			gui::add_column();

			gui::add_groupbox("Movement", 3); {
				gui::add_checkbox	("Thirdperson",				variables::misc::thirdperson);
				gui::add_hotkey		("Thirdperson toggle key",	variables::misc::thirdperson_key);
				gui::add_slider		("Thirdperson distance",	variables::misc::thirdperson_dist, 50.f, 200.f);
			}

			gui::add_groupbox("Interface", 4); {
				gui::add_checkbox("Hide cheat on screenshots",	variables::misc::clean_screenshots);
				gui::add_checkbox("Show watermark",				variables::misc::draw_watermark);
				gui::add_checkbox("Show stats",					variables::misc::draw_stats);
				gui::add_checkbox("Spectator list",				variables::ui::spectators::spectator_list);
			}

			gui::add_groupbox("Fov", 2); {
				gui::add_slider("Custom FOV",				variables::misc_visuals::custom_fov_slider, 80.f, 130.f);
				gui::add_slider("Custom viewmodel FOV",		variables::misc_visuals::custom_vmfov_slider, 0.5f, 2.f);
			}

			gui::add_groupbox("Fov", 5); {
				gui::add_checkbox	("Enable motion blur",		variables::motion_blur.enabled);
				gui::add_checkbox	("Forward motion blur",		variables::motion_blur.forwardEnabled);
				gui::add_slider		("Strenght",				variables::motion_blur.strength, 0.f, 15.f);
				gui::add_slider		("Falling intensity",		variables::motion_blur.fallingIntensity, 0.f, 10.f);
				gui::add_slider		("Rotation intensity",		variables::motion_blur.rotationIntensity, 0.f, 10.f);
			}

			/* ----- Misc - Buttons ----- */

			gui::add_bottom_groupbox(1);
			// Not as dynamic because its at the bottom and with the whole width
			gui::groupbox(gui::vars::o_container_left_pos, gui::vars::button_part_y, gui::vars::o_container_width, gui::vars::button_part_h, render::fonts::watermark_font, "Buttons", false); {
				gui::button(gui::vars::o_item_left_pos, gui::vars::button_base_item_y + (15 * 0), gui::vars::o_item_checkbox_pos - 20, render::fonts::watermark_font,		// Bigger "checkbox" as button
					"Exec autoexec", button_functions::exec_autoexec);																										// @todo: Pass size and pos
			}
			break;
		}
		case 3: {	// Config
			gui::init_tab();

			gui::add_groupbox("Skins", 4); {
				gui::add_button		("Refresh configs",			config::refresh_list);
				gui::add_textbox	("Config name...",			config::new_config_name, config::create_new_config);
				gui::add_button		("Load selected config",	config::load_selected_config);
				gui::add_button		("Save selected config",	config::save_selected_config);
			}

			gui::add_groupbox("Skins", 2); {
				gui::add_button("Load skin config",					skins::read_skins);
				gui::add_button("Update game skins (Full update)",	button_functions::full_update);
			}

			/* ----- Config - Second column ----- */
			gui::add_column();

			gui::add_groupbox("Movement", config::max_configs); {
				gui::config_selection(gui::vars::container_left_pos, gui::vars::cur_base_item_y, gui::vars::container_width, render::fonts::watermark_font,
					config::config_names);
			}
			break;
		}
	}

	#pragma endregion

	#pragma region WINDOW MOVEMENTS
	spectator_framework::spec_list_movement(variables::ui::spectators::x, variables::ui::spectators::y, variables::ui::spectators::w, variables::ui::spectators::h);
	gui::menu_movement(variables::ui::menu::x, variables::ui::menu::y, variables::ui::menu::w, 30);
	#pragma endregion

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