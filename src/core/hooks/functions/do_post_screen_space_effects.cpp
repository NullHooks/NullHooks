#include "core/features/features.hpp"
#include "core/menu/variables.hpp"
#include "core/hooks/hooks.hpp"

void __stdcall hooks::do_post_screen_space_effects::hook(const void* viewSetup) {
	if (!(interfaces::engine->is_taking_screenshot() && variables::misc::clean_screenshots))
		visuals::glow();

	original(interfaces::clientmode, viewSetup);
}
