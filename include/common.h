#ifndef COMMON_H
#define COMMON_H


struct controls{
    int action_button;
    int cancel_button;
    int start_button;
    int move_up;
    int move_down;
    int move_left;
    int move_right;
};

struct controls_locks{
    bool action_button = true;
    bool cancel_button = true;
    bool start_button  = true;
    bool move_up       = true;
    bool move_down     = true;
    bool move_left     = true;
    bool move_right    = true;
};

enum text_keys{
    TEXT_BLACK,
    TEXT_RED,
    TEXT_GREEN,
    TEXT_BLUE,
    TEXT_WHITE,
    TOTAL_TEXT,
};


#endif