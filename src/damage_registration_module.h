#pragma once

#include<godot_cpp/classes/node.hpp>

class damage_registration_module: public godot::Node{
    GDCLASS(damage_registration_module, godot::Node);
private:
    godot::TypedArray<godot::Node> player_ptr_array;
    godot::TypedArray<godot::Node> enemy_ptr_array;
protected:
    static void _bind_methods();
public:
    damage_registration_module();
    ~damage_registration_module();
    void detect_entity_and_update_state(uint64_t collider_object_id);
    void receive_player_and_enemy_ptrs(godot::TypedArray<godot::Node> player_ptr_array, godot::TypedArray<godot::Node> enemy_ptr_array);

    void _process(double delta) override; 
    void _physics_process(double delta) override;   
    void _ready() override;

};