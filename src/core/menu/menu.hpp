#pragma once
#include "core/menu/framework.hpp"
#include "core/menu/variables.hpp"

namespace menu {
	inline int current_tab{ 1 };

	void render();
	void check_toggle();
	void init_windows();
};
