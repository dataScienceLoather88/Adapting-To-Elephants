#include "player_interface.h"
#include "mech_movement.h"

#include <godot_cpp/classes/input.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/method_bind.hpp>
#include <godot_cpp/classes/engine.hpp>

player_interface::player_interface(){}
player_interface::~player_interface(){}

void player_interface::_bind_methods(){}

void player_interface::_process(double delta){}
void player_interface::_physics_process(double delta){
    if(godot::Engine::get_singleton()->is_editor_hint()){
        return;
    }

    godot::Input *input = godot::Input::get_singleton();

    if(input->is_action_pressed("look_up")){this->mech_movement_module_ptr->activate_look_up();}
    if(input->is_action_pressed("look_down")){this->mech_movement_module_ptr->activate_look_down();}
    if(input->is_action_just_pressed("lock_horizontal_rotation")){this->mech_movement_module_ptr->activate_lock_horizontal_rotation();}
    if(input->is_action_pressed("rotate_mech_right")){this->mech_movement_module_ptr->activate_rotate_mech_right();}
    if(input->is_action_pressed("rotate_mech_left")){this->mech_movement_module_ptr->activate_rotate_mech_left();}
    if(input->is_action_pressed("move_mech_forward")){this->mech_movement_module_ptr->activate_move_mech_forward();}
    if(input->is_action_pressed("move_mech_back")){this->mech_movement_module_ptr->activate_move_mech_back();}
    if(input->is_action_pressed("move_mech_right")){this->mech_movement_module_ptr->activate_move_mech_right();}
    if(input->is_action_pressed("move_mech_left")){this->mech_movement_module_ptr->activate_move_mech_left();}
    if(input->is_action_pressed("activate_thrust")){this->mech_movement_module_ptr->activate_activate_thrust();}
    if(input->is_action_pressed("fire_light_weapon")){this->mech_movement_module_ptr->activate_fire_light_weapon();}
}

void player_interface::_ready(){
    this->mech_movement_module_ptr = this->get_node<Mech_Movement>("mech_root");

    if(this->mech_movement_module_ptr)
        godot::UtilityFunctions::print("mech module loaded by player interface");
    else
        godot::UtilityFunctions::print("mech module NOT loaded by player interface");
}