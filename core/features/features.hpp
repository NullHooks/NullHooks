#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../helpers/misc_helpers.hpp"
#include "../menu/variables.hpp"

namespace misc {
	namespace movement {
		void bunny_hop(c_usercmd* cmd);
	};
	void spectator_list();
}

namespace visuals {
	void playeresp();
	void active_grenade_esp();

	void noflash();

	namespace misc {
		void nade_predict() noexcept;
	};

	namespace crosshair {
		void draw_custom_crosshair(int x, int y, int outline, color cross_color);
		void custom_crosshair();
		void recoil_crosshair();
	};
}

namespace commands_features {
	void disconnect();
	void test_command();
	void exec_autoexec();
}