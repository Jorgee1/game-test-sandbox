#include "action.h"

Action* Action::instance = nullptr;

Action::Action(){
    for (int i = 0; i < TOTAL_BUTTONS; ++i)
    {
        actions[i] = 0;
        key_flags[i] = true;
    }
    
    key_state = SDL_GetKeyboardState(NULL);
}

Action::~Action(){
    delete instance;
}

Action* Action::get_instance(){
    if (instance == nullptr){
        instance = new Action();
    }
    return instance;
}

void Action::init(
        int key_action_button,
        int key_cancel_button,
        int  key_start_button,
        int       key_move_up,
        int     key_move_down,
        int     key_move_left,
        int    key_move_right
    ){
    actions[BUTTON_ACTION]     = key_action_button;
    actions[BUTTON_CANCEL]     = key_cancel_button;
    actions[BUTTON_START]      =  key_start_button;
    actions[BUTTON_MOVE_UP]    =       key_move_up;
    actions[BUTTON_MOVE_DOWN]  =     key_move_down;
    actions[BUTTON_MOVE_LEFT]  =     key_move_left;
    actions[BUTTON_MOVE_RIGHT] =    key_move_right;
}

bool Action::get_state(int key_id){
    int key_value = actions[key_id];
    
	return key_state[key_value];
}

bool Action::check_action(int key_id){

    int key_value = actions[key_id];
    bool output = false;

    if(key_state[key_value] && !key_flags[key_id]){
        key_flags[key_id] = true;
        output = true;
    }

    if(!key_state[key_value]) {
        key_flags[key_id] = false;
    }

    return output;
}