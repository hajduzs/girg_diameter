#include "framework/graph.hpp"

struct IFUBResult {
    unsigned diameter;
    unsigned bfs_count;
    unsigned startvtx_degree;
    unsigned startvtx_ecc;
};

struct distanceResult {
    std::vector<unsigned> count_per_level;
};

enum class IFUBStartingNodeMethod {
    HighestDegree = 0,
    FourSweepHD = 1,
    UniformRandom = 2,
    Q1 = 3,
    Q2 = 4,
    Q3 = 5,
    Q4 = 6,
    Q5 = 7
};

IFUBResult iFUB(const Graph& G, IFUBStartingNodeMethod startingNodeMethod);
distanceResult BFSTree(const Graph& G, IFUBStartingNodeMethod startingNodeMethod);
