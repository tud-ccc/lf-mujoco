#ifndef COMMON_SAN_CHECK
#define COMMON_SAN_CHECK

#include "common_vector.hpp"

class SanityCheckerLogic {

private:
  double max_step_length_;
  double acceleration_cap_;
  VectorArithmetics va_;

public:

  SanityCheckerLogic(double acceleration_cap, double max_step_length );
  int planner_sanity_check_next_position(Vector last_position, Vector current_position, Vector next_position);
};

#endif
