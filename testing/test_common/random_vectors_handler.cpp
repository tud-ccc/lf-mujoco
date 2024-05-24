#include "random_vectors_handler.hpp"
#include "common_vector.hpp"

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

RandomVectorsHandler::RandomVectorsHandler(std::string path_to_vectors) {
  // retreive vectors once
  this->vector_of_triplets_ = this->get_csv_rows_vec_of_triplets(path_to_vectors);
}
std::vector<std::vector<std::string>> RandomVectorsHandler::get_csv_rows_vec_of_vecs(std::string path_to_vectors) {
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
std::vector<std::tuple<Vector, Vector, Vector>>
RandomVectorsHandler::get_csv_rows_vec_of_triplets(std::string path_to_vectors) {
  std::vector<std::vector<std::string>> csv_rows_vec_of_vecs = this->get_csv_rows_vec_of_vecs(path_to_vectors);
  assert(csv_rows_vec_of_vecs.size() % 3 == 0 && "Error: vector of VectorTriplets has an uncompatible size");

  std::vector<std::tuple<Vector, Vector, Vector>> vector_of_triplets;
  int size_of_triplet_vector = csv_rows_vec_of_vecs.size()/3 ;
  for (int i = 0; i < size_of_triplet_vector ; i++) {
    std::tuple<Vector, Vector, Vector> vector_triplet =
        this->get_vector_triplet_from_csv_rows(i * 3, csv_rows_vec_of_vecs);
    vector_of_triplets.push_back(vector_triplet);
    if (i % 1000 == 0) std::cout << "Another triplet finished" << std::endl;
  }

  std::cout << "Finished parsing the f cin Path" << std::endl;
  return vector_of_triplets;
}

std::tuple<Vector, Vector, Vector>
RandomVectorsHandler::get_vector_triplet_from_csv_rows(int index,
                                                       std::vector<std::vector<std::string>> &csv_rows_vec_of_vecs) {

  std::tuple<Vector, Vector, Vector> vectors;

  std::vector<std::string> vec_1 = csv_rows_vec_of_vecs.at(index);
  std::vector<std::string> vec_2 = csv_rows_vec_of_vecs.at(index + 1);
  std::vector<std::string> vec_3 = csv_rows_vec_of_vecs.at(index + 2);

  Vector last_position = Vector{std::stod(vec_1.at(0)), std::stod(vec_1.at(1)), std::stod(vec_1.at(2))};
  Vector current_position = Vector{std::stod(vec_2.at(0)), std::stod(vec_2.at(1)), std::stod(vec_2.at(2))};
  Vector raw_instruction = Vector{std::stod(vec_3.at(0)), std::stod(vec_3.at(1)), std::stod(vec_3.at(2))};

  vectors = std::make_tuple(last_position, current_position, raw_instruction);


  return vectors;
}

std::tuple<Vector, Vector, Vector> RandomVectorsHandler::get_vector_triplet_at_position(int index) const {
  assert(index >= 0 && index < this->vector_of_triplets_.size());
  return this->vector_of_triplets_.at(index);
}

bool RandomVectorsHandler::index_is_according_to_bounds(int index) const {
  return index >= 0 && index < this->vector_of_triplets_.size();
}
