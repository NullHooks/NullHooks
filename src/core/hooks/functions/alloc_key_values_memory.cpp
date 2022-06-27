#include "core/hooks/hooks.hpp"

void* __stdcall hooks::alloc_key_values_memory::hook(const std::int32_t size) {
	// If function is returning to speficied addresses, return nullptr to "bypass"
	if (const std::uint32_t address = reinterpret_cast<std::uint32_t>(_ReturnAddress());
		address == reinterpret_cast<std::uint32_t>(interfaces::key_values_engine) ||
		address == reinterpret_cast<std::uint32_t>(interfaces::key_values_client))
		return nullptr;

	// Return original
	return original(interfaces::key_values_system, size);
}