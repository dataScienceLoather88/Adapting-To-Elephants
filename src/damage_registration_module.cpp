#include "damage_registration_module.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/method_bind.hpp>
#include <godot_cpp/classes/engine.hpp>

class projectile_manager;

damage_registration_module::damage_registration_module(){}

damage_registration_module::~damage_registration_module(){}

void damage_registration_module::_bind_methods(){
    godot::ClassDB::bind_method(godot::D_METHOD("detect_entity_and_update_state", "collider_object_id"), &damage_registration_module::detect_entity_and_update_state);
}

void damage_registration_module::_ready(){
    
}

void damage_registration_module::_process(double delta){}

void damage_registration_module::_physics_process(double delta){}

void damage_registration_module::detect_entity_and_update_state(uint64_t collider_object_id){
    godot::UtilityFunctions::print(collider_object_id);
}

void damage_registration_module::receive_player_and_enemy_ptrs(godot::TypedArray<godot::Node> player_ptr_array, godot::TypedArray<godot::Node> enemy_ptr_array){
    this->player_ptr_array = player_ptr_array;
    this->enemy_ptr_array = enemy_ptr_array;

    for(int i = 0; i < this->player_ptr_array.size(); i++){
        godot::Node *player_ptr = godot::Object::cast_to<godot::Node>(this->player_ptr_array[i]);
        if(!player_ptr)
            continue;
        godot::TypedArray<godot::Node> projectile_manager_returns =  player_ptr->find_children("*", "projectile_manager", true);

        for(int j = 0; j < projectile_manager_returns.size(); j++){
            godot::Node *projectile_manager_player_ptr = godot::Object::cast_to<godot::Node>(projectile_manager_returns[j]);
            if(projectile_manager_player_ptr){
                projectile_manager_player_ptr->connect("bullet_collision", godot::Callable(this, "detect_entity_and_update_state"));
            }
        }
        
    }

    for(int i = 0; i < this->enemy_ptr_array.size(); i++){
        godot::Node *enemy_ptr = godot::Object::cast_to<godot::Node>(this->enemy_ptr_array[i]);
        if(!enemy_ptr)
            continue;
        godot::TypedArray<godot::Node> projectile_manager_returns =  enemy_ptr->find_children("*", "projectile_manager", true);

        for(int j = 0; j < projectile_manager_returns.size(); j++){
            godot::Node *projectile_manager_enemy_ptr = godot::Object::cast_to<godot::Node>(projectile_manager_returns[j]);
            if(projectile_manager_enemy_ptr){
                projectile_manager_enemy_ptr->connect("bullet_collision", godot::Callable(this, "detect_entity_and_update_state"));
            }
        }

    }

    //setting up radar signals for the enemy entities
    for(int i = 0; i < this->player_ptr_array.size(); i++){
        godot::Node *player_ptr = godot::Object::cast_to<godot::Node>(this->player_ptr_array[i]);
        if(!player_ptr)
            continue;

        for(int j = 0; j < this->enemy_ptr_array.size(); j++){
            godot::Node *enemy_ptr = godot::Object::cast_to<godot::Node>(this->enemy_ptr_array[j]);
            if(!enemy_ptr)
                continue;
            player_ptr->connect("send_player_radar_ping", godot::Callable(enemy_ptr, "receive_player_radar_ping"));
        }
    }
}
