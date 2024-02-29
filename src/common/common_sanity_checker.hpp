#ifndef COMMON_SANITY_CHECKER
#define COMMON_SANITY_CHECKER


#include <iostream>
#include <cmath>


class Vector {     

  public:             
    double X_;         
    double Y_;
    double Z_;
 
    Vector(){};  
    Vector(double X, double Y, double Z ) {
    this->X_ = X;
    this->Y_ = Y;
    this->Z_ = Z;
    }


    bool equals(Vector vec){
        return (this->X_ == vec.X_ && this->Y_ == vec.Y_&& this->Z_ == vec.Z_);
    }

    void to_string_id(){
        std::cout << "X : "<< typeid(this->X_).name() <<"(" <<this->X_ << "), Y : " << typeid(this->Y_).name() <<"(" <<this->Y_ << "), Z: " << typeid(this->Z_).name() <<"(" <<this->Z_ << ")" << std::endl;
    }

    void to_string(){
        std::cout << "X : " << this->X_ << ", Y : " <<this->Y_ << ", Z: " <<this->Z_ << std::endl;
    }

};

class PositionEvaluator {
  public : 
    Vector current_position_;
    Vector raw_instruction_;

    PositionEvaluator(){};

    PositionEvaluator(
        Vector current_position,
        Vector raw_instruction)
        {
        this->current_position_ = current_position;
        this->raw_instruction_ = raw_instruction;
        }

    double calculate_distance_two_points()
        {
        double diff_sq_X = pow(this->current_position_.X_ - this->raw_instruction_.X_, 2) ;
        double diff_sq_Y = pow(this->current_position_.Y_ - this->raw_instruction_.Y_, 2);
        double diff_sq_Z = pow(this->current_position_.Z_ - this->raw_instruction_.Z_, 2);
        return sqrt(diff_sq_X + diff_sq_Y + diff_sq_Z);
        }

    Vector get_delta_vector()
        {
        return Vector(
            this->raw_instruction_.X_- this->current_position_.X_,
            this->raw_instruction_.Y_- this->current_position_.Y_,
            this->raw_instruction_.Z_- this->current_position_.Z_
        );
        }

    double get_vector_length(
        Vector vec)
        {
            double sq_X = pow(vec.X_, 2);
            double sq_Y = pow(vec.Y_, 2);
            double sq_Z = pow(vec.Z_, 2);
            return sqrt(sq_X + sq_Y + sq_Z);
        }

    Vector scalar_product( double scalar, Vector vec)
        {
            return Vector(
            vec.X_ * scalar,
            vec.Y_ * scalar,
            vec.Z_ * scalar);

        }

    Vector get_normalized_delta_vector()
        {
        Vector delta_vec = this->get_delta_vector();
        return scalar_product(1.0 / get_vector_length(delta_vec), delta_vec );
        }

    Vector add_vectors(
        Vector vec_1,
        Vector vec_2)
        {
        return Vector( vec_1.X_ + vec_2.X_, vec_1.Y_ + vec_2.Y_, vec_1.Z_ + vec_2.Z_ );
        }

    Vector calculate_checked_instruction(){
        if(this->calculate_distance_two_points() < 2 ){
            return this->raw_instruction_;
        }
        else {
            return this->add_vectors(
            this->current_position_,
            this->scalar_product(2 ,this->get_normalized_delta_vector()));
        }
    }

};


#endif //SANITY_CHECKER