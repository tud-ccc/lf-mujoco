#ifndef RANDOM_VECTORS_HANDLER
#define RANDOM_VECTORS_HANDLER

#include <sstream>
#include <vector>

#include "common_vector.hpp"

class RandomVectorsHandler {
private:
  std::vector<std::tuple<Vector, Vector, Vector>> vector_of_triplets_;
  std::vector<std::vector<std::string>> get_csv_rows_vec_of_vecs(std::string path_to_vectors);
  std::tuple<Vector, Vector, Vector>
  get_vector_triplet_from_csv_rows(int index, std::vector<std::vector<std::string>> csv_rows_vec_of_vecs);
  std::vector<std::tuple<Vector, Vector, Vector>> get_csv_rows_vec_of_triplets(std::string path_to_vectors);

public:
  RandomVectorsHandler(std::string path_to_vectors);
  std::tuple<Vector, Vector, Vector> get_vector_triplet_at_position(int index) const;
  bool index_is_according_to_bounds(int index) const;
};

#endif // RANDOM_VECTORS_HANDLER