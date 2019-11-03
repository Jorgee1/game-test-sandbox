#include<SDL.h>
#include <SDL_ttf.h>

#include <string>

using namespace std;

#ifndef WINDOW_H
#define WINDOW_H

class Window{
    public:
        Window();
        ~Window();
        
        Window(string, int, int, SDL_Color);

        void init(string);

        bool check_exit();
        void clear_screen();
        void update_screen();

        SDL_Renderer* get_render();
        SDL_Window* get_window();

    private:
        int SCREEN_WIDTH;
        int SCREEN_HEIGHT;
        SDL_Window* window = NULL;
        SDL_Renderer* render = NULL;
        SDL_Event e;
        SDL_Color clear_color;
};

#endif