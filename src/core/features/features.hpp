#pragma once
#include "source-sdk/sdk.hpp"

namespace aim {
	void run_aimbot(c_usercmd* cmd);
	void draw_fov();
}

namespace visuals {
	void playeresp();
	void grenade_projectile_esp();

	void noflash();

	namespace glow {
		void draw_glow();
	}

	namespace chams {
		void draw_chams(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix);
	}

	namespace custom_models {
		void replace_model(char* path);
		// TODO: Make a models::init() that checks for file model paths and stores them in static bools (or downloads them)
		//       Call on top of hooks.cpp
	}

	namespace misc {
		void nade_predict() noexcept;
	}

	namespace entity_info {		// paint_traverse
		void bomb(entity_t* bomb_ent);
		void dropped_bomb(entity_t* bomb_ent);
		void weapon_name(entity_t* entity, const char* text, int y_offset);
		//void weapon_icon(entity_t* entity, int class_id);
	}

	namespace crosshair {
		void draw_custom_crosshair(int x, int y, int outline, color cross_color);
		void custom_crosshair();
		void recoil_crosshair();
	}
}

namespace misc {
	namespace movement {
		void bunny_hop(c_usercmd* cmd);
		void edgebug(c_usercmd* cmd, int old_flags);
	}
	void spectator_list();
	namespace speedgraph {
		void update(c_usercmd* cmd);
		void draw();
	}
}

namespace prediction {
	void start(c_usercmd *cmd);
	void end();

	inline player_move_data data;
	inline float old_cur_time;
	inline float old_frame_time;
	inline int *prediction_random_seed;
};

namespace commands_features {
	void exec_autoexec();
}
