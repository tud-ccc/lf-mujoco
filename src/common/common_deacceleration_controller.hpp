#ifndef common_deacceleration_controller
#define common_deacceleration_controller

#include "common_vector.hpp"

class DeaccelerationController {

private:
  Vector next_logical_step_offset_vector_;
  Vector next_logical_step_;
  Vector acceleration_vector_;
  Vector offset_vector_;

  VectorArithmetics va_;
  double max_step_length_;
  double threshold_deaccelerate_;
  double calculate_speed_next_step_wrapper(const Vector current_position, const Vector raw_instruction) const;
  double calculate_speed_next_step(const Vector current_position, const Vector raw_instruction,
                                   const double distance_to_target, const double max_speed, const double min_speed)const;
  double calculate_speed_selected_by_PID(const double distance_to_target) const;
  double calculate_max_speed() const;
  double calculate_min_speed() const;
  bool alpha_greater_than_threshold() const;
  double calclulate_min_speed_with_right_angled_triangle() const;

public:
  DeaccelerationController(const double max_step_length, const double threshold_deaccelerate);

  void set_next_logical_step_offset_vector(Vector next_logical_step_offset_vector);
  void set_next_logical_step(Vector next_logical_step);
  void set_acceleration_vector(Vector acceleration_vector);
  void set_offset_vector(Vector offset_vector);

  Vector get_next_logical_step_offset_vector() const;
  Vector get_next_logical_step() const;
  Vector get_acceleration_vector() const;
  Vector get_offset_vector() const;

  void print_all_collected_vectors() const;
  Vector shorten_for_deacceleration(const Vector current_position, const Vector raw_instruction) const;
};

#endif
