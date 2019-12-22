#include <SDL2/SDL.h>

#include "texture.h"

#ifndef ENTITY_H
#define ENTITY_H

class Entity{
    public:
        int speed;

        SDL_Point axis_speed;
        SDL_Rect collition_box;

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

        void move_up();
        void move_down();
        void move_left();
        void move_right();

        void stop_x();
        void stop_y();
};

#endif