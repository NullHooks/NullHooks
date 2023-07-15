#include "dependencies/utilities/csgo.hpp"
#include "core/hooks/hooks.hpp"
#include "core/features/features.hpp"

void __fastcall hooks::overridemouseinput::hook(void* thisptr, void* edx, float* x, float* y) {
	if (globals::disable_mouse)
		*x = 0.f;

	original(thisptr, x, y);
}
