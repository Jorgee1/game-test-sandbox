#include "overworld.h"

// OverWorld class

OverWorld::OverWorld(){
    window = NULL;
    control_rules = NULL;
    locks = NULL;
    view_selector = NULL;

    key_state = SDL_GetKeyboardState(NULL);
    camara = {0, 0, 10, 10};
    PLAYER = 0;
    colition = false;

}

OverWorld::OverWorld(
        Window &window,
        controls &control_rules,
        controls_locks &locks,
        int &view_selector,
        TextureText &general_text,
        string map_path
    ){

    this->window        = &window;
    this->control_rules = &control_rules;
    this->locks         = &locks;
    this->view_selector = &view_selector;
    this->text          = general_text;
    
    world_map.init(map_path);
    
    key_state = SDL_GetKeyboardState(NULL);
    camara = {0, 0, window.SCREEN_WIDTH, window.SCREEN_HEIGHT};
    PLAYER = 0;
    colition = false;

    SDL_Color color[] = {
        {0xDB, 0xD0, 0x53, 0xFF},
        {0x22, 0x45, 0xA7, 0xFF},
        {0xFF, 0xAF, 0x60, 0xFF},
        {0x00, 0x3D, 0x11, 0xFF},
    };

    for(int i=0; i<4; i++){
        floor_tiles[i].init(this->window->get_render(), color[i], {0, 0, tile_size, tile_size});
    }

}

void OverWorld::add_player(Entity &actor){
    PLAYER = actors.size();
    actors.push_back(&actor);
    update_camara();
}

void OverWorld::add_entity(Entity &actor){
    actors.push_back(&actor);
}

void OverWorld::check_player_actions(){
    if(key_state[control_rules->move_up]){
        actors[PLAYER]->move_up();
    }else if(key_state[control_rules->move_down]){
        actors[PLAYER]->move_down();
    }

    if(key_state[control_rules->move_left]){
        actors[PLAYER]->move_left();
    }else if(key_state[control_rules->move_right]){
        actors[PLAYER]->move_right();
    }

    if((key_state[control_rules->start_button]) && (!locks->start_button)){
        *view_selector = 1;
        locks->start_button = true;
    }

    if(!key_state[control_rules->start_button]){
        locks->start_button = false;
    }
}

void OverWorld::check_entity_colition(){
    SDL_Rect box1 = {
        actors[PLAYER]->collition_box.rect.x + actors[PLAYER]->axis_speed.x, 
        actors[PLAYER]->collition_box.rect.y + actors[PLAYER]->axis_speed.y,
        actors[PLAYER]->collition_box.rect.w,
        actors[PLAYER]->collition_box.rect.h
    };
    SDL_Rect box2;
    colition = false;
    for(int i=0; i<actors.size(); i++){
        if(PLAYER != i){
        
            box2 = actors[i]->collition_box.rect;

            if(box_colition(box1, box2)){

                if(box_colition({box1.x, actors[PLAYER]->collition_box.rect.y, box1.w, box1.h}, box2)){
                    if(actors[PLAYER]->axis_speed.x > 0){
                        actors[PLAYER]->axis_speed.x = 0;
                        actors[PLAYER]->collition_box.rect.x = box2.x - actors[PLAYER]->collition_box.rect.w;
                    }else if(actors[PLAYER]->axis_speed.x < 0){
                        actors[PLAYER]->axis_speed.x = 0;
                        actors[PLAYER]->collition_box.rect.x = box2.x + box2.w;
                    }
                    colition = true;
                }

                if(box_colition({actors[PLAYER]->collition_box.rect.x, box1.y, box1.w, box1.h}, box2)){
                    if(actors[PLAYER]->axis_speed.y > 0){
                        actors[PLAYER]->axis_speed.y = 0;
                        actors[PLAYER]->collition_box.rect.y = box2.y - actors[PLAYER]->collition_box.rect.h;
                    }else if(actors[PLAYER]->axis_speed.y < 0){
                        actors[PLAYER]->axis_speed.y = 0;
                        actors[PLAYER]->collition_box.rect.y = box2.y + box2.h;
                    }
                    colition = true;
                }

                if(!colition){
                    // Corners
                    if(actors[PLAYER]->axis_speed.x > 0){
                        if(actors[PLAYER]->axis_speed.y > 0){
                            //printf("Right Botom Corner Collitino\n");
                        }else if(actors[PLAYER]->axis_speed.y < 0){
                            //printf("Right Top Corner Collitino\n");
                        }
                    }else if(actors[PLAYER]->axis_speed.x < 0){
                        if(actors[PLAYER]->axis_speed.y > 0){
                            //printf("Left Botom Corner Collitino\n");
                        }else if(actors[PLAYER]->axis_speed.y < 0){
                            //printf("Left Top Corner Collitino\n");
                        }
                    }
                    actors[PLAYER]->axis_speed.x = 0;
                    colition = true;
                }

            }



        }
    }


    if (actors[PLAYER]->axis_speed.x < 0){
        if(box1.x <= 0){
            actors[PLAYER]->axis_speed.x = 0;
            actors[PLAYER]->collition_box.rect.x = 0;
            colition = true; 
        }
    }else if (actors[PLAYER]->axis_speed.x > 0){
        if(box1.x + box1.w >= world_map.w*tile_size){
            actors[PLAYER]->axis_speed.x = 0;
            actors[PLAYER]->collition_box.rect.x = world_map.w*tile_size - box1.w;
            colition = true;
        }
    }

    if (actors[PLAYER]->axis_speed.y < 0){
        if(box1.y <= 0){
            actors[PLAYER]->axis_speed.y = 0;
            actors[PLAYER]->collition_box.rect.y = 0;
            colition = true;
        }
    }else if (actors[PLAYER]->axis_speed.y > 0){
        if(box1.y + box1.h >= world_map.h*tile_size){
            actors[PLAYER]->axis_speed.y = 0;
            actors[PLAYER]->collition_box.rect.y = world_map.h*tile_size - box1.h;
            colition = true;
        }
    }

}

bool OverWorld::box_colition(SDL_Rect box1, SDL_Rect box2){
    if(
        (box1.x + box1.w > box2.x)
                    &&
        (box1.x < box2.x + box2.w)
                    &&
        (box1.y + box1.h > box2.y)
                    &&
        (box1.y < box2.y + box2.h)
    ){
        return true;
    }
    return false;
}

void OverWorld::update_actors_position(){
    for(int i=0; i<actors.size(); i++){
        actors[i]->update_position();
    }
}

void OverWorld::update_camara(){
    camara.x = actors[PLAYER]->collition_box.rect.x - (camara.w/2) + (actors[PLAYER]->collition_box.rect.w/2);
    camara.y = actors[PLAYER]->collition_box.rect.y - (camara.h/2) + (actors[PLAYER]->collition_box.rect.h/2);
}

void OverWorld::render_actors(){
    for(int i=0; i<actors.size(); i++){
        if(box_colition(camara, actors[i]->collition_box.rect)){
            actors[i]->render(-camara.x, -camara.y);
        }
    }
}

void OverWorld::render_floor(){
    int temp_x = 0;
    int temp_y = 0;

    for(int i=0; i<world_map.h; i++){
        for(int j=0; j<world_map.w; j++){
            temp_x = j*tile_size;
            temp_y = i*tile_size;
            if(box_colition(camara, {temp_x, temp_y, tile_size, tile_size})){
                floor_tiles[world_map.map_data[i][j]].render(temp_x-camara.x, temp_y-camara.y);
            }
        }
    }
}

void OverWorld::render_overlay(){
    string coordenates = "X:" + to_string(actors[PLAYER]->collition_box.rect.x) + " Y:" + to_string(actors[PLAYER]->collition_box.rect.y);
    
    int acc_x = 1;
    int acc_y = 1;

    text.create_texture(coordenates, true);
    text.render(acc_x, acc_y);

    acc_y += text.rect.h + 1;
    if(colition){
        text.create_texture("COLITION", true);
        text.render(acc_x, acc_y);
    }
}

void OverWorld::update_word(){
    check_player_actions();
    check_entity_colition();
    update_actors_position();
    update_camara();
}

void OverWorld::render_world(){
    render_floor();
    render_actors();


    render_overlay();
}

// Map Layer class

LayerMap::LayerMap(){
    map_data = NULL;
    h = 0;
    w = 0;
}

LayerMap::~LayerMap(){
    for(int i=0; i<h; i++){
        free(map_data[i]);
    }
    free(map_data);
}

void LayerMap::create_map_array(){
    map_data = (int**) malloc(sizeof *map_data * h);

    for(int i=0; i<h; i++){
        map_data[i] = (int*) malloc(sizeof *map_data[i] * w);
    }
}

void LayerMap::init(string map_file_path){
    FILE *map_file;
    unsigned int temp = 0;
    map_file = fopen(map_file_path.c_str(), "rb");

    int index_x = 0;
    int index_y = 0;
    while ( !feof(map_file) ){
        if(fread(&temp, 1, 1, map_file)){
            if(temp != 0xFF){
                index_x++;
            }else{
                w = index_x;
                index_y++;
                index_x = 0;
            }
        }
    }
    h = index_y;

    create_map_array();

    rewind(map_file);

    index_x = 0;
    index_y = 0;

    while ( !feof(map_file) ){
        if(fread(&temp, 1, 1, map_file)){
            if(temp != 0xFF){
                map_data[index_y][index_x] = temp;
                index_x++;
            }else{
                index_y++;
                index_x = 0;
            }
        }
    }

    fclose (map_file);
    map_file = NULL;
}


// Entity class


Entity::Entity(){
    speed = 0;
    axis_speed.x = 0;
    axis_speed.y = 0;
}

Entity::Entity(SDL_Renderer* render, SDL_Color color, SDL_Rect collition_box, int speed){
    axis_speed.x = 0;
    axis_speed.y = 0;
    this->collition_box.init(render, color, collition_box);
    this->speed = speed;
}

Entity::~Entity(){
    speed = 0;
    axis_speed.x = 0;
    axis_speed.y = 0;
}

void Entity::render(int offset_x, int offset_y){
    collition_box.render(collition_box.rect.x + offset_x, collition_box.rect.y + offset_y);
}

void Entity::update_position(){
    collition_box.rect.x += axis_speed.x;
    collition_box.rect.y += axis_speed.y;
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