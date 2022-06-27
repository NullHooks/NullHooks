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
#include "dependencies/utilities/csgo.hpp" //lol

//interfaces
#define sig_client_state "A1 ? ? ? ? 8B 80 ? ? ? ? C3"
//#define sig_client_state "A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0"
#define sig_directx "A1 ? ? ? ? 50 8B 08 FF 51 0C"
#define sig_input "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10"
#define sig_glow_manager "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00"
#define sig_player_move_helper "8B 0D ? ? ? ? 8B 46 08 68"
#define sig_weapon_data "8B 35 ? ? ? ? FF 10 0F B7 C0"

//misc
//#define sig_set_angles "55 8B EC 83 E4 F8 83 EC 64 53"
#define sig_set_angles "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8"
#define sig_prediction_random_seed "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04"

// custom for the hooks
#define sig_key_values_engine "85 C0 74 ? 51 6A ? 56 8B C8 E8 ? ? ? ? 8B F0"
#define sig_key_values_client "85 C0 74 ? 6A ? 6A ? 56 8B C8 E8 ? ? ? ? 8B F0"

namespace csgo {
	extern player_t* local_player;
}