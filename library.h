#ifndef FORMULAEXO_CONTROLLER_LIBRARY_H
#define FORMULAEXO_CONTROLLER_LIBRARY_H

#include "GLM/glm.hpp"

// The EXtraOrdinary steering library

struct wheel_geometry_t{
    glm::vec2 offset;
    float diameter;
};

struct wheel_state_t {
    float angular_speed;
    float angular_speed_d_dt;
    float steering_angle;
    float steering_angle_d_dt;
};

struct command_t {
    glm::vec2 translation_rate;
    float rotation_rate;
};

struct heading_t {
    float heading;
    float offset;
};

// Set current heading as zero
void zero_heading(heading_t* heading);

// First order steering function
void wheel_state_update_v1(const wheel_geometry_t* wheels, int count, command_t command, const heading_t& heading, wheel_state_t* wheel_states_out);

void hello();

#endif // FORMULAEXO_CONTROLLER_LIBRARY_H
