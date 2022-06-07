#include "global_input.hpp"

// Get key states and store them. Then check that instead of GetAsyncKeyState()
void GlobalInput::Update() {
    for (int n = 0; n < 256; n++) {
        const short sKey = GetAsyncKeyState(n);

        key_states[n].held = sKey;                  // 0x8000 for held
        key_states[n].pressed = (sKey & 0x0001);    // 0x0001 for press (changed state)
    }
}

void GlobalInput::WndProcUpdate(UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN: {
            if (wparam == VK_INSERT)    // Toggle menu
                variables::ui::menu::opened = !variables::ui::menu::opened;

            key_states[wparam].pressed = true;
            break;
        }
        case WM_SYSKEYUP:
        case WM_KEYUP: {
            key_states[wparam].pressed = false;
            break;
        }
    }
}

bool GlobalInput::IsPressed(const int vKey) const {
    return key_states[vKey].pressed;
}

bool GlobalInput::IsHeld(const int vKey) const {
    return key_states[vKey].held;
}