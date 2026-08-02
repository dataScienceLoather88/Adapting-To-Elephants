#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/spring_arm3d.hpp>

class light_weapon;

class Mech_Movement: public godot::CharacterBody3D {
    GDCLASS(Mech_Movement, godot::CharacterBody3D);

private:
    double horizontal_acceleration = 15.0f;
    double ground_resistance = 10.0f;
    double air_resistance = 3.0f + 2.0f;
    double horizontal_ground_speed_limit = 10.0f + 10.0f;
    double horizontal_air_speed_limit = 15.0f + 15.0f;
    double vertical_acceleration = 30.0f;
    double booster_limit = 150.0f;
    double gravity = 100.0f;
    double terminal_velocity = -200.0f;

    //horizontal rotation variables
    double saddle_horizontal_rotation_degrees_per_second = 45.0f/2.0f;
    double saddle_horizontal_rotation_limit = 30.0f;
    double saddle_horizontal_accumulator = 0.0f;

    //vertical rotation variables
    double saddle_vertical_rotation_degrees_per_second = 45.0f/2.0f;
    double saddle_vertical_rotation_limit = 30.0f;
    double saddle_vertical_accumulator = 0.0f;

    godot::Vector3 direction_vector;
    godot::Vector3 resistance_vector = godot::Vector3(0,0,0);

    //pointers for model components/camera stuff
    godot::Node3D *saddle_ptr = nullptr;
    godot::Node3D *camera_pivot_ptr = nullptr;
    godot::SpringArm3D *spring_arm_ptr = nullptr;

    light_weapon *light_weapon_ptr = nullptr;
    godot::Node3D *right_forearm_ptr = nullptr;

    //movement lock
    bool horizontal_lock = false;

    //resistance changes start
    godot::Vector3 cached_direction_vector = godot::Vector3(0,0,0);
    float timer = 0.0f;
    float timer_threshold = 1.0f/3.0f;
    int jump_counter = 0;
    int jump_threshold = 10;
    float jump_incr_x = 0.0f;
    float jump_incr_z = 0.0f;
    //resistance changes end

protected:
    static void _bind_methods();

public:
    Mech_Movement();
    ~Mech_Movement();
    void _process(double delta) override; 
    void _physics_process(double delta) override;   
    void _ready() override;
};
