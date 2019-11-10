#include <SDL.h>
#include <SDL_TTF.h>

#include <cstdlib>
#include <string>

#include "window.h"
#include "texture.h"


using namespace std;


int main(int arpc, char* args[]){
    Window window("Test", 800, 600, {0xFF, 0xFF, 0xFF, 0xFF});
    bool exit = false;

    TTF_Font* font = TTF_OpenFont("fonts/LiberationMono-Regular.ttf", 18);


    SDL_Color text_color = {0x00, 0x00, 0x00, 0xFF};

    SDL_Color rect_color = {0x55,0x00,0xA3,0xFF};
    SDL_Rect rectangle = {100,100,100,100};


    SDL_Surface* temp_surface = NULL;

    SDL_Texture* texture_chars[128];

    for(int i=0; i<128; i++){
        char x = (char)i;
        char y[] = {x, '\0'};
        
        temp_surface = TTF_RenderText_Blended(font, y, text_color);
        texture_chars[i] = SDL_CreateTextureFromSurface(window.get_render(), temp_surface);
        SDL_FreeSurface(temp_surface);
        temp_surface = NULL;
    }

    string text = "ASDF 123";

    int acc = 0;
    int h,w;
    while(!exit){
        if(window.check_exit()){
            exit = true;
        }else{
            window.clear_screen();

            
            window.set_render_draw_color(rect_color);
            SDL_RenderFillRect(window.get_render(), &rectangle);

            acc=0;
            for(int i=0, n=(int)strlen(text.c_str()); i<n; i++){
                SDL_Texture* temp_texture = texture_chars[(int)text.c_str()[i]];
                SDL_QueryTexture(temp_texture, NULL, NULL, &w, &h);
                SDL_Rect temp_rect = {acc, 0, w, h};
                SDL_RenderCopy(window.get_render(), temp_texture, NULL, &temp_rect);
                acc += w;
                temp_texture = NULL;
            }

            window.update_screen();
        }
    }
    TTF_CloseFont(font);
    font=NULL;
    for(int i=0; i<128; i++){
        SDL_DestroyTexture(texture_chars[i]);
        texture_chars[i] = NULL;
    }
    return 0;
}