#include "dependencies/utilities/csgo.hpp"
#include "core/hooks/hooks.hpp"
#include "core/features/features.hpp"

void __fastcall hooks::emit_sound::hook(void* thisptr, void* edx, int& filter, int ent_index, int channel, const char* sound_entry, unsigned int sound_entry_hash, const char* sample, float volume, int seed, float attenuation, int flags, int pitch, const vec3_t* origin, const vec3_t* direction, void* utl_vec_origins, bool update_positions, float soundtime, int speakerentity, StartSoundParams_t& params) {
	// Avoid prediction noises
	if (!prediction::custom_inpred)
		original(thisptr, filter, ent_index, channel, sound_entry, sound_entry_hash, sample, volume, seed, attenuation, flags, pitch, origin, direction, utl_vec_origins, update_positions, soundtime, speakerentity, params);
}
