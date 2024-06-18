#ifndef CORRDINATE_TRAFO_HPP
#define CORRDINATE_TRAFO_HPP

#include "common_vector.hpp"
#include <Eigen/Dense>

class TransformationArithmetics {
private:
public:
  TransformationArithmetics(){};

  Eigen::Matrix3d forward_get_rotation_matrix(double roll, double pitch, double yaw) {

    double roll_in_rad = roll * M_PI / 180.0;
    double pitch_in_rad = pitch * M_PI / 180.0;
    double yaw_in_rad = yaw * M_PI / 180.0;

    // Create an AngleAxis rotation vector and convert it to a rotation matrix

    Eigen::AngleAxisd rollAngle(roll_in_rad, Eigen::Vector3d::UnitZ());
    Eigen::AngleAxisd yawAngle(pitch_in_rad, Eigen::Vector3d::UnitY());
    Eigen::AngleAxisd pitchAngle(yaw_in_rad, Eigen::Vector3d::UnitX());

    Eigen::Quaternion<double> q = rollAngle * yawAngle * pitchAngle;

    Eigen::Matrix3d rotationMatrix = q.matrix();
    return rotationMatrix;
  };

  Eigen::Vector3d compute_respective_point_in_coordinate_system(Eigen::Vector3d src_vec3d_coordinates,
                                                                Eigen::Vector3d trgt_vec3d_offset_from_origin,
                                                                double trgt_roll, double trgt_pitch, double trgt_yaw) {
    Eigen::Matrix3d inv_rot_matrix3d = forward_get_rotation_matrix(trgt_roll, trgt_pitch, trgt_yaw);
    Eigen::Vector3d src_vec3d_coordinates_applied_inverse_apply_trans =
        src_vec3d_coordinates + trgt_vec3d_offset_from_origin;
    Eigen::Vector3d src_vec3d_coordinates_applied_inverse_apply_rot =
        inv_rot_matrix3d * src_vec3d_coordinates_applied_inverse_apply_trans;
    return src_vec3d_coordinates_applied_inverse_apply_rot;
  }

  Vector compute_respective_point_in_coordinate_system_align(Vector src_vec_coordinates,
                                                             Vector trgt_vec_offset_from_origin,
                                                             double trgt_roll_not_aligned,
                                                             double trgt_pitch_not_aligned,
                                                             double trgt_yaw_not_aligned) {
    double trgt_roll = trgt_yaw_not_aligned;
    double trgt_pitch = trgt_pitch_not_aligned;
    double trgt_yaw = trgt_roll_not_aligned;

    Eigen::Vector3d src_vec3d_coordinates;

    src_vec3d_coordinates << src_vec_coordinates.X_, src_vec_coordinates.Y_, src_vec_coordinates.Z_;

    Eigen::Vector3d trgt_vec3d_offset_from_origin;

    trgt_vec3d_offset_from_origin << trgt_vec_offset_from_origin.X_, trgt_vec_offset_from_origin.Y_,
        trgt_vec_offset_from_origin.Z_;

    Eigen::Vector3d resp_point = compute_respective_point_in_coordinate_system(
        src_vec3d_coordinates, trgt_vec3d_offset_from_origin, trgt_roll, trgt_pitch, trgt_yaw);
    Vector vec_position_in_xArm_coordinate_system = Vector{resp_point.x(), resp_point.y(), resp_point.z()};
    return vec_position_in_xArm_coordinate_system;
  }
  Vector compute_respective_point_in_coordinate_system_wrapper(Vector src_vec_coordinates, Position current_position) {
    Vector roll_pitch_yaw = current_position.get_roll_pitch_yaw();
    return compute_respective_point_in_coordinate_system_align(src_vec_coordinates, current_position.get_coordinates(),
                                                               roll_pitch_yaw.X_, roll_pitch_yaw.Y_, roll_pitch_yaw.Z_);
  }
};

#endif
