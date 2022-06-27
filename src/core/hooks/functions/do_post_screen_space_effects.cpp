#include "core/hooks/hooks.hpp"

void __stdcall hooks::do_post_screen_space_effects::hook(const void* viewSetup) {
	if (!(interfaces::engine->is_taking_screenshot() && variables::misc::clean_screenshots))
		visuals::glow::draw_glow();

	original(interfaces::clientmode, viewSetup);
}
