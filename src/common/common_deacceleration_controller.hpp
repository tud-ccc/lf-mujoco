#ifndef COMMON_DEACC_CONTROLLER
#define COMMON_DEACC_CONTROLLER

#include "common_vector.hpp"

class DeaccelerationController {

private:
  VectorArithmetics va_;

  double max_step_length_;
  double threshold_deaccelerate_;
  double threshold_near_target_stop_moving_;
  double acceleration_cap_;

  VectorCollectionVelocityControl vcvc_;
  double calculate_speed_next_step_wrapper(const Vector current_position, const Vector raw_instruction,
                                           const Vector next_logical_step_offset_vector,
                                           const Vector acceleration_vector, const Vector offset_vector) const;
  double calculate_speed_next_step(const Vector current_position, const Vector raw_instruction,
                                   const double distance_to_target, const double max_speed,
                                   const double min_speed) const;
  double calculate_speed_selected_by_PID(const double distance_to_target) const;
  double calculate_max_speed(const Vector offset_vector) const;
  double calculate_min_speed(const Vector next_logical_step_offset_vector, const Vector acceleration_vector,
                             const Vector offset_vector) const;
  double calclulate_min_speed_with_law_of_sines(const Vector next_logical_step_offset_vector,
                                                const Vector acceleration_vector, const Vector offset_vector) const;
  Vector shorten_for_deacceleration(const Vector current_position, const Vector raw_instruction,
                                    const Vector next_logical_step_offset_vector, const Vector acceleration_vector,
                                    const Vector offset_vector) const;
  Vector compute_next_position_with_prints(const Vector last_position, const Vector current_position, const Vector raw_instruction);
  Vector compute_next_position(const Vector last_position, const Vector current_position, const Vector raw_instruction);

public:
  DeaccelerationController(const double max_step_length, const double threshold_deaccelerate,
                           const double threshold_near_target_stop_moving, const double acceleration_cap);

  VectorCollectionVelocityControl get_vector_collection_velocity_control() const { return this->vcvc_; };

  Vector compute_next_position_wrapper(const Vector last_position, const Vector current_position, const Vector raw_instruction);
};

#endif
