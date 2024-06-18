#ifndef COMMON_ROLL_PTICH_YAW_CONTROL
#define COMMON_ROLL_PTICH_YAW_CONTROL

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

    double dist_roll = (current_roll_pitch_yaw.X_ - target_roll_pitch_yaw.X_);
    if (dist_roll < 0)
      dist_roll = -dist_roll;
    if (dist_roll > 180)
      dist_roll = 360 - dist_roll;

    double dist_pitch = (current_roll_pitch_yaw.Y_ - target_roll_pitch_yaw.Y_);
    if (dist_pitch < 0)
      dist_pitch = -dist_pitch;
    if (dist_pitch > 180)
      dist_pitch = 360 - dist_pitch;

    double dist_yaw = (current_roll_pitch_yaw.Z_ - target_roll_pitch_yaw.Z_);
    if (dist_yaw < 0)
      dist_yaw = -dist_yaw;
    if (dist_yaw > 180)
      dist_yaw = 360 - dist_yaw;

    double dis = sqrt(pow(dist_roll, 2) + pow(dist_pitch, 2) + pow(dist_yaw, 2));
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
    if (source < 0 || target < 0 || source > 360 || target > 360) {
      std::cout << "src: " << source << " target: " << target << std::endl;
      assert(false && "angle out of bounds");
    } else {
      double diff = (target - source);
      if (diff < 0)
        diff = -diff;
      return diff > 180;
    }
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

  Vector back_to_angles(Vector vec) {
    if (vec.X_ >= 360)
      vec.X_ -= 360;
    if (vec.X_ < 0)
      vec.X_ += 360;

    if (vec.Y_ >= 360)
      vec.Y_ -= 360;
    if (vec.Y_ < 0)
      vec.Y_ += 360;

    if (vec.Z_ >= 360)
      vec.Z_ -= 360;
    if (vec.Z_ < 0)
      vec.Z_ += 360;
    return vec;
  }
  Vector compute_next_roll_pitch_yaw(const Vector current_roll_pitch_yaw, const Vector target_roll_pitch_yaw) {
    if (near_target(current_roll_pitch_yaw, target_roll_pitch_yaw)) {
      std::cout << "Near Target !! " << std::endl;
      return current_roll_pitch_yaw;
    } else {
      Vector offest_roll_pitch_yaw = get_adjusted_offset_vector(current_roll_pitch_yaw, target_roll_pitch_yaw);
      offest_roll_pitch_yaw = offest_roll_pitch_yaw.normalize().scale(this->offest_scale_);

      Vector next_rol_pitch_yaw = this->va_.add_vectors(current_roll_pitch_yaw, offest_roll_pitch_yaw);
      next_rol_pitch_yaw = back_to_angles(next_rol_pitch_yaw);
      next_rol_pitch_yaw.to_string();
      return next_rol_pitch_yaw;
    }
  }
  Vector convert_vector_forwards(Vector vec) {
    if (vec.X_ < 0) {
      vec.X_ += 360;
    };
    if (vec.Y_ < 0) {
      vec.Y_ += 360;
    };
    if (vec.Z_ < 0) {
      vec.Z_ += 360;
    };
    return vec;
  }
  Vector convert_vector_backwards(Vector vec) {
    if (vec.X_ > 180) {
      vec.X_ -= 360;
    };
    if (vec.Y_ > 180) {
      vec.Y_ -= 360;
    };
    if (vec.Z_ > 180) {
      vec.Z_ -= 360;
    };
    return vec;
  }

  Vector wrapper_compute_next_roll_pitch_yaw(const Vector current_roll_pitch_yaw, const Vector target_roll_pitch_yaw) {

    return convert_vector_backwards(compute_next_roll_pitch_yaw(convert_vector_forwards(current_roll_pitch_yaw),
                                                                convert_vector_forwards(target_roll_pitch_yaw)));
  }
};
#endif
