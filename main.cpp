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
            collition_box.render_fill(offset_x, offset_y);
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

class OverWorld{
    public:
    Window *window;
    vector<Entity*> actors;
    controls *control_rules;
    TextureText text;
    TextureBlock temp_block;

    int *view_selector;

    int PLAYER = 0;
    const Uint8* key_state = SDL_GetKeyboardState(NULL);
    
    SDL_Rect camara = {(-800/2)+50, (-600/2)+50, 800, 600};

    bool colition = false;

    bool enter_lock = true;
    int world_map[10][10] = {
        {0, 1, 2, 1, 2, 0, 1, 2, 1, 2},
        {2, 1, 2, 0, 1, 2, 1, 2, 0, 0},
        {0, 1, 1, 2, 1, 2, 2, 1, 2, 0},
        {1, 2, 1, 2, 0, 1, 2, 1, 2, 0},
        {0, 1, 2, 1, 2, 0, 1, 2, 1, 2},
        {2, 1, 2, 0, 1, 0, 1, 2, 1, 2},
        {0, 1, 2, 1, 2, 1, 2, 0, 1, 2},
        {0, 1, 2, 1, 2, 2, 1, 2, 0, 1},
        {0, 1, 0, 1, 2, 1, 2, 2, 1, 2},
        {0, 1, 1, 2, 1, 0, 1, 2, 0, 2},
    };

    OverWorld(){
        window = NULL;
        control_rules = NULL;
        view_selector = NULL;
    }

    OverWorld(Window &window, controls &control_rules, int &view_selector){
        this->window = &window;
        this->control_rules = &control_rules;
        this->view_selector = &view_selector;
        text.init(this->window->get_render(), "fonts/LiberationMono-Regular.ttf", {0x00, 0x00, 0x00, 0xFF}, 18);
        temp_block.init(this->window->get_render(), {0xFF, 0xFF, 0xFF, 0xFF}, {0,0,0,0});
    }

    void add_player(Entity &actor){
        PLAYER = actors.size();
        actors.push_back(&actor);
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

        if((key_state[control_rules->start_button]) && (!enter_lock)) {
            *view_selector = 1;
            enter_lock = true;
        }

        if(!key_state[control_rules->start_button]) {
            enter_lock = false;
        }
    }

    void check_entity_colition(){
        SDL_Rect box1 = actors[PLAYER]->collition_box.rect;
        SDL_Rect box2;

        box1 = {box1.x  + actors[PLAYER]->axis_speed.x, box1.y + actors[PLAYER]->axis_speed.y, box1.w, box1.h};

        colition = false;

        for(int i=0; i<actors.size(); i++){

            if(PLAYER != i){
            
                box2 = actors[i]->collition_box.rect;

                if(
                    (box1.x + box1.w > box2.x)
                                &&
                    (box1.x < box2.x + box2.w)
                                &&
                    (box1.y + box1.h > box2.y)
                                &&
                    (box1.y < box2.y + box2.h)
                ){
                    colition = true;

                    if(
                        (box1.x + box1.w > box2.x)
                                    &&
                        (box1.x < box2.x + box2.w)
                                    &&
                        (actors[PLAYER]->collition_box.rect.y + box1.h > box2.y)
                                    &&
                        (actors[PLAYER]->collition_box.rect.y < box2.y + box2.h)
                    ){
                        if(actors[PLAYER]->axis_speed.x > 0){
                            actors[PLAYER]->axis_speed.x = box2.x - actors[PLAYER]->collition_box.rect.x - actors[PLAYER]->collition_box.rect.w;
                        }else if(actors[PLAYER]->axis_speed.x < 0){
                            actors[PLAYER]->axis_speed.x = box2.x + box2.w - actors[PLAYER]->collition_box.rect.x;
                        }
                    }

                    else if(
                        (actors[PLAYER]->collition_box.rect.x + box1.w > box2.x)
                                    &&
                        (actors[PLAYER]->collition_box.rect.x < box2.x + box2.w)
                                    &&
                        (box1.y + box1.h > box2.y)
                                    &&
                        (box1.y < box2.y + box2.h)
                    ){
                        if(actors[PLAYER]->axis_speed.y > 0){
                            actors[PLAYER]->axis_speed.y = box2.y - actors[PLAYER]->collition_box.rect.y - actors[PLAYER]->collition_box.rect.h;
                        }else if(actors[PLAYER]->axis_speed.y < 0){
                            actors[PLAYER]->axis_speed.y = box2.y + box2.h - actors[PLAYER]->collition_box.rect.y;
                        }
                    }
                }
            }

        }
    }

    bool box_collition(SDL_Rect box1, SDL_Rect box2){
        if( 
            ( 
                range_colition(box1.x,  box1.w, box2.x,  box2.w)
            )
            &&
            (
                range_colition(box1.y,  box1.h, box2.y,  box2.h)
            )
        ){
            return true;
        }
        return false;
    }

    bool range_colition(int x1, int d1, int x2, int d2){
        if(
            ( 
                (
                    (x1 < x2)
                    &&
                    (x1 + d1 > x2)
                )
                ||
                (
                    (x1 < x2 + d2) 
                    &&
                    (x1 + d1 > x2 + d2)
                )
            )
            ||
            ( 
                (
                    (x2 < x1) 
                    &&
                    (x2 + d2 > x1))
                ||
                (
                    (x2 < x1 + d1)
                    &&
                    (x2 + d2 > x1 + d1)
                )
            )
        ){
            return true;
        }
        return false;
    }

    void render_floor(){
        SDL_Color color[4] = {
            {0x22, 0x45, 0xA7, 0xFF},
            {0xFF, 0xAF, 0x60, 0xFF},
            {0x00, 0x3D, 0x11, 0xFF},
            {0x60, 0x15, 0xFF, 0xFF},
        };
        TextureBlock temp(window->get_render(), {0xFF, 0, 0}, 0, 0, 100, 100);
        for(int i=0; i<10; i++){
            for(int j=0; j<10; j++){
                temp.rect.x = j*100;
                temp.rect.y = i*100;
                if(box_collition(camara, temp.rect)){
                    temp.color = color[world_map[j][i]];
                    temp.render_fill(-camara.x, -camara.y);
                }
            }
        }
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
            if(box_collition(camara, actors[i]->collition_box.rect)){
                actors[i]->render(-camara.x, -camara.y);
            }
        }
    }


    void render(){
        
        update_actors_position();
        update_camara();

        render_floor();
        render_actors();



        text.set_text_size("X:" + to_string(camara.x) + ", Y:" + to_string(camara.y));
        temp_block.rect.x = 0;
        temp_block.rect.y = 100;
        temp_block.rect.h = text.h;
        temp_block.rect.w = text.w;
        temp_block.render_fill();
        text.render(0, 100);


        if(colition){
            text.set_text_size("COLITION");
            temp_block.rect.x = 0;
            temp_block.rect.y = 0;
            temp_block.rect.h = text.h;
            temp_block.rect.w = text.w;
            temp_block.render_fill();
            text.render(0, 0);
        }


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

    Entity player(
        window.get_render(),
        colors["white"],
        {15, 15, 100, 100},
        10
    );

    Entity dummy(window.get_render(),
        colors["green"],
        {300, 140, 20, 100},
        10
    );

    Entity dummy2(
        window.get_render(),
        colors["red"],
        {43, 322, 100, 100},
        10
    );

    OverWorld test_room(window, game_controls, view_selector);
    test_room.add_entity(dummy);
    test_room.add_entity(dummy2);

    test_room.add_player(player);

    Menu menu_screen(window, game_controls, colors, view_selector);

    bool exit = false;
    while(!exit){
        if(window.check_exit()){
            exit = true;
        }else{
            window.clear_screen();

            if (view_selector == 0){
                test_room.check_player_actions();
                test_room.check_entity_colition();
                test_room.render();
            }else if (view_selector == 1){
                menu_screen.check_player_actions();
                menu_screen.render();
            }

            SDL_SetRenderDrawColor(window.get_render(),0x00, 0x00, 0xFF, 0xFF);
            window.update_screen();
            //SDL_Delay(3000);
        }
    }

    return 0;
};