#include "level_manager.h"
#include "damage_registration_module.h"

class player_interface;
class enemy_interface;

void level_manager::_bind_methods() {}

level_manager::level_manager() {}

level_manager::~level_manager() {}

void level_manager::_process(double delta) {}

void level_manager::_physics_process(double delta) {}

void level_manager::_ready() {
    this->damage_registration_module_ptr = this->get_node<damage_registration_module>("damage_registration_module_root");

    if(this->damage_registration_module_ptr){
        godot::UtilityFunctions::print("DRM loaded by level manager");

        godot::TypedArray<godot::Node> player_ptr_array = this->find_children("*", "player_interface", false);
        godot::TypedArray<godot::Node> enemy_ptr_array = this->find_children("*", "enemy_interface", false);
        this->damage_registration_module_ptr->receive_player_and_enemy_ptrs(player_ptr_array, enemy_ptr_array);
    }
    else
        godot::UtilityFunctions::print("DRM NOT loaded by level manager");
}
