#include "core/hooks/hooks.hpp"
#include "core/menu/menu.hpp"

LRESULT __stdcall hooks::WndProc_hook::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	// Get inputs from user and store them. Same as we did in paint_traverse
	input::global_input.WndProcUpdate(msg, wparam, lparam);

	return CallWindowProcW(original, hwnd, msg, wparam, lparam);
}