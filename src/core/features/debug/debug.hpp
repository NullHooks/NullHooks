#include <vector>
#include <deque>
#include "source-sdk/math/vector3d.hpp"
#include "core/features/misc/backtrack.hpp"

namespace debug {

	struct point_t {
		vec3_t origin;
		bool   enabled;
	};

	struct trace_t {
		vec3_t start, end;
		float damage_dropoff;
		bool  in_wall;
	};

	struct shot_t {
		std::vector<trace_t> traces;
		entity_t *entity;
		float estimated_damage;
	};

	inline std::vector<point_t> points;
	inline std::vector<shot_t>  shots;
	inline player_record best_record;
	inline int best_target_idx = 0;
	inline float sv_yaw = 0.0f;
	inline float cl_yaw = 0.0f;

	namespace log {
		constexpr size_t MAX_SIZE = 10;
		inline std::deque<std::string> strs;
		void puts(std::string);
		void draw();
	}

	void draw_angles();
	void draw_aimbot_targets();
	void draw_autowall_traces();

}