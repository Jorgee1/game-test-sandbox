#include "entity.h"

Entity::Entity(){
    speed = 0;
    axis_speed = {0, 0};
    collition_box = {0, 0, 0, 0};
}

Entity::~Entity(){
    speed = 0;
    axis_speed = {0, 0};
}

Entity::Entity(
        SDL_Renderer* render,
        SDL_Color color,
        SDL_Rect collition_box,
        int speed
    ){

    this->speed = speed;

    this->axis_speed = {0, 0};
    this->collition_box = collition_box;

    this->bound_box.init(render, color, collition_box);
}


void Entity::render(int offset_x, int offset_y){
    bound_box.render(
        collition_box.x + offset_x,
        collition_box.y + offset_y
    );
}

void Entity::update_position(){
    collition_box.x += axis_speed.x;
    collition_box.y += axis_speed.y;
    stop_x();
    stop_y(); 
}



void Entity::move_up(){
    axis_speed.y = -speed;
}

void Entity::move_down(){
    axis_speed.y = +speed;
}

void Entity::move_left(){
    axis_speed.x = -speed;
}

void Entity::move_right(){
    axis_speed.x = +speed;
}



void Entity::stop_x(){
    axis_speed.x = 0;
}

void Entity::stop_y(){
    axis_speed.y = 0;
}