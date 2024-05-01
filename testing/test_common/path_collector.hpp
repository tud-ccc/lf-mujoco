#ifndef PATH_COLLECTOR
#define PATH_COLLECTOR

#include "common_vector.hpp"
#include <fstream>
#include <iostream>
#include <vector>

class PathCollector {
private:
  std::vector<Vector> path_;
  std::string file_;

public:
  PathCollector(std::vector<Vector> path, std::string file) {
    this->path_ = path;
    this->file_ = file;
  }

  PathCollector() noexcept = default;
  ~PathCollector() noexcept = default;

  void write_path_csv_header() {
    // FiXME : we have got hard coded array lengths !

    auto write_header = [](std::ofstream& file_handle, const std::string& column_name) {
      file_handle << column_name << ",";
    };

    std::ofstream csvfile;
    csvfile.open(this->file_, std::ios_base::openmode::_S_trunc);
    write_header(csvfile, "X");
    write_header(csvfile, "Y");
    write_header(csvfile, "Z");
    csvfile << "\n";
  }

  void write_path_to_csv() {

    auto write_Vector_to_file = [](std::ofstream& file_handle, Vector vec) {
      file_handle << std::to_string(vec.X_) << ", ";
      file_handle << std::to_string(vec.Y_) << ", ";
      file_handle << std::to_string(vec.Z_) << ", ";
      file_handle << "\n";
    };

    // TODO: global file pointer which can be shared by thing kind of objects
    std::ofstream csvfile;
    csvfile.open(this->file_, std::ios_base::openmode::_S_app);

    for (Vector position : this->path_) {
      write_Vector_to_file(csvfile, position);
    }
  }
};

#endif // PATH_COLLECTOR