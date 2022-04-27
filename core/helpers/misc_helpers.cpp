#include "misc_helpers.hpp"

void custom_helpers::print_to_console(const char* text) {
	if (text == nullptr) return;
	interfaces::console->console_printf(text);
}

void custom_helpers::state_to_console(const char* tag,  const char* text) {
	if (text == nullptr || tag == nullptr) return;
	interfaces::console->console_printf("[NullHooks] [%s] %s\n", tag, text);
}

void custom_helpers::state_to_console_color(const char* tag, const char* text) {
	if (text == nullptr || tag == nullptr) return;
	
	// ???
	interfaces::console->console_color_printf(color::red(255), "[NullHooks] [%s] %s\n", tag, text);
	interfaces::console->console_color_printf(color::blue(255), "[NullHooks] [%s] %s\n", tag, text);
	interfaces::console->console_color_printf(color::green(255), "[NullHooks] [%s] %s\n", tag, text);
}