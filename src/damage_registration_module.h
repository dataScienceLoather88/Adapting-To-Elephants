#pragma once

#include<godot_cpp/classes/node.hpp>

class damage_registration_module: public godot::Node{
    GDCLASS(damage_registration_module, godot::Node);
private:

protected:
    static void _bind_methods();
public:
    damage_registration_module();
    ~damage_registration_module();
    void detect_entity_and_update_state(uint64_t collider_object_id);

    void _process(double delta) override; 
    void _physics_process(double delta) override;   
    void _ready() override;

};