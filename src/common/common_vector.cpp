#include "common_vector.hpp"
#include "common_error_handling.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#define PI 3.14159265

Vector::Vector(double X, double Y, double Z) {
  this->X_ = X;
  this->Y_ = Y;
  this->Z_ = Z;
}

bool Vector::is_NULL_vector() const {
  assert_for_NaNs(*this);
  return (this->X_ == 0 && this->Y_ == 0 && this->Z_ == 0);
}

bool Vector::equals(Vector vec) const {
  assert_for_NaNs(*this, vec);
  return (this->X_ == vec.X_ && this->Y_ == vec.Y_ && this->Z_ == vec.Z_);
}

void Vector::to_string_id() const {
  std::cout << "X : " << typeid(this->X_).name() << "(" << this->X_ << "), Y : " << typeid(this->Y_).name() << "("
            << this->Y_ << "), Z: " << typeid(this->Z_).name() << "(" << this->Z_ << ")" << std::endl;
}

void Vector::to_string() const {
  std::cout << "X : " << this->X_ << ", Y : " << this->Y_ << ", Z: " << this->Z_ << std::endl;
}

void Position::to_string() const {
  Vector coord = this->get_coordinates();
  Vector roll_pitch_yaw = this->get_coordinates();
  std::cout << "X: " << coord.X_ << ", Y: " << coord.Y_ << ", Z: " << coord.Z_ << ", Roll: " << roll_pitch_yaw.X_
            << ", Pitch: " << roll_pitch_yaw.Y_ << ", Yaw: " << roll_pitch_yaw.Z_ << std::endl;
}

std::vector<std::string> Vector::to_vector() const {
  std::vector<std::string> vector_of_x_Y_Z;
  vector_of_x_Y_Z.push_back(std::to_string(this->X_));
  vector_of_x_Y_Z.push_back(std::to_string(this->Y_));
  vector_of_x_Y_Z.push_back(std::to_string(this->Z_));
  return vector_of_x_Y_Z;
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

Vector Vector::modulo_angles() const {
  double x = this->X_;
  double y = this->Y_;
  double z = this->Z_;
  if (x > 180) {
    x -= 360;
  } else if (x < -180) {
    x += 360;
  }
  if (y > 180) {
    y -= 360;
  } else if (y < -180) {
    y += 360;
  }
  if (z > 180) {
    z -= 360;
  } else if (z < -180) {
    z += 360;
  }
  return Vector(x, y, z);
}

double VectorArithmetics::get_distance_between_point_vectors(Vector vec1, Vector vec2) const {
  assert_for_NaNs(vec1, vec2);
  double diff_sq_X = pow(vec1.X_ - vec2.X_, 2);
  double diff_sq_Y = pow(vec1.Y_ - vec2.Y_, 2);
  double diff_sq_Z = pow(vec1.Z_ - vec2.Z_, 2);
  return sqrt(diff_sq_X + diff_sq_Y + diff_sq_Z);
}

double VectorArithmetics::get_dot_product(Vector vec1, Vector vec2) const {
  assert_for_NaNs(vec1, vec2);
  return vec1.X_ * vec2.X_ + vec1.Y_ * vec2.Y_ + vec1.Z_ * vec2.Z_;
}
double VectorArithmetics::get_angle_in_degree(Vector vec1, Vector vec2) const {
  assert_for_NaNs(vec1, vec2);
  assert(!(vec1.X_ == 0 && vec1.Y_ == 0 && vec1.Z_ == 0));
  assert(!(vec2.X_ == 0 && vec2.Y_ == 0 && vec2.Z_ == 0));
  if (this->linear_dependent(vec1, vec2)) {
    return 0;
  }
  double cos_alpha = this->get_dot_product(vec1, vec2) / (vec1.get_arithmetic_length() * vec2.get_arithmetic_length());
  if (cos_alpha > 1) {
    std::cout << "cos_alpha > 0" << cos_alpha << std::endl;
    return 0;
  }
  double angle_in_radians = acos(cos_alpha);
  return angle_in_radians * 180 / PI;
}

double VectorArithmetics::get_angle_in_radians(Vector vec1, Vector vec2) const {
  assert_for_NaNs(vec1, vec2);
  assert(!(vec1.X_ == 0 && vec1.Y_ == 0 && vec1.Z_ == 0));
  assert(!(vec2.X_ == 0 && vec2.Y_ == 0 && vec2.Z_ == 0));
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

bool VectorArithmetics::linear_dependent(Vector vec1, Vector vec2) const {
  assert_for_NaNs(vec1, vec2);
  return !((vec1.X_ * vec2.Y_ - vec2.X_ * vec1.Y_) != 0 || (vec1.X_ * vec2.Z_ - vec2.X_ * vec1.Z_) != 0 ||
           (vec1.Y_ * vec2.Z_ - vec2.Y_ * vec1.Z_) != 0);
}

Vector VectorArithmetics::get_delta_vector(Vector vec1, Vector vec2) const {
  assert_for_NaNs(vec1, vec2);
  return Vector(vec2.X_ - vec1.X_, vec2.Y_ - vec1.Y_, vec2.Z_ - vec1.Z_);
}

Vector VectorArithmetics::get_normalized_delta_vector(Vector vec1, Vector vec2) const {
  assert_for_NaNs(vec1, vec2);
  Vector delta_vec = this->get_delta_vector(vec1, vec2);
  return delta_vec.normalize();
}

Vector VectorArithmetics::add_vectors(Vector vec1, Vector vec2) const {
  assert_for_NaNs(vec1, vec2);
  return Vector(vec1.X_ + vec2.X_, vec1.Y_ + vec2.Y_, vec1.Z_ + vec2.Z_);
}
