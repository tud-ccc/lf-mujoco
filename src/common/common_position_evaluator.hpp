#ifndef COMMON_POSITION_EVALUATOR
#define COMMON_POSITION_EVALUATOR

#include "common_vector.hpp"

class PositionEvaluator {
private:
  double acceleration_cap_;
  VectorArithmetics va_;

public:
  PositionEvaluator(){};

  PositionEvaluator(double acceleration_cap);
  Vector calculate_offset_vector_given_max_acceleration(Vector last_position, Vector current_position,
                                                        Vector raw_instruction, double acceleration_length);

  Vector calculate_next_position(Vector last_position, Vector current_position, Vector raw_instruction);
};

#endif // COMMON_POSITION_EVALUATOR