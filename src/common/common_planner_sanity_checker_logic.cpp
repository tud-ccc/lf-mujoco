#include "common_planner_sanity_checker_logic.hpp"
#include "common_vector.hpp"
#include <cassert>
#include <iostream>

SanityCheckerLogic::SanityCheckerLogic(double acceleration_cap, double max_step_length) {

  this->acceleration_cap_ = acceleration_cap;
  this->max_step_length_ = max_step_length;
  this->va_ = VectorArithmetics{};
};

int SanityCheckerLogic::planner_sanity_check_next_position(Vector last_position, Vector current_position,
                                                   Vector next_position) {

  Vector next_logical_step_offset_vector = this->va_.get_delta_vector(last_position, current_position);

  Vector next_step_offset_vector = this->va_.get_delta_vector(current_position, next_position);

  Vector acceleration_vector = this->va_.get_delta_vector(next_logical_step_offset_vector, next_step_offset_vector);

  if (acceleration_vector.get_arithmetic_length() > this->acceleration_cap_) {

    std::cout << " <====> Entering the error case <====> " << std::endl;

    std::cout << "Here the length of a : " << acceleration_vector.get_arithmetic_length() << std::endl;

    std::cout << "Last position: ";
    last_position.to_string();

    std::cout << "current position: ";
    current_position.to_string();

    std::cout << "Next position: ";
    next_position.to_string();

    // assert(false);
  }

  return 0;
}
