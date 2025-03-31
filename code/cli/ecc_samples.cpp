#include "stats/ecc_stats.hpp"

#include <iostream>

#include "framework/app.hpp"
#include "framework/graph.hpp"

constexpr auto description =
    R"(Compute stats on the eccentricity of randomly sampled nodes)";

constexpr auto footer =
    R"(The eccentriccity of a node is the length of the longest shortest path to any other node.)";

int main(int argc, char** argv) {
  App app{description};
  app.cli_app.footer(footer);

  std::filesystem::path input_file;
  app.arg(input_file, "input_file", "Name of the file containing the graph.");

  app.arg_header("avg_ecc, median_ecc, sampleSize, ecc_to_degree");

  app.parse(argc, argv);

  Graph G(input_file);
  EccResult res = ecc_stats(G);
  std::cout << res.avg_ecc << "," << res.median_ecc << "," <<
  res.sample_size << ",";
  std::cout << "[";
  for (unsigned i = 0; i < res.sample_size; ++i) {
      std::cout << "[" << res.ED[i].first << ", " << res.ED[i].second << "]";
      if (i != res.sample_size - 1) {
          std::cout << ",";
      }
  }
  std::cout << "]" << std::endl;

  return 0;
}
