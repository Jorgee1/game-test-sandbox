#include "texture.h"

Texture::Texture(){
    texture = NULL;
    renderer = NULL;
}

Texture::~Texture(){
    free();
}

Texture::Texture(SDL_Renderer* renderer){
    texture = NULL;
    this->renderer = renderer;
}

void Texture::free(){
    if(texture!=NULL){
        SDL_DestroyTexture(texture);
        texture=NULL;
    }
}

SDL_Rect Texture::get_size(){
    SDL_Rect dimensions = {0, 0, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &dimensions.w, &dimensions.h);
    return dimensions;
}

void Texture::render(int x, int y){
    SDL_Rect temp_rect = get_size();
    temp_rect.x = x;
    temp_rect.y = y;
    SDL_RenderCopy(renderer, texture, NULL, &temp_rect);
}







TextureBlock::TextureBlock(){}

TextureBlock::TextureBlock(SDL_Renderer* renderer, SDL_Color color, int h, int w){
    init(renderer, color, {0, 0, w, h});
}

void TextureBlock::init(SDL_Renderer* renderer, SDL_Color color, SDL_Rect rect){
    this->renderer = renderer;
    create_texture(color, rect);
}

void TextureBlock::create_texture(SDL_Color color, SDL_Rect rect){
    free();
    SDL_Rect temp_rect = {0, 0, rect.w, rect.h};
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);

    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &temp_rect);
    SDL_SetRenderTarget(renderer, NULL);
}







TextureText::TextureText(){
    for(int i=0; i<128; i++){
        texture_chars[i].renderer = renderer;
    }
}

TextureText::TextureText(SDL_Renderer* renderer, std::string path, SDL_Color color, int size){
    init(renderer, path, color, size);
}

void TextureText::init(SDL_Renderer* renderer, std::string path, SDL_Color color, int size){
    this->renderer = renderer;

    TTF_Font* font = TTF_OpenFont(path.c_str(), size);
    if( font == NULL ){
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
    }

    SDL_Surface* temp_surface = NULL;
    for(int i=0; i<128; i++){      
        // Solo acepta strings
        char y[] = {(char)i, '\0'};
        temp_surface = TTF_RenderText_Blended(font, y, color);

        texture_chars[i].renderer = this->renderer;
        texture_chars[i].texture = SDL_CreateTextureFromSurface(renderer, temp_surface);

        SDL_FreeSurface(temp_surface);
        temp_surface = NULL;
    }

    TTF_CloseFont(font);
    font=NULL;
}

SDL_Rect TextureText::get_text_size(std::string text){
    int acc = 0;

    SDL_Rect dimensions = {0, 0, 0, 0};

    for(int i=0, n=text.size(); i<n; i++){
        dimensions = char_to_texture(text[i])->get_size();
        acc += dimensions.w;
    }
    dimensions.w = acc;
    return dimensions;

}


Texture* TextureText::char_to_texture(char character){
    int temp_char = (int)character;

    if (temp_char > 127){
        temp_char = 63;
    }else if (temp_char < 0){
        temp_char = 63;
    }

    return &texture_chars[temp_char];
}

void TextureText::render(int x, int y, std::string text, bool background_box, SDL_Color box_color){

    if(background_box){
        SDL_Rect size = get_text_size(text);
        size.x = x;
        size.y = y;

        SDL_SetRenderDrawColor(renderer, box_color.r, box_color.g, box_color.b, box_color.a);
        SDL_RenderFillRect(renderer, &size);
    }

    for(int i=0, n=text.size(); i<n; i++){
        char_to_texture(text[i])->render(x, y);
        x += char_to_texture(text[i])->get_size().w;
    }

}