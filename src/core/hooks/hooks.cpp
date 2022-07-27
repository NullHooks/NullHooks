#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/hooks/hooks.hpp"
#include "core/menu/menu.hpp"
#include "core/config/config.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"	// For init

#include <format>

bool hooks::initialize() {
	const auto alloc_key_values_target = reinterpret_cast<void*>(get_virtual(interfaces::key_values_system, 2));
	const auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	const auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	const auto post_screen_space_effects_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 44));
	const auto get_viewmodel_fov_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 35));
	const auto override_view_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 18));
	const auto draw_model_execute_target = reinterpret_cast<void*>(get_virtual(interfaces::model_render, 21));	// 29 - DrawModel | 21 - DrawModelExecute
	const auto findmdl_target = reinterpret_cast<void*>(get_virtual(interfaces::mdl_cache, 10));
	const auto list_leaves_in_box_target = reinterpret_cast<void*>(get_virtual(interfaces::engine->get_bsp_tree_query(), 6));
	const auto frame_stage_notify_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 37));
	const auto render_smoke_overlay_target = reinterpret_cast<void*>(get_virtual(interfaces::view_render, 41));
	const auto on_screen_size_changed_target = reinterpret_cast<void*>(get_virtual(interfaces::surface, 116));
	const auto emit_sound_target = reinterpret_cast<void*>(get_virtual(interfaces::engine_sound, 5));
	const auto loose_files_allowed_target = reinterpret_cast<void*>(get_virtual(interfaces::filesystem, 128));
	const auto check_for_pure_server_white_list_target = reinterpret_cast<void*>(utilities::pattern_scan("engine", sig_check_for_pure_server_white_list));
	const auto is_depth_of_field_enabled_target = reinterpret_cast<void*>(utilities::pattern_scan("client.dll", sig_depth_of_field));
	const auto get_client_model_renderable_target = reinterpret_cast<void*>(utilities::pattern_scan("client.dll", sig_client_model_renderable));
	const auto supports_resolve_depth_target = reinterpret_cast<void*>(utilities::pattern_scan("shaderapidx9.dll", sig_supports_resolve_depth));
	const auto fire_event_target = reinterpret_cast<void*>(utilities::pattern_scan("engine.dll", sig_fire_event));
	const auto viewmodel_sequence_target = reinterpret_cast<void*>(utilities::pattern_scan("client.dll", sig_viewmodel_sequence));

	{
		config::init();						// Initialize config folder and skins
		helpers::console::state_to_console_color("Init", "Config initialized!");
		menu::init_windows();				// For window positions on smaller screens
		helpers::console::state_to_console_color("Init", "Windows initialized!");
		backtrack::init();					// Init backtrack cvars
		helpers::console::state_to_console_color("Init", "Backtrack initialized!");
		input::gobal_input.Init();			// Start arrays empty and all that, needed before WndProc
		helpers::console::state_to_console_color("Init", "Global input initialized!");

		// WndProc
		WndProc_hook::csgo_window = FindWindowW(L"Valve001", nullptr);		// Get window for SetWindowLongPtrW()
		WndProc_hook::original = WNDPROC(SetWindowLongPtrW(WndProc_hook::csgo_window, GWLP_WNDPROC, LONG_PTR(WndProc_hook::WndProc)));	// Replace wnproc with our own, call original later
		helpers::console::state_to_console_color("Hooks", "WndProc initialized!");
	}

	{
		if (MH_Initialize() != MH_OK)
			throw std::runtime_error("failed to initialize minhook.");

		// @todo - use std::vformat (im just lazy)
		#define MAKE_HOOK(target, new_function, original, function_name) \
			if ( MH_CreateHook( target, &new_function, reinterpret_cast< void** >( &original ) ) != MH_OK ) \
				throw std::runtime_error( std::string("failed to initialize ").append(function_name) ); \
			else \
				helpers::console::state_to_console_color( "Hooks", std::string(function_name).append(" initialized!").c_str());

		// @todo - organise order of hooks based on what interface (clientmode, client, engine, surface etc...)
		MAKE_HOOK(alloc_key_values_target, alloc_key_values_memory::hook, alloc_key_values_memory::original, "alloc_key_values_memory");
		MAKE_HOOK(create_move_target, create_move::proxy, create_move::original, "create_move");
		MAKE_HOOK(paint_traverse_target, paint_traverse::hook, paint_traverse::original, "paint_traverse");
		MAKE_HOOK(post_screen_space_effects_target, do_post_screen_space_effects::hook, do_post_screen_space_effects::original, "do_post_screen_space_effects");
		MAKE_HOOK(get_viewmodel_fov_target, get_viewmodel_fov::hook, get_viewmodel_fov::original, "get_viewmodel_fov");
		MAKE_HOOK(findmdl_target, findmdl::hook, findmdl::original, "findmdl");
		MAKE_HOOK(override_view_target, override_view::hook, override_view::original, "override_view");
		MAKE_HOOK(draw_model_execute_target, draw_model_execute::hook, draw_model_execute::original, "draw_model_execute");
		MAKE_HOOK(list_leaves_in_box_target, list_leaves_in_box::hook, list_leaves_in_box::original, "list_leaves_in_box");
		MAKE_HOOK(frame_stage_notify_target, frame_stage_notify::hook, frame_stage_notify::original, "frame_stage_notify");
		MAKE_HOOK(render_smoke_overlay_target, render_smoke_overlay::hook, render_smoke_overlay::original, "render_smoke_overlay");
		MAKE_HOOK(loose_files_allowed_target, loose_files_allowed::hook, loose_files_allowed::original, "loose_files_allowed");
		MAKE_HOOK(check_for_pure_server_white_list_target, check_for_pure_server_white_list::hook, check_for_pure_server_white_list::original, "check_for_pure_server_white_list");
		MAKE_HOOK(on_screen_size_changed_target, on_screen_size_changed::hook, on_screen_size_changed::original, "on_screen_size_changed");
		MAKE_HOOK(emit_sound_target, emit_sound::hook, emit_sound::original, "emit_sound");
		MAKE_HOOK(is_depth_of_field_enabled_target, is_depth_of_field_enabled::hook, is_depth_of_field_enabled::original, "is_depth_of_field_enabled");
		MAKE_HOOK(get_client_model_renderable_target, get_client_model_renderable::hook, get_client_model_renderable::original, "get_client_model_renderable");
		MAKE_HOOK(supports_resolve_depth_target, supports_resolve_depth::hook, supports_resolve_depth::original, "supports_resolve_depth");
		MAKE_HOOK(fire_event_target, fire_event::hook, fire_event::original, "fire_event");
		MAKE_HOOK(viewmodel_sequence_target, viewmodel_sequence::hook, viewmodel_sequence::original, "fire_event");

		if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
			throw std::runtime_error("failed to enable hooks.");

		helpers::console::state_to_console_color("Hooks", "Hooks initialized!");
	}

	/* ------------------------------------------------------------------------ */

	// Reset crosshair
	if (!variables::misc_visuals::crosshair) {
		interfaces::engine->execute_cmd("crosshair 1");
		helpers::console::state_to_console_color("Crosshair", "Crosshair reset!");
	}

	interfaces::console->color_printf(valve_color_t{ 255, 255, 255, 255 }, "--------------- ");
	interfaces::console->color_printf(valve_color_t{ 200,   0,   0, 255 }, "Welcome to NullHooks");
	interfaces::console->color_printf(valve_color_t{ 255, 255, 255, 255 }, " ---------------\n\n");

	return true;
}

void hooks::release() {
	helpers::console::state_to_console_color("Unhook", "Unhooking the cheat...\n");

	// Restore thirdperson
	variables::misc::thirdperson = false;
	misc::reset_thirdperson();

	// Restore crosshair
	if (variables::misc_visuals::crosshair)
		interfaces::engine->execute_cmd("crosshair 1");

	// Restore old WndProc
	SetWindowLongPtrW(WndProc_hook::csgo_window, GWLP_WNDPROC, LONG_PTR(WndProc_hook::original));

	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

