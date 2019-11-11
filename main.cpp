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
#include "overworld.h"



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
        printf("%ul\n", (long)( frame_count/( (SDL_GetTicks()-start_timer) /1000.f)) );
    }

    return 0;
};