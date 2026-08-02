#pragma once

#include<godot_cpp/classes/node.hpp>
#include<godot_cpp/classes/character_body3d.hpp>
#include<godot_cpp/classes/packed_scene.hpp>

#define PROJECTILE_LIMIT_INT 10

class projectile_manager: public godot::Node{
    GDCLASS(projectile_manager, godot::Node);

private:
    //static number of projectiles for now
    const int projectile_limit = PROJECTILE_LIMIT_INT;
    struct projectile_data{
        godot::Vector3 starting_position_vector;
        godot::Vector3 direction_vector;
        float starting_velocity;
    } projectile_data_array[PROJECTILE_LIMIT_INT];  
    godot::CharacterBody3D** projectile_array = nullptr;
    int projectile_iterator; 
    int projectile_count;

    godot::Ref<godot::PackedScene> projectile_scene;

    

protected:
    static void _bind_methods();

public:
    projectile_manager();
    ~projectile_manager();
    void register_projectile(godot::Vector3 starting_position_vector, godot::Vector3 direction_vector, float starting_velocity);

    void _process(double delta) override; 
    void _physics_process(double delta) override;   
    void _ready() override;
};