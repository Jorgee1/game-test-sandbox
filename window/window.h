#include<SDL.h>
#include <SDL_ttf.h>

#include <string>

using namespace std;

#ifndef WINDOW_H
#define WINDOW_H

class Window{
    public:
        int SCREEN_WIDTH;
        int SCREEN_HEIGHT;
        
        Window();
        ~Window();
        
        Window(string, int, int, SDL_Color);

        void init(string);

        bool check_exit();
        void clear_screen();
        void update_screen();
        void set_render_draw_color(SDL_Color);
        void set_icon(string icon_path);
        
        SDL_Renderer* get_render();
        SDL_Window* get_window();

    private:
        SDL_Window* window = NULL;
        SDL_Renderer* render = NULL;
        SDL_Event e;
        SDL_Color clear_color;
};

#endif