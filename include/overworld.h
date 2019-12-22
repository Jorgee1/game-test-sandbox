#include <string>
#include <vector>
#include <SDL2/SDL.h>

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
        Window *window;
        TextureText text;
        SDL_Rect camara;

        Action *action;
        
        int *view_selector;
        
        std::vector<Entity*> actors;
        TextureBlock floor_tiles[4];

        int PLAYER;
        int tile_size = 100;

        const Uint8* key_state;
        
        bool colition = false;

        LayerMap world_map;

        OverWorld();

        OverWorld(
            Window &,
            Action *,
            int &,
            TextureText &,
            std::string mappath,
            int
        );

        void add_player(Entity &);

        void add_entity(Entity &);

        void check_player_actions();

        void check_entity_colition();

        bool box_colition(
            SDL_Rect,
            SDL_Rect);

        void update_actors_position();

        void update_camara();

        void render_actors();

        void render_floor();

        void render_overlay();

        void update_word();

        void render_world();
};

#endif