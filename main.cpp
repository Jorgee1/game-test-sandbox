#include <SDL.h>
#include <SDL_ttf.h>

#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>
#include <map>

#include "texture.h"
#include "window.h"

struct controls{
    int action_button;
    int cancel_button;
    int start_button;
    int move_up;
    int move_down;
    int move_left;
    int move_right;
};


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

    int *view_selector;

    const int PLAYER = 0;
    const Uint8* key_state = SDL_GetKeyboardState(NULL);
    
    SDL_Rect camara = {(-800/2)+50, (-600/2)+50, 800, 600};

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
    }

    void add_entity(Entity &actor, bool collition = true){
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
        for(int i=1; i<actors.size(); i++){
            if( box_collition(actors[PLAYER]->collition_box.rect, actors[i]->collition_box.rect)){
                //printf("COLITION PLAYER x %i\n", i);
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
                    (x1 + d1 > x2 + d2))
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
        TextureBlock temp(window->get_render(), {0, 0, 0}, 0, 0, 100, 100);
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

    void render(){
        camara.y += actors[PLAYER]->axis_speed.y;
        camara.x += actors[PLAYER]->axis_speed.x;
        for(int i=0; i<actors.size(); i++){
            if(box_collition(camara, actors[i]->collition_box.rect)){
                actors[i]->update_position();
                actors[i]->render(-camara.x, -camara.y);
            }
        }
    }
};

class Menu{
    public:
        TextureText text;
        Window *window;
        const Uint8* key_state = SDL_GetKeyboardState(NULL);
        int *view_selector;
        controls *control_rules;
        map<string, SDL_Color> *colors;

        bool enter_lock = true;

        Menu(){
            window = NULL;
            view_selector = NULL;
            colors = NULL;
        }

        Menu(Window &window, controls &control_rules, map<string, SDL_Color> &colors_main,  int &view_selector){
            this->window = &window;
            this->control_rules = &control_rules;
            this->view_selector = &view_selector;
            colors = &colors_main;
            text.init(this->window->get_render(), "fonts/LiberationMono-Regular.ttf", (*colors)["white"], 18);
        }

        void check_player_actions(){
            if((key_state[control_rules->start_button]) && (!enter_lock)){
                *view_selector=0;
                enter_lock = true;
            }

            if(!key_state[control_rules->start_button]) {
                enter_lock = false;
            }
        }

        void render(){
            text.render_text(0, 0, "ASDf");
        }
};

int main(int argc, char* args[] ){
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

    Entity player(
        window.get_render(),
        colors["white"],
        {0, 0, 100, 100},
        10
    );

    Entity dummy(window.get_render(),
        colors["green"],
        {320, 145, 200, 100},
        10
    );

    Entity dummy2(
        window.get_render(),
        colors["red"],
        {43, 322, 100, 100},
        10
    );

    OverWorld test_room(window, game_controls, view_selector);
    test_room.add_entity(player);
    test_room.add_entity(dummy);
    test_room.add_entity(dummy2);

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
                test_room.render_floor();
                test_room.render();
            }else if (view_selector == 1){
                menu_screen.check_player_actions();
                menu_screen.render();
            }
            window.update_screen();
        }
    }

    return 0;
};