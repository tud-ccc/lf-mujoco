#ifndef COMMON_VECTOR
#define COMMON_VECTOR

#include <iostream>
#include <cmath>

#define PI 3.14159265

class Vector
{

public:
    double X_;
    double Y_;
    double Z_;

    Vector(){};
    Vector(double X, double Y, double Z)
    {
        this->X_ = X;
        this->Y_ = Y;
        this->Z_ = Z;
    }

    bool equals(Vector vec)
    {
        return (this->X_ == vec.X_ && this->Y_ == vec.Y_ && this->Z_ == vec.Z_);
    }

    void to_string_id()
    {
        std::cout << "X : " << typeid(this->X_).name() << "(" << this->X_ << "), Y : " << typeid(this->Y_).name() << "(" << this->Y_ << "), Z: " << typeid(this->Z_).name() << "(" << this->Z_ << ")" << std::endl;
    }

    void to_string()
    {
        std::cout << "X : " << this->X_ << ", Y : " << this->Y_ << ", Z: " << this->Z_ << std::endl;
    }

    double get_arithmetic_length()
    {
        double sq_X = pow(this->X_, 2);
        double sq_Y = pow(this->Y_, 2);
        double sq_Z = pow(this->Z_, 2);
        return sqrt(sq_X + sq_Y + sq_Z);
    }

    Vector normalize()
    {
        return this->scale((1.0 / this->get_arithmetic_length()));
    }

    Vector scale(double scalar)
    {
        return Vector(
            this->X_ * scalar,
            this->Y_ * scalar,
            this->Z_ * scalar);
    }
};

class VectorArithmetics
{

public:
    double X_;
    double Y_;
    double Z_;

    VectorArithmetics(){};

    double get_distance_between_point_vectors(Vector vec1, Vector vec2)
    {
        double diff_sq_X = pow(vec1.X_ - vec2.X_, 2);
        double diff_sq_Y = pow(vec1.Y_ - vec2.Y_, 2);
        double diff_sq_Z = pow(vec1.Z_ - vec2.Z_, 2);
        return sqrt(diff_sq_X + diff_sq_Y + diff_sq_Z);
    }

    double get_dot_product(Vector vec1, Vector vec2)
    {
        return vec1.X_ * vec2.X_ + vec1.Y_ * vec2.Y_ + vec1.Z_ * vec2.Z_;
    }
    double get_angle(Vector vec1, Vector vec2)
    {
        double angle_in_degree = acos(this->get_dot_product(vec1, vec2) / (vec1.get_arithmetic_length() * vec2.get_arithmetic_length()));
        return angle_in_degree * 180 / PI;
    }

    Vector get_delta_vector(Vector vec1, Vector vec2)
    {
        return Vector(
            vec2.X_ - vec1.X_,
            vec2.Y_ - vec1.Y_,
            vec2.Z_ - vec1.Z_);
    }

    Vector get_normalized_delta_vector(Vector vec1, Vector vec2)
    {
        Vector delta_vec = this->get_delta_vector(vec1, vec2);
        return delta_vec.normalize();
    }

    Vector add_vectors(Vector vec1,
                       Vector vec2)
    {
        return Vector(vec1.X_ + vec2.X_,
                      vec1.Y_ + vec2.Y_,
                      vec1.Z_ + vec2.Z_);
    }

 


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