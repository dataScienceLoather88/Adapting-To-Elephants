#pragma once

#include<godot_cpp/classes/node.hpp>

class damage_registration_module;

class level_manager: public godot::Node{
    GDCLASS(level_manager, godot::Node);

private:
    damage_registration_module *damage_registration_module_ptr = nullptr;
protected:
    static void _bind_methods();
public:
    level_manager();
    ~level_manager();

    void _process(double delta) override; 
    void _physics_process(double delta) override;   
    void _ready() override;
};