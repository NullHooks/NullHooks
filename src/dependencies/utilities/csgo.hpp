#pragma once
#include <windows.h>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <istream>
#include <unordered_map>
#include <intrin.h>
#include <filesystem>

#include "dependencies/utilities/singleton.hpp"
#include "dependencies/utilities/fnv.hpp"
#include "dependencies/utilities/utilities.hpp"
#include "dependencies/minhook/minhook.h"
#include "dependencies/interfaces/interfaces.hpp"
#include "source-sdk/sdk.hpp"
#include "core/hooks/hooks.hpp"
#include "dependencies/math/math.hpp"
#include "dependencies/utilities/renderer/renderer.hpp"
#include "dependencies/utilities/console/console.hpp"

#include "core/helpers/globals.hpp"

//interfaces
#define sig_client_state		"A1 ? ? ? ? 8B 80 ? ? ? ? C3"
#define sig_directx				"A1 ? ? ? ? 50 8B 08 FF 51 0C"
#define sig_input				"B9 ? ? ? ? F3 0F 11 04 24 FF 50 10"
#define sig_glow_manager		"0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00"
#define sig_player_move_helper	"8B 0D ? ? ? ? 8B 46 08 68"
#define sig_weapon_data			"8B 35 ? ? ? ? FF 10 0F B7 C0"

//misc
#define sig_set_angles				"55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8"
#define sig_prediction_player		"89 35 ? ? ? ? F3 0F 10 48"
#define sig_prediction_random_seed	"A3 ? ? ? ? 66 ? ? 86"
#define sig_viewmatrix				"0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"
#define sig_trace_to_exit			"55 8B EC 83 EC 4C F3 0F 10 75"					// For autowall

// custom for the hooks
#define sig_key_values_engine					"85 C0 74 ? 51 6A ? 56 8B C8 E8 ? ? ? ? 8B F0"
#define sig_key_values_client					"85 C0 74 ? 6A ? 6A ? 56 8B C8 E8 ? ? ? ? 8B F0"
#define sig_client_model_renderable				"56 8B F1 80 BE ? ? ? ? ? 0F 84 ? ? ? ? 80 BE"
#define sig_depth_of_field						"8B 0D ? ? ? ? 56 8B 01 FF 50 ? 8B F0 85 F6 75 ?"
#define sig_draw_screen_effect_material			"55 8B EC 83 E4 ? 83 EC ? 53 56 57 8D 44 24 ? 89 4C 24 ?"
#define sig_supports_resolve_depth				"A1 ? ? ? ? A8 ? 75 ? 83 C8 ? B9 ? ? ? ? 68 ? ? ? ? A3"
#define sig_fire_event							"55 8B EC 83 E4 F8 83 EC 0C 53 8B D9 56 57 89 5C 24 0C"
#define sig_view_render							"8B 0D ? ? ? ? FF 75 0C 8B 45 08"
#define sig_viewmodel_sequence					"55 8B EC 53 8B 5D 08 57 8B 7D 0C 8B"
#define sig_filesystem							"8B 0D ? ? ? ? 8D 95 ? ? ? ? 6A 00 C6"
#define sig_check_for_pure_server_white_list			"8B 0D ? ? ? ? 56 83 B9 ? ? ? ? ? 7E 6E"
#define sig_update_visibility					"55 8B EC 83 E4 F8 83 EC 24 53 56 57 8B F9 8D"
#define sig_update_visibility_all_enttities		"53 56 66 8B 35 ? ? ? ? BB ? ? ? ? 57 90 66 3B F3 74 ? A1"

namespace csgo {
	extern player_t* local_player;
}