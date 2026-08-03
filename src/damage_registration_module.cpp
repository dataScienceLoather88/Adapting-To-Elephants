#include "damage_registration_module.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/method_bind.hpp>
#include <godot_cpp/classes/engine.hpp>

damage_registration_module::damage_registration_module(){}

damage_registration_module::~damage_registration_module(){}

void damage_registration_module::_bind_methods(){
    godot::ClassDB::bind_method(godot::D_METHOD("detect_entity_and_update_state", "collider_object_id"), &damage_registration_module::detect_entity_and_update_state);
}

void damage_registration_module::_ready(){
    godot::Node *projectile_manager_player_ptr = get_node<godot::Node>("../player_interface_root/mech_root/light_weapon_root/projectile_manager_root");
    if(projectile_manager_player_ptr){
        godot::UtilityFunctions::print("projectile_manager loaded in damage registration class");
        projectile_manager_player_ptr->connect("bullet_collision", godot::Callable(this, "detect_entity_and_update_state"));
    }
    else{
        godot::UtilityFunctions::print("projectile_manager NOT loaded in damage registration class");
    }
}

void damage_registration_module::_process(double delta){}

void damage_registration_module::_physics_process(double delta){}

void damage_registration_module::detect_entity_and_update_state(uint64_t collider_object_id){
    godot::UtilityFunctions::print(collider_object_id);
}