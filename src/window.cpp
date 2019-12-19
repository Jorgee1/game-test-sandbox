#include "window.h"

Window::Window(){
    SCREEN_WIDTH = 640;
    SCREEN_HEIGHT = 480;
    SDL_Color clear_color = {0x00, 0x00, 0x00, 0x00};
    init("");
}

Window::~Window(){
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

Window::Window(string window_name, int width, int height, SDL_Color color){
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
    clear_color = color;
    init(window_name);
}

void Window::init(string window_name){
    if(SDL_Init( SDL_INIT_VIDEO ) < 0){
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }else{
        window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window == NULL){
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        }else{
            render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(render==NULL){
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            }else{
                clear_screen();
                if(TTF_Init()<0){
                    printf("TTF_Init: %s\n", TTF_GetError());
                }
            }
        }
    }
}



bool Window::check_exit(){
    while( SDL_PollEvent( &e ) != 0 )
    {
        if( e.type == SDL_QUIT )
        {
            return true;
        }
    }
    return false;
}

void Window::clear_screen(){
    set_render_draw_color(clear_color);
    SDL_RenderClear(render);
}

void Window::update_screen(){
    SDL_RenderPresent(render);
}


void Window::set_render_draw_color(SDL_Color color){
   SDL_SetRenderDrawColor(render, color.r, color.g, color.b, color.a); 
}

void Window::set_icon(string icon_path){
    SDL_Surface* temp_surface = SDL_LoadBMP(icon_path.c_str());
    if(temp_surface != NULL){
        SDL_SetWindowIcon(window, temp_surface);
        SDL_FreeSurface(temp_surface);
    }else{
        printf("ICON NOT FOUND\n");
    }

}

SDL_Renderer* Window::get_render(){
    return render;
}

SDL_Window* Window::get_window(){
    return window;
}
