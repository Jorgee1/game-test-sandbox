#include <SDL2/SDL.h>

#include "texture.h"

#ifndef ENTITY_H
#define ENTITY_H

class Entity{
    private:
        int speed;
        int direction;
        
    public:
        enum direction_id{
            FACE_UP,
            FACE_DOWN,
            FACE_LEFT,
            FACE_RIGHT,
            FACE_TOTAL
        };

        SDL_Point axis_speed;
        SDL_Rect collition_box;

        Texture sprite;
        TextureBlock bound_box;

        Entity();
        ~Entity();


        Entity(
            SDL_Renderer*,
            SDL_Color,
            SDL_Rect,
            int
        );


        void render(int, int);

        void update_position();

        void load_sprite(std::string, int scale = 1);

        void move_up();
        void move_down();
        void move_left();
        void move_right();

        void stop_x();
        void stop_y();

        int get_direction();
        void set_direction(int);
};

#endif