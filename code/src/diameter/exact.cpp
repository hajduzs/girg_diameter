#include "diameter/exact.hpp"

#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector> 

#include "bfs/bfs.hpp"
#include "diameter/eccentricity.hpp"
#include "diameter/sweep.hpp"
#include "framework/random.hpp"

// Adapted from https://who.rocq.inria.fr/Laurent.Viennot/road/papers/ifub.pdf
// Returns exact diameter, and number of BFSs
IFUBResult iFUB(const Graph& G, IFUBStartingNodeMethod startingNodeMethod) {
    // Find node of highest degree
    unsigned node_hd = 0;
    for (unsigned i = 0; i < G.n(); ++i)
        if (G.degree(i) > G.degree(node_hd))
            node_hd = i;

    unsigned starting_node;
    unsigned bfs_count = 0;
    if (startingNodeMethod == IFUBStartingNodeMethod::HighestDegree) {
        starting_node = node_hd;
    } else if (startingNodeMethod == IFUBStartingNodeMethod::FourSweepHD) {
        auto [_, mid] = four_sweep(G, node_hd);
        starting_node = mid;
        bfs_count += 4;
    } else if (startingNodeMethod == IFUBStartingNodeMethod::UniformRandom){
        starting_node = Random::natural_number(G.n() -1);
    } else {
        // random vertices 
        int q;
        switch (startingNodeMethod)
        {
        case IFUBStartingNodeMethod::Q1:
            q = 0;
            break;
        case IFUBStartingNodeMethod::Q2:
            q = 1;
            break;
        case IFUBStartingNodeMethod::Q3:
            q = 2;
            break;
        case IFUBStartingNodeMethod::Q4:
            q = 3;
            break;
        default:
            q = 4;
            break;
        }
        // now we calculate the log-scale division
        std::vector<std::pair<unsigned, unsigned>> deg_index;
        deg_index.reserve(G.n());
        for(unsigned i = 0; i < G.n(); ++i)
            deg_index.emplace_back(G.degree(i), i);
        std::sort(deg_index.begin(), deg_index.end());
        int d_min = deg_index.front().first;
        int d_max = deg_index.back().first;
        double log_d = std::log10(d_min) + q * (std::log10(d_max) - std::log10(d_min)) / 4;
        unsigned target_d = static_cast<unsigned>(std::pow(10, log_d));
        
        auto it = std::lower_bound(deg_index.begin(), deg_index.end(), std::make_pair(target_d, 0),
        [](const std::pair<unsigned, unsigned> & a, const std::pair<unsigned, unsigned>& b) {return a.first < b.first;}
        );
        starting_node = it->second;
    }

    auto dist = BFS(G.n())(G, starting_node);
    auto lb = *std::max_element(dist.begin(), dist.end());
    auto ecc = lb;
    bfs_count += 1;

    // Group nodes by distance from u
    int i = *std::max_element(dist.begin(), dist.end());
    std::vector<std::vector<unsigned>> nodes_by_layer(i + 1);
    for (unsigned v = 0; v < G.n(); ++v) {
        nodes_by_layer[dist[v]].emplace_back(v);
    }

    for (; i >= 0; --i) {
        unsigned remaining_ub = 2 * i;
        for (auto v : nodes_by_layer[i]) {
            lb = std::max(lb, eccentricity(G, v));
            bfs_count++;
            if (lb >= remaining_ub)
                return {lb, bfs_count, G.degree(starting_node), ecc};

        }
    }
    return {lb, bfs_count, G.degree(starting_node), ecc};
}

distanceResult BFSTree(const Graph& G, IFUBStartingNodeMethod startingNodeMethod) {
    unsigned node_hd = 0;
    for (unsigned i = 0; i < G.n(); ++i)
        if (G.degree(i) > G.degree(node_hd))
            node_hd = i;

    unsigned starting_node;
    if (startingNodeMethod == IFUBStartingNodeMethod::HighestDegree) {
        starting_node = node_hd;
    } else if (startingNodeMethod == IFUBStartingNodeMethod::FourSweepHD) {
        auto [_, mid] = four_sweep(G, node_hd);
        starting_node = mid;
    } else if (startingNodeMethod == IFUBStartingNodeMethod::UniformRandom){
        starting_node = Random::natural_number(G.n() -1);
    }

    // Get the BFS results! 
    auto dist = BFS(G.n())(G, starting_node);
    auto i = *std::max_element(dist.begin(), dist.end());
    std::vector<unsigned> V(i + 1);
    for (unsigned v = 0; v < G.n(); ++v) {
        V[dist[v]] += 1;
    }
    return {V};

}
