#include "projectile_manager.h"

#include<godot_cpp/classes/resource_loader.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/method_bind.hpp>
#include <godot_cpp/classes/engine.hpp>

projectile_manager::projectile_manager(){
    this->projectile_array = godot::memnew_arr_template<godot::CharacterBody3D*>(this->projectile_limit); 
    for(int i = 0; i < this->projectile_limit; i++){
        this->projectile_array[i] = nullptr;
    }

    this->projectile_iterator = 0;
    this->projectile_count = 0; 
    this->projectile_scene = godot::Ref<godot::PackedScene>(godot::ResourceLoader::get_singleton()->load("res://projectile.tscn")); 
}

//dont delete projectiles in destructor, double free segfaults possible because queue_free and scene tree auto deletion upon end of scene arent synced 
projectile_manager::~projectile_manager(){
    for(int i = 0; i < this->projectile_limit; i++){
        this->projectile_array[i] = nullptr;
    }
    godot::memdelete_arr(this->projectile_array);
}

void projectile_manager::_bind_methods(){
    ADD_SIGNAL(godot::MethodInfo("bullet_collision", godot::PropertyInfo(godot::Variant::INT, "collider_object_id")));
}

void projectile_manager::_process(double delta){

}

void projectile_manager::_ready(){

}

void projectile_manager::_physics_process(double delta){
    if(this->projectile_count == 0)
        return;
    
    for(int i = 0; i < this->projectile_limit; i++){
        if(this->projectile_array[i] == nullptr)
            continue;
        
        godot::Vector3 velocity = this->projectile_data_array[i].direction_vector * this->projectile_data_array[i].starting_velocity;
        godot::Ref<godot::KinematicCollision3D> returned_collision_data = this->projectile_array[i]->move_and_collide(velocity * delta);
        if(returned_collision_data.is_valid()){
            uint64_t collider_id = returned_collision_data->get_collider_id();
            //send collider_id to damage manager class here, to be implemented later
            this->emit_signal("bullet_collision", collider_id);

            this->projectile_array[i]->queue_free();
            this->projectile_array[i] = nullptr;
            this->projectile_count -= 1;
        }
    }
}

void projectile_manager::register_projectile(godot::Vector3 starting_position_vector, godot::Vector3 direction_vector, float starting_velocity){
    if(this->projectile_count == this->projectile_limit)
        return;
    
    bool spawn_flag = false;
    int archived_iterator_val = this->projectile_iterator;
    do{
        if(this->projectile_array[this->projectile_iterator] == nullptr){
            spawn_flag = true;
            break;
        }
        this->projectile_iterator = (this->projectile_iterator + 1) % this->projectile_limit;
    }
    while(this->projectile_iterator != archived_iterator_val);

    if(spawn_flag == false)
        return;
    
    godot::Node *temp_node = this->projectile_scene->instantiate();
    if(temp_node == nullptr)
        return;

    this->projectile_array[this->projectile_iterator] = godot::Object::cast_to<godot::CharacterBody3D>(temp_node);
    if(this->projectile_array[this->projectile_iterator] == nullptr){
        godot::memdelete(temp_node);
        return;
    }

    this->projectile_data_array[this->projectile_iterator].starting_position_vector = starting_position_vector;
    this->projectile_data_array[this->projectile_iterator].direction_vector = direction_vector;
    this->projectile_data_array[this->projectile_iterator].starting_velocity = starting_velocity;

    this->projectile_array[this->projectile_iterator]->set_as_top_level(true); //bug fix test
    this->add_child(this->projectile_array[this->projectile_iterator]);
    this->projectile_array[this->projectile_iterator]->set_global_position(this->projectile_data_array[this->projectile_iterator].starting_position_vector);

    this->projectile_iterator = (this->projectile_iterator + 1) % this->projectile_limit;
    this->projectile_count += 1;
}
