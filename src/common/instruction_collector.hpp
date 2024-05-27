#ifndef INSTRUCTION_COLLECTOR
#define INSTRUCTION_COLLECTOR

#include "common_vector.hpp"
#include <fstream>
#include <iostream>
#include <vector>

class Instruction_Collector {
private:
  std::chrono::nanoseconds physical_elapsed_time_;
 
  std::vector<Vector> positions_;

public:
  Instruction_Collector(std::chrono::nanoseconds physical_elapsed_time,
                        std::vector<Vector> positions){


        this->physical_elapsed_time_ = physical_elapsed_time;
        this->positions_ = positions;                       
  }

  Instruction_Collector() noexcept = default;
  ~Instruction_Collector() noexcept = default;

  void write_instructions_csv_header(const std::string file) {
    // FiXME : we have got hard coded array lengths !

    auto write_header = [](std::ofstream& file_handle, const std::string& column_name, std::size_t count) {
      for (std::size_t i = 0; i < count; i++) {
        file_handle << column_name << "_" << std::to_string(i) << ",";
      }
    };

    std::ofstream csvfile;
    csvfile.open(file, std::ios_base::openmode::_S_trunc);

    csvfile << "time,";
    write_header(csvfile, "last_position", 3);
    write_header(csvfile, "current_position", 3);
    write_header(csvfile, "target", 3);
    write_header(csvfile, "next_position", 3);
    write_header(csvfile, "next_logical_step_offset_vector", 3);
    write_header(csvfile, "next_logical_step", 3);
    write_header(csvfile, "acceleration_vector", 3);
    write_header(csvfile, "offset_vector", 3);


    csvfile << "";
    csvfile << "\n";
  }

  void write_instructions_to_csv(const std::string file) {

    auto write_Vector_to_file = [](std::ofstream& file_handle, std::vector<Vector> positions) {
        for (Vector vec : positions){
                file_handle << std::to_string(vec.X_) << ", ";
                file_handle << std::to_string(vec.Y_) << ", ";
                file_handle << std::to_string(vec.Z_) << ", ";
        }

    };

    // TODO: global file pointer which can be shared by thing kind of objects
    std::ofstream csvfile;
    csvfile.open(file, std::ios_base::openmode::_S_app);

    std::chrono::milliseconds phys_elaps_t_in_secs;
    phys_elaps_t_in_secs = std::chrono::duration_cast<std::chrono::milliseconds>(physical_elapsed_time_);

    csvfile << std::to_string(phys_elaps_t_in_secs.count()) << ", ";
    write_Vector_to_file(csvfile, this->positions_);   
    csvfile << "\n";
  }
};

#endif // LF_MUJOCO_WORLD_DATA