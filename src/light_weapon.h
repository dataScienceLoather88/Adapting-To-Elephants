#pragma once

#include<godot_cpp/classes/node3d.hpp>

class projectile_manager;

class light_weapon: public godot::Node3D{
    GDCLASS(light_weapon, godot::Node3D); 

private:
    godot::Transform3D camera_global_transform;
    bool fire_request_approved_flag = false;
    double timer_limit = 0.15f; //limit in seconds
    double timer_count = 0.0f;
    double bullet_velocity = 600.0f;

    projectile_manager *projectile_manager_ptr = nullptr;

protected:
    static void _bind_methods();

public:
    light_weapon();
    ~light_weapon();
    void fire_request(godot::Transform3D camera_global_transform);

    void _process(double delta) override; 
    void _physics_process(double delta) override;   
    void _ready() override;
};