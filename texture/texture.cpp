#include <texture.h>

Texture::Texture(){
    h = 0;
    w = 0;
    texture = NULL;
    renderer = NULL;
}

Texture::~Texture(){
    free();
}

Texture::Texture(SDL_Renderer* renderer){
    texture = NULL;
    h = 0;
    w = 0;
    this->renderer = renderer;
}

void Texture::free(){
    if(texture!=NULL){
        SDL_DestroyTexture(texture);
        texture=NULL;
        h=0;
        w=0;
    }
}

void Texture::render(int x, int y){
    SDL_Rect renderQuad = { x, y, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
}

/*
    Class for rendering text
*/
TextureText::TextureText(){
    for(int i=0; i<128; i++){
        texture_chars[i] = NULL;
    }
}

TextureText::TextureText(const TextureText &original){
    this->renderer = original.renderer;
    for(int i=0; i<128; i++){
        texture_chars[i] = original.texture_chars[i];
    }
}

TextureText::~TextureText(){
    for(int i=0; i<128; i++){
        SDL_DestroyTexture(texture_chars[i]);
        texture_chars[i] = NULL;
    }
}

TextureText::TextureText(SDL_Renderer* renderer, string path, SDL_Color color, int size){
    init(renderer, path, color, size);
}

void TextureText::init(SDL_Renderer* renderer, string path, SDL_Color color, int size){
    this->renderer = renderer;

    TTF_Font* font = TTF_OpenFont(path.c_str(), size);
    if( font == NULL ){
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    }

    SDL_Surface* temp_surface = NULL;
    for(int i=0; i<128; i++){
        char x = (char)i;
        char y[] = {x, '\0'};
        
        temp_surface = TTF_RenderText_Blended(font, y, color);
        texture_chars[i] = SDL_CreateTextureFromSurface(renderer, temp_surface);
        SDL_FreeSurface(temp_surface);
        temp_surface = NULL;
    }

    TTF_CloseFont(font);
    font=NULL;
}

void TextureText::set_text_size(string text){
    int acc = 0;
    int temp_character = 0;
    int temp_w = 0;
    for(int i=0, n=(int)strlen(text.c_str()); i<n; i++){
        temp_character = (int)text.c_str()[i];
        if (temp_character > 127){
            temp_character = 63;
        }

        SDL_QueryTexture(texture_chars[temp_character], NULL, NULL, &temp_w, &h);
        acc += temp_w;
    }
    w = acc;
    target_text = text;
}

void TextureText::render(int x, int y, string text){
    int acc=0;
    int temp_character = 0;
    int temp_w = 0;
    SDL_Rect temp_rect;

    for(int i=0, n=(int)strlen(text.c_str()); i<n; i++){
        temp_character = (int)text.c_str()[i];
        if (temp_character > 127){
            temp_character = 63;
        }

        SDL_QueryTexture(texture_chars[temp_character], NULL, NULL, &w, &h);
        temp_rect = {acc, 0, w, h};
        SDL_RenderCopy(renderer, texture_chars[temp_character], NULL, &temp_rect);
        acc += w;
    }
    w = acc;
}

void TextureText::render(int x, int y){
    int acc=x;
    int temp_character = 0;
    int temp_w = 0;
    SDL_Rect temp_rect;

    for(int i=0, n=(int)strlen(target_text.c_str()); i<n; i++){
        temp_character = (int)target_text.c_str()[i];
        if (temp_character > 127){
            temp_character = 63;
        }

        SDL_QueryTexture(texture_chars[temp_character], NULL, NULL, &w, &h);
        temp_rect = {acc, y, w, h};
        SDL_RenderCopy(renderer, texture_chars[temp_character], NULL, &temp_rect);
        acc += w;
    }
    w = acc;
}

/*
    Class for rendering blocks
*/

TextureBlock::TextureBlock(){
    rect = {0, 0, 0, 0};
    color = {0x00, 0x00, 0x00, 0xFF};
}
TextureBlock::~TextureBlock(){
    rect = {0, 0, 0, 0};
    color = {0x00, 0x00, 0x00, 0xFF};
}

TextureBlock::TextureBlock(SDL_Renderer* render, SDL_Color color, int x, int y, int h, int w){
    SDL_Rect temp_rect = {x, y, w, h};
    init(render, color, temp_rect);
}

void TextureBlock::init(SDL_Renderer* render, SDL_Color color, SDL_Rect rect){
    renderer = render;
    this->color = color;
    this->rect = rect;
}

void TextureBlock::render(){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(renderer, &rect);
}
void TextureBlock::render_fill(){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void TextureBlock::render_fill(int offset_x, int offset_y){
    SDL_Rect temp_rect = rect;
    temp_rect.x += offset_x;
    temp_rect.y += offset_y;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &temp_rect);
}