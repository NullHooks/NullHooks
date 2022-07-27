#include "dependencies/utilities/csgo.hpp"
#include "core/hooks/hooks.hpp"

// https://github.com/cazzwastaken/model-frog/blob/master/src/core/hooks.cpp
// MA MAN CAZZ DID IT AGAIN YESSIR

bool __stdcall hooks::loose_files_allowed::hook() {
	return true;	// Allow the files :think:
}

void __fastcall hooks::check_for_pure_server_white_list::hook(void* edx, void* ecx) {
	return;			// Stop the game from checking the whitelist lmao
}