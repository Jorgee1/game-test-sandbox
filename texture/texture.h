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
        Texture(SDL_Renderer *);
        
        void free();
        void render(int, int);
};

class TextureText: public Texture{
    public:
        SDL_Texture* texture_chars[128];
        string target_text;
        int h, w;

        TextureText();
        ~TextureText();
        TextureText(const TextureText &);
        TextureText(SDL_Renderer*, string, SDL_Color, int);

        void set_text_size(string);
        void fill_array_null();
        void init(SDL_Renderer*, string, SDL_Color, int);
        void render(int, int, string);
        void render(int, int);
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
        void render_fill();
        void render_fill(int, int);
};

#endif