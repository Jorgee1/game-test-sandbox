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
    font = NULL;
    text_size = 0;
    color = {0,0,0};
}

TextureText::TextureText(const TextureText &original){
    this->renderer = original.renderer;
    font_path = original.font_path;
    text_size = original.text_size;
    color = original.color;
    font = TTF_OpenFont(font_path.c_str(), text_size);
}

TextureText::~TextureText(){
    if(font != NULL){
        TTF_CloseFont(font);
        font = NULL;
        text_size = 0;
        color = {0,0,0};
    }
}

TextureText::TextureText(SDL_Renderer* renderer, string path, SDL_Color color, int size){
    this->renderer = renderer;
    this->color = color;
    
    font_path = path;
    text_size = size;

    font = TTF_OpenFont(font_path.c_str(), text_size);
    if( font == NULL ){
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    }
}

bool TextureText::create_text_texture(string text){
    free();
    SDL_Surface* temp_surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if( temp_surface == NULL )
    {
        printf("Unable to create text! SDL_TTF Error: %s\n", TTF_GetError());
        return false;
    }else{
        texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
        if( texture == NULL ){
            printf( "Unable to create texture! SDL Error: %s\n", SDL_GetError() );
            return false;
        }else{
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            SDL_FreeSurface(temp_surface);
        }
    }
    return true;
}

void TextureText::render_text(int x, int y, string text){
    if (create_text_texture(text)){
        render(x, y);
    }else{
        printf("No texture to print\n");
    }
}

/*
    Class for rendering blocks
*/

TextureBlock::TextureBlock(){
    rect = {0, 0, 0, 0};
    color = {0x00, 0x00, 0x00, 0x00};
}
TextureBlock::~TextureBlock(){
    rect = {0, 0, 0, 0};
    color = {0x00, 0x00, 0x00, 0x00};
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