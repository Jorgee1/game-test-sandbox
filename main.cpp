#include <SDL.h>
#include <SDL_ttf.h>

#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>
#include <map>

#include "common.h"
#include "texture.h"
#include "window.h"
#include "menu.h"



class Entity{
    public:
        TextureBlock collition_box;
        int speed;
        struct{
            int x;
            int y;
        }axis_speed;

        Entity(){
            speed = 0;
            axis_speed.x = 0;
            axis_speed.y = 0;
        }

        Entity(SDL_Renderer* render, SDL_Color color, SDL_Rect collition_box, int speed){
            axis_speed.x = 0;
            axis_speed.y = 0;
            this->collition_box.init(render, color, collition_box);
            this->speed = speed;
        }

        ~Entity(){
            speed = 0;
            axis_speed.x = 0;
            axis_speed.y = 0;
        }

        void render(int offset_x, int offset_y){
            collition_box.render(collition_box.rect.x + offset_x, collition_box.rect.y + offset_y);
        }

        void update_position(){
            collition_box.rect.x += axis_speed.x;
            collition_box.rect.y += axis_speed.y;
            stop_x();
            stop_y(); 
        }

        void move_up(){
            axis_speed.y = -speed;
        }

        void move_down(){
            axis_speed.y = +speed;
        }

        void move_left(){
            axis_speed.x = -speed;
        }

        void move_right(){
            axis_speed.x = +speed;
        }

        void stop_x(){
            axis_speed.x = 0;
        }

        void stop_y(){
            axis_speed.y = 0;
        }

};

class LayerMap{
    public:
        int **map_data;
        int h, w;

        LayerMap(){
            map_data = NULL;
            h = 0;
            w = 0;
        }

        ~LayerMap(){
            for(int i=0; i<h; i++){
                free(map_data[i]);
            }
            free(map_data);
        }

        void create_map_array(){
            map_data = (int**) malloc(sizeof *map_data * h);

            for(int i=0; i<h; i++){
                map_data[i] = (int*) malloc(sizeof *map_data[i] * w);
            }
        }

        void init(string map_file_path){
            FILE *map_file;
            unsigned int temp = 0;
            map_file = fopen(map_file_path.c_str(), "rb");

            int index_x = 0;
            int index_y = 0;
            while ( !feof(map_file) ){
                if(fread(&temp, 1, 1, map_file)){
                    if(temp != 0xFF){
                        index_x++;
                    }else{
                        w = index_x;
                        index_y++;
                        index_x = 0;
                    }
                }
            }
            h = index_y;

            create_map_array();

            rewind(map_file);

            index_x = 0;
            index_y = 0;

            while ( !feof(map_file) ){
                if(fread(&temp, 1, 1, map_file)){
                    if(temp != 0xFF){
                        map_data[index_y][index_x] = temp;
                        index_x++;
                    }else{
                        index_y++;
                        index_x = 0;
                    }
                }
            }

            fclose (map_file);
            map_file = NULL;
        }
};

class OverWorld{
    public:
    Window *window;
    TextureText text;
    SDL_Rect camara;

    controls *control_rules;
    int *view_selector;
    
    vector<Entity*> actors;
    TextureBlock floor_tiles[4];

    int PLAYER;
    int tile_size = 100;

    const Uint8* key_state;
    
    bool colition = false;
    bool enter_lock = true;

    LayerMap world_map;

    OverWorld(){
        window = NULL;
        control_rules = NULL;
        view_selector = NULL;

        key_state = SDL_GetKeyboardState(NULL);
        camara = {0, 0, 10, 10};
        PLAYER = 0;
        colition = false;
        enter_lock = true;
    }

    OverWorld(Window &window, controls &control_rules, int &view_selector, TextureText &general_text, string map_path){
        this->window = &window;
        this->control_rules = &control_rules;
        this->view_selector = &view_selector;
        this->text = general_text;
        
        world_map.init(map_path);
        
        key_state = SDL_GetKeyboardState(NULL);
        camara = {0, 0, window.SCREEN_WIDTH, window.SCREEN_HEIGHT};
        PLAYER = 0;
        colition = false;
        enter_lock = true;



        SDL_Color color[] = {
            {0xDB, 0xD0, 0x53, 0xFF},
            {0x22, 0x45, 0xA7, 0xFF},
            {0xFF, 0xAF, 0x60, 0xFF},
            {0x00, 0x3D, 0x11, 0xFF},
        };

        for(int i=0; i<4; i++){
            floor_tiles[i].init(this->window->get_render(), color[i], {0, 0, tile_size, tile_size});
        }

    }

    void add_player(Entity &actor){
        PLAYER = actors.size();
        actors.push_back(&actor);
        update_camara();
    }

    void add_entity(Entity &actor){
        actors.push_back(&actor);
    }

    void check_player_actions(){
        if(key_state[control_rules->move_up]){
            actors[PLAYER]->move_up();
        }else if(key_state[control_rules->move_down]){
            actors[PLAYER]->move_down();
        }

        if(key_state[control_rules->move_left]){
            actors[PLAYER]->move_left();
        }else if(key_state[control_rules->move_right]){
            actors[PLAYER]->move_right();
        }

        if((key_state[control_rules->start_button]) && (!enter_lock)){
            *view_selector = 1;
            enter_lock = true;
        }

        if(!key_state[control_rules->start_button]){
            enter_lock = false;
        }
    }

    void check_entity_colition(){
        SDL_Rect box1 = {
            actors[PLAYER]->collition_box.rect.x + actors[PLAYER]->axis_speed.x, 
            actors[PLAYER]->collition_box.rect.y + actors[PLAYER]->axis_speed.y,
            actors[PLAYER]->collition_box.rect.w,
            actors[PLAYER]->collition_box.rect.h
        };
        SDL_Rect box2;
        colition = false;
        for(int i=0; i<actors.size(); i++){
            if(PLAYER != i){
            
                box2 = actors[i]->collition_box.rect;

                if(box_colition(box1, box2)){

                    if(box_colition({box1.x, actors[PLAYER]->collition_box.rect.y, box1.w, box1.h}, box2)){
                        if(actors[PLAYER]->axis_speed.x > 0){
                            actors[PLAYER]->axis_speed.x = 0;
                            actors[PLAYER]->collition_box.rect.x = box2.x - actors[PLAYER]->collition_box.rect.w;
                        }else if(actors[PLAYER]->axis_speed.x < 0){
                            actors[PLAYER]->axis_speed.x = 0;
                            actors[PLAYER]->collition_box.rect.x = box2.x + box2.w;
                        }
                        colition = true;
                    }

                    if(box_colition({actors[PLAYER]->collition_box.rect.x, box1.y, box1.w, box1.h}, box2)){
                        if(actors[PLAYER]->axis_speed.y > 0){
                            actors[PLAYER]->axis_speed.y = 0;
                            actors[PLAYER]->collition_box.rect.y = box2.y - actors[PLAYER]->collition_box.rect.h;
                        }else if(actors[PLAYER]->axis_speed.y < 0){
                            actors[PLAYER]->axis_speed.y = 0;
                            actors[PLAYER]->collition_box.rect.y = box2.y + box2.h;
                        }
                        colition = true;
                    }

                    if(!colition){
                        // Corners
                        if(actors[PLAYER]->axis_speed.x > 0){
                            if(actors[PLAYER]->axis_speed.y > 0){
                                //printf("Right Botom Corner Collitino\n");
                            }else if(actors[PLAYER]->axis_speed.y < 0){
                                //printf("Right Top Corner Collitino\n");
                            }
                        }else if(actors[PLAYER]->axis_speed.x < 0){
                            if(actors[PLAYER]->axis_speed.y > 0){
                                //printf("Left Botom Corner Collitino\n");
                            }else if(actors[PLAYER]->axis_speed.y < 0){
                                //printf("Left Top Corner Collitino\n");
                            }
                        }
                        actors[PLAYER]->axis_speed.x = 0;
                        colition = true;
                    }

                }



            }
        }


        if (actors[PLAYER]->axis_speed.x < 0){
            if(box1.x <= 0){
                actors[PLAYER]->axis_speed.x = 0;
                actors[PLAYER]->collition_box.rect.x = 0;
                colition = true; 
            }
        }else if (actors[PLAYER]->axis_speed.x > 0){
            if(box1.x + box1.w >= world_map.w*tile_size){
                actors[PLAYER]->axis_speed.x = 0;
                actors[PLAYER]->collition_box.rect.x = world_map.w*tile_size - box1.w;
                colition = true;
            }
        }

        if (actors[PLAYER]->axis_speed.y < 0){
            if(box1.y <= 0){
                actors[PLAYER]->axis_speed.y = 0;
                actors[PLAYER]->collition_box.rect.y = 0;
                colition = true;
            }
        }else if (actors[PLAYER]->axis_speed.y > 0){
            if(box1.y + box1.h >= world_map.h*tile_size){
                actors[PLAYER]->axis_speed.y = 0;
                actors[PLAYER]->collition_box.rect.y = world_map.h*tile_size - box1.h;
                colition = true;
            }
        }

    }

    bool box_colition(SDL_Rect box1, SDL_Rect box2){
        if(
            (box1.x + box1.w > box2.x)
                        &&
            (box1.x < box2.x + box2.w)
                        &&
            (box1.y + box1.h > box2.y)
                        &&
            (box1.y < box2.y + box2.h)
        ){
            return true;
        }
        return false;
    }


    void update_actors_position(){
        for(int i=0; i<actors.size(); i++){
            actors[i]->update_position();
        }
    }

    void update_camara(){
        camara.x = actors[PLAYER]->collition_box.rect.x - (camara.w/2) + (actors[PLAYER]->collition_box.rect.w/2);
        camara.y = actors[PLAYER]->collition_box.rect.y - (camara.h/2) + (actors[PLAYER]->collition_box.rect.h/2);
    }

    void render_actors(){
        for(int i=0; i<actors.size(); i++){
            if(box_colition(camara, actors[i]->collition_box.rect)){
                actors[i]->render(-camara.x, -camara.y);
            }
        }
    }

    void render_floor(){
        int temp_x = 0;
        int temp_y = 0;

        for(int i=0; i<world_map.h; i++){
            for(int j=0; j<world_map.w; j++){
                temp_x = j*tile_size;
                temp_y = i*tile_size;
                if(box_colition(camara, {temp_x, temp_y, tile_size, tile_size})){
                    floor_tiles[world_map.map_data[i][j]].render(temp_x-camara.x, temp_y-camara.y);
                }
            }
        }
    }

    void render_overlay(){
        string coordenates = "X:" + to_string(actors[PLAYER]->collition_box.rect.x) + " Y:" + to_string(actors[PLAYER]->collition_box.rect.y);
        
        int acc_x = 1;
        int acc_y = 1;

        text.create_texture(coordenates, true);
        text.render(acc_x, acc_y);

        acc_y += text.rect.h + 1;
        if(colition){
            text.create_texture("COLITION", true);
            text.render(acc_x, acc_y);
        }
    }

    void update_word(){
        check_player_actions();
        check_entity_colition();
        update_actors_position();
        update_camara();
    }

    void render_world(){
        render_floor();
        render_actors();


        render_overlay();
    }



};




int main(int argc, char* args[]){
    int view_selector = 0;

    map<string, SDL_Color> colors = {
        {"black", {0x00, 0x00, 0x00, 0xFF}},
        {"red"  , {0xFF, 0x00, 0x00, 0xFF}},
        {"green", {0x00, 0xFF, 0x00, 0xFF}},
        {"blue" , {0x00, 0x00, 0xFF, 0xFF}},
        {"white", {0xFF, 0xFF, 0xFF, 0xFF}},
    };

    controls game_controls = {
        SDL_SCANCODE_X,
        SDL_SCANCODE_Z,
        SDL_SCANCODE_RETURN,
        SDL_SCANCODE_UP,
        SDL_SCANCODE_DOWN,
        SDL_SCANCODE_LEFT,
        SDL_SCANCODE_RIGHT
    };

    Window window("Test", 800, 600, colors["black"]);
    window.set_icon("icon.bmp");

    TextureText normal_text(window.get_render(), "fonts/LiberationMono-Regular.ttf", colors["black"], 18);

    Entity player(
        window.get_render(),
        colors["white"],
        {10, 10, 100, 100},
        10
    );

    Entity dummy(window.get_render(),
        colors["blue"],
        {143, 141, 20, 100},
        10
    );

    Entity dummy2(
        window.get_render(),
        colors["red"],
        {43, 322, 100, 100},
        10
    );

    OverWorld test_room(window, game_controls, view_selector, normal_text, "map/test.map");
    test_room.add_player(player);
    test_room.add_entity(dummy);
    test_room.add_entity(dummy2);



    Menu menu_screen(window, game_controls, colors, view_selector);

    bool exit = false;
    Uint32 frame_count = 0;
    Uint32 start_timer = 0;
    
    start_timer = SDL_GetTicks();
    while(!exit){

        if(window.check_exit()){
            exit = true;
        }else{
            window.clear_screen();

            if (view_selector == 0){
                test_room.update_word();
                test_room.render_world();
            }else if (view_selector == 1){
                menu_screen.check_player_actions();
                menu_screen.render();
            }

            window.update_screen();
        }
        frame_count++;
        //printf("%ul\n", (long)( frame_count/( (SDL_GetTicks()-start_timer) /1000.f)) );
    }

    return 0;
};