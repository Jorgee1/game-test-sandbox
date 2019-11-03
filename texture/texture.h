#include <SDL.h>
#include <SDL_ttf.h>
#include <cstdlib>
#include <string>
#include <map>

using namespace std;

#ifndef TEXTURE_H
#define TEXTURE_H

class Texture{
    public:
        int h;
        int w;
        SDL_Texture* texture;
        SDL_Renderer* renderer;

        Texture();
        ~Texture();
        Texture(SDL_Renderer*);
        
        void free();
        void render(int, int);
};

class TextureText: public Texture{
    public:
        TTF_Font *font;
        SDL_Color color;
        int text_size; 
        string font_path;


        TextureText();

        TextureText(const TextureText &);

        ~TextureText();

        TextureText(SDL_Renderer*, string, SDL_Color, int);

        bool create_text_texture(string);

        void render_text(int, int, string);
};

#endif