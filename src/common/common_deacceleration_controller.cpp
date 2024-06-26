
#include "common_deacceleration_controller.hpp"
#include "common_vector.hpp"
// #include "common_error_handling.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

DeaccelerationController::DeaccelerationController(const double max_step_length, const double threshold_deaccelerate,
                                                   const double threshold_near_target_stop_moving,
                                                   const double acceleration_cap) {

  this->max_step_length_ = max_step_length;
  this->threshold_deaccelerate_ = threshold_deaccelerate;
  this->threshold_near_target_stop_moving_ = threshold_near_target_stop_moving;
  this->acceleration_cap_ = acceleration_cap;

  this->va_ = VectorArithmetics{};
  this->vcvc_ = VectorCollectionVelocityControl{};
};

double DeaccelerationController::calclulate_min_speed_with_law_of_sines(const Vector next_logical_step_offset_vector,
                                                                        const Vector acceleration_vector,
                                                                        const Vector offset_vector) const {

  double a = acceleration_vector.get_arithmetic_length();
  // std::cout << "a :  " << a << std::endl;

  double alpha = this->va_.get_angle_in_radians(next_logical_step_offset_vector, offset_vector);
  // std::cout << "alpha :  " << alpha << std::endl;

  double c = next_logical_step_offset_vector.get_arithmetic_length();
  // std::cout << "b :  " << b << std::endl;

  double gamma = asin((sin(alpha) * c) / a);
  gamma = PI - gamma;
  // std::cout << "beta : " << beta << std::endl;

  double beta = PI - gamma - alpha;
  // std::cout << "gamma : " << gamma << std::endl;

  double b = (a * sin(beta)) / sin(alpha);
  // std::cout << "c : " << c << std::endl;

  if (std::isnan(c)) {
    std::cout << "Next_logical_step_offset_vector:";
    next_logical_step_offset_vector.to_string();
    std::cout << "Offset_vector:";
    offset_vector.to_string();
    std::cout << "a :  " << a << std::endl;
    std::cout << "alpha :  " << alpha << std::endl;
    std::cout << "b :  " << b << std::endl;
    std::cout << "beta : " << beta << std::endl;
    std::cout << "c : " << c << std::endl;
    std::cout << "gamma : " << gamma << std::endl;
    assert(false && "Preferred speed has a wrong value");
  }
  return b;
}

double DeaccelerationController::calculate_min_speed(const Vector next_logical_step_offset_vector,
                                                     const Vector acceleration_vector,
                                                     const Vector offset_vector) const {
  if (next_logical_step_offset_vector.is_NULL_vector())
    return 0;

  bool next_logical_step_offset_vector_shorter_than_acc_vec =
      next_logical_step_offset_vector.get_arithmetic_length() < acceleration_vector.get_arithmetic_length();

  bool nlsov_and_ov_are_ld = this->va_.linear_dependent(next_logical_step_offset_vector, offset_vector);
  bool alpha_is_less_than_1 = this->va_.get_angle_in_degree(next_logical_step_offset_vector, offset_vector) <= 1;
  bool next_logical_step_offset_vector_near_to_linear_dependent_to_acc_vec =
      nlsov_and_ov_are_ld || alpha_is_less_than_1;

  if (next_logical_step_offset_vector_shorter_than_acc_vec) {
    return 0;
  } else if (next_logical_step_offset_vector_near_to_linear_dependent_to_acc_vec) {
    return next_logical_step_offset_vector.get_arithmetic_length() - this->acceleration_cap_;
  } else {
    return this->calclulate_min_speed_with_law_of_sines(next_logical_step_offset_vector, acceleration_vector,
                                                        offset_vector);
  }
}

double DeaccelerationController::calculate_max_speed(const Vector offset_vector) const {
  double offset_vector_length = offset_vector.get_arithmetic_length();
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
  // std::cout << "Preferred_speed : " << preferred_speed << std::endl;
  // std::cout << "max_speed : " << max_speed << std::endl;
  // std::cout << "min_speed : " << min_speed << std::endl;
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
                                                                   const Vector raw_instruction,
                                                                   const Vector next_logical_step_offset_vector,
                                                                   const Vector acceleration_vector,
                                                                   const Vector offset_vector) const {
  double distance_to_target = this->va_.get_distance_between_point_vectors(current_position, raw_instruction);
  double max_speed = this->calculate_max_speed(offset_vector);
  double min_speed = this->calculate_min_speed(next_logical_step_offset_vector, acceleration_vector, offset_vector);

  return this->calculate_speed_next_step(current_position, raw_instruction, distance_to_target, max_speed, min_speed);
}

Vector DeaccelerationController::shorten_for_deacceleration(const Vector current_position, const Vector raw_instruction,
                                                            const Vector next_logical_step_offset_vector,
                                                            const Vector acceleration_vector,
                                                            const Vector offset_vector) const {
  // assert_for_NaNs(this->next_logical_step_offset_vector_);
  // assert_for_NaNs(this->next_logical_step_);
  // assert_for_NaNs(this->acceleration_vector_);
  // assert_for_NaNs(this->offset_vector_);

  double distance_to_target = this->va_.get_distance_between_point_vectors(current_position, raw_instruction);

  if (distance_to_target <= this->threshold_near_target_stop_moving_) {
    return Vector{0, 0, 0};
  } else {
    bool near_to_target_start_deaccelerating = distance_to_target < this->threshold_deaccelerate_;
    if (near_to_target_start_deaccelerating) {
      double next_speed = this->calculate_speed_next_step_wrapper(
          current_position, raw_instruction, next_logical_step_offset_vector, acceleration_vector, offset_vector);
      return offset_vector.normalize().scale(next_speed);
    } else {
      return offset_vector;
    }
  }
}

Vector DeaccelerationController::compute_next_position_with_prints(const Vector last_position,
                                                                   const Vector current_position,
                                                                   const Vector raw_instruction) {

  auto shorten_if_longer_than_max_step_length = [](Vector vec, double max_step_length) {
    if (vec.get_arithmetic_length() > max_step_length)
      return vec.normalize().scale(max_step_length);
    else {
      return vec;
    }
  };
  auto normalize_if_not_NULL_vector = [](Vector acceleration_vector, double acceleration_cap) {
    if (acceleration_vector.get_arithmetic_length() > 0) {
      return acceleration_vector.normalize().scale(acceleration_cap);
    } else {
      return acceleration_vector;
    }
  };

  std::cout << "Last position: (distance to current position: "
            << this->va_.get_distance_between_point_vectors(last_position, current_position) << ")";
  last_position.to_string();

  std::cout << "Current position: (distance to target position: "
            << this->va_.get_distance_between_point_vectors(current_position, raw_instruction) << ")";
  current_position.to_string();

  std::cout << "Raw_instruction: ";
  raw_instruction.to_string();

  Vector next_logical_step_offset_vector = this->va_.get_delta_vector(last_position, current_position);
  next_logical_step_offset_vector =
      shorten_if_longer_than_max_step_length(next_logical_step_offset_vector, this->max_step_length_);
  std::cout << "Next_logical_step_offset_vector(" << next_logical_step_offset_vector.get_arithmetic_length() << ") ";
  next_logical_step_offset_vector.to_string();

  Vector next_logical_step = this->va_.add_vectors(current_position, next_logical_step_offset_vector);
  std::cout << "Next_logical_step: ";
  next_logical_step.to_string();

  Vector acceleration_vector = this->va_.get_delta_vector(next_logical_step, raw_instruction);
  std::cout << "Acceleration_vector (unshortnened)(" << acceleration_vector.get_arithmetic_length() << ") ";
  acceleration_vector.to_string();
  acceleration_vector = normalize_if_not_NULL_vector(acceleration_vector, this->acceleration_cap_);
  std::cout << "Acceleration_vector (shortened)(" << acceleration_vector.get_arithmetic_length() << "): ";
  acceleration_vector.to_string();

  Vector offset_vector = this->va_.add_vectors(next_logical_step_offset_vector, acceleration_vector);
  std::cout << "Offset_vector (unshortened)(" << offset_vector.get_arithmetic_length() << "): ";
  offset_vector.to_string();
  offset_vector = shorten_if_longer_than_max_step_length(offset_vector, this->max_step_length_);
  std::cout << "Offset_vector (shortened)(" << offset_vector.get_arithmetic_length() << "): ";
  offset_vector.to_string();

  Vector shortened_offset_vector = this->shorten_for_deacceleration(
      current_position, raw_instruction, next_logical_step_offset_vector, acceleration_vector, offset_vector);
  std::cout << "shortened_offset_vector(" << shortened_offset_vector.get_arithmetic_length() << "): ";
  shortened_offset_vector.to_string();

  Vector next_position = this->va_.add_vectors(current_position, shortened_offset_vector);
  std::cout << "Next Position: ";
  next_position.to_string();
  std::cout << "##### ##### Finished this run ##### #####\n";

  // collect data for deug purposes
  this->vcvc_ = VectorCollectionVelocityControl{next_logical_step_offset_vector, next_logical_step, acceleration_vector,
                                                offset_vector};
  return next_position;
}

Vector DeaccelerationController::compute_next_position(const Vector last_position, const Vector current_position,
                                                       const Vector raw_instruction) {

  auto shorten_if_longer_than_max_step_length = [](Vector vec, double max_step_length) {
    if (vec.get_arithmetic_length() > max_step_length)
      return vec.normalize().scale(max_step_length);
    else {
      return vec;
    }
  };
  auto normalize_if_not_NULL_vector = [](Vector acceleration_vector, double acceleration_cap) {
    if (acceleration_vector.get_arithmetic_length() > 0) {
      return acceleration_vector.normalize().scale(acceleration_cap);
    } else {
      return acceleration_vector;
    }
  };

  Vector next_logical_step_offset_vector = this->va_.get_delta_vector(last_position, current_position);
  next_logical_step_offset_vector =
      shorten_if_longer_than_max_step_length(next_logical_step_offset_vector, this->max_step_length_);

  Vector next_logical_step = this->va_.add_vectors(current_position, next_logical_step_offset_vector);

  Vector acceleration_vector = this->va_.get_delta_vector(next_logical_step, raw_instruction);

  acceleration_vector = normalize_if_not_NULL_vector(acceleration_vector, this->acceleration_cap_);

  Vector offset_vector = this->va_.add_vectors(next_logical_step_offset_vector, acceleration_vector);

  offset_vector = shorten_if_longer_than_max_step_length(offset_vector, this->max_step_length_);

  Vector shortened_offset_vector = this->shorten_for_deacceleration(
      current_position, raw_instruction, next_logical_step_offset_vector, acceleration_vector, offset_vector);

  Vector next_position = this->va_.add_vectors(current_position, shortened_offset_vector);

  // collect data for deug purposes
  this->vcvc_ = VectorCollectionVelocityControl{next_logical_step_offset_vector, next_logical_step, acceleration_vector,
                                                offset_vector};
  return next_position;
}

Vector DeaccelerationController::compute_next_position_wrapper(const Vector last_position,
                                                               const Vector current_position,
                                                               const Vector raw_instruction) {
  return this->compute_next_position(last_position, current_position, raw_instruction);
}
