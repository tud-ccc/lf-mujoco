#include "common_world_data.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <mujoco/mujoco.h>
#include <vector>



  WorldData::WorldData(std::chrono::nanoseconds physical_elapsed_time, std::vector<double> robo_joint_angles,
            std::vector<double> robo_joint_velocities, std::vector<double> robo_joint_effort,
            std::vector<double> robo_pose) {
    physical_elapsed_time_ = physical_elapsed_time;
    robo_joint_angles_ = robo_joint_angles;
    robo_joint_velocity_ = robo_joint_velocities;
    robo_joint_effort_ = robo_joint_effort;
    robo_pose_ = robo_pose;

    float sim_sensor_data[1] = {std::nan("1")};
    std::fill_n(sim_sensor_data, 1, std::nan("1"));

    sensor_data_ = std::vector<double>(std::begin(sim_sensor_data), std::end(sim_sensor_data));
  }
  WorldData::WorldData(mjData* data, mjModel* model)  noexcept{
    //physical_elapsed_time_ = physical_elapsed_time;
    long time = data->time;
    physical_elapsed_time_ = std::chrono::microseconds(time);

    for (auto i = 0; i < model->nbody;i++) {
      std::cout << "body" << i << ": " << data->xpos[i * 3 + 0] << "/" << data->xpos[i * 3 + 1] << "/" << data->xpos[i * 3 + 2] << std::endl;
    }

    sensor_data_ = std::vector<mjtNum>{data->sensordata, data->sensordata + model->nsensordata};
    robo_joint_angles_ = std::vector<mjtNum>{data->qpos, data->qpos + model->nq};
    robo_joint_velocity_ = std::vector<mjtNum>{data->qvel, data->qvel + model->nv};
    robo_joint_effort_ = std::vector<mjtNum>{data->act, data->act + model->na};
  }
