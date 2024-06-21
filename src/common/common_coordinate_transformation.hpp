#ifndef COMMON_COORDIANTE_TRANSFORMATION
#define COMMON_COORDIANTE_TRANSFORMATION

#include "common_vector.hpp"
#include <Eigen/Dense>
#include <cmath>
#include <iostream>

Eigen::Matrix3d eulerAnglesToRotationMatrix(double roll, double pitch, double yaw); 

class TransformationArithmetics
{
private:
public:
    TransformationArithmetics(){};

    Vector transform_coordinates(Vector pos_by_cam,
                        Vector end_eff_pos,
                        double trgt_roll,
                        double trgt_pitch,
                        double trgt_yaw);
    Vector transform_coordinates_wrapper(Vector pos_by_cam, Position cur_pos);
    Vector sanity_check_coordinates(Vector coord) ;
}

;

#endif
