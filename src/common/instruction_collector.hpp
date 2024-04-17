#ifndef INSTRUCTION_COLLECTOR
#define INSTRUCTION_COLLECTOR

#include <vector>
#include <fstream>
#include <iostream>
#include "common_vector.hpp"

class Instruction_Collector{
    private:
    std::chrono::nanoseconds physical_elapsed_time_;
    Vector  current_position_;
    Vector  raw_instruction_;
    Vector  checked_instruction_;
    

    public:

    Instruction_Collector(
        std::chrono::nanoseconds physical_elapsed_time,
        Vector current_position,
        Vector  raw_instruction,
        Vector  checked_instruction
    )
    {
        physical_elapsed_time_ = physical_elapsed_time;
        current_position_ = current_position;
        raw_instruction_ = raw_instruction;
        checked_instruction_ = checked_instruction;
        
    }


    Instruction_Collector() noexcept = default;
    ~Instruction_Collector() noexcept = default;


    void write_instructions_csv_header(const std::string file) {
        //FiXME : we have got hard coded array lengths !

        auto write_header = [](std::ofstream& file_handle, const std::string& column_name, std::size_t count) {
            for (std::size_t i = 0; i < count; i++) {
                file_handle << column_name << "_" << std::to_string(i) << ",";
            }
        };

        std::ofstream csvfile;
        csvfile.open(file, std::ios_base::openmode::_S_trunc);

        csvfile << "time,";
        write_header(csvfile, "current_position", 3);   
        write_header(csvfile, "raw_instruction",3);   
        write_header(csvfile, "checked_instruction", 3);            
         
        csvfile << "";
        csvfile << "\n";
    }

    void write_instructions_to_csv(const std::string file)  {

        auto write_Vector_to_file = [](std::ofstream& file_handle,Vector vec) {
           
            file_handle << std::to_string(vec.X_) << ", ";
            file_handle << std::to_string(vec.Y_) << ", ";
            file_handle << std::to_string(vec.Z_) << ", ";
        };

        // TODO: global file pointer which can be shared by thing kind of objects
        std::ofstream csvfile;
        csvfile.open(file, std::ios_base::openmode::_S_app);

        std::chrono::milliseconds phys_elaps_t_in_secs;
        phys_elaps_t_in_secs = std::chrono::duration_cast<std::chrono::milliseconds> (physical_elapsed_time_);

        csvfile << std::to_string(phys_elaps_t_in_secs.count()) << ", ";
        write_Vector_to_file(csvfile, current_position_);
        write_Vector_to_file(csvfile, raw_instruction_);
        write_Vector_to_file(csvfile, checked_instruction_);
        csvfile << "\n";
      
    }

};


#endif //LF_MUJOCO_WORLD_DATA