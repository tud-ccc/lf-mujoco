#ifndef COMMON_VECTOR
#define COMMON_VECTOR

#include <vector>
#include <string>
#define PI 3.14159265

class Vector {

public:
  double X_;
  double Y_;
  double Z_;

  Vector(){};
  Vector(double X, double Y, double Z);

  bool equals(Vector vec) const;
  void to_string_id() const;
  void to_string() const;
  std::vector<std::string> to_vector() const;
  bool is_NULL_vector() const;

  double get_arithmetic_length() const;
  Vector normalize() const;
  Vector scale(double scalar) const;
};

class VectorArithmetics {

public:
  VectorArithmetics(){};

  double get_distance_between_point_vectors(Vector vec1, Vector vec2) const;
  double get_dot_product(Vector vec1, Vector vec2) const;
  double get_angle_in_degree(Vector vec1, Vector vec2) const;
  double get_angle_in_radians(Vector vec1, Vector vec2) const;
  Vector get_delta_vector(Vector vec1, Vector vec2) const;
  Vector get_normalized_delta_vector(Vector vec1, Vector vec2) const;
  Vector add_vectors(Vector vec1, Vector vec2) const;
  bool linear_dependent(Vector vec1, Vector vec2) const;
};

class VectorCollectionVelocityControl {
private:
  Vector next_logical_step_offset_vector_;
  Vector next_logical_step_;
  Vector acceleration_vector_;
  Vector offset_vector_;

public:
  VectorCollectionVelocityControl() {}
  VectorCollectionVelocityControl(Vector next_logical_step_offset_vector, Vector next_logical_step,
                                  Vector acceleration_vector, Vector offset_vector) {
    this->next_logical_step_offset_vector_ = next_logical_step_offset_vector;
    this->next_logical_step_ = next_logical_step;
    this->acceleration_vector_ = acceleration_vector;
    this->offset_vector_ = offset_vector;
  }

  Vector get_next_logical_step_offset_vector() const { return this->next_logical_step_offset_vector_; }
  Vector get_next_logical_step() const { return this->next_logical_step_; }
  Vector get_acceleration_vector() const { return this->acceleration_vector_; }
  Vector get_offset_vector() const { return this->offset_vector_; }
};

#endif // COMMON_VECTOR