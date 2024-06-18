#ifndef COMMON_ROLL_PITCH_YAW
#define COMMON_ROLL_PITCH_YAW

#include "common_vector.hpp"
#include <cassert>
#include <cmath>
#include <stdlib.h>

class RollPitchYawController {

private:
  VectorArithmetics va_;
  double stop_near_target_;
  double offest_scale_;

public:
  RollPitchYawController(double stop_near_target, double offest_scale) {
    this->stop_near_target_ = stop_near_target;
    this->offest_scale_ = offest_scale;
  }
  double distance(const Vector current_roll_pitch_yaw, const Vector target_roll_pitch_yaw) {

    double dis_roll = target_roll_pitch_yaw.X_ - current_roll_pitch_yaw.X_;
    double dis_pitch = target_roll_pitch_yaw.Y_ - current_roll_pitch_yaw.Y_;
    double dis_yaw = target_roll_pitch_yaw.Z_ - current_roll_pitch_yaw.Z_;
    if (dis_roll < 0) {
      dis_roll = -dis_roll;
    }
    if (dis_roll > 180) {
      dis_roll -= 360;
    }
    if (dis_pitch < 0) {
      dis_pitch = -dis_pitch;
    }
    if (dis_pitch > 180) {
      dis_pitch -= 360;
    }
    if (dis_yaw < 0) {
      dis_yaw = -dis_yaw;
    }
    if (dis_yaw > 180) {
      dis_yaw -= 360;
    }

    double dis = sqrt(pow(dis_roll, 2) + pow(dis_pitch, 2) + pow(dis_yaw, 2));
    // std::cout << "here the distance: " << dis << std::endl;
    return dis;
  }
  bool near_target(const Vector current_roll_pitch_yaw, const Vector target_roll_pitch_yaw) {
    if (this->distance(current_roll_pitch_yaw, target_roll_pitch_yaw) <= this->stop_near_target_) {
      return true;
    } else {
      return false;
    }
  }
  bool prefer_add(double source, double target) {

    double dis = target - source;
    if (dis < 0) {
      dis = -dis;
    }
    return dis > 180;
  }

  Vector get_adjusted_offset_vector(Vector current_roll_pitch_yaw, Vector target_roll_pitch_yaw) {
    Vector offest_roll_pitch_yaw = this->va_.get_delta_vector(current_roll_pitch_yaw, target_roll_pitch_yaw);
    if (prefer_add(current_roll_pitch_yaw.X_, target_roll_pitch_yaw.X_)) {
      offest_roll_pitch_yaw.X_ = -offest_roll_pitch_yaw.X_;
    }
    if (prefer_add(current_roll_pitch_yaw.Y_, target_roll_pitch_yaw.Y_)) {
      offest_roll_pitch_yaw.Y_ = -offest_roll_pitch_yaw.Y_;
    }
    if (prefer_add(current_roll_pitch_yaw.Z_, target_roll_pitch_yaw.Z_)) {
      offest_roll_pitch_yaw.Z_ = -offest_roll_pitch_yaw.Y_;
    }
    return offest_roll_pitch_yaw;
  }

  Vector modulo_angles(Vector vec) {
    if (vec.X_ > 180) {
      vec.X_ -= 360;
    } else if (vec.X_ < -180) {
      vec.X_ += 360;
    }
    if (vec.Y_ > 180) {
      vec.Y_ -= 360;
    } else if (vec.Y_ < -180) {
      vec.Y_ += 360;
    }
    if (vec.Z_ > 180) {
      vec.Z_ -= 360;
    } else if (vec.Z_ < -180) {
      vec.Z_ += 360;
    }
    return vec;
  }

  void test_value(double val) {
    if (val < -180 || val > 180) {
      assert(false && "angle out of bounds");
    } else {
      return;
    }
  }
  void test_vector(Vector vec) {
    test_value(vec.X_);
    test_value(vec.Y_);
    test_value(vec.Z_);
  }

  Vector compute_next_roll_pitch_yaw(const Vector current_roll_pitch_yaw, const Vector target_roll_pitch_yaw) {
    test_vector(current_roll_pitch_yaw);
    test_vector(target_roll_pitch_yaw);

        if (near_target(current_roll_pitch_yaw, target_roll_pitch_yaw)) {
      std::cout << "Near Target !! " << std::endl;
      return current_roll_pitch_yaw;
    }
    else {
      Vector offest_roll_pitch_yaw = get_adjusted_offset_vector(current_roll_pitch_yaw, target_roll_pitch_yaw);
      offest_roll_pitch_yaw = offest_roll_pitch_yaw.normalize().scale(this->offest_scale_);

      Vector next_rol_pitch_yaw = this->va_.add_vectors(current_roll_pitch_yaw, offest_roll_pitch_yaw);
      next_rol_pitch_yaw = next_rol_pitch_yaw = modulo_angles(next_rol_pitch_yaw);
      return next_rol_pitch_yaw;
    }
  }
};
#endif
