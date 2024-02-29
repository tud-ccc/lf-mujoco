#ifndef COMMON_VECTOR
#define COMMON_VECTOR

#include <iostream>

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


#endif //SANITY_CHECKER