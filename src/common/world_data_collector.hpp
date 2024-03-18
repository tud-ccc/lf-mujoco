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
        sim_joint_acceleration_= std::vector<double>(std::begin(sim_joint_acc), std::end(sim_joint_acc));     

    }
     WorldData(mjData* data, mjModel* model, std::chrono::nanoseconds physical_elapsed_time) noexcept {

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
        robo_pose_= std::vector<double>(std::begin(robo_pose), std::end(robo_pose));     

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
                file_handle << column_name << "_" << std::to_string(i) << ",";
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
        //FIXME : where is the sensor data, where is the joint acceleration ?
 
        write_header(csvfile, "sim_joint_position", 27);
        write_header(csvfile, "sim_joint_velocity", 25);
        write_header(csvfile, "sim_sensor_data", 1);
        write_header(csvfile, "sim_joint_acceleration", 1);

        csvfile << "\n";
    }

   
    void write_to_csv(const std::string& file, bool readable_not_csv_style)  {

        auto write_vec_to_file = [](std::ofstream& file_handle, const std::vector<double>& data) {
            for (double value : data) {
                file_handle << std::to_string(value) << ", ";
            }
        };
        
        //for better readability cast to seconds!

        std::chrono::milliseconds phys_elaps_t_in_secs;
        phys_elaps_t_in_secs = std::chrono::duration_cast<std::chrono::milliseconds> (physical_elapsed_time_);

        // TODO: global file pointer which can be shared by thing kind of objects
        std::ofstream csvfile;
        csvfile.open(file, std::ios_base::openmode::_S_app);
        if (readable_not_csv_style){
            csvfile << "time : " ;
            csvfile << std::to_string(phys_elaps_t_in_secs.count());
            csvfile << "\n" <<  "robo_joint_angles :" ;
            write_vec_to_file(csvfile, robo_joint_angles_);
            csvfile << "\n" <<  "robo_joint_velocity :";
            write_vec_to_file(csvfile, robo_joint_velocity_);
            csvfile << "\n" <<  "robo_joint_effort :" ;
            write_vec_to_file(csvfile, robo_joint_effort_);
            csvfile << "\n" <<  "robo_pose :" ;
            write_vec_to_file(csvfile, robo_pose_);
            csvfile << "\n" <<  "sim_joint_positions :";
            write_vec_to_file(csvfile, sim_joint_positions_);
            csvfile << "\n" <<  "sim_joint_velocity :" ;
            write_vec_to_file(csvfile, sim_joint_velocity_);
            csvfile << "\n" <<  "sim_sensor_data :" ;
            write_vec_to_file(csvfile, sim_sensor_data_);
            csvfile << "\n" <<  "sim_joint_acceleration :" ;
            write_vec_to_file(csvfile, sim_joint_acceleration_);
            csvfile << "\n";
        }
        else {
            //xArm
            csvfile << std::to_string(phys_elaps_t_in_secs.count()) << ", ";
            write_vec_to_file(csvfile, robo_joint_angles_);
            write_vec_to_file(csvfile, robo_joint_velocity_);
            write_vec_to_file(csvfile, robo_joint_effort_);
            write_vec_to_file(csvfile, robo_pose_);
            //simulator
            write_vec_to_file(csvfile, sim_joint_positions_);
            write_vec_to_file(csvfile, sim_joint_velocity_);
            write_vec_to_file(csvfile, sim_sensor_data_);
            write_vec_to_file(csvfile, sim_joint_acceleration_);

            csvfile << "\n";
        }
    }

  
};

#endif //LF_MUJOCO_WORLD_DATA