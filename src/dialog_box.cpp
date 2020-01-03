#include "dialog_box.h"
DialogBox::DialogBox(){
	window = nullptr;
	text   = nullptr;
}

DialogBox::DialogBox(
		Window* window,
		TextureText* text,
		SDL_Color window_color
	){
	init(window, text, window_color);
}

void DialogBox::init(
		Window* window,
		TextureText* text,
		SDL_Color window_color
	){
	this->window = window;
	this->text   =   text;

	this->dialog.init(
		window->get_render(),
		window_color,
		{
			0,
			0,
			window->SCREEN_WIDTH,
			window->SCREEN_HEIGHT/3
		}
	);
	
	this->window_color = window_color;
}

void DialogBox::render(){
	dialog.render(0, (2*window->SCREEN_HEIGHT)/3);
}