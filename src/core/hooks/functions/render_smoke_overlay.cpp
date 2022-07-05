#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/hooks/hooks.hpp"
#include "core/menu/variables.hpp"

// https://www.unknowncheats.me/forum/counterstrike-global-offensive/257450-removing-overlay-smoke.html#9
// https://github.com/rollraw/qo0-base/blob/cb140182ee647dc3fd9ebcf156f29b1d7457ad95/base/core/hooks.cpp#L526-L535

#define flSmokeIntensity_offset 0x588

void __fastcall hooks::render_smoke_overlay::hook(void* thisptr, void* edx, bool previewmodel) {
	// Only call original if we want to render smoke
	if (variables::misc_visuals::wireframe_smoke)
		*reinterpret_cast<float*>(std::uintptr_t(interfaces::view_render) + flSmokeIntensity_offset) = 0.0f;	// Used to remove small overlay from hands, etc.
	else 
		original(thisptr, previewmodel);
}