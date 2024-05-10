#ifndef COMMON_POSITION_EVALUATOR
#define COMMON_POSITION_EVALUATOR

#include "common_vector.hpp"

class PositionEvaluator {
private:
  double acceleration_cap_;
  double max_step_length_;

  VectorArithmetics va_;

  Vector shorten_if_longer_than_max_stop_length(Vector next_logical_step_offset_vector) const;
  Vector calculate_offset_vector_given_max_acceleration(Vector last_position, Vector current_position,
                                                        Vector raw_instruction) const;

public:
  PositionEvaluator(){};

  PositionEvaluator(double acceleration_cap, double max_step_length);

  Vector calculate_next_position(Vector last_position, Vector current_position, Vector raw_instruction) const;
};

#endif // COMMON_POSITION_EVALUATOR