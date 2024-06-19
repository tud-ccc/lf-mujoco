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
  bool robot_malfunctioning_check_coordinates(Position penultimate_current_position,
                                              Position preceding_current_position, Position current_position_by_robot,
                                              bool print_error_case) {
    Vector pen_coords = penultimate_current_position.get_coordinates();
    Vector preced_coords = preceding_current_position.get_coordinates();
    Vector curr_coords = current_position_by_robot.get_coordinates();

    Vector delta_penultimate_preceding_position = this->va_.get_delta_vector(pen_coords, preced_coords);

    Vector delta_preceding_current_position_by_robot = this->va_.get_delta_vector(preced_coords, curr_coords);

    Vector deacceleration_vector =
        this->va_.get_delta_vector(delta_penultimate_preceding_position, delta_preceding_current_position_by_robot);

    if (deacceleration_vector.get_arithmetic_length() > this->acceleration_cap_) {

      if (print_error_case) {
        std::cout << " *---------* Entering the error case, malfunctioning*---------* " << std::endl;
        std::cout << "Here the length of da : " << deacceleration_vector.get_arithmetic_length() << " > "
                  << this->acceleration_cap_ << std::endl;
        std::cout << "Deacceleration Vector: ";
        deacceleration_vector.to_string();

        std::cout << "Penultimate current position: ";
        pen_coords.to_string();

        std::cout << "Preceding current position: ";
        preced_coords.to_string();

        std::cout << "Current position(by Robot): ";
        curr_coords.to_string();
      }

      return true;
    } else {
      return false;
    }
  }
  bool robot_pretended_to_stop(Position penultimate_current_position, Position preceding_current_position,
                               Position current_position_by_robot) {
    Vector pen_coords = penultimate_current_position.get_coordinates();
    Vector preced_coords = preceding_current_position.get_coordinates();
    Vector curr_coords = current_position_by_robot.get_coordinates();

    bool robo_malfunctioning = this->robot_malfunctioning_check_coordinates(
        penultimate_current_position, preceding_current_position, current_position_by_robot, false);

    if (robo_malfunctioning && !penultimate_current_position.equals(preceding_current_position) &&
        preceding_current_position.equals(current_position_by_robot)) {

      if (this->print_error_active_) {
        std::cout << " *---------* Entering the error case, the robot pretended to stop *---------* " << std::endl;

        std::cout << "Penultimate current position: ";
        pen_coords.to_string();

        std::cout << "Preceding current position: ";
        preced_coords.to_string();

        std::cout << "Current position(by Robot): ";
        curr_coords.to_string();
      }
      return true;
    } else {
      return false;
    }
  }
  Position predict_position(Position penultimate_current_position, Position preceding_current_position) {
    Vector pen_coord = penultimate_current_position.get_coordinates();
    Vector prec_coord = preceding_current_position.get_coordinates();

    Vector pen_roll_pitch_yaw = penultimate_current_position.get_roll_pitch_yaw();
    Vector prec_roll_pitch_yaw = preceding_current_position.get_roll_pitch_yaw();

    Vector next_coordinates = this->va_.add_vectors(prec_coord, this->va_.get_delta_vector(pen_coord, prec_coord));
    Vector next_roll_pitch_yaw =
        this->va_.add_vectors(prec_roll_pitch_yaw, this->va_.get_delta_vector(pen_roll_pitch_yaw, prec_roll_pitch_yaw));

    Position predicted_position = Position{next_coordinates, next_roll_pitch_yaw};
    return predicted_position;
  }
};

#endif
