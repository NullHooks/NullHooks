#include "core/menu/global_input.hpp"

// Initialize the arrays and what not
void GlobalInput::Init() {
    for (int n = 0; n < 256; n++) {
        key_states[n].pressed = false;
        key_states[n].held = false;
    }
}

// Get key states and store them. Then check that instead of GetAsyncKeyState()
void GlobalInput::UpdateGetKeyState() {
    for (int n = 0; n < 256; n++) {
        const short sKey = GetAsyncKeyState(n);

        key_states[n].held = sKey;                  // 0x8000 for held
        key_states[n].pressed = (sKey & 0x0001);    // 0x0001 for press (changed state)
    }
}

// Every paint_traverse iteration
void GlobalInput::UpdatePressed() {
    for (int n = 0; n < 256; n++) {
        // Pressed will be true on the first paint_traverse iteration that the key was pressed
        if (key_states[n].held && !key_states_old[n].held)
            key_states[n].pressed = true;
            //interfaces::console->color_printf(valve_color_t{0,255,0,255}, "Key pressed: 0x%04x\n", n);
        else
            key_states[n].pressed = false;

        // Move all the array to the old one
        key_states_old[n].held = key_states[n].held;
        key_states_old[n].pressed = key_states[n].pressed;
    }
}

// Every WndProc iteration
void GlobalInput::WndProcUpdate(UINT msg, WPARAM wparam, LPARAM lparam) {
    //static std::array<KeyStateInfo, 256> key_states_alt;
    switch (msg) {
        /* -------------- Keyboard -------------- */
        case WM_SYSKEYDOWN:                         // Alt + Key
        case WM_KEYDOWN: {                          // Key being held
            // Just store when the key is held. We will check pressed in paint_traverse (see GlobalInput::UpdatePressed())
            key_states[wparam].held = true;         // Start holding on keydown
            break;
        }
        case WM_SYSKEYUP:
        case WM_KEYUP: {                            // The WndProc iteration that the key was released
            key_states[wparam].pressed = false;     // Reset both states
            key_states[wparam].held = false;        // Stop holding in keyup
            break;
        }
        /* -------------- Mouse -------------- */
        case WM_LBUTTONDBLCLK:                      // Also get fast clicking
        case WM_LBUTTONDOWN: {                      // WndProc iteration where the click is being held
            key_states[VK_LBUTTON].held = true;
            break;
        }
        case WM_LBUTTONUP: {                        // The WndProc iteration that the click was released
            key_states[VK_LBUTTON].pressed = false;
            key_states[VK_LBUTTON].held = false;
            break;
        }
        default: return;
    }
}

// Moved IsPressed and IsHeld inline functions to hpp file