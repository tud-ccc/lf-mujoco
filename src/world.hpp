#ifndef LF_MUJOCO_WORLD_DATA
#define LF_MUJOCO_WORLD_DATA

#include <mujoco/mujoco.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <chrono>


class WorldData {
private:
    double current_time_{0};
    std::chrono::nanoseconds physical_elapsed_time_;
    std::vector<double> joint_effort_{};
    std::vector<double> sensor_data_{};
    std::vector<double> joint_positions_{};
    std::vector<double> joint_angles_{};
    std::vector<double> joint_velocity_{};
    std::vector<double> joint_acceleration_{};
    std::vector<double> pose_{};
    std::vector<double> orientation_{};

public:
    WorldData() noexcept = default;
    WorldData(mjData* data, mjModel* model) noexcept {
        this->current_time_ = data->time;

        sensor_data_ = std::vector<mjtNum>{data->sensordata, data->sensordata + model->nsensordata};
        joint_positions_ = std::vector<mjtNum>{data->qpos, data->qpos + model->nq};
        joint_velocity_ = std::vector<mjtNum>{data->qvel, data->qvel + model->nv};
        joint_acceleration_ = std::vector<mjtNum>{data->act, data->act + model->na};
    };
    WorldData(
        std::chrono::nanoseconds physical_elapsed_time,
        std::vector<double> joint_angles,
        std::vector<double> joint_velocities,
        std::vector<double> joint_effort,
        std::vector<double> pose
    ) {
        physical_elapsed_time_ = physical_elapsed_time;
        joint_angles_ = joint_angles;
        joint_velocity_ = joint_velocities;
        joint_effort_ = joint_effort;
        pose_= pose;
    }
    ~WorldData() noexcept = default;


    auto time() const noexcept -> double {
        return current_time_;
    }
    
    auto physical_elapsed_time() const noexcept -> std::chrono::nanoseconds {
        return physical_elapsed_time_;
    }

    auto sensor_data() const noexcept -> std::vector<double> {
        return sensor_data_;
    }

    auto position() const noexcept -> std::vector<double> {
        return pose_;
    }

    void write_csv_mujoco_header(const std::string file, const mjModel* model) {

        auto write_header = [](std::ofstream& file_handle, const std::string& column_name, std::size_t count) {
            for (std::size_t i = 0; i < count; i++) {
                file_handle << column_name << "_" << std::to_string(i) << ", ";
            }
        };

        std::ofstream csvfile;
        csvfile.open(file, std::ios_base::openmode::_S_trunc);

        csvfile << "time, ";

        write_header(csvfile, "sensor", model->nsensordata);
        write_header(csvfile, "joint_position", model->nq);
        write_header(csvfile, "joint_velocity", model->nv);
        write_header(csvfile, "joint_acceleration", model->na);

        csvfile << "\n";
    }

    void write_csv_xArm_header(const std::string file) {
        //FiXME : we have got hard coded array lengths !

        auto write_header = [](std::ofstream& file_handle, const std::string& column_name, std::size_t count) {
            for (std::size_t i = 0; i < count; i++) {
                file_handle << column_name << "_" << std::to_string(i) << ",";
            }
        };

        std::ofstream csvfile;
        csvfile.open(file, std::ios_base::openmode::_S_trunc);

        csvfile << "time, ";

        write_header(csvfile, "joint_angles", 7);
        write_header(csvfile, "joint_velocity", 7);
        write_header(csvfile, "joint_effort", 7);
        write_header(csvfile, "pose",6 );

        csvfile << "\n";
    }

    void write_to_csv_xArm(const std::string& file, bool readable_not_csv_style)  {

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
            csvfile << "\n" <<  "joint_angles :" ;
            write_vec_to_file(csvfile, joint_angles_);
            csvfile << "\n" <<  "joint_velocity :";
            write_vec_to_file(csvfile, joint_velocity_);
            csvfile << "\n" <<  "joint_effort :" ;
            write_vec_to_file(csvfile, joint_effort_);
            csvfile << "\n" <<  "pose :" ;
            write_vec_to_file(csvfile, pose_);
            csvfile << "\n";
        }
        else {
            csvfile << std::to_string(physical_elapsed_time_.count()) << ", ";
            write_vec_to_file(csvfile, joint_angles_);
            write_vec_to_file(csvfile, joint_velocity_);
            write_vec_to_file(csvfile, joint_effort_);
            write_vec_to_file(csvfile, pose_);
            csvfile << "\n";
        }
    }

  
};

#endif //LF_MUJOCO_WORLD_DATA
