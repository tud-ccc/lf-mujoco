#ifndef COMMON_WORLD_DATA
#define COMMON_WORLD_DATA

#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include "common_world_data.hpp"

class WorldDataCollector {
private:
  WorldData world_data_ ;

public:
  WorldDataCollector() noexcept = default;

  WorldDataCollector(WorldData world_data) {
    world_data_ = world_data;
  }

  void write_csv_header(const std::string file) {

    auto write_header = [](std::ofstream& file_handle, const std::string& column_name, std::size_t count) {
      for (std::size_t i = 0; i < count; i++) {
        file_handle << column_name << "_" << std::to_string(i) << ",";
      }
    };

    std::ofstream csvfile;
    csvfile.open(file, std::ios_base::openmode::_S_trunc);

    csvfile << "time,";

    // robo data
    write_header(csvfile, "robo_joint_angles", 7);
    write_header(csvfile, "robo_joint_velocity", 7);
    write_header(csvfile, "robo_joint_effort", 7);
    write_header(csvfile, "robo_pose", 6);

    // simulation data FIXME : hard coded length
    // FIXME : where is the sensor data, where is the joint acceleration ?

    write_header(csvfile, "sim_joint_position", 27);
    write_header(csvfile, "sim_joint_velocity", 25);
    write_header(csvfile, "sim_sensor_data", 1);
    write_header(csvfile, "sim_joint_acceleration", 1);

    csvfile << "\n";
  }

  void write_to_csv(const std::string& file, bool readable_not_csv_style) {

    auto write_vec_to_file = [](std::ofstream& file_handle, const std::vector<double>& data) {
      for (double value : data) {
        file_handle << std::to_string(value) << ", ";
      }
    };

    // for better readability cast to seconds!

    std::chrono::milliseconds phys_elaps_t_in_secs;
    phys_elaps_t_in_secs = std::chrono::duration_cast<std::chrono::milliseconds>(world_data_.physical_elapsed_time_);

    // TODO: global file pointer which can be shared by thing kind of objects
    std::ofstream csvfile;
    csvfile.open(file, std::ios_base::openmode::_S_app);
    if (readable_not_csv_style) {
      csvfile << "time : ";
      csvfile << std::to_string(phys_elaps_t_in_secs.count());
      csvfile << "\n"
              << "robo_joint_angles :";
      write_vec_to_file(csvfile, world_data_.robo_joint_angles_);
      csvfile << "\n"
              << "robo_joint_velocity :";
      write_vec_to_file(csvfile, world_data_.robo_joint_velocity_);
      csvfile << "\n"
              << "robo_joint_effort :";
      write_vec_to_file(csvfile, world_data_.robo_joint_effort_);
      csvfile << "\n"
              << "robo_pose :";
      write_vec_to_file(csvfile, world_data_.robo_pose_);
      csvfile << "\n"
              << "sim_joint_positions :";
      write_vec_to_file(csvfile, world_data_.sim_joint_positions_);
      csvfile << "\n"
              << "sim_joint_velocity :";
      write_vec_to_file(csvfile, world_data_.sim_joint_velocity_);
      csvfile << "\n"
              << "sim_sensor_data :";
      write_vec_to_file(csvfile, world_data_.sim_sensor_data_);
      csvfile << "\n"
              << "sim_joint_acceleration :";
      write_vec_to_file(csvfile, world_data_.sim_joint_acceleration_);
      csvfile << "\n";
    } else {
      // xArm
      csvfile << std::to_string(phys_elaps_t_in_secs.count()) << ", ";
      write_vec_to_file(csvfile, world_data_.robo_joint_angles_);
      write_vec_to_file(csvfile, world_data_.robo_joint_velocity_);
      write_vec_to_file(csvfile, world_data_.robo_joint_effort_);
      write_vec_to_file(csvfile, world_data_.robo_pose_);
      // simulator
      write_vec_to_file(csvfile, world_data_.sim_joint_positions_);
      write_vec_to_file(csvfile, world_data_.sim_joint_velocity_);
      write_vec_to_file(csvfile, world_data_.sim_sensor_data_);
      write_vec_to_file(csvfile, world_data_.sim_joint_acceleration_);

      csvfile << "\n";
    }
  }
};

#endif // COMMON_WORLD_DATA