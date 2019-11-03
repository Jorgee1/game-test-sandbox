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

        void render(){
            collition_box.render();
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

    OverWorld(){
        this->window = NULL;
        this->control_rules = NULL;
    }

    OverWorld(Window &window, controls &control_rules){
        this->window = &window;
        this->control_rules = &control_rules;
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

        if(key_state[control_rules->action_button]){

        }
    }

    void render(){
        for(int i=0; i<actors.size(); i++){
            actors[i]->update_position();
            actors[i]->render();
        }
    }
};

class Menu{
    TextureText text;
    Window *window;
    const Uint8* key_state = SDL_GetKeyboardState(NULL);

    Menu(){
        Window *window = NULL;
    }

    Menu(Window &window){
        this->window = &window;
    }

    void check_player_actions(){
        if(key_state[SDL_SCANCODE_UP]){

        }
    }

    void render(){
        text.render_text("ASDf", 0,0);
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

    OverWorld test_room(window, game_controls, view_selector);
    test_room.add_entity(player);
    test_room.add_entity(
        Entity(window.get_render(),
            colors["white"],
            {100, 100, 20, 100},
            10
        )
    );
    test_room.add_entity(
        Entity(
            window.get_render(),
            colors["red"],
            {40, 300, 20, 100},
            10
        )
    );

    bool exit = false;
    while(!exit){
        if(window.check_exit()){
            exit = true;
        }else{
            window.clear_screen();

            if (view_selector == 0){
                test_room.check_player_actions();
                test_room.render();
            }else if (view_selector == 1){

            }


            window.update_screen();
        }
    }

    return 0;
};