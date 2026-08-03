#pragma once

#include <godot_cpp/classes/node3d.hpp>

class Mech_Movement;

class player_interface: public godot::Node3D{
    GDCLASS(player_interface, godot::Node3D);
private:
    Mech_Movement *mech_movement_module_ptr;
protected:
    static void _bind_methods();
public:
    player_interface();
    ~player_interface();

    void _process(double delta) override; 
    void _physics_process(double delta) override;   
    void _ready() override;
};