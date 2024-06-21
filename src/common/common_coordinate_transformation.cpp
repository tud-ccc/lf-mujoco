#include "common_coordinate_transformation.hpp"

double degreesToRadians(double degrees) { return degrees * (PI / 180.0); }

Eigen::Matrix3d eulerAnglesToRotationMatrix(double roll, double pitch, double yaw) {
  roll = degreesToRadians(roll);
  pitch = degreesToRadians(pitch);
  yaw = degreesToRadians(yaw);

  Eigen::Matrix3d Rx, Ry, Rz;
  Rx << 1, 0, 0, 0, cos(roll), -sin(roll), 0, sin(roll), cos(roll);
  Ry << cos(pitch), 0, sin(pitch), 0, 1, 0, -sin(pitch), 0, cos(pitch);
  Rz << cos(yaw), -sin(yaw), 0, sin(yaw), cos(yaw), 0, 0, 0, 1;
  return Rz * Ry * Rx;
}

Vector TransformationArithmetics::transform_coordinates(Vector pos_by_cam, Vector end_eff_pos, double trgt_roll,
                                                        double trgt_pitch, double trgt_yaw) {
  Eigen::Vector3d coordinates_by_camera;
  coordinates_by_camera << pos_by_cam.X_ * 1000, pos_by_cam.Y_ * 1000, pos_by_cam.Z_ * 1000;
  Eigen::Vector3d coordinates_of_end_effector;
  coordinates_of_end_effector << end_eff_pos.X_, end_eff_pos.Y_, end_eff_pos.Z_;
  Eigen::Matrix3d R = eulerAnglesToRotationMatrix(trgt_roll, trgt_pitch, trgt_yaw);
  Eigen::Vector3d world_coords = R * coordinates_by_camera + coordinates_of_end_effector;
  return Vector{world_coords.x(), world_coords.y(), world_coords.z()};
}

Vector TransformationArithmetics::transform_coordinates_wrapper(Vector pos_by_cam, Position cur_pos) {
  Vector end_eff_coords = cur_pos.get_coordinates();
  Vector end_eff_roll_pitch_yaw = cur_pos.get_roll_pitch_yaw();

  double trgt_roll = end_eff_roll_pitch_yaw.X_;
  double trgt_pitch = end_eff_roll_pitch_yaw.Y_;
  double trgt_yaw = end_eff_roll_pitch_yaw.Z_;
  return transform_coordinates(pos_by_cam, end_eff_coords, trgt_roll, trgt_pitch , trgt_yaw);
}


