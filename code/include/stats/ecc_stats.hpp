#pragma once

#include "framework/graph.hpp"

struct EccResult {
  double avg_ecc;
  double median_ecc;
  unsigned long sample_size;
  std::vector<std::pair<unsigned, unsigned>> ED;
};


// Exact algorithm.
EccResult ecc_stats(const Graph& G);
