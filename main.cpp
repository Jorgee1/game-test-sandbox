#include <SDL.h>
#include <SDL_ttf.h>

#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>
#include <map>

#include "texture.h"
#include "window.h"

int main(int argc, char* args[] ){
    map<string, SDL_Color> colors = {
        {"black", {0x00, 0x00, 0x00, 0xFF}},
        {"red"  , {0xFF, 0x00, 0x00, 0xFF}},
        {"green", {0x00, 0xFF, 0x00, 0xFF}},
        {"blue" , {0x00, 0x00, 0xFF, 0xFF}},
        {"white", {0xFF, 0xFF, 0xFF, 0xFF}},
    };

    Window window("Test", 800, 600, colors["black"]);
    bool exit = false;
    
    while(!exit){
        if(window.check_exit()){
            exit = true;
        }else{
            window.clear_screen();
            window.update_screen();
        }
    }

    return 0;
};