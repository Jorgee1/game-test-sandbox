#include <SDL.h>

#ifndef ACTION_H
#define ACTION_H

class Action{

    private:

        static Action* instance;
        Action();

    public:

        enum action_ids{
            BUTTON_ACTION,
            BUTTON_CANCEL,
            BUTTON_START,
            BUTTON_MOVE_UP,
            BUTTON_MOVE_DOWN,
            BUTTON_MOVE_LEFT,
            BUTTON_MOVE_RIGHT,
            TOTAL_BUTTONS
        };

        int actions[TOTAL_BUTTONS];
        bool key_flags[TOTAL_BUTTONS];

        const Uint8* key_state;

        static Action* get_instance();

        ~Action();

        void init(
            int key_action_button,
            int key_cancel_button,
            int  key_start_button,
            int       key_move_up,
            int     key_move_down,
            int     key_move_left,
            int    key_move_right
        );

        bool get_state(int key_id);

        bool check_action(int key_id);
};

#endif