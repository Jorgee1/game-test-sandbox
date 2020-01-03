#include <SDL.h>

#include <string>
#include <vector>

#include "window.h"
#include "texture.h"

#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H

class DialogBox{
    public:
        SDL_Color window_color;
        
        Window* window;
        TextureText* text;
        TextureBlock dialog;

        std::vector<std::string> queue;
        
        DialogBox();

        DialogBox(
            Window*,
            TextureText*,
            SDL_Color
        );

        void init(
            Window*,
            TextureText*,
            SDL_Color
        );
        
        void render();

};

#endif