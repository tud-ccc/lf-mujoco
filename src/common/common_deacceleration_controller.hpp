#ifndef common_deacceleration_controller
#define common_deacceleration_controller

#include "common_vector.hpp"

class DeaccelerationController {

private:
  VectorArithmetics va_;

  double max_step_length_;
  double threshold_deaccelerate_;
  double threshold_when_to_stop_;
  double acceleration_cap_;
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

public:
  DeaccelerationController(const double max_step_length, const double threshold_deaccelerate,
                           const double threshold_when_to_stop, const double acceleration_cap);

  Vector get_next_logical_step_offset_vector() const;
  Vector get_next_logical_step() const;
  Vector get_acceleration_vector() const;
  Vector get_offset_vector() const;

  void print_all_collected_vectors() const;
  Vector compute_next_position(const last_positionq, const Vector current_position, const Vector raw_instruction);
};

#endif
