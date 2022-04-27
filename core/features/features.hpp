#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../menu/variables.hpp"

namespace misc {
	namespace movement {
		void bunny_hop(c_usercmd* cmd);
	};
	void spectators();
	void custom_crosshair();
	//void custom_net_graph();
}

namespace visuals {
	void boxesp();
    void nameesp();
	namespace noflash {
		void handle();
	}
}

// -----------------------------------------------------

/*
class cbullet_tracer
{
public:
    void log(i_game_event* event);
    void render();
private:
    class cbullet_tracer_info
    {
    public:
        
        cbullet_tracer_info(vec3_t src, vec3_t dst, float time, color color)
        {
            this->src = src;
            this->dst = dst;
            this->time = time;
            this->color = color;
        }
        
        vec3_t src, dst;
        float time;
        color color;
    };

    std::vector<cbullet_tracer_info> logs;
};

*/