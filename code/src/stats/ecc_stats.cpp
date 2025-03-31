#include "stats/ecc_stats.hpp"
#include <set>

#include <utility>
#include <algorithm>

#include "bfs/bfs.hpp"
#include "framework/graph.hpp"
#include "framework/random.hpp"

EccResult ecc_stats(const Graph& G){

    // Sample some nodes
    unsigned sampleSize = std::min(500u, G.n() / 20);
    std::set<unsigned> sample;
    while (sample.size() < sampleSize){
        node w = Random::natural_number(G.n() - 1);
        sample.insert(w);
    }

    // Calculate eccentricity for all of them 
    unsigned total_ecc = 0;
    std::vector<std::pair<unsigned,unsigned>> ecc_deg;
    ecc_deg.reserve(sampleSize);
    for(node w : sample){
        auto dist = BFS(G.n())(G, w);
        auto ecc = *std::max_element(dist.begin(), dist.end());
        total_ecc += ecc;
        ecc_deg.emplace_back(ecc, G.degree(w));
    }

    // calculate the median amond all ecccentricities
    std::sort(ecc_deg.begin(), ecc_deg.end());
    double median;
    if (G.n() % 2 != 0)
        median = (double)ecc_deg[G.n() / 2].first;
    else
        median = (double)(ecc_deg[(G.n() - 1) / 2].first + ecc_deg[G.n() / 2].first) / 2.0;
    
    return {
        (double) total_ecc / sampleSize,
        median, 
        sampleSize,
        ecc_deg
    };
}