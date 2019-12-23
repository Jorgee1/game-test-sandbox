#include <SDL2/SDL.h>

#include <string>
#include <vector>

#include "window.h"
#include "texture.h"
#include "entity.h"
#include "action.h"

#ifndef OVERWORLD_H
#define OVERWORLD_H

class LayerMap{
    public:
        int **map_data;
        int h, w;

        LayerMap();
        ~LayerMap();

        void create_map_array();

        void init(std::string);
};


class OverWorld{
    public:
        int         PLAYER;
        int      tile_size;
        int *view_selector;
        bool      colition;

        SDL_Rect    camara;

        Window*     window;
        Action*     action;
        std::vector<Entity*> actors;

        TextureText*  text;
        TextureBlock floor_tiles[4];

        LayerMap world_map;

        OverWorld();

        OverWorld(
            Window*,  Action*,
            TextureText*,
            std::string mappath,
            int *, int
        );

        void add_player(Entity &);
        void add_entity(Entity &);

        void check_player_actions();
        void check_entity_colition();
        bool box_colition(
            SDL_Rect,
            SDL_Rect
        );

        void update_actors_position();
        void update_camara();
        void update_word();

        void render_actors();
        void render_floor();
        void render_overlay();
        void render_world();
};

#endif