#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "../menu/menu.hpp"
#include "hooks.hpp"
#include "../helpers/misc_helpers.hpp"
#include "../../source-sdk/sdk.hpp"

hooks::create_move::fn create_move_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;

bool hooks::initialize() {
	const auto alloc_key_values_target = reinterpret_cast<void*>(get_virtual(interfaces::key_values_system, 1));
	const auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	const auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	const auto post_screen_space_effects_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 44));
	const auto get_viewmodel_fov_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 35));
	const auto override_view_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 18));

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize MH_Initialize.");

	if (MH_CreateHook(alloc_key_values_target, &AllocKeyValuesMemory, reinterpret_cast<void**>(&AllocKeyValuesMemoryOriginal)) != MH_OK)
		throw std::runtime_error("failed to initialize AllocKeyValuesMemory.");
	custom_helpers::state_to_console("Hooks", "AllocKeyValuesMemory initialized!");
	
	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK)
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");
	custom_helpers::state_to_console("Hooks", "create_move initialized!");
	
	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK)
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");
	custom_helpers::state_to_console("Hooks", "paint_traverse initialized!");

	if (MH_CreateHook(post_screen_space_effects_target, &DoPostScreenSpaceEffects, reinterpret_cast<void**>(&DoPostScreenSpaceEffectsOriginal)) != MH_OK)
		throw std::runtime_error("failed to initialize DoPostScreenSpaceEffects.");
	custom_helpers::state_to_console("Hooks", "DoPostScreenSpaceEffects initialized!");

	if (MH_CreateHook(get_viewmodel_fov_target, &get_viewmodel_fov, reinterpret_cast<void**>(&get_viewmodel_fov_original)) != MH_OK)
		throw std::runtime_error("failed to initialize DoPostScreenSpaceEffects.");
	custom_helpers::state_to_console("Hooks", "get_viewmodel_fov initialized!");

	if (MH_CreateHook(override_view_target, &override_view, reinterpret_cast<void**>(&override_view_original)) != MH_OK)
		throw std::runtime_error("failed to initialize DoPostScreenSpaceEffects.");
	custom_helpers::state_to_console("Hooks", "override_view initialized!");

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");

	// Reset crosshair
	if (!variables::crosshair_bool) {
		interfaces::engine->execute_cmd("crosshair 1");
		custom_helpers::state_to_console("Crosshair", "Crosshair reset!");
	}

	custom_helpers::state_to_console("Hooks", "Hooks initialized!");
	custom_helpers::print_to_console("---------- Welcome to NullHooks ----------\n\n");

	return true;
}

void hooks::release() {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

void* __stdcall hooks::AllocKeyValuesMemory(const std::int32_t size) {
	// If function is returning to speficied addresses, return nullptr to "bypass"
	if (const std::uint32_t address = reinterpret_cast<std::uint32_t>(_ReturnAddress());
		address == reinterpret_cast<std::uint32_t>(interfaces::key_values_engine) ||
		address == reinterpret_cast<std::uint32_t>(interfaces::key_values_client))
		return nullptr;

	// Return original
	return AllocKeyValuesMemoryOriginal(interfaces::key_values_system, size);
}

bool __stdcall hooks::create_move::hook(float input_sample_frametime, c_usercmd* cmd) {
	create_move_original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number) return false;

	csgo::local_player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	misc::movement::bunny_hop(cmd);

	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;

	prediction::start(cmd); {
	} prediction::end();
	
	math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

	cmd->viewangles.normalize();
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;

	return false;
}

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	switch (panel_to_draw) {
		case fnv::hash("MatSystemTopPanel"):
			if (interfaces::engine->is_taking_screenshot()) break;

			watermark::draw();
			watermark::draw_stats();

			visuals::playeresp();
			visuals::grenade_projectile_esp();

			visuals::noflash();
			visuals::misc::nade_predict();
			visuals::crosshair::custom_crosshair();
			visuals::crosshair::recoil_crosshair();

			misc::spectator_list();

			menu::toggle();
			menu::render();

			break;
		case fnv::hash("FocusOverlayPanel"):
			//interfaces::panel->set_keyboard_input_enabled(panel, variables::menu::opened);	// Let em use the keyboard, why the fuck not
			interfaces::panel->set_mouse_input_enabled(panel, variables::menu::opened);
			break;
		case fnv::hash("HudZoom"):	// No sniper scope
			if (!variables::noscope_bool) break;
			if (!csgo::local_player) break;
			if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game()) break;
			if (!csgo::local_player->is_scoped()) break;
			if (interfaces::engine->is_taking_screenshot()) break;

			int screen_w, screen_h;
			interfaces::engine->get_screen_size(screen_w, screen_h);
			const int mid_x = screen_w / 2;
			const int mid_y = screen_h / 2;

			render::draw_line(0, mid_y,	screen_w, mid_y, color::black(255));	// X
			render::draw_line(mid_x, 0,	mid_x, screen_h, color::black(255));	// Y

			return;
			break;
	}

	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}

void __stdcall hooks::DoPostScreenSpaceEffects(const void* viewSetup) noexcept {
	visuals::glow::draw_glow();

	DoPostScreenSpaceEffectsOriginal(interfaces::clientmode, viewSetup);
}

float __fastcall hooks::get_viewmodel_fov(uintptr_t, uintptr_t) {
	if (interfaces::engine->is_taking_screenshot())
		return get_viewmodel_fov_original(interfaces::clientmode);

	return get_viewmodel_fov_original(interfaces::clientmode) * variables::custom_vmfov_slider;
	/*
	 * Return original:
	 * return get_viewmodel_fov_original(interfaces::clientmode);	// Original: 68.f
	 */
}

void __fastcall hooks::override_view(uintptr_t, uintptr_t, view_setup_t* setup) {
	if (csgo::local_player
		&& interfaces::engine->is_connected()
		&& interfaces::engine->is_in_game()
		&& !csgo::local_player->is_scoped()
		&& !interfaces::engine->is_taking_screenshot())
		setup->fov = variables::custom_fov_slider;

	override_view_original(interfaces::clientmode, setup);
}