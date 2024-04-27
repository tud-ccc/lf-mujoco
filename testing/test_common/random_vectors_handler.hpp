#ifndef RANDOM_VECTORS_HANDLER
#define RANDOM_VECTORS_HANDLER

#include <vector>
#include <sstream>

#include "common_vector.hpp"

class RandomVectorsHandler {
private:
  std::vector<std::vector<std::string>> vector_of_rows_;
  std::vector<std::vector<std::string>> get_csv_rows(std::string path_to_vectors);

public:
  RandomVectorsHandler(std::string path_to_vectors);
  std::tuple<Vector, Vector, Vector> get_vector_triplet_from_csv(int index);
  void print_csvRows();
};

#endif // RANDOM_VECTORS_HANDLER