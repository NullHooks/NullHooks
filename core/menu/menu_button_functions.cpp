#include "framework.hpp"
#include "../features/features.hpp"

void menu_button_fuctions::exec_button_function(int id) {
	switch (id) {
		case 0:		commands_features::test_command();		break;
		case 1:		commands_features::disconnect();		break;
	}
}