#ifndef TEST_COMMON
#define TEST_COMMON

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>
std::tuple<Vector, Vector, Vector> get_vector_triplet_from_csv(int index) {
  std::tuple<Vector, Vector, Vector> vectors;

  std::vector<std::string> vec_1 = this->get_csv_rows().at(index);
  std::vector<std::string> vec_2 = this->get_csv_rows().at(index + 1);
  std::vector<std::string> vec_3 = this->get_csv_rows().at(index + 2);

  Vector last_position = Vector{std::stod(vec_1.at(0)), std::stod(vec_1.at(1)), std::stod(vec_1.at(2))};
  Vector current_position = Vector{std::stod(vec_2.at(0)), std::stod(vec_2.at(1)), std::stod(vec_2.at(2))};
  Vector raw_instruction = Vector{std::stod(vec_3.at(0)), std::stod(vec_3.at(1)), std::stod(vec_3.at(2))};

  last_position.to_string();

  current_position.to_string();
  raw_instruction.to_string();

  vectors = std::make_tuple(last_position, current_position, raw_instruction);

  return vectors;
}

std::vector<std::vector<std::string>> get_csv_rows(std::string filename) {
  std::ifstream input{filename};
  std::vector<std::vector<std::string>> csvRows;

  if (!input.is_open()) {
    std::cerr << "Couldn't read file: " << filename << "\n";
    return csvRows;
  }

  for (std::string line; std::getline(input, line);) {
    std::istringstream ss(std::move(line));
    std::vector<std::string> row;
    if (!csvRows.empty()) {
      // We expect each row to be as big as the first row
      row.reserve(csvRows.front().size());
    }
    // std::getline can split on other characters, here we use ','
    for (std::string value; std::getline(ss, value, ',');) {
      row.push_back(std::move(value));
    }
    csvRows.push_back(std::move(row));
  }
  return csvRows;
}
void print_csvRows() {
  for (const std::vector<std::string>& row : this->get_csv_rows()) {
    for (const std::string& value : row) {
      std::cout << std::setw(10) << value;
    }
    std::cout << "\n";
  }
}

#endif /*TEST_COMMON*/
