#include "../hooks.hpp"

void __stdcall hooks::do_post_screen_space_effects::hook(const void* viewSetup) {
	visuals::glow::draw_glow();

	original(interfaces::clientmode, viewSetup);
}