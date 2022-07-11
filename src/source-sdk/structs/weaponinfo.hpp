#pragma once

class weapon_info_t {
public:
	char		    u0[20];
	__int32		    weapon_max_clip;
	char		    u2[112];
	const char*     weapon_name;
	char		    u3[60];
	__int32		    weapon_type;	// See WEAPONTYPE_*
	char		    u4[4];
	__int32		    weapon_price;
	char		    u5[8];
	float           weapon_cycle_time;
	char		    u6[12];
	unsigned char	weapon_full_auto;		// bool?
	char		    u7[3];
	__int32		    weapon_damage;
	float           weapon_headshot_multiplier;
	float		    weapon_armor_ratio;
	__int32		    weapon_bullets;
	float		    weapon_penetration;
	char		    u8[8];
	float		    weapon_range;
	float		    weapon_range_mod;
	char		    u9[16];
	unsigned char	weapon_has_silencer;	// bool?
	char		    u10[15];
	float		    weapon_max_speed;
	float		    weapon_max_speed_alt;
	char		    u11[100];
	float           weapon_recoil_magnitude;
	float           weapon_recoil_magnitude_alt;
	char		    u12[16];
	float           recovery_time_stand;
};