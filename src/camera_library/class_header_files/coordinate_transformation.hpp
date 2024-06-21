#ifndef SECND_TRY_COORD_TRANS
#define SECND_TRY_COORD_TRANS

#include "common_vector.hpp"
#include <Eigen/Dense>
#include <cmath>
#include <iostream>

class CoordinateTransformations {
private:
public:
  CoordinateTransformations(){};

  double degreesToRadians(double degrees) { return degrees * (PI / 180.0); }

  Eigen::Matrix4f createTransformationMatrix(float roll, float pitch, float yaw, Eigen::Vector3f translation) {

    roll = degreesToRadians(roll);
    pitch = degreesToRadians(pitch);
    yaw = degreesToRadians(yaw);

    Eigen::Matrix3f Pre_rot, R_x, R_y, R_z;

    // Rotationsmatrix um die X-Achse (Roll)
    R_x << 1, 0, 0, 0, cos(roll), -sin(roll), 0, sin(roll), cos(roll);

    // Rotationsmatrix um die Y-Achse (Pitch)
    R_y << cos(pitch), 0, sin(pitch), 0, 1, 0, -sin(pitch), 0, cos(pitch);

    // Rotationsmatrix um die Z-Achse (Yaw)
    R_z << cos(yaw), -sin(yaw), 0, sin(yaw), cos(yaw), 0, 0, 0, 1;

    // Gesamte Rotationsmatrix
    Eigen::Matrix3f R = R_z * R_y * R_x;

    // Transformationsmatrix 4x4
    Eigen::Matrix4f T = Eigen::Matrix4f::Identity();
    T.block<3, 3>(0, 0) = R;
    T.block<3, 1>(0, 3) = translation;

    return T;
  }
  Eigen::Vector3f transform_coordinates(Eigen::Matrix4f T, Eigen::Vector3f point) {
    Eigen::Vector4f point_h;
    point_h << point, 1.0f;
    Eigen::Vector4f transformed_point_h = T * point_h;
    return transformed_point_h.head<3>();
  }
  Vector align_vector(Vector pos_by_cam) {
    Vector xarm_system_ref = Vector{pos_by_cam.Y_, -pos_by_cam.X_, pos_by_cam.Z_};
    return xarm_system_ref;
  }

  Vector transform_coordinates_wrapper(Vector pos_by_cam, Position cur_pos) {

    pos_by_cam = align_vector(pos_by_cam);
    Eigen::Vector3f coordinates_by_camera;
    coordinates_by_camera << pos_by_cam.X_ * 1000, pos_by_cam.Y_ * 1000, pos_by_cam.Z_ * 1000;

    Vector end_eff_coords = cur_pos.get_coordinates();
    Eigen::Vector3f coordinates_of_end_effector;
    coordinates_of_end_effector << end_eff_coords.X_, end_eff_coords.Y_, end_eff_coords.Z_;

    Vector end_eff_roll_pitch_yaw = cur_pos.get_roll_pitch_yaw();
    double trgt_roll = end_eff_roll_pitch_yaw.X_;
    double trgt_pitch = end_eff_roll_pitch_yaw.Y_;
    double trgt_yaw = end_eff_roll_pitch_yaw.Z_;

    Eigen::Matrix4f T = createTransformationMatrix(trgt_roll, trgt_pitch, trgt_yaw, coordinates_of_end_effector);
    Eigen::Vector3f object_position_robot = transform_coordinates(T, coordinates_by_camera);
    std::cout << object_position_robot.transpose() << std::endl;
    return Vector{object_position_robot.x(), object_position_robot.y(), object_position_robot.z()};
  }

  void start_main_() {
    Vector pos_by_cam;
    Position cur_pos;
    pos_by_cam = Vector{0.5, 0, 0.5};
    cur_pos = Position{Vector{0, 0, 0}, Vector{0, 0, 0}};
    transform_coordinates_wrapper(pos_by_cam, cur_pos);
  }
};

#endif
