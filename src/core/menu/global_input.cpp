#include "core/menu/global_input.hpp"

// (UNUSED) Get key states and store them. Then check that instead of GetAsyncKeyState()
void GlobalInput::UpdateGetKeyState() {
    for (int n = 0; n < 256; n++) {
        const short sKey = GetAsyncKeyState(n);

        key_states[n].held = sKey;                  // 0x8000 for held
        key_states[n].pressed = (sKey & 0x0001);    // 0x0001 for press (changed state)
    }
}

// Initialize the arrays and what not (Called on top of hooks.cpp)
void GlobalInput::Init() {
    for (int n = 0; n < 256; n++) {
        key_states[n].pressed = false;
        key_states[n].held = false;
    }

    // Digits (VK_0 - VK_9)
    for (int n = 0x30; n <= 0x39; n++) {
        input::key_names.insert({ n, std::string(1, n) });
    }

    // Letters (VK_A - VK_Z)
    for (int n = 0x41; n <= 0x5A; n++) {
        input::key_names.insert({ n, std::string(1, n ) });
    }
}

// Every paint_traverse iteration
void GlobalInput::UpdatePressed() {
    for (int n = 0; n < 256; n++) {
        // Pressed will be true on the first paint_traverse iteration that the key was pressed
        if (key_states[n].held && !key_states_old[n].held)
            key_states[n].pressed = true;
        else
            key_states[n].pressed = false;

        // Avoid toggling the key when assigning
        if (latest_hotkey == n) {
            if (key_states[n].held) {
                key_states[n].held = false;
                key_states[n].pressed = false;
            } else {
                latest_hotkey = INPUT_KEY_NONE;        // Reset key to none
            }
        }

        // Move all the array to the old one
        key_states_old[n].held = key_states[n].held;
        key_states_old[n].pressed = key_states[n].pressed;
    }
}

// Every WndProc iteration
void GlobalInput::WndProcUpdate(UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        /* -------------- Misc -------------- */
        case WM_KILLFOCUS: {                        // Window just unfocused
            // Reset alt + tab to avoid false states
            key_states[VK_MENU].held = false;
            key_states[VK_MENU].pressed = false;
            key_states[VK_TAB].held = false;
            key_states[VK_TAB].pressed = false;
            break;
        }
        case WM_CHAR: {                             // For gettting characters for textbox
            if (input::gobal_input.reading_textbox) {
                if (wparam == 27 || wparam == 8 || wparam == 10) break;                         // Unfocus checks
                if (wparam >= 32 && wparam < 126) {                                             // Valid characters
                    input::gobal_input.wndproc_textbox_buffer += static_cast<char>(wparam);     // Add characters if we reading
                }
            } else {
                // Empty otherwise
                input::gobal_input.wndproc_textbox_buffer = "";
            }
            break;
        }
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
        case WM_RBUTTONDBLCLK:
        case WM_RBUTTONDOWN: {
            key_states[VK_RBUTTON].held = true;
            break;
        }
        case WM_RBUTTONUP: {
            key_states[VK_RBUTTON].pressed = false;
            key_states[VK_RBUTTON].held = false;
            break;
        }
        case WM_MBUTTONDBLCLK:
        case WM_MBUTTONDOWN: {
            key_states[VK_MBUTTON].held = true;
            break;
        }
        case WM_MBUTTONUP: {
            key_states[VK_MBUTTON].pressed = false;
            key_states[VK_MBUTTON].held = false;
            break;
        }
        case WM_XBUTTONDBLCLK:
        case WM_XBUTTONDOWN: {
            if (wparam & MK_XBUTTON1)      key_states[VK_XBUTTON1].held = true;
            else if (wparam & MK_XBUTTON2) key_states[VK_XBUTTON2].held = true;
            break;
        }
        case WM_XBUTTONUP: {
            if (wparam & 0x10000) {             // Mouse4
                key_states[VK_XBUTTON1].pressed = false;
                key_states[VK_XBUTTON1].held = false;
            } else if (wparam & 0x20000) {      // Mouse5
                key_states[VK_XBUTTON2].pressed = false;
                key_states[VK_XBUTTON2].held = false;
            }
            break;
        }
        default: return;
    }
}

// Moved IsPressed and IsHeld inline functions to hpp file