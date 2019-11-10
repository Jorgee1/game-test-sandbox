#include <map>

#include "common.h"
#include "texture.h"
#include "window.h"

#ifndef MENU_H
#define MENU_H



class Menu{
    public:
        TextureText text;
        Window *window;
        const Uint8* key_state = SDL_GetKeyboardState(NULL);
        int *view_selector;
        controls *control_rules;
        map<string, SDL_Color> *colors;

        bool enter_lock = true;

        Menu();
        Menu(Window &, controls &, map<string, SDL_Color> &,  int &);

        void check_player_actions();
        void render();
};



#endif