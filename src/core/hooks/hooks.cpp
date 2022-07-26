#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/hooks/hooks.hpp"
#include "core/menu/menu.hpp"
#include "core/config/config.hpp"
#include "core/features/visuals/skin_changer/skin_changer.hpp"	// For init

bool hooks::initialize() {
	const auto alloc_key_values_target               = reinterpret_cast<void*>(get_virtual(interfaces::key_values_system, 2));
	const auto create_move_target                    = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	const auto paint_traverse_target                 = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	const auto post_screen_space_effects_target      = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 44));
	const auto get_viewmodel_fov_target              = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 35));
	const auto override_view_target                  = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 18));
	const auto draw_model_execute_target             = reinterpret_cast<void*>(get_virtual(interfaces::model_render, 21));	// 29 - DrawModel | 21 - DrawModelExecute
	const auto findmdl_target                        = reinterpret_cast<void*>(get_virtual(interfaces::mdl_cache, 10));
	const auto list_leaves_in_box_target             = reinterpret_cast<void*>(get_virtual(interfaces::engine->get_bsp_tree_query(), 6));
	const auto frame_stage_notify_target             = reinterpret_cast<void*>(get_virtual(interfaces::client, 37));
	const auto render_smoke_overlay_target           = reinterpret_cast<void*>(get_virtual(interfaces::view_render, 41));
	const auto on_screen_size_changed_target         = reinterpret_cast<void*>(get_virtual(interfaces::surface, 116));
	const auto emit_sound_target                     = reinterpret_cast<void*>(get_virtual(interfaces::engine_sound, 5));
	const auto loose_files_allowed_target            = reinterpret_cast<void*>(get_virtual(interfaces::filesystem, 128));
	const auto CL_CheckForPureServerWhitelist_target = reinterpret_cast<void*>(utilities::pattern_scan("engine", sig_CheckForPureServerWhitelist));
	const auto is_depth_of_field_enabled_target      = reinterpret_cast<void*>(utilities::pattern_scan("client.dll", sig_depth_of_field));
	const auto get_client_model_renderable_target    = reinterpret_cast<void*>(utilities::pattern_scan("client.dll", sig_client_model_renderable));
	const auto supports_resolve_depth_target         = reinterpret_cast<void*>(utilities::pattern_scan("shaderapidx9.dll", sig_supports_resolve_depth));
	const auto fire_event_target                     = reinterpret_cast<void*>(utilities::pattern_scan("engine.dll", sig_fire_event));
	const auto viewmodel_sequence_target             = reinterpret_cast<void*>(utilities::pattern_scan("client.dll", sig_viewmodel_sequence));

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

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize minhook.");

	if (MH_CreateHook(alloc_key_values_target, &alloc_key_values_memory::hook, reinterpret_cast<void**>(&alloc_key_values_memory::original)) != MH_OK)
		throw std::runtime_error("failed to initialize alloc_key_values_memory.");
	helpers::console::state_to_console_color("Hooks", "alloc_key_values_memory initialized!");

	if (MH_CreateHook(create_move_target, &create_move::proxy, reinterpret_cast<void**>(&create_move::original)) != MH_OK)
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");
	helpers::console::state_to_console_color("Hooks", "create_move initialized!");

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse::original)) != MH_OK)
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");
	helpers::console::state_to_console_color("Hooks", "paint_traverse initialized!");

	if (MH_CreateHook(post_screen_space_effects_target, &do_post_screen_space_effects::hook, reinterpret_cast<void**>(&do_post_screen_space_effects::original)) != MH_OK)
		throw std::runtime_error("failed to initialize do_post_screen_space_effects.");
	helpers::console::state_to_console_color("Hooks", "do_post_screen_space_effects initialized!");

	if (MH_CreateHook(get_viewmodel_fov_target, &get_viewmodel_fov::hook, reinterpret_cast<void**>(&get_viewmodel_fov::original)) != MH_OK)
		throw std::runtime_error("failed to initialize get_viewmodel_fov.");
	helpers::console::state_to_console_color("Hooks", "get_viewmodel_fov initialized!");

	if (MH_CreateHook(findmdl_target, &findmdl::hook, reinterpret_cast<void**>(&findmdl::original)) != MH_OK)
		throw std::runtime_error("failed to initialize findmdl.");
	helpers::console::state_to_console_color("Hooks", "findmdl initialized!");

	if (MH_CreateHook(override_view_target, &override_view::hook, reinterpret_cast<void**>(&override_view::original)) != MH_OK)
		throw std::runtime_error("failed to initialize override_view.");
	helpers::console::state_to_console_color("Hooks", "override_view initialized!");

	if (MH_CreateHook(draw_model_execute_target, &draw_model_execute::hook, reinterpret_cast<void**>(&draw_model_execute::original)) != MH_OK)
		throw std::runtime_error("failed to initialize draw_model_execute.");
	helpers::console::state_to_console_color("Hooks", "draw_model_execute initialized!");

	if(MH_CreateHook(list_leaves_in_box_target, &list_leaves_in_box::hook, reinterpret_cast<void**>(&list_leaves_in_box::original)) != MH_OK)
		throw std::runtime_error("failed to initialize list_leaves_in_box.");
	helpers::console::state_to_console_color("Hooks", "list_leaves_in_box initialized!");

	if (MH_CreateHook(frame_stage_notify_target, &frame_stage_notify::hook, reinterpret_cast<void**>(&frame_stage_notify::original)) != MH_OK)
		throw std::runtime_error("failed to initialize frame_stage_notify.");
	helpers::console::state_to_console_color("Hooks", "frame_stage_notify initialized!");

	if (MH_CreateHook(render_smoke_overlay_target, &render_smoke_overlay::hook, reinterpret_cast<void**>(&render_smoke_overlay::original)) != MH_OK)
		throw std::runtime_error("failed to initialize render_smoke_overlay.");
	helpers::console::state_to_console_color("Hooks", "render_smoke_overlay initialized!");

	if (MH_CreateHook(loose_files_allowed_target, &loose_files_allowed::hook, reinterpret_cast<void**>(&loose_files_allowed::original)) != MH_OK)
		throw std::runtime_error("failed to initialize loose_files_allowed.");
	helpers::console::state_to_console_color("Hooks", "loose_files_allowed initialized!");

	if (MH_CreateHook(CL_CheckForPureServerWhitelist_target, &CL_CheckForPureServerWhitelist::hook, reinterpret_cast<void**>(&CL_CheckForPureServerWhitelist::original)) != MH_OK)
		throw std::runtime_error("failed to initialize CL_CheckForPureServerWhitelist.");
	helpers::console::state_to_console_color("Hooks", "CL_CheckForPureServerWhitelist initialized!");

	if (MH_CreateHook(on_screen_size_changed_target, &on_screen_size_changed::hook, reinterpret_cast<void**>(&on_screen_size_changed::original)) != MH_OK)
		throw std::runtime_error("failed to initialize on_screen_size_changed.");
	helpers::console::state_to_console_color("Hooks", "on_screen_size_changed initialized!");

	if (MH_CreateHook(emit_sound_target, &emit_sound::hook, reinterpret_cast<void**>(&emit_sound::original)) != MH_OK)
		throw std::runtime_error("failed to initialize emit_sound.");
	helpers::console::state_to_console_color("Hooks", "emit_sound initialized!");

	if (MH_CreateHook(is_depth_of_field_enabled_target, &is_depth_of_field_enabled::hook, reinterpret_cast<void**>(&is_depth_of_field_enabled::original)) != MH_OK)
		throw std::runtime_error("failed to initialize is_depth_of_field_enabled.");
	helpers::console::state_to_console_color("Hooks", "is_depth_of_field_enabled initialized!");

	if(MH_CreateHook(get_client_model_renderable_target, &get_client_model_renderable::hook, reinterpret_cast<void **>(&get_client_model_renderable::original)) != MH_OK)
		throw std::runtime_error("failed to initialize get_client_model_renderable.");
	helpers::console::state_to_console_color("Hooks", "get_client_model_renderable initialized!");

	if(MH_CreateHook(supports_resolve_depth_target, &supports_resolve_depth::hook, reinterpret_cast<void **>(&supports_resolve_depth::original)) != MH_OK)
		throw std::runtime_error("failed to initialize supports_resolve_depth.");
	helpers::console::state_to_console_color("Hooks", "supports_resolve_depth initialized!");

	if (MH_CreateHook(fire_event_target, &fire_event::hook, reinterpret_cast<void**>(&fire_event::original)) != MH_OK)
		throw std::runtime_error("failed to initialize fire_event.");
	helpers::console::state_to_console_color("Hooks", "fire_event initialized!");

	if(MH_CreateHook(viewmodel_sequence_target, &viewmodel_sequence::hook, reinterpret_cast<void **>(&viewmodel_sequence::original)) != MH_OK)
		throw std::runtime_error("failed to initialize viewmodel_sequence.");
	helpers::console::state_to_console_color("Hooks", "viewmodel_sequence initialized!");

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");

	helpers::console::state_to_console_color("Hooks", "Hooks initialized!");
	
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

