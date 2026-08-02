#include "light_weapon.h"
#include "projectile_manager.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/method_bind.hpp>
#include <godot_cpp/classes/engine.hpp>

light_weapon::light_weapon(){}
light_weapon::~light_weapon(){} 

void light_weapon::_bind_methods(){
}

void light_weapon::_process(double delta){

}

void light_weapon::_ready(){
    this->projectile_manager_ptr = this->get_node<projectile_manager>("projectile_manager_root");

    if(this->projectile_manager_ptr)
        godot::UtilityFunctions::print("projectile manager loaded by light weapon");
    else
        godot::UtilityFunctions::print("projectile manager NOT loaded by light weapon");
}

void light_weapon::_physics_process(double delta){
    this->timer_count += delta;

    if(this->fire_request_approved_flag == true){
        //do the necessary calculations and spawn a projectile
       
        this->projectile_manager_ptr->register_projectile(this->get_global_position(), this->camera_global_transform.basis.get_column(2), this->bullet_velocity);

        this->fire_request_approved_flag = false;
        this->timer_count = 0.0f;

        godot::UtilityFunctions::print("firing");
        godot::UtilityFunctions::print(this->camera_global_transform);
    }    

}

void light_weapon::fire_request(godot::Transform3D camera_global_transform){
    if(this->timer_count < this->timer_limit){return;}

    this->camera_global_transform = camera_global_transform;
    this->fire_request_approved_flag = true;
}