#include <cstdlib>
#include <string>
#include <map>
#include <vector>

#include "common.h"
#include "window.h"
#include "texture.h"

#ifndef MENU_H
#define MENU_H

class Menu{
    public:
        Window *window;
        const Uint8* key_state;
        int *view_selector;
        TextureText *text;

        controls *control_rules;
        controls_locks *locks;

        int option;

        enum options{
            OPTION1,
            OPTION2,
            OPTION3,
            TOTAL_OPTION
        };

        Menu();
        Menu(
            Window &,
            controls &,
            controls_locks &,
            int &,
            TextureText[]
        );

        void next_option();
        void prev_option();
        void action();
        void reset_option();

        void check_player_actions();
        void render();
};

#endif