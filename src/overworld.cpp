#include "overworld.h"

// OverWorld class

OverWorld::OverWorld(){
    window = nullptr;
    action = nullptr;
    view_selector = nullptr;

    camara = {0, 0, 10, 10};
    PLAYER = 0;
    colition = false;
    tile_size = 10;
}

OverWorld::OverWorld(
        Window* window,
        Action* action,
        TextureText* general_text,
        SDL_Color dialog_color,
        std::string map_path,
        int *view_selector,
        int size
    ){

    this->window        = window;
    this->action        = action;
    this->view_selector = view_selector;
    this->text          = general_text;
    
    dialog_box.init(this->window, text, dialog_color);

    world_map.init(map_path);
    
    camara = {
        0, 0, 
        this->window->SCREEN_WIDTH,
        this->window->SCREEN_HEIGHT
    };

    tile_size = size;
    PLAYER = 0;
    colition = false;

    SDL_Color color[] = {
        {0xDB, 0xD0, 0x53, 0xFF},
        {0x22, 0x45, 0xA7, 0xFF},
        {0xFF, 0xAF, 0x60, 0xFF},
        {0x00, 0x3D, 0x11, 0xFF},
    };

    for(int i=0; i<4; i++){
        floor_tiles[i].init(
            this->window->get_render(),
            color[i],
            {0, 0, tile_size, tile_size}
        );
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
    if(action->get_state(action->BUTTON_MOVE_UP)){
        actors[PLAYER]->move_up();
    }else if(action->get_state(action->BUTTON_MOVE_DOWN)){
        actors[PLAYER]->move_down();
    }

    if(action->get_state(action->BUTTON_MOVE_LEFT)){
        actors[PLAYER]->move_left();
    }else if(action->get_state(action->BUTTON_MOVE_RIGHT)){
        actors[PLAYER]->move_right();
    }

    if(action->check_action(action->BUTTON_ACTION)){
        dialog_box.queue.push_back("asdf");
    }

    if(action->check_action(action->BUTTON_START)){
        *view_selector = 1;
    }
}

void OverWorld::check_entity_colition(){
    SDL_Rect box1 = {
        actors[PLAYER]->collition_box.x + actors[PLAYER]->axis_speed.x, 
        actors[PLAYER]->collition_box.y + actors[PLAYER]->axis_speed.y,
        actors[PLAYER]->collition_box.w,
        actors[PLAYER]->collition_box.h
    };

    SDL_Rect box2;
    colition = false;
    for(int i=0; i<actors.size(); i++){
        if(PLAYER != i){
        
            box2 = actors[i]->collition_box;

            if(box_colition(box1, box2)){

                if(box_colition({box1.x, actors[PLAYER]->collition_box.y, box1.w, box1.h}, box2)){
                    if(actors[PLAYER]->axis_speed.x > 0){
                        actors[PLAYER]->axis_speed.x = 0;
                        actors[PLAYER]->collition_box.x = box2.x - actors[PLAYER]->collition_box.w;
                    }else if(actors[PLAYER]->axis_speed.x < 0){
                        actors[PLAYER]->axis_speed.x = 0;
                        actors[PLAYER]->collition_box.x = box2.x + box2.w;
                    }
                    colition = true;
                }

                if(box_colition({actors[PLAYER]->collition_box.x, box1.y, box1.w, box1.h}, box2)){
                    if(actors[PLAYER]->axis_speed.y > 0){
                        actors[PLAYER]->axis_speed.y = 0;
                        actors[PLAYER]->collition_box.y = box2.y - actors[PLAYER]->collition_box.h;
                    }else if(actors[PLAYER]->axis_speed.y < 0){
                        actors[PLAYER]->axis_speed.y = 0;
                        actors[PLAYER]->collition_box.y = box2.y + box2.h;
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
            actors[PLAYER]->collition_box.x = 0;
            colition = true; 
        }
    }else if (actors[PLAYER]->axis_speed.x > 0){
        if(box1.x + box1.w >= world_map.w*tile_size){
            actors[PLAYER]->axis_speed.x = 0;
            actors[PLAYER]->collition_box.x = world_map.w*tile_size - box1.w;
            colition = true;
        }
    }

    if (actors[PLAYER]->axis_speed.y < 0){
        if(box1.y <= 0){
            actors[PLAYER]->axis_speed.y = 0;
            actors[PLAYER]->collition_box.y = 0;
            colition = true;
        }
    }else if (actors[PLAYER]->axis_speed.y > 0){
        if(box1.y + box1.h >= world_map.h*tile_size){
            actors[PLAYER]->axis_speed.y = 0;
            actors[PLAYER]->collition_box.y = world_map.h*tile_size - box1.h;
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
    camara.x = actors[PLAYER]->collition_box.x - (camara.w/2) + (actors[PLAYER]->collition_box.w/2);
    camara.y = actors[PLAYER]->collition_box.y - (camara.h/2) + (actors[PLAYER]->collition_box.h/2);
}

void OverWorld::update_word(){
    check_player_actions();
    check_entity_colition();
    update_actors_position();
    update_camara();
}



void OverWorld::render_actors(){
    for(int i=0; i<actors.size(); i++){
        if(box_colition(camara, actors[i]->collition_box)){
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
    std::string coordenates = "X:" + std::to_string(actors[PLAYER]->collition_box.x) + 
                             " Y:" + std::to_string(actors[PLAYER]->collition_box.y);
    
    int acc_x = 1;
    int acc_y = 1;

    text->render(acc_x, acc_y, coordenates, true);

    acc_y += text->get_text_size(coordenates).h + 1;

    if(colition){
        text->render(acc_x, acc_y, "COLITION", true);
    }

    if(dialog_box.queue.size()>0){
        dialog_box.render();
    }
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

void LayerMap::init(std::string map_file_path){
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