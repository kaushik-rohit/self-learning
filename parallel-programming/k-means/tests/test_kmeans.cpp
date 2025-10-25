#include<kmeans.hpp>
#include<vector>
#include <cassert>
#include <iostream>

int main() {
    std::vector<std::vector<float>> X = {
        {0.f, 0.f}, {0.1f, -0.1f}, {-0.2f, 0.2f},
        {5.f, 5.f}, {5.2f, 4.8f}, {4.9f, 5.1f}
    };

    KMeansConfig cfg;
    cfg.k = 2;
    cfg.max_iters = 100;
    cfg.tol = 0.0001;
    cfg.threads = 2;
    cfg.seed = 7;
    auto res = kmeans_fit(X, cfg);

    int a = res.labels[0];
    int b = res.labels[3];

    assert(a != b);
    for(int i = 0; i < 3; ++i) assert(res.labels[i] == a);
    for(int i = 3; i < 6; ++i) assert(res.labels[i] == b);

    std::cout<<"OK\n";
    return 0;
}