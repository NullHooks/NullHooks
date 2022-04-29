#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "framework.hpp"
#include "variables.hpp"

namespace menu {
	inline int current_tab{0};

	void render();
	void toggle();
};

namespace watermark {
	void draw();
	void draw_stats();

	namespace helpers {
		int  get_fps() noexcept;
		std::string get_timestamp_string() noexcept;
	};
};