#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cstdlib>
#include <string>

#include "texture.h"
#include "window.h"
#include "action.h"


int main(int argc, char* args[]){

    std::string PATH_FONT   = "asset/font/LiberationMono-Regular.ttf";
    std::string PATH_ICON   = "asset/icon.bmp";
    std::string WINDOW_NAME = "Test";
    
    std::string text = "Hello";

    int SCREEN_WIDTH  = 800;
    int SCREEN_HEIGHT = 600;
    int TEXT_SIZE     =  40;

    bool exit = false;

    SDL_Color COLOR_BLACK = {0x00, 0x00, 0x00, 0xFF};
    SDL_Color COLOR_RED   = {0xFF, 0x00, 0x00, 0xFF};
    SDL_Color COLOR_GREEN = {0x00, 0xFF, 0x00, 0xFF};
    SDL_Color COLOR_BLUE  = {0x00, 0x00, 0xFF, 0xFF};
    SDL_Color COLOR_WHITE = {0xFF, 0xFF, 0xFF, 0xFF};

    Window window(WINDOW_NAME, SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_BLACK);
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


    TextureBlock block_red(window.get_render(), COLOR_RED, 100, 100);

    while(!exit){

        if(window.check_exit()){
            exit = true;
        }else{
            
            window.clear_screen();

            text_red.render(0, 0, text);
            block_red.render(100, 100);

            if(action->check_action(action->BUTTON_MOVE_UP)){
                printf("HYAAAA\n");
            }

            if(action->get_state(action->BUTTON_MOVE_DOWN)){
                printf("DOWWWWN\n");
            }
            
            window.update_screen();
        }
    }

    return 0;
}