#pragma once
#include <windows.h>
#include <cstdint>
#include <string>
#include <stdexcept>

#include "dependencies/interfaces/i_base_client_dll.hpp"
#include "dependencies/interfaces/i_client_entity_list.hpp"
#include "dependencies/interfaces/iv_engine_client.hpp"
#include "dependencies/interfaces/i_client_state.hpp"
#include "dependencies/interfaces/i_panel.hpp"
#include "dependencies/interfaces/i_surface.hpp"
#include "dependencies/interfaces/c_global_vars_base.hpp"
#include "dependencies/interfaces/i_material_system.hpp"
#include "dependencies/interfaces/iv_model_info.hpp"
#include "dependencies/interfaces/iv_model_render.hpp"
#include "dependencies/interfaces/iv_debug_overlay.hpp"
#include "dependencies/interfaces/i_console.hpp"
#include "dependencies/interfaces/i_localize.hpp"
#include "dependencies/interfaces/i_game_event_manager.hpp"
#include "dependencies/interfaces/i_input.hpp"
#include "dependencies/interfaces/i_input_system.hpp"
#include "dependencies/interfaces/i_trace.hpp"
#include "dependencies/interfaces/i_render_view.hpp"
#include "dependencies/interfaces/i_view_render.hpp"
#include "dependencies/interfaces/glow_manager.hpp"
#include "dependencies/interfaces/i_player_movement.hpp"
#include "dependencies/interfaces/i_weapon_system.hpp"
#include "dependencies/interfaces/i_studio_render.h"
#include "dependencies/interfaces/i_mdlcache.hpp"
#include "dependencies/interfaces/i_physics_surface_props.hpp"
#include "dependencies/interfaces/i_client_string_table_container.hpp"
#include "dependencies/interfaces/i_filesystem.hpp"
#include "dependencies/interfaces/i_engine_sound.hpp"
#include "dependencies/interfaces/i_panoramaui_engine.hpp"

namespace interfaces {
	enum class interface_type { index, bruteforce };

	template <typename ret, interface_type type>
	ret* get_interface(const std::string& module_name, const std::string& interface_name) {
		using create_interface_fn = void* (*)(const char*, int*);
		const auto fn = reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandle(module_name.c_str()), "CreateInterface"));

		if (fn) {
			void* result = nullptr;

			switch (type) {
			case interface_type::index:
				result = fn(interface_name.c_str(), nullptr);

				break;
			case interface_type::bruteforce:
				char buf[128];

				for (uint32_t i = 0; i <= 100; ++i) {
					memset(static_cast<void*>(buf), 0, sizeof buf);

					result = fn(interface_name.c_str(), nullptr);

					if (result)
						break;
				}

				break;
			}
		
			if (!result)
				throw std::runtime_error( interface_name + " wasn't found in " + module_name );

			return static_cast<ret*>(result);
		}

		throw std::runtime_error( module_name + " wasn't found" );
	}

	inline i_base_client_dll* client;
	inline i_input* input;
	inline i_client_entity_list* entity_list;
	inline iv_engine_client* engine;
	inline i_client_mode* clientmode;
	inline i_client_state* clientstate;
	inline i_panel* panel;
	inline i_surface* surface;
	inline c_global_vars_base* globals;
	inline i_material_system* material_system;
	inline iv_model_info* model_info;
	inline iv_model_render* model_render;
	inline i_render_view* render_view;
	inline i_view_render* view_render;
	inline iv_studio_render* studio_render;		// For chams
	inline iv_debug_overlay* debug_overlay;
	inline i_console* console;
	inline i_localize* localize;
	inline i_game_event_manager2* event_manager;
	inline i_input_system* input_system;
	inline IDirect3DDevice9* directx;
	inline trace* trace_ray;
	inline glow_manager_t* glow_manager;
	inline player_game_movement* game_movement;
	inline player_prediction* prediction;
	inline player_move_helper* move_helper;
	inline mdlcache* mdl_cache;
	inline i_weapon_system* weapon_system;
	inline physics_surface_props* surface_props_physics;
	inline i_client_string_table_container* client_string_table_container;
	inline i_filesystem* filesystem;
	inline i_engine_sound* engine_sound;
	inline i_panoramaui_engine* panorama_engine;

	// https://github.com/cazzwastaken/based/search?q=keyValuesSystem
	inline void* key_values_system = nullptr;
	inline std::uint8_t* key_values_engine = nullptr;
	inline std::uint8_t* key_values_client = nullptr;
	
	inline std::uint8_t* draw_screen_effect_material = nullptr;

	bool initialize();
}
