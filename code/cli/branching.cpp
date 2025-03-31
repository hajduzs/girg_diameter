#include <filesystem>
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

#include "framework/app.hpp"
#include "framework/graph.hpp"
#include "diameter/exact.hpp"


int main(int argc, char** argv) {
    App app{"Calculatine iFUB BFS tree levels and width. "};

    std::filesystem::path input_file;
    app.arg(input_file, "input_file", "Name of the file containing the graph");

    std::string algo = "ifub_foursweephd";
    app.arg(algo, "--algo", "The algorithm that should be used.",
            {"ifub_foursweephd", "ifub_hd", "random"});
    app.arg_header(
        "algo,levels");

    app.parse(argc, argv);

    Graph G(input_file);
    
    IFUBStartingNodeMethod method = IFUBStartingNodeMethod::FourSweepHD;
    if (algo == "ifub_foursweephd")
        method = IFUBStartingNodeMethod::FourSweepHD;
    else if (algo == "ifub_hd")
        method = IFUBStartingNodeMethod::HighestDegree;
    else if (algo == "random")
        method = IFUBStartingNodeMethod::UniformRandom;
    
    auto result = BFSTree(G, method).count_per_level;
    std::cout << algo << ", [";
    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << result[i];
        if (i + 1 < result.size()) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n";
    return 0;
}