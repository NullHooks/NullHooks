#include "../hooks.hpp"
#include "../../menu/menu.hpp"

LRESULT __stdcall hooks::WndProc_hook::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	// Get inputs from user and store them. Same as we did in paint_traverse
	input::gobal_input.WndProcUpdate(msg, wparam, lparam);

	/*
	if (variables::ui::menu::opened)
		return true;	// Disable input while in menu
	*/

	return CallWindowProcW(original, hwnd, msg, wparam, lparam);
}