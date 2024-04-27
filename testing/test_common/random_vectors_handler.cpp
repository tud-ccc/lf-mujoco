#include "random_vectors_handler.hpp"
#include "common_vector.hpp"


#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

RandomVectorsHandler::RandomVectorsHandler(std::string path_to_vectors) {

  //retreive vectors once 
  this->vector_of_rows_ = this->get_csv_rows(path_to_vectors);

}
std::vector<std::vector<std::string>> RandomVectorsHandler::get_csv_rows(std::string path_to_vectors) {
  std::ifstream input{path_to_vectors};
  std::vector<std::vector<std::string>> csvRows;

  if (!input.is_open()) {
    std::cerr << "Couldn't read file: " << path_to_vectors << "\n";
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

std::tuple<Vector, Vector, Vector> RandomVectorsHandler::get_vector_triplet_from_csv(int index) {
  std::tuple<Vector, Vector, Vector> vectors;

  std::vector<std::string> vec_1 = this->vector_of_rows_.at(index);
  std::vector<std::string> vec_2 = this->vector_of_rows_.at(index + 1);
  std::vector<std::string> vec_3 = this->vector_of_rows_.at(index + 2);

  Vector last_position = Vector{std::stod(vec_1.at(0)), std::stod(vec_1.at(1)), std::stod(vec_1.at(2))};
  Vector current_position = Vector{std::stod(vec_2.at(0)), std::stod(vec_2.at(1)), std::stod(vec_2.at(2))};
  Vector raw_instruction = Vector{std::stod(vec_3.at(0)), std::stod(vec_3.at(1)), std::stod(vec_3.at(2))};

  last_position.to_string();

  current_position.to_string();
  raw_instruction.to_string();

  vectors = std::make_tuple(last_position, current_position, raw_instruction);

  return vectors;
}


void RandomVectorsHandler::print_csvRows() {
  for (const std::vector<std::string>& row : this->vector_of_rows_) {
    for (const std::string& value : row) {
      std::cout << std::setw(10) << value;
    }
    std::cout << "\n";
  }
}
