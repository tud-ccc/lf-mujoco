
#include <cmath>
#include <iostream>

#include "common_position_evaluator.hpp"
#include "common_vector.hpp"
#include "common_deacceleration_controller.hpp"

PositionEvaluator::PositionEvaluator(double acceleration_cap, double max_step_length) {
  this->max_step_length_ = max_step_length;
  this->acceleration_cap_ = acceleration_cap; // in millimeter
  this->va_ = VectorArithmetics{};
}
Vector PositionEvaluator::calculate_offset_vector_given_max_acceleration(Vector last_position, Vector current_position,
                                                                         Vector raw_instruction)  const{

  // initialize vector collection
  DeaccelerationController vcfd = DeaccelerationController{this->max_step_length_, 5, 0.1};

  Vector next_logical_step_offset_vector = this->va_.get_delta_vector(last_position, current_position);
  next_logical_step_offset_vector = shorten_if_longer_than_max_stop_length(next_logical_step_offset_vector);
  vcfd.set_next_logical_step_offset_vector(next_logical_step_offset_vector);
  // std::cout << "Now the next_logical_step_offset_vector:";
  // next_logical_step_offset_vector.to_string();

  Vector next_logical_step = this->va_.add_vectors(current_position, next_logical_step_offset_vector);
  vcfd.set_next_logical_step(next_logical_step);
  // std::cout << "Now the next_logical_step:";
  // next_logical_step.to_string();

  Vector acceleration_vector = this->va_.get_delta_vector(next_logical_step, raw_instruction);
  acceleration_vector = acceleration_vector.normalize().scale(this->acceleration_cap_);
  vcfd.set_acceleration_vector(acceleration_vector);
  // std::cout << "Now the acceleration_vector:";
  // acceleration_vector.to_string();

  Vector offset_vector = this->va_.add_vectors(next_logical_step_offset_vector, acceleration_vector);
  vcfd.set_offset_vector(offset_vector);
  // std::cout << "Now the offset_vector:";
  // offset_vector.to_string();

  return vcfd.shorten_for_deacceleration(current_position, raw_instruction);
}

Vector PositionEvaluator::shorten_if_longer_than_max_stop_length(Vector next_logical_step_offset_vector) const{
  //reduce if the previous state was unstable
  if (next_logical_step_offset_vector.get_arithmetic_length() > this->max_step_length_)
    next_logical_step_offset_vector = next_logical_step_offset_vector.normalize().scale(this->max_step_length_);
  return next_logical_step_offset_vector;
}

Vector PositionEvaluator::calculate_next_position(Vector last_position, Vector current_position,
                                                  Vector raw_instruction) const{

  Vector offset_vector =
      this->calculate_offset_vector_given_max_acceleration(last_position, current_position, raw_instruction);
  Vector next_position = this->va_.add_vectors(current_position, offset_vector);
  return next_position;
}