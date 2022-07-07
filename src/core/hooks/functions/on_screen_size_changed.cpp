#include "dependencies/utilities/csgo.hpp"
#include "core/hooks/hooks.hpp"

void __fastcall hooks::on_screen_size_changed::hook(void* thisptr, void* edx, int old_width, int old_height) {
	original(thisptr, old_width, old_height);
	
	// Fix fonts (#33)
	render::initialize();
}