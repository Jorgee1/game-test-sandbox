#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cstdlib>
#include <string>

#include "common.h"
#include "texture.h"
#include "window.h"


int main(int argc, char* args[]){

    string PATH_FONT = "assets/fonts/LiberationMono-Regular.ttf";
    string PATH_ICON = "assets/icon.bmp";

    int SCREEN_WIDTH  = 800;
    int SCREEN_HEIGHT = 600;
    bool exit = false;

    SDL_Color colors[TOTAL_TEXT] = {
        {0x00, 0x00, 0x00, 0xFF},
        {0xFF, 0x00, 0x00, 0xFF},
        {0x00, 0xFF, 0x00, 0xFF},
        {0x00, 0x00, 0xFF, 0xFF},
        {0xFF, 0xFF, 0xFF, 0xFF}
    };

    Window window("Test", SCREEN_WIDTH, SCREEN_HEIGHT, colors[TEXT_BLACK]);
    window.set_icon(PATH_ICON);

    while(!exit){

        if(window.check_exit()){
            exit = true;
        }else{
            window.clear_screen();
            window.update_screen();
        }
    }
    return 0;

}