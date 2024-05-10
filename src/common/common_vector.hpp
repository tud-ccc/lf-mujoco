#ifndef COMMON_VECTOR
#define COMMON_VECTOR

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

  double get_arithmetic_length() const;
  Vector normalize() const;
  Vector scale(double scalar) const;
};

class VectorArithmetics {

public:
  double X_;
  double Y_;
  double Z_;

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

  class PositionTriplet {
  private:
    Vector last_position_;
    Vector current_position_;
    Vector raw_instruction_;

  public:
    PositionTriplet(Vector last_position, Vector current_position, Vector raw_instruction) {
      this->last_position_ = last_position;
      this->current_position_ = current_position;
      this->raw_instruction_ = raw_instruction;
    }

    Vector get_last_position() { return this->last_position_; }
    Vector get_current_position() { return this->current_position_; }
    Vector get_raw_instruction() { return this->raw_instruction_; }
  };

#endif // COMMON_VECTOR