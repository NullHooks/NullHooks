#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "framework.hpp"
#include "variables.hpp"

namespace menu {
	inline int current_tab {1};

	void render();
	void check_toggle();
};

namespace watermark {
	// Will draw the User@NullHooks watermark
	void draw();
	// Will draw the date, fps and ping stats
	void draw_stats();
};