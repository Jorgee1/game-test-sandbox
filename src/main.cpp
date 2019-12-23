#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cstdlib>
#include <string>
#include <ctime>

#include "action.h"
#include "entity.h"
#include "menu.h"
#include "overworld.h"
#include "texture.h"
#include "window.h"


int main(int argc, char* args[]){
    std::string PATH_ASSETS = "asset/";

    std::string PATH_FONT = PATH_ASSETS + "font/LiberationMono-Regular.ttf";
    std::string PATH_MAP  = PATH_ASSETS + "map/test.map";
    std::string PATH_ICON = PATH_ASSETS + "icon.bmp";

    std::string IMG_MAIN_CHAR = PATH_ASSETS + "image/main.bmp";

    int SCREEN_WIDTH  = 800;
    int SCREEN_HEIGHT = 600;
    int TILE_SIZE     = 100;
    int TEXT_SIZE     =  20;
    int view_selector =   0;

    bool exit = false;

    Uint32 frame_count = 0;
    Uint32 start_timer = SDL_GetTicks();

    SDL_Color COLOR_BLACK = {0x00, 0x00, 0x00, 0xFF};
    SDL_Color COLOR_RED   = {0xFF, 0x00, 0x00, 0xFF};
    SDL_Color COLOR_GREEN = {0x00, 0xFF, 0x00, 0xFF};
    SDL_Color COLOR_BLUE  = {0x00, 0x00, 0xFF, 0xFF};
    SDL_Color COLOR_WHITE = {0xFF, 0xFF, 0xFF, 0xFF};

    Window window("Test", SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_BLACK);
    window.set_icon(PATH_ICON);

    Action* action = Action::get_instance();
    action->init(
        SDL_SCANCODE_Z,
        SDL_SCANCODE_X,
        SDL_SCANCODE_RETURN,
        SDL_SCANCODE_UP,
        SDL_SCANCODE_DOWN,
        SDL_SCANCODE_LEFT,
        SDL_SCANCODE_RIGHT
    );

    TextureText text_black(window.get_render(), PATH_FONT, COLOR_BLACK, TEXT_SIZE);
    TextureText text_red  (window.get_render(), PATH_FONT, COLOR_RED  , TEXT_SIZE);
    TextureText text_green(window.get_render(), PATH_FONT, COLOR_GREEN, TEXT_SIZE);
    TextureText text_blue (window.get_render(), PATH_FONT, COLOR_BLUE , TEXT_SIZE);
    TextureText text_white(window.get_render(), PATH_FONT, COLOR_WHITE, TEXT_SIZE);
    
    Texture texture_main_char(window.get_render());
    texture_main_char.load_bmp(IMG_MAIN_CHAR);

    OverWorld test_room(
        &window,
        action,
        &text_black,
        PATH_MAP,
        &view_selector,
        TILE_SIZE
    );

    Entity player(
        window.get_render(),
        COLOR_WHITE,
        {10, 10, TILE_SIZE, TILE_SIZE},
        10
    );
    player.load_sprite(IMG_MAIN_CHAR, 10);


    Entity dummy(window.get_render(),
        COLOR_BLUE,
        {143, 141, TILE_SIZE/2, TILE_SIZE},
        10
    );

    Entity dummy2(
        window.get_render(),
        COLOR_GREEN,
        {43, 322, TILE_SIZE, TILE_SIZE},
        10
    );

    test_room.add_player(player);
    test_room.add_entity(dummy);
    test_room.add_entity(dummy2);

    Menu menu_screen(
        window,
        action,
        view_selector,
        &text_white,
        &text_red
    );

    
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
            //printf("%ul\n", (long)( frame_count/( (SDL_GetTicks()-start_timer) /1000.f)) );
            window.update_screen();
        }
        frame_count++;

    }
    return 0;
};