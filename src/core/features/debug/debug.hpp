#include <vector>
#include <deque>
#include "source-sdk/math/vector3d.hpp"

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

	namespace log {
		constexpr size_t MAX_SIZE = 10;
		inline std::deque<std::string> strs;
		void puts(const char *);
		void draw();
	}

	void draw_aimbot_targets();
	void draw_autowall_traces();

}