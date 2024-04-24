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

    // Nulled arrays

    float sim_joint_pos[27] = {std::nan("1")};
    std::fill_n(sim_joint_pos, 27, std::nan("1"));
    float sim_joint_velocities[25] = {std::nan("1")};
    std::fill_n(sim_joint_velocities, 25, std::nan("1"));
    float sim_sensor_data[1] = {std::nan("1")};
    std::fill_n(sim_sensor_data, 1, std::nan("1"));
    float sim_joint_acc[1] = {std::nan("1")};
    std::fill_n(sim_joint_acc, 1, std::nan("1"));

    sim_sensor_data_ = std::vector<double>(std::begin(sim_sensor_data), std::end(sim_sensor_data));
    sim_joint_positions_ = std::vector<double>(std::begin(sim_joint_pos), std::end(sim_joint_pos));
    sim_joint_velocity_ = std::vector<double>(std::begin(sim_joint_velocities), std::end(sim_joint_velocities));
    sim_joint_acceleration_ = std::vector<double>(std::begin(sim_joint_acc), std::end(sim_joint_acc));
  }
  WorldData::WorldData(mjData* data, mjModel* model, std::chrono::nanoseconds physical_elapsed_time)  noexcept{

    // Nulled arrays

    physical_elapsed_time_ = physical_elapsed_time;
    float robo_angles[7] = {std::nan("1")};
    std::fill_n(robo_angles, 7, std::nan("1"));
    float robo_velocities[7] = {std::nan("1")};
    std::fill_n(robo_velocities, 7, std::nan("1"));
    float robo_effort[7] = {std::nan("1")};
    std::fill_n(robo_effort, 7, std::nan("1"));
    float robo_pose[6] = {std::nan("1")};
    std::fill_n(robo_pose, 6, std::nan("1"));

    robo_joint_angles_ = std::vector<double>(std::begin(robo_angles), std::end(robo_angles));
    robo_joint_velocity_ = std::vector<double>(std::begin(robo_velocities), std::end(robo_velocities));
    robo_joint_effort_ = std::vector<double>(std::begin(robo_effort), std::end(robo_effort));
    robo_pose_ = std::vector<double>(std::begin(robo_pose), std::end(robo_pose));

    sim_sensor_data_ = std::vector<mjtNum>{data->sensordata, data->sensordata + model->nsensordata};
    sim_joint_positions_ = std::vector<mjtNum>{data->qpos, data->qpos + model->nq};
    sim_joint_velocity_ = std::vector<mjtNum>{data->qvel, data->qvel + model->nv};
    sim_joint_acceleration_ = std::vector<mjtNum>{data->act, data->act + model->na};
  }
