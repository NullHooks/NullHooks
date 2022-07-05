#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/hooks/hooks.hpp"
#include "core/menu/variables.hpp"

// https://www.unknowncheats.me/forum/counterstrike-global-offensive/257450-removing-overlay-smoke.html#9

#define flSmokeIntensity_offset 0x588

void __fastcall hooks::render_smoke_overlay::hook(int edx, bool previewmodel) {
	// Only call original if we want to render smoke
	if (variables::misc_visuals::wireframe_smoke)
		*reinterpret_cast<float*>(reinterpret_cast<std::uintptr_t>(interfaces::view_render) + flSmokeIntensity_offset) = 0.0f;
	else
		original(interfaces::view_render, edx, previewmodel);
}