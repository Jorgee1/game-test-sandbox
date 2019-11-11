#include "menu.h"

Menu::Menu(){
    key_state = SDL_GetKeyboardState(NULL);
    window = NULL;
    view_selector = NULL;
    option = 0;
    text = NULL;

    control_rules = NULL;
    locks = NULL;
}

Menu::Menu(
        Window &window,
        controls &controls_rules,
        controls_locks &locks,
        int &view_selector,
        TextureText text_array[]
    ){
    this->window        = &window;
    this->view_selector = &view_selector;
    this->control_rules = &controls_rules;
    this->locks         = &locks;

    text = text_array;
    option = 0;
    
    key_state = SDL_GetKeyboardState(NULL);

}

void Menu::next_option(){
    if(option+1 < TOTAL_OPTION){
        option++;
    }
}

void Menu::prev_option(){
    if(option > 0){
        option--;
    }
}

void Menu::action(){
    printf("OPTION %i WAS SELECTED\n", option);
}

void Menu::reset_option(){
    option = 0;
}

void Menu::check_player_actions(){
    if((key_state[control_rules->start_button]) && (!locks->start_button)){
        *view_selector = 0;
        locks->start_button = true;
    }

    if(key_state[control_rules->action_button] && (!locks->action_button)){
        locks->action_button = true;
        action();
    }

    if(key_state[control_rules->move_left] && (!locks->move_left)){
        locks->move_left = true;
        prev_option();
    }
    if(key_state[control_rules->move_right] && (!locks->move_right)){
        locks->move_right = true;
        next_option();
    }


    if(!key_state[control_rules->start_button]) {
        locks->start_button = false;
    }
    
    if(!key_state[control_rules->action_button]) {
        locks->action_button = false;
    }

    if(!key_state[control_rules->move_left]) {
        locks->move_left = false;
    }

    if(!key_state[control_rules->move_right]) {
        locks->move_right = false;
    }
}

void Menu::render(){
    int refernce_color = TEXT_WHITE;
    int selected_color = TEXT_RED;

    text[refernce_color].create_texture("PAUSE MENU");
    text[refernce_color].render(0, 0);

    SDL_Rect temp;
    string x[] = {"OPCION1", "OPCION2", "OPCION3"};

    int ref_x = 0;
    int ref_y = 100;
    int temp_selected_color = refernce_color;

    for(int i=0; i<TOTAL_OPTION; i++){
        if (option == i){
            temp_selected_color = selected_color;
        }else{
            temp_selected_color = refernce_color;
        }
        text[temp_selected_color].create_texture(x[i]);
        text[temp_selected_color].render(ref_x, ref_y);
        ref_x += text[temp_selected_color].rect.w + 10;
    }
}