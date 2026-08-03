
#include "mech_movement.h"
#include "light_weapon.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/method_bind.hpp>
#include <godot_cpp/classes/engine.hpp>


Mech_Movement::Mech_Movement(){}
Mech_Movement::~Mech_Movement(){}

void Mech_Movement::_bind_methods(){
}

void Mech_Movement::_process(double delta) {
    //godot::UtilityFunctions::print("testing integration of mech movement class");
}

void Mech_Movement::_physics_process(double delta){
    if(godot::Engine::get_singleton()->is_editor_hint()){
        return;
    }

    double saddle_horizontal_rotation_direction;
    double saddle_vertical_rotation_direction;
    godot::Vector3 velocity_vector = this->get_velocity();

    //input and execution for vertical rotation
    if(this->look_up){saddle_vertical_rotation_direction = 1.0f; this->look_up = false;}
    else if(this->look_down){saddle_vertical_rotation_direction = -1.0f; this->look_down = false;}
    else {saddle_vertical_rotation_direction = 0.0f;}

    double saddle_vertical_rotation_rate_rad = godot::Math::deg_to_rad(this->saddle_vertical_rotation_degrees_per_second);
    double saddle_vertical_angle_change_rad = saddle_vertical_rotation_rate_rad * delta * saddle_vertical_rotation_direction;
    double saddle_vertical_rotation_limit_rad = godot::Math::deg_to_rad(this->saddle_vertical_rotation_limit);

    double old_vertical_accum = this->saddle_vertical_accumulator;
    this->saddle_vertical_accumulator += saddle_vertical_angle_change_rad;

    if(this->saddle_vertical_accumulator < - saddle_vertical_rotation_limit_rad || this->saddle_vertical_accumulator > saddle_vertical_rotation_limit_rad){
        this->saddle_vertical_accumulator = (this->saddle_vertical_accumulator < 0.0) ? saddle_vertical_rotation_limit_rad * -1 : saddle_vertical_rotation_limit_rad;
        saddle_vertical_angle_change_rad = this->saddle_vertical_accumulator - old_vertical_accum;
    }

    godot::Transform3D saddle_vertical_transform = this->saddle_ptr->get_transform();
    saddle_vertical_transform = saddle_vertical_transform.rotated_local(godot::Vector3(1,0,0), saddle_vertical_angle_change_rad);
    saddle_vertical_transform.orthonormalize();
    this->saddle_ptr->set_transform(saddle_vertical_transform);

    godot::Transform3D camera_vertical_transform = this->camera_pivot_ptr->get_transform();
    camera_vertical_transform = camera_vertical_transform.rotated_local(godot::Vector3(1,0,0), saddle_vertical_angle_change_rad);
    camera_vertical_transform.orthonormalize();
    this->camera_pivot_ptr->set_transform(camera_vertical_transform); 

    //input and execution for horizontal rotation 
    if(this->lock_horizontal_rotation){this->horizontal_lock = !this->horizontal_lock; this->lock_horizontal_rotation = false;}
    if(this->rotate_mech_right){saddle_horizontal_rotation_direction = -1.0f; this->rotate_mech_right = false;}
    else if(this->rotate_mech_left){saddle_horizontal_rotation_direction = 1.0f; this->rotate_mech_left = false;}
    else {saddle_horizontal_rotation_direction = 0.0f;}

    double saddle_horizontal_rotation_rate_rad = godot::Math::deg_to_rad(this->saddle_horizontal_rotation_degrees_per_second);
    double saddle_horizontal_angle_change_rad = saddle_horizontal_rotation_rate_rad * delta * saddle_horizontal_rotation_direction;
    double saddle_horizontal_rotation_limit_rad = godot::Math::deg_to_rad(this->saddle_horizontal_rotation_limit);
    bool model_move_no_lock = false;
    double model_horizontal_angle_change = saddle_horizontal_angle_change_rad;

    if(!this->horizontal_lock){
        double old_accum = this->saddle_horizontal_accumulator;
        this->saddle_horizontal_accumulator += saddle_horizontal_angle_change_rad;

        if(this->saddle_horizontal_accumulator < -saddle_horizontal_rotation_limit_rad || this->saddle_horizontal_accumulator > saddle_horizontal_rotation_limit_rad){
            model_move_no_lock = true;

            double clamp_value = (this->saddle_horizontal_accumulator < 0.0) ? saddle_horizontal_rotation_limit_rad * -1 : saddle_horizontal_rotation_limit_rad;
            model_horizontal_angle_change = this->saddle_horizontal_accumulator - clamp_value;

            this->saddle_horizontal_accumulator = clamp_value;
            saddle_horizontal_angle_change_rad = this->saddle_horizontal_accumulator - old_accum;               
        }

        godot::Transform3D saddle_horizontal_transform = this->saddle_ptr->get_transform();
        saddle_horizontal_transform = saddle_horizontal_transform.rotated(godot::Vector3(0,1,0), saddle_horizontal_angle_change_rad);
        saddle_horizontal_transform.orthonormalize();
        this->saddle_ptr->set_transform(saddle_horizontal_transform);

        godot::Transform3D camera_horizontal_transform = this->camera_pivot_ptr->get_transform();
        camera_horizontal_transform = camera_horizontal_transform.rotated(godot::Vector3(0,1,0), saddle_horizontal_angle_change_rad);
        camera_horizontal_transform.orthonormalize();
        this->camera_pivot_ptr->set_transform(camera_horizontal_transform);
    }

    if(model_move_no_lock || this->horizontal_lock){
        godot::Transform3D model_horizontal_transform = this->get_transform();
        model_horizontal_transform = model_horizontal_transform.rotated_local(godot::Vector3(0,1,0), model_horizontal_angle_change);
        model_horizontal_transform.orthonormalize();
        this->set_transform(model_horizontal_transform);
    }

    //input for movement 
    if(this->move_mech_forward){this->direction_vector.z = 1.0f; this->move_mech_forward = false;}
    else if(this->move_mech_back){this->direction_vector.z = -1.0f; this->move_mech_back = false;}
    else {this->direction_vector.z = 0.0f;}

    if(this->move_mech_right){this->direction_vector.x = -1.0f; this->move_mech_right = false;}
    else if(this->move_mech_left){this->direction_vector.x = 1.0f; this->move_mech_left = false;}
    else {this->direction_vector.x = 0.0f;}

    if(this->activate_thrust){
        if(velocity_vector.y < this->booster_limit){
            velocity_vector.y += (this->vertical_acceleration * delta);
        }
        this->activate_thrust = false;
    }
    else if (!this->is_on_floor()){ //is_on_floor() returns true on collision. SHould work cause negative y vector will be <0 if the mech collided with the floor
        if(velocity_vector.y > this->terminal_velocity){velocity_vector.y -= (this->gravity * delta);}
    }

    godot::Vector3 orientation_vector = this->get_basis().get_euler();
    double yaw = orientation_vector.y;

    //used a clockwise matrix here because of the inverted x axis iirc
    double new_x_direction = (this->direction_vector.x * godot::Math::cos(yaw)) + (this->direction_vector.z * godot::Math::sin(yaw));
    double new_z_direction = (-1 * this->direction_vector.x * godot::Math::sin(yaw)) + (this->direction_vector.z * godot::Math::cos(yaw));

    this->direction_vector.x = new_x_direction;
    this->direction_vector.z = new_z_direction;

    if(direction_vector != godot::Vector3(0,0,0)){
        this->direction_vector.normalize();
    }

    //resistance changes start 2
    //resistance vector updates
    if(this->resistance_vector == godot::Vector3(0,0,0)){
        this->resistance_vector.x = (this->direction_vector.x != 0) ? godot::Math::abs(this->direction_vector.x) : this->resistance_vector.x;
        this->resistance_vector.z = (this->direction_vector.z != 0) ? godot::Math::abs(this->direction_vector.z) : this->resistance_vector.z;
    }
    //resistance changes end 2

    //resistance changes start 3
    if(this->timer >= this->timer_threshold){
        if(this->jump_counter < this->jump_threshold){
            this->resistance_vector.x += this->jump_incr_x;
            this->resistance_vector.z += this->jump_incr_z;
            this->jump_counter += 1;
        }

        if(this->direction_vector != godot::Vector3(0,0,0) && 
            (this->cached_direction_vector.x != godot::Math::abs(this->direction_vector.x)
            || this->cached_direction_vector.y != godot::Math::abs(this->direction_vector.y)
            || this->cached_direction_vector.z != godot::Math::abs(this->direction_vector.z))){

            this->cached_direction_vector.x = godot::Math::abs(this->direction_vector.x);
            this->cached_direction_vector.z = godot::Math::abs(this->direction_vector.z); 

            this->jump_incr_x = (this->cached_direction_vector.x - this->resistance_vector.x) / (float)this->jump_threshold;
            this->jump_incr_z = (this->cached_direction_vector.z - this->resistance_vector.z) / (float)this->jump_threshold;

            this->jump_counter = 0;
        }
        this->timer = 0;
    }
    //resistance changes end 3

    //final velocity calculations
    double resistance;
    double speed_limit;
    if(this->is_on_floor()){
        resistance = this->ground_resistance;
        speed_limit = this->horizontal_ground_speed_limit;
    }
    else {
        resistance = this->air_resistance;
        speed_limit = this->horizontal_air_speed_limit;
    }

    if(godot::Math::abs(velocity_vector.x) < speed_limit * godot::Math::abs(this->direction_vector.x)){
        velocity_vector.x += this->direction_vector.x * this->horizontal_acceleration * delta;
    }
    if(godot::Math::abs(velocity_vector.z) < speed_limit * godot::Math::abs(this->direction_vector.z)){
        velocity_vector.z += this->direction_vector.z * this->horizontal_acceleration * delta;
    }

    if(velocity_vector.x != 0.0f){
        if(velocity_vector.x > 0.0f){
            velocity_vector.x -= (resistance * delta * this->resistance_vector.x); 
            if(velocity_vector.x < 0.0f){
                velocity_vector.x = 0.0f;
            }
        }
        else{
            velocity_vector.x += (resistance * delta * this->resistance_vector.x);
            if(velocity_vector.x > 0.0f){
                velocity_vector.x = 0.0f;
            }
        }
    }

    if(velocity_vector.z != 0.0f){
        if(velocity_vector.z > 0.0f){
            velocity_vector.z -= (resistance * delta * this->resistance_vector.z);
            if(velocity_vector.z < 0.0f){
                velocity_vector.z = 0.0f;
            } 
        }
        else{
            velocity_vector.z += (resistance * delta * this->resistance_vector.z);
            if(velocity_vector.z > 0.0f){
                velocity_vector.z = 0.0f;
            }
        }
    }

    //godot::UtilityFunctions::print("-----------------");
    //godot::UtilityFunctions::print(this->direction_vector);
    //godot::UtilityFunctions::print(this->resistance_vector);
    //godot::UtilityFunctions::print(velocity_vector);
    this->set_velocity(velocity_vector);
    this->move_and_slide();
    velocity_vector = this->get_velocity();

    //resistance changes start 1
    if(velocity_vector == godot::Vector3(0,0,0)){
        this->resistance_vector = godot::Vector3(0,0,0);
        this->cached_direction_vector = godot::Vector3(0,0,0);
        this->direction_vector = godot::Vector3(0,0,0);
        this->jump_incr_x = 0;
        this->jump_incr_z = 0;
        this->jump_counter = 0;
    }
    this->timer += delta;
    //resistance changes end 1

    //firing input handling                     
    if(this->fire_light_weapon){
        this->light_weapon_ptr->fire_request(this->camera_pivot_ptr->get_global_transform());
        this->fire_light_weapon = false;
    }
}

void Mech_Movement::_ready(){
    this->saddle_ptr = this->get_node<godot::Node3D>("mech_model/waist/Saddle"); //possible hardcode, change later
    this->camera_pivot_ptr = this->get_node<godot::Node3D>("camera_pivot");
    this->spring_arm_ptr = this->get_node<godot::SpringArm3D>("camera_pivot/SpringArm3D");
    this->light_weapon_ptr = this->get_node<light_weapon>("light_weapon_root");
    this->right_forearm_ptr = this->get_node<godot::Node3D>("mech_model/waist/Saddle/right upper arm/right forearm");

    if(this->saddle_ptr)
        godot::UtilityFunctions::print("saddle loaded");
    else
        godot::UtilityFunctions::print("saddle NOT loaded");

    if(this->camera_pivot_ptr)
        godot::UtilityFunctions::print("camera loaded");
    else
        godot::UtilityFunctions::print("camera NOT loaded");

    if(this->spring_arm_ptr){
        this->spring_arm_ptr->add_excluded_object(this->get_rid());
        godot::UtilityFunctions::print("spring arm loaded");
    }
    else
        godot::UtilityFunctions::print("spring arm NOT loaded");

    if(this->light_weapon_ptr){
        godot::UtilityFunctions::print("light weapon loaded");
    }
    else 
        godot::UtilityFunctions::print("light weapon NOT loaded");

    if(this->right_forearm_ptr){
        godot::UtilityFunctions::print("right forearm loaded");
    }
    else 
        godot::UtilityFunctions::print("right forearm NOT loaded");

    if(this->right_forearm_ptr && this->light_weapon_ptr){
        godot::UtilityFunctions::print("moving light weapon to right forearm");
        this->light_weapon_ptr->set_global_position(this->right_forearm_ptr->get_global_position());
    }
    else{
        godot::UtilityFunctions::print("can not move light weapon to right forearm");
    }
}

//input state activation functions
void Mech_Movement::activate_look_up() {this->look_up = true;}
void Mech_Movement::activate_look_down() {this->look_down = true;}
void Mech_Movement::activate_lock_horizontal_rotation() {this->lock_horizontal_rotation = true;}
void Mech_Movement::activate_rotate_mech_right() {this->rotate_mech_right = true;}
void Mech_Movement::activate_rotate_mech_left() {this->rotate_mech_left = true;}
void Mech_Movement::activate_move_mech_forward() {this->move_mech_forward = true;}
void Mech_Movement::activate_move_mech_back() {this->move_mech_back = true;}
void Mech_Movement::activate_move_mech_right() {this->move_mech_right = true;}
void Mech_Movement::activate_move_mech_left() {this->move_mech_left = true;}
void Mech_Movement::activate_activate_thrust() {this->activate_thrust = true;}
void Mech_Movement::activate_fire_light_weapon() {this->fire_light_weapon = true;}
