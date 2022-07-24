#include "dependencies/interfaces/interfaces.hpp"
#include "dependencies/utilities/csgo.hpp"

bool interfaces::initialize() {
	// https://gitlab.com/KittenPopo/csgo-2018-source/-/blob/main/public/interfaces/interfaces.h
	client                        = get_interface<i_base_client_dll, interface_type::index>("client.dll", "VClient018");
	entity_list                   = get_interface<i_client_entity_list, interface_type::index>("client.dll", "VClientEntityList003");
	engine                        = get_interface<iv_engine_client, interface_type::index>("engine.dll", "VEngineClient014");
	panel                         = get_interface<i_panel, interface_type::index>("vgui2.dll", "VGUI_Panel009");
	surface                       = get_interface<i_surface, interface_type::index>("vguimatsurface.dll", "VGUI_Surface031");
	material_system               = get_interface<i_material_system, interface_type::index>("materialsystem.dll", "VMaterialSystem080");
	model_info                    = get_interface<iv_model_info, interface_type::index>("engine.dll", "VModelInfoClient004");
	model_render                  = get_interface<iv_model_render, interface_type::index>("engine.dll", "VEngineModel016");
	studio_render                 = get_interface<iv_studio_render, interface_type::index>("studiorender.dll", "VStudioRender026");		// For chams
	render_view                   = get_interface<i_render_view, interface_type::index>("engine.dll", "VEngineRenderView014");
	console                       = get_interface<i_console, interface_type::index>("vstdlib.dll", "VEngineCvar007");
	localize                      = get_interface<i_localize, interface_type::index>("localize.dll", "Localize_001");
	event_manager                 = get_interface<i_game_event_manager2, interface_type::index>("engine.dll", "GAMEEVENTSMANAGER002");
	debug_overlay                 = get_interface<iv_debug_overlay, interface_type::index>("engine.dll", "VDebugOverlay004");
	input_system                  = get_interface<i_input_system, interface_type::index>("inputsystem.dll", "InputSystemVersion001");
	trace_ray                     = get_interface<trace, interface_type::index>("engine.dll", "EngineTraceClient004");
	game_movement                 = get_interface<player_game_movement, interface_type::index>("client.dll", "GameMovement001");
	prediction                    = get_interface<player_prediction, interface_type::index>("client.dll", "VClientPrediction001");
	mdl_cache                     = get_interface<mdlcache, interface_type::index>("datacache.dll", "MDLCache004");
	surface_props_physics         = get_interface<physics_surface_props, interface_type::index>("vphysics.dll", "VPhysicsSurfaceProps001");
	client_string_table_container = get_interface<i_client_string_table_container, interface_type::index>("engine.dll", "VEngineClientStringTable001");
	filesystem                    = get_interface<i_filesystem, interface_type::index>("filesystem_stdio.dll", "VFileSystem017");
	engine_sound = get_interface<i_engine_sound, interface_type::index>("engine.dll", "IEngineSoundClient003");

	/* ------------------ Custom interfaces ------------------ */

	clientmode = **reinterpret_cast<i_client_mode***>((*reinterpret_cast<uintptr_t**>(client))[10] + 5);
	globals    = **reinterpret_cast<c_global_vars_base***>((*reinterpret_cast<uintptr_t**>(client))[11] + 10);

	clientstate   = **reinterpret_cast<i_client_state***>(utilities::pattern_scan("engine.dll", sig_client_state) + 1);
	directx       = **reinterpret_cast<IDirect3DDevice9***>(utilities::pattern_scan("shaderapidx9.dll", sig_directx) + 1);
	input         = *reinterpret_cast<i_input**>(utilities::pattern_scan("client.dll", sig_input) + 1);
	glow_manager  = *reinterpret_cast<glow_manager_t**>(utilities::pattern_scan("client.dll", sig_glow_manager) + 3);
	move_helper   = **reinterpret_cast<player_move_helper***>(utilities::pattern_scan("client.dll", sig_player_move_helper) + 2);
	weapon_system = *reinterpret_cast<i_weapon_system**>(utilities::pattern_scan("client.dll", sig_weapon_data) + 2);
	view_render   = **reinterpret_cast<i_view_render***>(utilities::pattern_scan("client.dll", sig_view_render) + 2);

	// KeyValuesSystem
	if (const HINSTANCE handle = GetModuleHandle("vstdlib.dll"))												// Get the exported KeyValuesSystem function
		key_values_system = reinterpret_cast<void* (__cdecl*)()>(GetProcAddress(handle, "KeyValuesSystem"))();	// Set our pointer by calling the function
	key_values_engine = utilities::pattern_scan("engine.dll", sig_key_values_engine);
	key_values_client = utilities::pattern_scan("client.dll", sig_key_values_client);

	// For motion blur
	draw_screen_effect_material = utilities::pattern_scan("client.dll", sig_draw_screen_effect_material);

	/* ------------------------------------------------------- */

	interfaces::console->color_printf(valve_color_t{ 255, 255, 255, 255 }, "\n----------------------------------------------------\n");
	helpers::console::state_to_console_color("Setup", "Interfaces initialized!");

	return true;
}
