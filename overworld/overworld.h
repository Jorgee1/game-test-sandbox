#include <cstdlib>
#include <string>
#include <vector>
#include <SDL.h>

#include "window.h"
#include "texture.h"
#include "common.h"

#ifndef OVERWORLD_H
#define OVERWORLD_H


class Entity{
    public:
        TextureBlock collition_box;
        int speed;
        struct{
            int x;
            int y;
        }axis_speed;

        Entity();
        ~Entity();


        Entity(SDL_Renderer*, SDL_Color, SDL_Rect, int);


        void render(int, int);

        void update_position();

        void move_up();
        void move_down();
        void move_left();
        void move_right();

        void stop_x();
        void stop_y();
};


class LayerMap{
    public:
        int **map_data;
        int h, w;

        LayerMap();
        ~LayerMap();

        void create_map_array();

        void init(string);
};


class OverWorld{
    public:
        Window *window;
        TextureText text;
        SDL_Rect camara;

        controls *control_rules;
        controls_locks *locks;
        
        int *view_selector;
        
        vector<Entity*> actors;
        TextureBlock floor_tiles[4];

        int PLAYER;
        int tile_size = 100;

        const Uint8* key_state;
        
        bool colition = false;

        LayerMap world_map;

        OverWorld();

        OverWorld(
            Window &,
            controls &,
            controls_locks &,
            int &,
            TextureText &,
            string mappath);

        void add_player(Entity &);

        void add_entity(Entity &);

        void check_player_actions();

        void check_entity_colition();

        bool box_colition(SDL_Rect, SDL_Rect);

        void update_actors_position();

        void update_camara();

        void render_actors();

        void render_floor();

        void render_overlay();

        void update_word();

        void render_world();
};


#endif