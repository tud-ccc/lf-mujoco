#ifndef LF_MUJOCO_WORLD_DATA
#define LF_MUJOCO_WORLD_DATA

#include <mujoco/mujoco.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <chrono>


class WorldData {
private:
    //robo attributes
    std::chrono::nanoseconds physical_elapsed_time_;
    std::vector<double> robo_joint_angles_{};
    std::vector<double> robo_joint_velocity_{};
    std::vector<double> robo_joint_effort_{};
    std::vector<double> robo_pose_{};

    // simulator attributes 
    std::vector<double> sim_sensor_data_{};
    std::vector<double> sim_joint_positions_{};
    std::vector<double> sim_joint_velocity_{};
    std::vector<double> sim_joint_acceleration_{};
     

public:
    WorldData() noexcept = default;
   
    WorldData(
        std::chrono::nanoseconds physical_elapsed_time,
        std::vector<double> robo_joint_angles,
        std::vector<double> robo_joint_velocities,
        std::vector<double> robo_joint_effort,
        std::vector<double> robo_pose
    ) {
        physical_elapsed_time_ = physical_elapsed_time;
        robo_joint_angles_ = robo_joint_angles;
        robo_joint_velocity_ = robo_joint_velocities;
        robo_joint_effort_ = robo_joint_effort;
        robo_pose_= robo_pose;
    }
     WorldData(mjData* data, mjModel* model) noexcept {
        sim_sensor_data_ = std::vector<mjtNum>{data->sensordata, data->sensordata + model->nsensordata};
        sim_joint_positions_ = std::vector<mjtNum>{data->qpos, data->qpos + model->nq};
        sim_joint_velocity_ = std::vector<mjtNum>{data->qvel, data->qvel + model->nv};
        sim_joint_acceleration_ = std::vector<mjtNum>{data->act, data->act + model->na};
    };

    ~WorldData() noexcept = default;


    // auto time() const noexcept -> double {
    //     return current_time_;
    // }
    
    // auto physical_elapsed_time() const noexcept -> std::chrono::nanoseconds {
    //     return physical_elapsed_time_;
    // }

    // auto sensor_data() const noexcept -> std::vector<double> {
    //     return sensor_data_;
    // }

    auto position() const noexcept -> std::vector<double> {
        return robo_pose_;
    }

    void write_csv_header(const std::string file) {

        auto write_header = [](std::ofstream& file_handle, const std::string& column_name, std::size_t count) {
            for (std::size_t i = 0; i < count; i++) {
                file_handle << column_name << "_" << std::to_string(i) << ", ";
            }
        };

        std::ofstream csvfile;
        csvfile.open(file, std::ios_base::openmode::_S_trunc);

        csvfile << "time,";

        //robo data
        write_header(csvfile, "robo_joint_angles", 7);
        write_header(csvfile, "robo_joint_velocity", 7);
        write_header(csvfile, "robo_joint_effort", 7);
        write_header(csvfile, "robo_pose",6 );
        
        //simulation data FIXME : hard coded length
        write_header(csvfile, "sim_sensor_data", 6);
        write_header(csvfile, "sim_joint_position", 6);
        write_header(csvfile, "sim_joint_velocity", 6);
        write_header(csvfile, "sim_joint_acceleration", 6);

        csvfile << "\n";
    }

   
    void write_to_csv(const std::string& file, bool readable_not_csv_style)  {

        auto write_vec_to_file = [](std::ofstream& file_handle, const std::vector<double>& data) {
            for (double value : data) {
                file_handle << std::to_string(value) << ", ";
            }
        };

        // TODO: global file pointer which can be shared by thing kind of objects
        std::ofstream csvfile;
        csvfile.open(file, std::ios_base::openmode::_S_app);
        if (readable_not_csv_style){
            csvfile << "time : " ;
            csvfile << std::to_string(physical_elapsed_time_.count());
            csvfile << "\n" <<  "robo_joint_angles :" ;
            write_vec_to_file(csvfile, robo_joint_angles_);
            csvfile << "\n" <<  "robo_joint_velocity :";
            write_vec_to_file(csvfile, robo_joint_velocity_);
            csvfile << "\n" <<  "robo_joint_effort :" ;
            write_vec_to_file(csvfile, robo_joint_effort_);
            csvfile << "\n" <<  "robo_pose :" ;
            write_vec_to_file(csvfile, robo_pose_);
            csvfile << "\n" <<  "sim_sensor_data :" ;
            write_vec_to_file(csvfile, sim_sensor_data_);
            csvfile << "\n" <<  "sim_joint_positions :";
            write_vec_to_file(csvfile, sim_joint_positions_);
            csvfile << "\n" <<  "sim_joint_velocity :" ;
            write_vec_to_file(csvfile, sim_joint_velocity_);
            csvfile << "\n" <<  "sim_joint_acceleration :" ;
            write_vec_to_file(csvfile, sim_joint_acceleration_);
            csvfile << "\n";
        }
        else {
            //xArm
            csvfile << std::to_string(physical_elapsed_time_.count()) << ", ";
            write_vec_to_file(csvfile, robo_joint_angles_);
            write_vec_to_file(csvfile, robo_joint_velocity_);
            write_vec_to_file(csvfile, robo_joint_effort_);
            write_vec_to_file(csvfile, robo_pose_);
            //simulator
            write_vec_to_file(csvfile, sim_sensor_data_);
            write_vec_to_file(csvfile, sim_joint_positions_);
            write_vec_to_file(csvfile, sim_joint_velocity_);
            write_vec_to_file(csvfile, sim_joint_acceleration_);

            csvfile << "\n";
        }
    }

  
};

class RoboFedData{
    private:
    std::chrono::nanoseconds physical_elapsed_time_;
    std::vector<double> instructions_{};

    public:

    RoboFedData(
        std::chrono::nanoseconds physical_elapsed_time,
        std::vector<double> instructions
        
    )
    {
        physical_elapsed_time_ = physical_elapsed_time;
        instructions_ = instructions;
    }


    RoboFedData() noexcept = default;
    ~RoboFedData() noexcept = default;


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
        write_header(csvfile, "instruction", 6);            
        csvfile << "";
        csvfile << "\n";
    }

    void write_instructions_to_csv(const std::string& file)  {

        auto write_vec_to_file = [](std::ofstream& file_handle, const std::vector<double>& data) {
            for (double value : data) {
                file_handle << std::to_string(value) << ", ";
            }
        };

        // TODO: global file pointer which can be shared by thing kind of objects
        std::ofstream csvfile;
        csvfile.open(file, std::ios_base::openmode::_S_app);

        csvfile << std::to_string(physical_elapsed_time_.count()) << ", ";
        write_vec_to_file(csvfile, instructions_);
        csvfile << "\n";
      
    }




};

#endif //LF_MUJOCO_WORLD_DATA
