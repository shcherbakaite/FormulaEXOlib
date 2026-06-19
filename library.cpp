#include "library.h"

#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtx/rotate_vector.hpp"

// CROSSFIRE - PARSE COMMANDS
// IMU
// UPDATE ENGINE, GET COMMANDS, GET IMU, CALCULATE OUTPUTS, SEND OUT

void wheel_state_update_v1(const wheel_geometry_t* wheels, int count, command_t command, const heading_t& heading, wheel_state_t* wheel_states_out) {
    // Ground velocity and acceleration vectors of each wheel
    std::vector<glm::vec2> V(count);
    std::vector<glm::vec2> dVdt(count);
    std::vector<glm::vec2> dVdt_t(count);
    std::vector<glm::vec2> dVdt_p(count);

    // Compute wheel velocities
    for (int i = 0; i < count; ++i) {
        // Add velocity due to translation
        V[i] = command.translation_rate;
        dVdt[i] = glm::vec2(0.0f); // No contribution from translation
        // Add velocity due to rotation around vehicle center
        const wheel_geometry_t* wheel = &wheels[i];
        // Object -> World transformation of wheel position
        const glm::vec2 P = glm::rotate(wheel->offset, heading.heading);
        float r = P.length();
        float w = command.rotation_rate;
        // Construct perpendicular unit vector to radius vector of P
        glm::vec2 u_n = glm::normalize(glm::vec2(-P.y, P.x));
        // Add velocity vector due to rotation around vehicle center
        V[i] += u_n*w*r;
        // d/dt(V) = -r*w^2 r
        dVdt[i] = -P*w*w;
    }

    // Split dVdt into tangential and perpendicular components
    for (int i = 0; i < count; ++i) {
        dVdt_t[i] = V[i]*(glm::dot(dVdt[i], V[i])/glm::dot(V[i], V[i]));
        dVdt_p[i] = dVdt[i] - dVdt_t[i];
    }

    // Set wheel speeds
    for (int i = 0; i < count; ++i) {
        float circumference = wheels[i].diameter*glm::pi<float>();
        float ground_speed = V[i].length();
        wheel_states_out[i].angular_speed = (ground_speed/circumference)*2*glm::pi<float>();
        float ground_acceleration_t = dVdt_t[i].length();
        wheel_states_out[i].angular_speed_d_dt = (ground_acceleration_t/circumference)*2*glm::pi<float>();
    }

    // Orient wheels to their velocity vector
    for (int i = 0; i < count; ++i) {
        // Convert ground velocity vector to angle
        wheel_states_out[i].steering_angle = atan2(V[i].x, V[i].y) + heading.heading + heading.offset;
        // Convert normal component of dVdt to steering angle rate of change
        // w = | V x dVdt_p / |V|^2 | in cross-product notation OR in expanded coordinate notation
        wheel_states_out[i].steering_angle_d_dt = (V[i].x*dVdt_p[i].y - V[i].y*dVdt_p[i].x) / (V[i].x*V[i].x + V[i].y*V[i].y);
    }
}

void hello() {
    std::cout << "Hello, World!" << std::endl;
}
