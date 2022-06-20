#pragma once
#include "hooks.hpp"
#include "../menu/menu.hpp"

HWND						hooks::hCSGOWindow = nullptr;
WNDPROC						hooks::pOriginalWNDProc = nullptr;

extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	menu.apply_fonts();
	ImGui_ImplDX9_Init(hooks::hCSGOWindow, pDevice);
}

bool hooks::initialize() {
	const auto alloc_key_values_target = reinterpret_cast<void*>(get_virtual(interfaces::key_values_system, 1));
	const auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	const auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	const auto post_screen_space_effects_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 44));
	const auto get_viewmodel_fov_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 35));
	const auto override_view_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 18));
	const auto draw_model_execute_target = reinterpret_cast<void*>(get_virtual(interfaces::model_render, 21));	// 29 - DrawModel | 21 - DrawModelExecute
	const auto findmdl_target = reinterpret_cast<void*>(get_virtual(interfaces::mdl_cache, 10));
	const auto present = reinterpret_cast<void*>(get_virtual(interfaces::directx, 17));
	const auto reset = reinterpret_cast<void*>(get_virtual(interfaces::directx, 16));

	// Get window handle
	while (!(hooks::hCSGOWindow = FindWindowA("Valve001", nullptr)))
	{
		using namespace std::literals::chrono_literals;
		std::this_thread::sleep_for(50ms);
	}

	if (hooks::hCSGOWindow)        // Hook WNDProc to capture mouse / keyboard input
		hooks::pOriginalWNDProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hooks::hCSGOWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hooks::WndProc)));
	else
	{
		throw std::runtime_error("failed to hook wndproc.");
		return false;
	}

	input::gobal_input.Init();	// Start arrays empty and all that, needed before WndProc
	custom_helpers::state_to_console_color("Input", "Global input initialized!");

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize minhook.");

	if (MH_CreateHook(alloc_key_values_target, &alloc_key_values_memory::hook, reinterpret_cast<void**>(&alloc_key_values_memory::original)) != MH_OK)
		throw std::runtime_error("failed to initialize alloc_key_values_memory.");
 	custom_helpers::state_to_console_color("Hooks", "alloc_key_values_memory initialized!");

	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move::original)) != MH_OK)
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");
	custom_helpers::state_to_console_color("Hooks", "create_move initialized!");

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse::original)) != MH_OK)
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");
	custom_helpers::state_to_console_color("Hooks", "paint_traverse initialized!");

	if (MH_CreateHook(post_screen_space_effects_target, &do_post_screen_space_effects::hook, reinterpret_cast<void**>(&do_post_screen_space_effects::original)) != MH_OK)
		throw std::runtime_error("failed to initialize do_post_screen_space_effects.");
	custom_helpers::state_to_console_color("Hooks", "do_post_screen_space_effects initialized!");

	if (MH_CreateHook(get_viewmodel_fov_target, &get_viewmodel_fov::hook, reinterpret_cast<void**>(&get_viewmodel_fov::original)) != MH_OK)
		throw std::runtime_error("failed to initialize get_viewmodel_fov.");
	custom_helpers::state_to_console_color("Hooks", "get_viewmodel_fov initialized!");

	if (MH_CreateHook(findmdl_target, &findmdl::hook, reinterpret_cast<void**>(&findmdl::original)) != MH_OK)
		throw std::runtime_error("failed to initialize findmdl.");
	custom_helpers::state_to_console_color("Hooks", "findmdl initialized!");

	if (MH_CreateHook(override_view_target, &override_view::hook, reinterpret_cast<void**>(&override_view::original)) != MH_OK)
		throw std::runtime_error("failed to initialize override_view.");
	custom_helpers::state_to_console_color("Hooks", "override_view initialized!");

	if (MH_CreateHook(draw_model_execute_target, &draw_model_execute::hook, reinterpret_cast<void**>(&draw_model_execute::original)) != MH_OK)
		throw std::runtime_error("failed to initialize draw_model_execute.");
	custom_helpers::state_to_console_color("Hooks", "draw_model_execute initialized!");

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");

	custom_helpers::state_to_console_color("Hooks", "Hooks initialized!");
	
	/* ------------------------------------------------------------------------ */

	// Reset crosshair
	if (!variables::misc_visuals::crosshair) {
		interfaces::engine->execute_cmd("crosshair 1");
		custom_helpers::state_to_console_color("Crosshair", "Crosshair reset!");
	}

	interfaces::console->color_printf(valve_color_t{ 255, 255, 255, 255 }, "--------------- ");
	interfaces::console->color_printf(valve_color_t{ 200,   0,   0, 255 }, "Welcome to NullHooks");
	interfaces::console->color_printf(valve_color_t{ 255, 255, 255, 255 }, " ---------------\n\n");

	return true;
}

void hooks::release() {
	custom_helpers::state_to_console_color("Unhook", "Unhooking the cheat...\n");

	// Restore old WndProc
	SetWindowLongPtrW(WndProc_hook::csgo_window, GWLP_WNDPROC, LONG_PTR(WndProc_hook::original));
	menu.menu_opened = false;

	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

bool init = false;

long __stdcall hooks::present::hook(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region)
{
	if (!init)
	{
		menu.apply_fonts();
		menu.setup_resent(device);
		init = true;
	}
	if (init) {
		menu.pre_render(device);
		menu.post_render();

		//menu.run_popup();
		menu.render();
		menu.end_present(device);
	}

	return present::original(device, source_rect, dest_rect, dest_window_override, dirty_region);
}

long __stdcall hooks::reset::hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters)
{
	if (!init)
		reset::original(device, present_parameters);

	menu.invalidate_objects();
	long hr = reset::original(device, present_parameters);
	menu.create_objects(device);

	return hr;
}

LRESULT __stdcall hooks::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	if (GetAsyncKeyState(0x2D) & 1)
		menu.menu_opened = !menu.menu_opened;

	if (menu.menu_opened)
		interfaces::input_system->enable_input(false);
	else if (!menu.menu_opened)
		interfaces::input_system->enable_input(true);

	if (menu.menu_opened && ImGui_ImplDX9_WndProcHandler(hwnd, message, wparam, lparam))
		return true;

	return CallWindowProcA(hooks::pOriginalWNDProc, hwnd, message, wparam, lparam);
}