#ifndef COMMON_SAN_CHECK_ROBO
#define COMMON_SAN_CHECK_ROBO
#include "common_vector.hpp"
#include <iostream>

class RoboSanityChecker {

private:
  double acceleration_cap_;
  VectorArithmetics va_;

public:
  RoboSanityChecker(double acceleration_cap) {
    this->acceleration_cap_ = acceleration_cap;
  }
  Vector robo_sanity_check_current_position(Vector penultimate_current_position, Vector preceding_current_position,
                                            Vector current_position_by_robot, Vector current_position_by_planner) {

    Vector delta_penultimate_preceding_position =
        this->va_.get_delta_vector(penultimate_current_position, preceding_current_position);

    Vector delta_preceding_current_position_by_robot =
        this->va_.get_delta_vector(preceding_current_position, current_position_by_robot);

    Vector deacceleration_vector =
        this->va_.get_delta_vector(delta_penultimate_preceding_position, delta_preceding_current_position_by_robot);

    if (deacceleration_vector.get_arithmetic_length() > this->acceleration_cap_) {

      std::cout << " *---------* Entering the error case, the robot pretended to stop *---------* " << std::endl;

      std::cout << "Here the length of da : " << deacceleration_vector.get_arithmetic_length() << std::endl;

      std::cout << "Penultimate current position: ";
      penultimate_current_position.to_string();

      std::cout << "Preceding current position: ";
      preceding_current_position.to_string();

      std::cout << "Current position(by Robot): ";
      current_position_by_robot.to_string();

      std::cout << "Current position(by Planner): ";
      current_position_by_planner.to_string();

      return current_position_by_planner;
    } else {
      return current_position_by_robot;
    }
  }
};

#endif
