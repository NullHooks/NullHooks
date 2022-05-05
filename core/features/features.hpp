#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../helpers/misc_helpers.hpp"
#include "../menu/variables.hpp"

namespace visuals {
	void playeresp();
	void grenade_projectile_esp();

	void noflash();

	namespace glow {
		void draw_c4();
	};

	namespace misc {
		void nade_predict() noexcept;
	};

	namespace entity_info {		// paint_traverse
		void bomb(entity_t* bomb_ent);
		void dropped_bomb(entity_t* bomb_ent);
		void weapon_name(entity_t* entity, const char* text);
	};

	namespace crosshair {
		void draw_custom_crosshair(int x, int y, int outline, color cross_color);
		void custom_crosshair();
		void recoil_crosshair();
	};
};

namespace misc {
	namespace movement {
		void bunny_hop(c_usercmd* cmd);
	};
	void spectator_list();
};

namespace commands_features {
	void disconnect();
	void test_command();
	void exec_autoexec();
};