#include "menu.h"

Menu::Menu(){
    window = NULL;
    view_selector = NULL;
    colors = NULL;
}

Menu::Menu(Window &window, controls &control_rules, map<string, SDL_Color> &colors_main,  int &view_selector){
    this->window = &window;
    this->control_rules = &control_rules;
    this->view_selector = &view_selector;
    colors = &colors_main;
    text.init(this->window->get_render(), "fonts/LiberationMono-Regular.ttf", (*colors)["green"], 18);
}

void Menu::check_player_actions(){
    if((key_state[control_rules->start_button]) && (!enter_lock)){
        *view_selector=0;
        enter_lock = true;
    }

    if(!key_state[control_rules->start_button]) {
        enter_lock = false;
    }
}

void Menu::render(){
    text.render(0, 0, "ASDf");
    text.render(0, 100, "ASDf2");
}