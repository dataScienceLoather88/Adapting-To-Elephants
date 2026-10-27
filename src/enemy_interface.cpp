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
    
    godot::Vector3 self_position = this->mech_movement_module_ptr->get_global_position();

    //euclidean dist from self to player
    this->euc_dist_to_player = self_position.distance_to(position);

    //vertical angle diff from self to player (will be used 2 tell whether the enemy is approaching from above or below)
    {
        godot::Vector2 xz_projection_player = godot::Vector2(position.x, position.z);
        godot::Vector2 xz_projection_self = godot::Vector2(self_position.x, self_position.z);
        double base = xz_projection_player.distance_to(xz_projection_self);
        double hyp = self_position.distance_to(position);
        this->vertical_angle_diff_body = godot::Math::rad_to_deg(godot::Math::acos(base/hyp));
    }

    //calculating the uhhh orientation differences for aiming
    {
        godot::Vector3 vec_enemy_to_player = position - self_position;
        godot::Vector3 vec_camera_forward = this->camera_ptr->get_global_transform().basis.get_column(2);

        godot::Vector2 xz_E2P = godot::Vector2(vec_enemy_to_player.x, vec_enemy_to_player.z);
        godot::Vector2 xz_CF = godot::Vector2(vec_camera_forward.x, vec_camera_forward.z);
        this->horizontal_angle_diff_camera = godot::Math::acos((xz_E2P.dot(xz_CF))/((xz_E2P.length() * xz_CF.length()) + 0.000001f));
        this->horizontal_angle_diff_camera = godot::Math::rad_to_deg(this->horizontal_angle_diff_camera);

        godot::Vector2 xy_E2P = godot::Vector2(godot::Math::abs(vec_enemy_to_player.x), vec_enemy_to_player.y);
        godot::Vector2 xy_CF = godot::Vector2(godot::Math::abs(vec_camera_forward.x), vec_camera_forward.y);
        this->vertical_angle_diff_camera = godot::Math::acos((xy_E2P.dot(xy_CF))/(xy_E2P.length() * xy_CF.length()));
        this->vertical_angle_diff_camera = godot::Math::rad_to_deg(this->vertical_angle_diff_camera);
    }

    this->player_movement_vector = position - this->player_position;

    //update player position at the end after calculations
    this->player_position = position;

    godot::UtilityFunctions::print("player position update on enemy radar:");
    godot::UtilityFunctions::print(this->player_position);
    godot::UtilityFunctions::print(this->euc_dist_to_player);
    godot::UtilityFunctions::print(this->vertical_angle_diff_body);
    godot::UtilityFunctions::print(this->horizontal_angle_diff_camera);
    godot::UtilityFunctions::print(this->vertical_angle_diff_camera);
    godot::UtilityFunctions::print(this->player_movement_vector);
    
}