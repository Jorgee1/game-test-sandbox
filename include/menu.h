#include <string>

#include "window.h"
#include "action.h"
#include "texture.h"

#ifndef MENU_H
#define MENU_H

class Menu{
    public:


        int option;
        int *view_selector;

        const Uint8* key_state;

        Action* action;

        Window *window;

        TextureText *refernce_color;
        TextureText *selected_color;


        enum options{
            OPTION1,
            OPTION2,
            OPTION3,
            TOTAL_OPTION
        };

        Menu();
        Menu(
            Window &,
            Action *,
            int &,
            TextureText *,
            TextureText *
        );

        void next_option();
        void prev_option();
        void do_something();
        void reset_option();

        void check_player_actions();
        void render();
};

#endif