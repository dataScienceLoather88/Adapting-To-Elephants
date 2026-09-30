#include "enemy_interface.h"
#include "mech_movement.h"
#include "collision_layers_and_masks.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/method_bind.hpp>
#include <godot_cpp/classes/engine.hpp>

enemy_interface::enemy_interface(){}
enemy_interface::~enemy_interface(){}

void enemy_interface::_bind_methods(){
    godot::ClassDB::bind_method(godot::D_METHOD("receive_player_radar_ping", "position"), &enemy_interface::receive_player_radar_ping);

}

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

    if(this->mech_movement_module_ptr){
        godot::UtilityFunctions::print("mech module loaded by enemy interface");

        uint32_t layer = PROJECTILE_LAYER;
        uint32_t mask = WALL_LAYER | PLAYER_LAYER;
        this->mech_movement_module_ptr->pass_down_projectile_mask_and_layer(layer, mask);

        this->mech_movement_module_ptr->set_collision_layer(ENEMY_LAYER);
        this->mech_movement_module_ptr->set_collision_mask(WALL_LAYER | PLAYER_LAYER);
    }
    else
        godot::UtilityFunctions::print("mech module NOT loaded by enemy interface");

    if(this->camera_ptr){
        godot::UtilityFunctions::print("camera node loaded by enemy interface");
        this->camera_ptr->clear_current();
    }
    else
        godot::UtilityFunctions::print("camera node NOT loaded by enemy interface");
}

void enemy_interface::receive_player_radar_ping(godot::Vector3 position){
    this->player_position = position;
    godot::UtilityFunctions::print("player position update on enemy radar:");
    godot::UtilityFunctions::print(this->player_position);
}