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

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize MH_Initialize.");

	if (MH_CreateHook(alloc_key_values_target, &AllocKeyValuesMemory, reinterpret_cast<void**>(&AllocKeyValuesMemoryOriginal)) != MH_OK)
		throw std::runtime_error("failed to initialize alloc_key_values. (outdated index?)");
	custom_helpers::state_to_console("Hooks", "alloc_key_values initialized!");

	// TODO: Fix team selection thing (createmove)
	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK)
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");
	custom_helpers::state_to_console("Hooks", "create_move initialized!");

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK)
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");
	custom_helpers::state_to_console("Hooks", "paint_traverse initialized!");

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");

	//interfaces::engine->execute_cmd("echo Hooks initialized!");
	custom_helpers::state_to_console("Hooks", "Hooks initialized!");
	custom_helpers::print_to_console("---------- Welcome to NullHooks ----------\n\n");

	return true;
}

void hooks::release() {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

void* __stdcall hooks::AllocKeyValuesMemory(const std::int32_t size) noexcept
{
	// if function is returning to speficied addresses, return nullptr to "bypass"
	if (const std::uint32_t address = reinterpret_cast<std::uint32_t>(_ReturnAddress());
		address == reinterpret_cast<std::uint32_t>(interfaces::key_values_engine) ||
		address == reinterpret_cast<std::uint32_t>(interfaces::key_values_client))
		
		return nullptr;

	// return original
	return AllocKeyValuesMemoryOriginal(interfaces::key_values_system, size);
}

// TODO: Fix team selection thing (createmove)
bool __stdcall hooks::create_move::hook(float input_sample_frametime, c_usercmd* cmd) {
	create_move_original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number) return false;

	csgo::local_player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast<bool*>(*frame_pointer - 0x1C);

	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;

	misc::movement::bunny_hop(cmd);

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
			render::draw_text_string(15, 15, render::fonts::watermark_font, "NullHooks Lite", false, color::red(255));

			visuals::boxesp();
			visuals::nameesp();
			visuals::noflash::handle();

			misc::spectators();
			misc::custom_crosshair();

			menu::toggle();
			menu::render();

			break;
		case fnv::hash("FocusOverlayPanel"):
			//interfaces::panel->set_keyboard_input_enabled(panel, variables::menu::opened);	// Let em use the keyboard, why the fuck not
			interfaces::panel->set_mouse_input_enabled(panel, variables::menu::opened);
			break;
	}

	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}
