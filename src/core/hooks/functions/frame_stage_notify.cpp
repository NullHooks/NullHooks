#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/hooks/hooks.hpp"

void __stdcall hooks::frame_stage_notify::hook(client_frame_stage_t frame_stage) {
	original(interfaces::client, frame_stage);
}