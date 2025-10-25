#pragma once
#include <vector>
#include <cstddef>
#include <string>

struct KMeansConfig {
    int k = 8;
    int max_iters = 100;
    float tol = 1e-4f;
    int threads = 0;
    unsigned int seed = 42;
};

struct KMeansResult {
    std::vector<int> labels;
    std::vector<float> centroids;
    int iters = 0;
    float inertia = 0.0f;
};

// Fit K-Means on row major data
KMeansResult kmeans_fit(const float* data, std::size_t N, std::size_t D, const KMeansConfig& cfg);

KMeansResult kmeans_fit(const std::vector<std::vector<float>>& X, const KMeansConfig& cfg);

std::string summarize(const KMeansResult& r, std::size_t D);