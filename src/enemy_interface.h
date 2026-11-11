#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>

class Mech_Movement;

class enemy_interface: public godot::Node3D{
    GDCLASS(enemy_interface, godot::Node3D);
private:
    Mech_Movement *mech_movement_module_ptr = nullptr;
    godot::Camera3D *camera_ptr = nullptr;

    //positional variables relative to player
    godot::Vector3 player_position = godot::Vector3(0,0,0);
    double euc_dist_to_player = 0.0f;
    double vertical_angle_diff_body = 0.0f;
    double horizontal_angle_diff_camera = 0.0f;
    double vertical_angle_diff_camera = 0.0f;
    godot::Vector3 player_movement_vector = godot::Vector3(0,0,0);

protected:
    static void _bind_methods();
public:
    enemy_interface();
    ~enemy_interface();
    void receive_player_radar_ping(godot::Vector3 position);

    void _process(double delta) override; 
    void _physics_process(double delta) override;   
    void _ready() override;
};