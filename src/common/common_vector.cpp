#include "common_vector.hpp"
#include "common_error_handling.hpp"
#include <cmath>
#include <iostream>

#define PI 3.14159265

Vector::Vector(double X, double Y, double Z) {
  this->X_ = X;
  this->Y_ = Y;
  this->Z_ = Z;
}

bool Vector::equals(Vector vec) const{
  assert_for_NaNs(*this, vec);
  return (this->X_ == vec.X_ && this->Y_ == vec.Y_ && this->Z_ == vec.Z_);
}

void Vector::to_string_id() const{
  std::cout << "X : " << typeid(this->X_).name() << "(" << this->X_ << "), Y : " << typeid(this->Y_).name() << "("
            << this->Y_ << "), Z: " << typeid(this->Z_).name() << "(" << this->Z_ << ")" << std::endl;
}

void Vector::to_string() const {
  std::cout << "X : " << this->X_ << ", Y : " << this->Y_ << ", Z: " << this->Z_ << std::endl;
}

double Vector::get_arithmetic_length() const {
  assert_for_NaNs(*this);
  double sq_X = pow(this->X_, 2);
  double sq_Y = pow(this->Y_, 2);
  double sq_Z = pow(this->Z_, 2);
  return sqrt(sq_X + sq_Y + sq_Z);
}

Vector Vector::normalize() const {
  assert_for_NaNs(*this);
  assert_for_null_vector(*this);
  return this->scale((1.0 / this->get_arithmetic_length()));
}

Vector Vector::scale(double scalar) const {
  assert_for_NaNs(*this);
  assert_for_NaNs(scalar);
  return Vector(this->X_ * scalar, this->Y_ * scalar, this->Z_ * scalar);
}

double VectorArithmetics::get_distance_between_point_vectors(Vector vec1, Vector vec2) {
  assert_for_NaNs(vec1, vec2);
  double diff_sq_X = pow(vec1.X_ - vec2.X_, 2);
  double diff_sq_Y = pow(vec1.Y_ - vec2.Y_, 2);
  double diff_sq_Z = pow(vec1.Z_ - vec2.Z_, 2);
  return sqrt(diff_sq_X + diff_sq_Y + diff_sq_Z);
}

double VectorArithmetics::get_dot_product(Vector vec1, Vector vec2) {
  assert_for_NaNs(vec1, vec2);
  return vec1.X_ * vec2.X_ + vec1.Y_ * vec2.Y_ + vec1.Z_ * vec2.Z_;
}
double VectorArithmetics::get_angle_in_degree(Vector vec1, Vector vec2) {
  assert_for_NaNs(vec1);
  assert_for_null_vector(vec1, vec2);
  double angle_in_degree =
      acos(this->get_dot_product(vec1, vec2) / (vec1.get_arithmetic_length() * vec2.get_arithmetic_length()));
  return angle_in_degree * 180 / PI;
}

double VectorArithmetics::get_angle_in_radians(Vector vec1, Vector vec2) {
  assert_for_NaNs(vec1, vec2);
  assert_for_null_vector(vec1, vec2);
  if (this->linear_dependent(vec1, vec2)) {
    return 0;
  }
  double cos_alpha = this->get_dot_product(vec1, vec2) / (vec1.get_arithmetic_length() * vec2.get_arithmetic_length());
  if (cos_alpha > 1) {
    std::cout << "cos_alpha > 0" << cos_alpha << std::endl;
    return 0;
  }
  double angle_in_radians = acos(cos_alpha);
  return angle_in_radians;
}

bool VectorArithmetics::linear_dependent(Vector vec1, Vector vec2) {
  assert_for_NaNs(vec1, vec2);
  return !((vec1.X_ * vec2.Y_ - vec2.X_ * vec1.Y_) != 0 || (vec1.X_ * vec2.Z_ - vec2.X_ * vec1.Z_) != 0 ||
           (vec1.Y_ * vec2.Z_ - vec2.Y_ * vec1.Z_) != 0);
}

Vector VectorArithmetics::get_delta_vector(Vector vec1, Vector vec2) {
  assert_for_NaNs(vec1, vec2);
  return Vector(vec2.X_ - vec1.X_, vec2.Y_ - vec1.Y_, vec2.Z_ - vec1.Z_);
}

Vector VectorArithmetics::get_normalized_delta_vector(Vector vec1, Vector vec2) {
  assert_for_NaNs(vec1, vec2);
  Vector delta_vec = this->get_delta_vector(vec1, vec2);
  return delta_vec.normalize();
}

Vector VectorArithmetics::add_vectors(Vector vec1, Vector vec2) {
  assert_for_NaNs(vec1, vec2);
  return Vector(vec1.X_ + vec2.X_, vec1.Y_ + vec2.Y_, vec1.Z_ + vec2.Z_);
}
