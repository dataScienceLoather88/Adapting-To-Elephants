#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>

class Mech_Movement;

class enemy_interface: public godot::Node3D{
    GDCLASS(enemy_interface, godot::Node3D);
private:
    Mech_Movement *mech_movement_module_ptr = nullptr;
    godot::Camera3D *camera_ptr = nullptr;
protected:
    static void _bind_methods();
public:
    enemy_interface();
    ~enemy_interface();

    void _process(double delta) override; 
    void _physics_process(double delta) override;   
    void _ready() override;
};