#pragma once
#include <cstdint>
#include "../../source-sdk/math/vector3d.hpp"

namespace utilities {
	std::uint8_t* pattern_scan(const char* module_name, const char* signature) noexcept;
}
