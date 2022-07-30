#pragma once
#include "source-sdk/sdk.hpp"
#include "core/features/misc/backtrack.hpp"

namespace antiaim {
	void run_antiaim(c_usercmd* cmd, bool& sendPacket);
}

namespace animations {
	// @todo: resolver
	namespace local {
		void run_local_animations();
	}
}

namespace aim {
	void triggerbot(c_usercmd* cmd);
	void run_aimbot(c_usercmd* cmd);

	bool can_fire(player_t* target);
	bool aimbot_weapon_check(bool check_scope);
	void draw_fov();

	namespace autowall {
		float get_damage_multiplier(int hit_group, float hs_multiplier);
		bool is_armored(int hit_group, bool helmet);
		bool trace_to_exit(trace_t& enter_trace, vec3_t& start, const vec3_t& direction, vec3_t& end, trace_t& exit_trace);
		static bool handle_bullet_penetration(surface_data* enter_surface_data, trace_t& enter_trace, const vec3_t& direction, vec3_t& start, float penetration, float& damage);
		bool handle_walls(player_t* local_player, entity_t* entity, const vec3_t& destination, const weapon_info_t* weapon_data, int min_damage, bool enabled_hitbox);
	}
}

namespace visuals {
	/* ------------------- ESP ------------------- */
	void playeresp();
	void entity_esp();

	namespace entity_info {
		void bomb(entity_t* bomb_ent);
		void dropped_bomb(entity_t* bomb_ent);
		void weapon_name(entity_t* entity, const char* text, int y_offset);
	}

	/* ------------- GLOW AND CHAMS ------------- */
	void glow();
	void draw_chams(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix);

	/* ------------------ MISC ------------------ */
	void motion_blur(view_setup_t* setup) noexcept;
	void noflash();
	void bullet_tracer(i_game_event *event);
	void nade_predict() noexcept;
	void nosmoke(client_frame_stage_t frame_stage);
	void worldcolor();

	namespace custom_models {
		void replace_model(char* path);
	}

	namespace crosshair {
		void draw_custom_crosshair(int x, int y, int outline, color cross_color);
		void custom_crosshair();
		void recoil_crosshair();
	}
}

namespace misc {
	void spectator_list();
	void thirdperson();
	void reset_thirdperson();

	namespace movement {
		void infinite_duck(c_usercmd* cmd);
		
		void bunny_hop(c_usercmd* cmd);
		void edgejump(c_usercmd* cmd, int old_flags);
		void edgebug(c_usercmd* cmd, int old_flags);
		
		void pre_pred_jumpbug(c_usercmd* cmd, int old_flags);
		void post_pred_jumpbug(c_usercmd* cmd, int old_flags);

		void slow_walk(c_usercmd* cmd);
	}

	namespace speedgraph {
		void update();
		void draw();
	}
}

namespace button_functions {
	void exec_autoexec();
	void full_update();
}

namespace prediction {
	void start(c_usercmd* cmd);
	void post_think();
	void end();

	inline player_move_data data{};
	inline bool custom_inpred{};
	inline float old_cur_time{};
	inline float old_frame_time{};
	inline int* prediction_random_seed{};
	inline int* prediction_player{};
};

