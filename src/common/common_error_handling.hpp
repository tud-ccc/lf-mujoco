#ifndef COMMON_ERROR_HANDLING
#define COMMON_ERROR_HANDLING

#include "common_vector.hpp"
#include <cassert>
#include <cmath>

void assert_for_NULLs(Vector vec1, Vector vec2) {
  assert(!std::isnan(vec1.X_) && !std::isnan(vec1.Y_) && !std::isnan(vec1.Z_));
  assert(!std::isnan(vec2.X_) && !std::isnan(vec2.Y_) && !std::isnan(vec2.Z_));
  return;
}

void assert_for_NULLs(Vector vec) {
  assert(!std::isnan(vec.X_) && !std::isnan(vec.Y_) && !std::isnan(vec.Z_));
  return;
}

void assert_for_NULLs(double value) {
  assert(!std::isnan(value));
  return;
}

void assert_for_null_vector(Vector vec){
  assert(vec.X_ == 0 && vec.Y_ == 0 && vec.Z_ == 0);
}

void assert_for_null_vector(Vector vec1 , Vector vec2){
  assert(vec1.X_ == 0 && vec1.Y_ == 0 && vec1.Z_ == 0);
  assert(vec2.X_ == 0 && vec2.Y_ == 0 && vec2.Z_ == 0);
}

#endif // COMMON_VECTOR