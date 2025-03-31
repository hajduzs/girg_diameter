#include <iostream>

#include "framework/app.hpp"
#include "framework/graph_operations.hpp"
#include "framework/timer.hpp"
#include "diameter/exact.hpp"
#include "framework/random.hpp"

int main(int argc, char** argv) {
  App app{"Calculating the exact diameter via iFUB."};

  std::filesystem::path input_file;
  app.arg(input_file, "input_file", "Name of the file containing the graph");

  std::string algo = "ifub_foursweephd";
  app.arg(algo, "--algo", "The algorithm that should be used.",
          {"ifub_foursweephd", "ifub_hd", "random", "q1", "q2", "q3", "q4", "q5"});
  app.arg_header(
      "algo,diameter,bfs_count,startvtx_degree,startvtx_ecc,time_diameter_exact");

  app.parse(argc, argv);

  Graph G(input_file);
  IFUBStartingNodeMethod method = IFUBStartingNodeMethod::FourSweepHD;
  if (algo == "ifub_foursweephd")
    method = IFUBStartingNodeMethod::FourSweepHD;
  else if (algo == "ifub_hd")
    method = IFUBStartingNodeMethod::HighestDegree;
  else if (algo == "random")
    method = IFUBStartingNodeMethod::UniformRandom;
  else if (algo == "q1")
    method = IFUBStartingNodeMethod::Q1;
  else if (algo == "q2")
    method = IFUBStartingNodeMethod::Q2;
  else if (algo == "q3")
    method = IFUBStartingNodeMethod::Q3;
  else if (algo == "q4")
    method = IFUBStartingNodeMethod::Q4;
  else if (algo == "q5")
    method = IFUBStartingNodeMethod::Q5;

  Timer::create_timer("diameter_exact");
  Timer::start_timer("diameter_exact");
  auto ifub_result = iFUB(G, method);
  Timer::stop_timer("diameter_exact");


  std::cout << algo << "," << ifub_result.diameter << "," << ifub_result.bfs_count << "," 
  << ifub_result.startvtx_degree << "," << ifub_result.startvtx_ecc << ","
  << Timer::time("diameter_exact").count() << std::endl;

  return 0;
}
