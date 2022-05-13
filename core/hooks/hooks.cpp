#pragma once
#include "hooks.hpp"
#include "../helpers/misc_helpers.hpp"

bool hooks::initialize() {
	const auto alloc_key_values_target = reinterpret_cast<void*>(get_virtual(interfaces::key_values_system, 1));
	const auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	const auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	const auto post_screen_space_effects_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 44));
	const auto get_viewmodel_fov_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 35));
	const auto override_view_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 18));
	const auto draw_model_execute_target = reinterpret_cast<void*>(get_virtual(interfaces::model_render, 21));	// 29 - DrawModel | 21 - DrawModelExecute

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize MH_Initialize.");

	if (MH_CreateHook(alloc_key_values_target, &alloc_key_values_memory::hook, reinterpret_cast<void**>(&alloc_key_values_memory::original)) != MH_OK)
		throw std::runtime_error("failed to initialize alloc_key_values_memory.");
	custom_helpers::state_to_console("Hooks", "alloc_key_values_memory initialized!");
	
	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move::original)) != MH_OK)
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");
	custom_helpers::state_to_console("Hooks", "create_move initialized!");
	
	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse::original)) != MH_OK)
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");
	custom_helpers::state_to_console("Hooks", "paint_traverse initialized!");

	if (MH_CreateHook(post_screen_space_effects_target, &do_post_screen_space_effects::hook, reinterpret_cast<void**>(&do_post_screen_space_effects::original)) != MH_OK)
		throw std::runtime_error("failed to initialize do_post_screen_space_effects.");
	custom_helpers::state_to_console("Hooks", "do_post_screen_space_effects initialized!");

	if (MH_CreateHook(get_viewmodel_fov_target, &get_viewmodel_fov::hook, reinterpret_cast<void**>(&get_viewmodel_fov::original)) != MH_OK)
		throw std::runtime_error("failed to initialize get_viewmodel_fov.");
	custom_helpers::state_to_console("Hooks", "get_viewmodel_fov initialized!");

	if (MH_CreateHook(override_view_target, &override_view::hook, reinterpret_cast<void**>(&override_view::original)) != MH_OK)
		throw std::runtime_error("failed to initialize override_view.");
	custom_helpers::state_to_console("Hooks", "override_view initialized!");

	if (MH_CreateHook(draw_model_execute_target, &draw_model_execute::hook, reinterpret_cast<void**>(&draw_model_execute::original)) != MH_OK)
		throw std::runtime_error("failed to initialize draw_model_execute.");
	custom_helpers::state_to_console("Hooks", "draw_model_execute initialized!");

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");

	/* ------------------------------------------------------------------------ */

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
