#ifndef COMMON_SAN_CHECK_ROBO
#define COMMON_SAN_CHECK_ROBO
#include "common_vector.hpp"
#include <iostream>

class RoboSanityChecker {

private:
  double acceleration_cap_;
  VectorArithmetics va_;
  bool print_error_active_;

public:
  RoboSanityChecker(double acceleration_cap, bool print_error_active) {
    this->acceleration_cap_ = acceleration_cap;
    this->print_error_active_ = print_error_active;
  }
  bool robo_sanity_check_current_position_malfunctioning(Vector penultimate_current_position, Vector preceding_current_position,
                                            Vector current_position_by_robot) {

    Vector delta_penultimate_preceding_position =
        this->va_.get_delta_vector(penultimate_current_position, preceding_current_position);

    Vector delta_preceding_current_position_by_robot =
        this->va_.get_delta_vector(preceding_current_position, current_position_by_robot);

    Vector deacceleration_vector =
        this->va_.get_delta_vector(delta_penultimate_preceding_position, delta_preceding_current_position_by_robot);

    if (deacceleration_vector.get_arithmetic_length() > this->acceleration_cap_) {

      if (this->print_error_active_) {
        std::cout << " *---------* Entering the error case, the robot pretended to stop *---------* " << std::endl;
        std::cout << "Here the length of da : " << deacceleration_vector.get_arithmetic_length() << " > "
                  << this->acceleration_cap_ << std::endl;

        std::cout << "Penultimate current position: ";
        penultimate_current_position.to_string();

        std::cout << "Preceding current position: ";
        preceding_current_position.to_string();

        std::cout << "Current position(by Robot): ";
        current_position_by_robot.to_string();

        std::cout << "Current position(by Planner): ";
        current_position_by_prediction.to_string();
      
      }

      return false;
    } else {
      return true;
    }
  }
};

#endif
