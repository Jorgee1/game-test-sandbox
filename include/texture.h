#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>
#include <string>
#include <map>


using namespace std;

#ifndef TEXTURE_H
#define TEXTURE_H

class Texture{
    public:
        SDL_Rect rect;
        SDL_Texture* texture;
        SDL_Renderer* renderer;
        
        Texture();
        Texture(const Texture &);

        Texture(SDL_Renderer *);
        ~Texture();
        
        void free();
        void render(int, int);
};

class TextureText: public Texture{
    public:
        SDL_Texture* texture_chars[128];
        string target_text;

        TextureText();
        ~TextureText();
        TextureText(const TextureText &);
        TextureText(SDL_Renderer*, string, SDL_Color, int);

        void init(SDL_Renderer*, string, SDL_Color, int);

        void set_text_dimension(string);
        void create_texture(string, bool background_box = false, SDL_Color box_color={0xFF, 0xFF, 0xFF, 0xFF});

        void fill_array_null();


};

class TextureBlock: public Texture{
    public:
        TextureBlock();
        TextureBlock(const TextureBlock &);
        ~TextureBlock();
        TextureBlock(SDL_Renderer*, SDL_Color, int, int);
        void init(SDL_Renderer*, SDL_Color, SDL_Rect);

        void create_block_texture(SDL_Color color);
        void create_fill_texture(SDL_Color color);

};

#endif