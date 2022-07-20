#pragma once
#include "source-sdk/math/vector3d.hpp"
#include "source-sdk/classes/c_usercmd.hpp"

class player_move_helper {
public:
	virtual	char const* get_name(/*c_handle handle*/) const = 0;
	virtual void set_host(player_t* host) = 0;
	virtual void reset_touch_list(void) = 0;
	virtual bool pad() = 0;
	virtual void process_impacts(void) = 0;
	virtual void con_nprintf(int idx, char const* fmt, ...) = 0;
	virtual void start_sound(const vec3_t& origin, int channel, char const* sample, float volume, int soundlevel, int fFlags, int pitch) = 0;
	virtual void start_sound(const vec3_t& origin, const char* soundname) = 0;
	virtual void playback_event_full(int flags, int clientindex, unsigned short eventindex, float delay, vec3_t& origin, vec3_t& angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2) = 0;
	virtual bool player_falling_damage(void) = 0;
	virtual void player_set_animation(/*player_anim_t playerAnim*/) = 0;
	virtual void* get_surface_props(void) = 0;
	virtual bool is_world_entity(/*const c_handle& handle*/) = 0;
};

class player_move_data {
public:
	bool    first_run_of_instructions : 1;
	bool    game_code_moved_player : 1;
	int     player_handle;
	int     impulse_command;
	vec3_t	view_angles;
	vec3_t	abs_view_angles;
	int     buttons;
	int     old_buttons;
	float   fw_move;
	float   sd_move;
	float   up_move;
	float   max_speed;
	float   client_max_speed;
	vec3_t	velocity;
	vec3_t	angles;
	vec3_t	old_angles;
	float   step_height;
	vec3_t	wish_velocity;
	vec3_t	jump_velocity;
	vec3_t	constraint_center;
	float   constraint_radius;
	float   constraint_width;
	float   constraint_speed_factor;
	float   u0[ 5 ];
	vec3_t	abs_origin;
};

class virtual_game_movement {

public:
	virtual					~virtual_game_movement( void ) {}
	virtual void			process_movement( player_t *player, player_move_data *move ) = 0;
	virtual void			reset( void ) = 0;
	virtual void			start_track_prediction_errors( player_t *player ) = 0;
	virtual void			finish_track_prediction_errors( player_t *player ) = 0;
	virtual void			diff_print( char const *fmt, ... ) = 0;
	virtual vec3_t const	&get_player_mins( bool ducked ) const = 0;
	virtual vec3_t const	&get_player_maxs( bool ducked ) const = 0;
	virtual vec3_t const	&get_player_view_offset( bool ducked ) const = 0;
	virtual bool			is_moving_player_stuck( void ) const = 0;
	virtual player_t		*get_moving_player( void ) const = 0;
	virtual void			unblock_posher( player_t *player, player_t *pusher ) = 0;
	virtual void			setup_movement_bounds( player_move_data *move ) = 0;
};

class player_game_movement : public virtual_game_movement {
public:
	virtual ~player_game_movement(void) { }
};

class player_prediction {
public:
	virtual ~player_prediction(void) = 0; // destructor

	virtual void init(void) = 0;
	virtual void shutdown(void) = 0;
	virtual void update(int startframe, bool validframe, int incoming_acknowledged, int outgoing_command) = 0;
	virtual void pre_entity_packet_received(int commands_acknowledged, int current_world_update_packet) = 0;
	virtual void post_entity_packet_received(void) = 0;
	virtual void post_network_data_received(int commands_acknowledged) = 0;
	virtual void on_received_uncompressed_packet(void) = 0;
	virtual void get_view_origin(vec3_t& org) = 0;
	virtual void set_view_origin(vec3_t& org) = 0;
	virtual void get_view_angles(vec3_t& ang) = 0;
	virtual void set_view_angles(vec3_t& ang) = 0;
	virtual void get_local_view_angles(vec3_t& ang) = 0;
	virtual void set_local_view_angles(vec3_t& ang) = 0;
	virtual bool in_prediction(void) const = 0;
	virtual bool is_first_time_predicted(void) const = 0;
	virtual int get_last_acknowledged_command_number(void) const = 0;
	virtual int get_incoming_packet_number(void) const = 0;
	virtual void check_moving_ground(player_t* player, double frametime) = 0;
	virtual void run_command(player_t* player, c_usercmd* cmd, player_move_helper* moveHelper) = 0;
	virtual void setup_move(player_t* player, c_usercmd* cmd, player_move_helper* pHelper, player_move_data* move) = 0;
	virtual void finish_move(player_t* player, c_usercmd* cmd, player_move_data* move) = 0;
	virtual void set_ideal_pitch(int nSlot, player_t* player, const vec3_t& origin, const vec3_t& angles, const vec3_t& viewheight) = 0;
	virtual void check_error(int nSlot, player_t* player, int commands_acknowledged) = 0;
	virtual void _update(int nSlot, bool received_new_world_update, bool validframe, int incoming_acknowledged, int outgoing_command) = 0;
};
