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
  void print_all_collected_vectors();
  double calculate_deacceleration_maximum();
  bool decide_trimming();
  double calculate_speed_next_step(const Vector current_position, const Vector raw_instruction,
                                   const Vector offset_vector);

public:
  DeaccelerationController(double max_step_length, double threshold_deaccelerate);

  void set_next_logical_step_offset_vector(Vector next_logical_step_offset_vector);
  void set_next_logical_step(Vector next_logical_step);
  void set_acceleration_vector(Vector acceleration_vector);
  void set_offset_vector(Vector offset_vector);

  Vector get_next_logical_step_offset_vector();
  Vector get_next_logical_step();
  Vector get_acceleration_vector();
  Vector get_offset_vector();

  Vector shorten_for_deacceleration(const Vector current_position, const Vector raw_instruction);
};

#endif