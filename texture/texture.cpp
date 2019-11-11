#include <texture.h>

Texture::Texture(){
    rect = {0, 0, 0, 0};
    texture = NULL;
    renderer = NULL;
}

Texture::Texture(const Texture &original){
    free();
    renderer = original.renderer;
    rect = original.rect;
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
    SDL_SetRenderTarget(renderer, texture);
    SDL_RenderCopy(renderer, original.texture, NULL, &rect);
    SDL_SetRenderTarget(renderer, NULL);
}

Texture::~Texture(){
    free();
}

Texture::Texture(SDL_Renderer* renderer){
    texture = NULL;
    rect = {0, 0, 0, 0};
    this->renderer = renderer;
}

void Texture::free(){
    if(texture!=NULL){
        SDL_DestroyTexture(texture);
        texture=NULL;
        rect = {0, 0, 0, 0};
    }
}

void Texture::render(int x, int y){
    SDL_Rect temp_rect= {x, y, rect.w, rect.h};
    SDL_RenderCopy(renderer, texture, NULL, &temp_rect);
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
    free();
    renderer = original.renderer;
    rect = original.rect;

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);

    for(int i=0; i<128; i++){
        SDL_SetRenderTarget(renderer, texture_chars[i]);
        SDL_RenderCopy(renderer, original.texture_chars[i], NULL, &rect);
    }

    SDL_SetRenderTarget(renderer, NULL);

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

void TextureText::set_text_dimension(string text){
    int acc = 0;
    int temp_character = 0;
    int temp_w = 0;
    int temp_h = 0;

    for(int i=0, n=(int)strlen(text.c_str()); i<n; i++){
        temp_character = (int)text.c_str()[i];
        if (temp_character > 127){
            temp_character = 63;
        }

        SDL_QueryTexture(texture_chars[temp_character], NULL, NULL, &temp_w, &temp_h);
        acc += temp_w;
    }

    rect.w = acc;
    rect.h = temp_h;
    target_text = text;
}

void TextureText::create_texture(string text, bool background_box, SDL_Color box_color){
    free();
    set_text_dimension(text);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
    SDL_SetRenderTarget(renderer, texture);

    int temp_character = 0;
    int acc=0;
    SDL_Rect temp_rect;

    if(background_box){
        SDL_Rect box = {0, 0, rect.w, rect.h};

        SDL_SetRenderDrawColor(renderer, box_color.r, box_color.g, box_color.b, box_color.a);
        SDL_RenderFillRect(renderer, &box);
    }

    for(int i=0, n=(int)strlen(text.c_str()); i<n; i++){
        temp_character = (int)text.c_str()[i];
        if (temp_character > 127){
            temp_character = 63;
        }

        SDL_QueryTexture(texture_chars[temp_character], NULL, NULL, &temp_rect.w, &temp_rect.h);
        temp_rect.x = acc;
        temp_rect.y = 0;
        SDL_RenderCopy(renderer, texture_chars[temp_character], NULL, &temp_rect);
        acc += temp_rect.w;
    }

    SDL_SetRenderTarget(renderer, NULL);
}


/*
    Class for rendering blocks
*/

TextureBlock::TextureBlock(){
    rect = {0, 0, 0, 0};
}

TextureBlock::TextureBlock(const TextureBlock &original){
    free();
    renderer = original.renderer;
    rect = original.rect;

    SDL_Rect temp_rect = {0, 0, rect.w, rect.h};
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
    SDL_SetRenderTarget(renderer, texture);
    SDL_RenderCopy(renderer, original.texture, NULL, &temp_rect);
    SDL_SetRenderTarget(renderer, NULL);
}

TextureBlock::~TextureBlock(){

}

TextureBlock::TextureBlock(SDL_Renderer* render, SDL_Color color, int h, int w){
    init(render, color, {0, 0, w, h});
}

void TextureBlock::init(SDL_Renderer* render, SDL_Color color, SDL_Rect rect){
    renderer = render;
    this->rect = rect;
    create_fill_texture(color);
}

void TextureBlock::create_block_texture(SDL_Color color){
    free();
    SDL_Rect temp_rect = {0, 0, rect.w, rect.h};
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);

    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(renderer, &temp_rect);
    SDL_SetRenderTarget(renderer, NULL);
}

void TextureBlock::create_fill_texture(SDL_Color color){
    free();
    SDL_Rect temp_rect = {0, 0, rect.w, rect.h};
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);

    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &temp_rect);
    SDL_SetRenderTarget(renderer, NULL);
}

