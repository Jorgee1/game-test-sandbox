#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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
    string PATH_FONT = "assets/fonts/LiberationMono-Regular.ttf";
    string PATH_ICON = "assets/icon.bmp";
    string PATH_MAP  = "assets/map/test.map";
    
    int SCREEN_WIDTH  = 800;
    int SCREEN_HEIGHT = 600;
    int view_selector = 0;
    int TILE_SIZE = 100;

    SDL_Color colors[TOTAL_TEXT] = {
        {0x00, 0x00, 0x00, 0xFF},
        {0xFF, 0x00, 0x00, 0xFF},
        {0x00, 0xFF, 0x00, 0xFF},
        {0x00, 0x00, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF}
    };

    controls controls_rules = {
        SDL_SCANCODE_Z,
        SDL_SCANCODE_X,

        SDL_SCANCODE_RETURN,
        
        SDL_SCANCODE_UP,
        SDL_SCANCODE_DOWN,
        SDL_SCANCODE_LEFT,
        SDL_SCANCODE_RIGHT
    };
    controls_locks locks;

    Window window("Test", SCREEN_WIDTH, SCREEN_HEIGHT, colors[TEXT_BLACK]);
    window.set_icon(PATH_ICON);

    TextureText text_normal_obj[TOTAL_TEXT];

    for(int i=0; i<TOTAL_TEXT; i++){
        text_normal_obj[i].init(window.get_render(), PATH_FONT, colors[i], 18);
    }


    OverWorld test_room(
        window,
        controls_rules,
        locks,
        view_selector,
        text_normal_obj[TEXT_BLACK],
        PATH_MAP,
        TILE_SIZE
    );

    Entity player(
        window.get_render(),
        colors[TEXT_WHITE],
        {10, 10, TILE_SIZE, TILE_SIZE},
        10
    );

    Entity dummy(window.get_render(),
        colors[TEXT_BLUE],
        {143, 141, TILE_SIZE/2, TILE_SIZE},
        10
    );

    Entity dummy2(
        window.get_render(),
        colors[TEXT_GREEN],
        {43, 322, TILE_SIZE, TILE_SIZE},
        10
    );

    test_room.add_player(player);
    test_room.add_entity(dummy);
    test_room.add_entity(dummy2);

    Menu menu_screen(
        window,
        controls_rules,
        locks,
        view_selector,
        text_normal_obj
    );

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

                if(view_selector==1){
                    menu_screen.reset_option();
                }
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