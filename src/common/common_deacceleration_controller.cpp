
#include "common_deacceleration_controller.hpp"
#include "common_vector.hpp"
// #include "common_error_handling.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

DeaccelerationController::DeaccelerationController(double max_step_length, double threshold_deaccelerate, double threshold_when_to_stop) {
  this->max_step_length_ = max_step_length;
  this->threshold_deaccelerate_ = threshold_deaccelerate;
  this->threshold_when_to_stop_ = threshold_when_to_stop;
  this->va_ = VectorArithmetics{};
};

void DeaccelerationController::set_next_logical_step_offset_vector(Vector next_logical_step_offset_vector) {
  this->next_logical_step_offset_vector_ = next_logical_step_offset_vector;
}
void DeaccelerationController::set_next_logical_step(Vector next_logical_step) {
  this->next_logical_step_ = next_logical_step;
}
void DeaccelerationController::set_acceleration_vector(Vector acceleration_vector) {
  this->acceleration_vector_ = acceleration_vector;
}
void DeaccelerationController::set_offset_vector(Vector offset_vector) {
  if (this->offset_vector_.get_arithmetic_length() > this->max_step_length_) {
    this->offset_vector_ = this->offset_vector_.normalize().scale(this->max_step_length_);
  }
  this->offset_vector_ = offset_vector;
}

Vector DeaccelerationController::get_next_logical_step_offset_vector() const {
  return this->next_logical_step_offset_vector_;
}
Vector DeaccelerationController::get_next_logical_step() const { return this->next_logical_step_; }
Vector DeaccelerationController::get_acceleration_vector() const { return this->acceleration_vector_; }
Vector DeaccelerationController::get_offset_vector() const { return this->offset_vector_; }

void DeaccelerationController::print_all_collected_vectors() const {
  std::cout << "next_logical_step_offset_vector: ";
  this->next_logical_step_offset_vector_.to_string();
  std::cout << "Now the next_logical_step:";
  this->next_logical_step_.to_string();
  std::cout << "Now the acceleration_vector:";
  this->acceleration_vector_.to_string();
  std::cout << "Now the offset_vector:";
  this->offset_vector_.to_string();
}

double DeaccelerationController::calclulate_min_speed_with_right_angled_triangle() const {

  double a = this->acceleration_vector_.get_arithmetic_length();
  // std::cout << "a :  " << a << std::endl;

  double alpha = this->va_.get_angle_in_radians(this->next_logical_step_offset_vector_, this->offset_vector_);
  // std::cout << "alpha :  " << alpha << std::endl;

  double b = this->next_logical_step_offset_vector_.get_arithmetic_length();
  // std::cout << "b :  " << b << std::endl;

  double beta = asin((sin(alpha) * b) / a);
  beta = PI - beta;
  // std::cout << "beta : " << beta << std::endl;

  double gamma = PI - beta - alpha;
  // std::cout << "gamma : " << gamma << std::endl;

  double c = (a * sin(gamma)) / sin(alpha);
  // std::cout << "c : " << c << std::endl;

  if (std::isnan(c)) {
    std::cout << "Next_logical_step_offset_vector:";
    this->next_logical_step_offset_vector_.to_string();
    std::cout << "Offset_vector:";
    this->offset_vector_.to_string();
    std::cout << "a :  " << a << std::endl;
    std::cout << "alpha :  " << alpha << std::endl;
    std::cout << "b :  " << b << std::endl;
    std::cout << "beta : " << beta << std::endl;
    std::cout << "c : " << c << std::endl;
    std::cout << "gamma : " << gamma << std::endl;
    assert(false && "Preferred speed has a wrong value");
  }
  return c;
}

double DeaccelerationController::calculate_min_speed() const {

  bool next_logical_step_offset_vector_shorter_than_acc_vec =
      this->next_logical_step_offset_vector_.get_arithmetic_length() < acceleration_vector_.get_arithmetic_length();

  bool nlsov_and_ov_are_ld = this->va_.linear_dependent(this->next_logical_step_offset_vector_, this->offset_vector_);
  bool alpha_is_0 = this->va_.get_angle_in_radians(this->next_logical_step_offset_vector_, this->offset_vector_) == 0;
  bool next_logical_step_offset_vector_linear_dependent_to_acc_vec = nlsov_and_ov_are_ld || alpha_is_0;

  if (next_logical_step_offset_vector_shorter_than_acc_vec) {
    return 0;
  } else if (next_logical_step_offset_vector_linear_dependent_to_acc_vec) {
    return this->next_logical_step_offset_vector_.get_arithmetic_length() -
           acceleration_vector_.get_arithmetic_length();
  } else {
    return this->calclulate_min_speed_with_right_angled_triangle();
  }
}

double DeaccelerationController::calculate_max_speed() const {
  double offset_vector_length = this->offset_vector_.get_arithmetic_length();
  bool next_step_would_be_bigger_than_max_step_length = offset_vector_length > this->max_step_length_;
  if (next_step_would_be_bigger_than_max_step_length) {
    return this->max_step_length_;
  } else {
    return offset_vector_length;
  }
}
double DeaccelerationController::calculate_speed_selected_by_PID(double distance_to_target) const {
  double selected_speed = (distance_to_target < this->threshold_deaccelerate_)
                              ? (distance_to_target * (this->max_step_length_ / this->threshold_deaccelerate_))
                              : this->max_step_length_;
  return selected_speed;
}

double DeaccelerationController::calculate_speed_next_step(const Vector current_position, const Vector raw_instruction,
                                                           const double distance_to_target, const double max_speed,
                                                           const double min_speed) const {
  assert(max_speed <= this->max_step_length_ && "We are moving too fast");
  assert(max_speed >= min_speed >= 0 && "Max_speed is smaller than Min_speed");
  assert(distance_to_target >= 0 && "We have a negative distance to the target");

  double preferred_speed = this->calculate_speed_selected_by_PID(distance_to_target);
  assert(preferred_speed >= 0 && "preferred_speed is negative");
  // std::cout << "Preferred_speed for next step : " << preferred_speed << std::endl;

  if (preferred_speed <= max_speed && preferred_speed >= min_speed) {
    return preferred_speed;
  } else if (preferred_speed > max_speed) {
    return max_speed;
  } else if (preferred_speed < min_speed) {
    return min_speed;
  } else {
    std::cout << "Preferred_speed : " << preferred_speed << std::endl;
    std::cout << "max_speed : " << max_speed << std::endl;
    std::cout << "min_speed : " << min_speed << std::endl;
    assert(false && "Preferred speed has a wrong value");
  }
}

double DeaccelerationController::calculate_speed_next_step_wrapper(const Vector current_position,
                                                                   const Vector raw_instruction) const {
  double distance_to_target = this->va_.get_distance_between_point_vectors(current_position, raw_instruction);
  double max_speed = this->calculate_max_speed();
  double min_speed = this->calculate_min_speed();

  return this->calculate_speed_next_step(current_position, raw_instruction, distance_to_target, max_speed, min_speed);
}


Vector DeaccelerationController::shorten_for_deacceleration(const Vector current_position,
                                                            const Vector raw_instruction) const {
  // assert_for_NaNs(this->next_logical_step_offset_vector_);
  // assert_for_NaNs(this->next_logical_step_);
  // assert_for_NaNs(this->acceleration_vector_);
  // assert_for_NaNs(this->offset_vector_);

  double distance_to_target = this->va_.get_distance_between_point_vectors(current_position, raw_instruction);

  if (distance_to_target < this->threshold_when_to_stop_) {
    return Vector{0, 0, 0};
  } else {
    bool near_to_target_start_deaccelerating = distance_to_target < this->threshold_deaccelerate_;
    if (near_to_target_start_deaccelerating) {
      Vector shortened_vector = this->offset_vector_;
      double next_speed = this->calculate_speed_next_step_wrapper(current_position, raw_instruction);
      return shortened_vector.normalize().scale(next_speed);
    } else {
      return this->offset_vector_;
    }
  }
}
