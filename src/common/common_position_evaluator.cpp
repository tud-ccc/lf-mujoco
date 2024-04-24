
#include <cmath>
#include <iostream>

#include "common_position_evaluator.hpp"
#include "common_vector.hpp"

PositionEvaluator::PositionEvaluator(double acceleration_cap) {

  this->acceleration_cap_ = acceleration_cap; // in millimeter
  this->va_ = VectorArithmetics{};
}
Vector PositionEvaluator::calculate_offset_vector_given_max_acceleration(Vector last_position, Vector current_position,
                                                                         Vector raw_instruction,
                                                                         double acceleration_length) {

  Vector next_logical_step_offset_vector = this->va_.get_delta_vector(last_position, current_position);
  std::cout << "Now the next_logical_step_offset_vector:" << std::endl;
  next_logical_step_offset_vector.to_string();

  Vector next_logical_step = this->va_.add_vectors(current_position, next_logical_step_offset_vector);
  std::cout << "Now the next_logical_step:" << std::endl;
  next_logical_step.to_string();

  Vector acceleration_vector = this->va_.get_delta_vector(next_logical_step, raw_instruction);
  acceleration_vector = acceleration_vector.normalize().scale(acceleration_length);
  std::cout << "Now the acceleration_vector:" << std::endl;
  acceleration_vector.to_string();

  Vector offset_vector = this->va_.add_vectors(next_logical_step_offset_vector, acceleration_vector);
  offset_vector = offset_vector.normalize().scale(1);
  std::cout << "Now the offset_vector:" << std::endl;
  offset_vector.to_string();

  std::cout << "##### End of calculation #####" << std::endl;

  return offset_vector;
}

Vector PositionEvaluator::calculate_next_position(Vector last_position, Vector current_position,
                                                  Vector raw_instruction) {

  Vector offset_vector = this->calculate_offset_vector_given_max_acceleration(last_position, current_position,
                                                                              raw_instruction, this->acceleration_cap_);
  Vector next_position = this->va_.add_vectors(current_position, offset_vector);
  return next_position;
}