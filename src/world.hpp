#ifndef LF_MUJOCO_WORLD_DATA
#define LF_MUJOCO_WORLD_DATA

#include <mujoco/mujoco.h>
#include <vector>
#include <fstream>
#include <iostream>

class WorldData {
private:
    double current_time_{0};
    std::vector<double> sensor_data_{};
    std::vector<double> joint_position_{};
    std::vector<double> joint_velocity_{};
    std::vector<double> joint_acceleration_{};
    std::vector<double> pose_{};
    std::vector<double> orientation_{};

public:
    WorldData() noexcept = default;
    WorldData(mjData* data, mjModel* model) noexcept {
        this->current_time_ = data->time;

        sensor_data_ = std::vector<mjtNum>{data->sensordata, data->sensordata + model->nsensordata};
        joint_position_ = std::vector<mjtNum>{data->qpos, data->qpos + model->nq};
        joint_velocity_ = std::vector<mjtNum>{data->qvel, data->qvel + model->nv};
        joint_acceleration_ = std::vector<mjtNum>{data->act, data->act + model->na};
    };
    WorldData(
        std::vector<float> joint_position,
        std::vector<float> joint_velocities,
        std::vector<float> joint_effort,
        std::vector<float> pose
    ) {
        joint_position_ = std::vector<double>(std::begin(joint_position), std::end(joint_position));
        joint_velocity_ = std::vector<double>(std::begin(joint_velocities), std::end(joint_velocities));
        pose_= std::vector<double>(std::begin(pose), std::end(pose));
    }
    ~WorldData() noexcept = default;


    auto time() const noexcept -> double {
        return current_time_;
    }

    auto sensor_data() const noexcept -> std::vector<double> {
        return sensor_data_;
    }

    auto position() const noexcept -> std::vector<double> {
        return pose_;
    }

    void write_csv_header(const std::string file, const mjModel* model) {

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

    void write_to_csv(const std::string& file)  {

        auto write_vec_to_file = [](std::ofstream& file_handle, const std::vector<double>& data) {
            for (double value : data) {
                file_handle << std::to_string(value) << ", ";
            }
        };

        // TODO: global file pointer which can be shared by thing kind of objects
        std::ofstream csvfile;
        csvfile.open(file, std::ios_base::openmode::_S_app);

        csvfile << std::to_string(current_time_) << ", ";

        write_vec_to_file(csvfile, sensor_data_);
        write_vec_to_file(csvfile, joint_position_);
        write_vec_to_file(csvfile, joint_velocity_);
        write_vec_to_file(csvfile, joint_acceleration_);

        csvfile << "\n";
    }
};

#endif //LF_MUJOCO_WORLD_DATA
