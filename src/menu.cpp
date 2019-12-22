#include "menu.h"

Menu::Menu(){
    key_state = SDL_GetKeyboardState(NULL);

    view_selector = NULL;
    option = 0;

    window = NULL;

    refernce_color = NULL;
    selected_color = NULL;

    action = NULL;
}

Menu::Menu(
        Window &window,
        Action *action,
        int &view_selector,
        TextureText *refernce_color,
        TextureText *selected_color
    ){
    this->action         = action;
    this->window         = &window;
    this->view_selector  = &view_selector;

    this->refernce_color = refernce_color;
    this->selected_color = selected_color;

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

void Menu::do_something(){
    printf("OPTION %i WAS SELECTED\n", option);
}

void Menu::reset_option(){
    option = 0;
}

void Menu::check_player_actions(){
    if(action->check_action(action->BUTTON_START)){
        *view_selector = 0;
    }

    if(action->check_action(action->BUTTON_ACTION)){
        do_something();
    }

    if(action->check_action(action->BUTTON_MOVE_LEFT)){
        prev_option();
    }

    if(action->check_action(action->BUTTON_MOVE_RIGHT)){
        next_option();
    }
}

void Menu::render(){
    refernce_color->render(0, 0, "PAUSE MENU");

    SDL_Rect temp;
    std::string x[] = {"OPCION1", "OPCION2", "OPCION3"};

    int ref_x =   0;
    int ref_y = 100;

    for(int i=0; i<TOTAL_OPTION; i++){
        if (option == i){
            selected_color->render(ref_x, ref_y, x[i]);
            ref_x += selected_color->get_text_size(x[i]).w + 10;
        }else{
            refernce_color->render(ref_x, ref_y, x[i]);
            ref_x += refernce_color->get_text_size(x[i]).w + 10;
        }
    }
}