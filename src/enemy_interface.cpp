#include "enemy_interface.h"
#include "mech_movement.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/method_bind.hpp>
#include <godot_cpp/classes/engine.hpp>

enemy_interface::enemy_interface(){}
enemy_interface::~enemy_interface(){}

void enemy_interface::_bind_methods(){}

void enemy_interface::_process(double delta){}

void enemy_interface::_physics_process(double delta){
    if(godot::Engine::get_singleton()->is_editor_hint()){
        return;
    }

    if(false){this->mech_movement_module_ptr->activate_look_up();}
    if(false){this->mech_movement_module_ptr->activate_look_down();}
    if(false){this->mech_movement_module_ptr->activate_lock_horizontal_rotation();}
    if(false){this->mech_movement_module_ptr->activate_rotate_mech_right();}
    if(false){this->mech_movement_module_ptr->activate_rotate_mech_left();}
    if(false){this->mech_movement_module_ptr->activate_move_mech_forward();}
    if(false){this->mech_movement_module_ptr->activate_move_mech_back();}
    if(false){this->mech_movement_module_ptr->activate_move_mech_right();}
    if(false){this->mech_movement_module_ptr->activate_move_mech_left();}
    if(false){this->mech_movement_module_ptr->activate_activate_thrust();}
    if(false){this->mech_movement_module_ptr->activate_fire_light_weapon();}
}

void enemy_interface::_ready(){
    this->mech_movement_module_ptr = this->get_node<Mech_Movement>("mech_root");
    this->camera_ptr = this->get_node<godot::Camera3D>("mech_root/camera_pivot/SpringArm3D/Camera3D");

    if(this->mech_movement_module_ptr)
        godot::UtilityFunctions::print("mech module loaded by enemy interface");
    else
        godot::UtilityFunctions::print("mech module NOT loaded by enemy interface");

    if(this->camera_ptr){
        godot::UtilityFunctions::print("camera node loaded by enemy interface");
        this->camera_ptr->clear_current();
    }
    else
        godot::UtilityFunctions::print("camera node NOT loaded by enemy interface");
}