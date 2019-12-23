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

    this->sprite.renderer = render;
    this->bound_box.init(render, color, collition_box);
}


void Entity::render(int offset_x, int offset_y){

    if (sprite.texture != nullptr){
        sprite.render(
            collition_box.x + offset_x,
            collition_box.y + offset_y
        );
    }else{
        bound_box.render(
            collition_box.x + offset_x,
            collition_box.y + offset_y
        );
    }

}

void Entity::update_position(){
    collition_box.x += axis_speed.x;
    collition_box.y += axis_speed.y;
    stop_x();
    stop_y(); 
}

void Entity::load_sprite(std::string PATH, int scale){
    sprite.load_bmp(PATH, scale);
}



void Entity::move_up(){
    axis_speed.y =  -speed;
    set_direction(FACE_UP);
}

void Entity::move_down(){
    axis_speed.y =    +speed;
    set_direction(FACE_DOWN);
}

void Entity::move_left(){
    axis_speed.x = -speed;
    set_direction(FACE_LEFT);
}

void Entity::move_right(){
    axis_speed.x = +speed;
    set_direction(FACE_RIGHT);
}



void Entity::stop_x(){
    axis_speed.x = 0;
}

void Entity::stop_y(){
    axis_speed.y = 0;
}


int Entity::get_direction(){
    return direction;
}

void Entity::set_direction(int new_direction){
    if ((direction>=FACE_UP) and (direction<=FACE_TOTAL)){
        direction = new_direction;

        switch(direction){
            case FACE_UP  :{
                sprite.rotate(180);
                break;
            };
            case FACE_DOWN:{
                sprite.rotate(0);
                break;
            };
            case FACE_LEFT:{
                sprite.rotate(90);
                break;
            };
            case FACE_RIGHT:{
                sprite.rotate(270);
                break;
            }
        }
    }
}