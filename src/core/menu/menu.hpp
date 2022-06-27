#pragma once
#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/framework.hpp"
#include "core/menu/variables.hpp"

namespace menu {
	inline int current_tab {1};

	void render();
	void check_toggle();
};

namespace watermark {
	void draw();
	void draw_stats();
	void draw_stats_string(std::string ts, color tscolor, std::string fps, color fpscolor, std::string speed, color speedcolor, bool draw_speed);

	namespace helpers {
		int get_fps() noexcept;
		std::string get_timestamp_string() noexcept;
	};
};