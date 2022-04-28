#include "../features.hpp"

void commands_features::test_command() {
	interfaces::engine->execute_cmd("echo Command test...");
	interfaces::console->console_printf("Print test...\n");
	//interfaces::console->console_color_printf({ 255, 0, 0, 255 }, "Color test...\n");
}

void commands_features::disconnect() {
	interfaces::engine->execute_cmd("disconnect");
	//interfaces::console->console_color_printf({ 255, 0, 0, 255 }, "Disconnected...\n");
}