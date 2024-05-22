#ifndef LF_MUJOCO_WORLD_DATA
#define LF_MUJOCO_WORLD_DATA

#include <vector>
#include <chrono>
#include <mujoco/mujoco.h>



class WorldData {

public:
  // robo attributes
  std::chrono::nanoseconds physical_elapsed_time_;
  std::vector<double> robo_joint_angles_{};
  std::vector<double> robo_joint_velocity_{};
  std::vector<double> robo_joint_effort_{};
  std::vector<double> robo_pose_{};

  // sensor values
  std::vector<double> sensor_data_{};

  WorldData() noexcept = default;

  WorldData(std::chrono::nanoseconds physical_elapsed_time, std::vector<double> robo_joint_angles,
            std::vector<double> robo_joint_velocities, std::vector<double> robo_joint_effort,
            std::vector<double> robo_pose);
  WorldData(mjData* data, mjModel* model) noexcept;

  ~WorldData() noexcept = default;

  auto position() const noexcept -> std::vector<double> { return robo_pose_; }
};

#endif // LF_MUJOCO_WORLD_DATA