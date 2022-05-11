#include "../features.hpp"

//void commands_features::test_command() {
//	interfaces::engine->execute_cmd("echo Command test...");
//	interfaces::console->console_printf("Print test...\n");
//}
//
//void commands_features::disconnect() {
//	interfaces::engine->execute_cmd("disconnect");
//	custom_helpers::state_to_console("Command", "Disconnecting...\n");
//}

void commands_features::exec_autoexec() {
	interfaces::engine->execute_cmd("exec autoexec");
	custom_helpers::state_to_console("Command", "Executed autoexec...\n");
}