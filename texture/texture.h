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
        ~TextureText();
        TextureText(const TextureText &);
        TextureText(SDL_Renderer*, string, SDL_Color, int);

        bool create_text_texture(string);
        void render_text(int, int, string);
};

class TextureBlock: public Texture{
    public:
        SDL_Rect rect;
        SDL_Color color;

        TextureBlock();
        ~TextureBlock();
        TextureBlock(SDL_Renderer*, SDL_Color, int, int, int, int);
        void init(SDL_Renderer*, SDL_Color, SDL_Rect);
        void render();
};

#endif