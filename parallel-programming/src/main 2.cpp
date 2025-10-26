#include<iostream>
#include<random>
#include "kmeans.hpp"
#include<timer.hpp>

static void gen_blobs(std::vector<float>& X, std::size_t N, std::size_t D, int K, unsigned seed){
    std::mt19937 rng(seed);
    std::normal_distribution<float> noise(0.f, 0.5f);
    std::uniform_real_distribution<float> uni(-10.f, 10.f);

    std::vector<float> centers((std::size_t) K * D);

    for (int c = 0; c < K; ++c) {
        for (std::size_t j = 0; j < D; ++j) centers[(std::size_t)c * D + j] = uni(rng);
    }

    X.resize(N * D);

    for (std::size_t i = 0; i < N; ++i) {
        int cid = (int)(i % (std::size_t)K);
        for (std::size_t j = 0; j < D; ++j) {
            X[i * D + j] = centers[(std::size_t)cid * D + j] + noise(rng);
        }
    }
}

int main(int argc, char** argv) {
    // Defaults
std::size_t N = 200000; // points
std::size_t D = 32; // dims
int K = 64;
int max_iters = 50;
int threads = 0;


// Parse quick flags
for (int i = 1; i < argc; ++i) {
    if (!std::strcmp(argv[i], "--n") && i + 1 < argc) N = std::stoull(argv[++i]);
    else if (!std::strcmp(argv[i], "--d") && i + 1 < argc) D = std::stoull(argv[++i]);
    else if (!std::strcmp(argv[i], "--k") && i + 1 < argc) K = std::stoi(argv[++i]);
    else if (!std::strcmp(argv[i], "--max-iters") && i + 1 < argc) max_iters = std::stoi(argv[++i]);
    else if (!std::strcmp(argv[i], "--threads") && i + 1 < argc) threads = std::stoi(argv[++i]);
}


std::vector<float> X;
    gen_blobs(X, N, D, K, 123);

    KMeansConfig cfg; cfg.k = K; cfg.max_iters = max_iters; cfg.threads = threads; cfg.seed = 1234;

    Timer t; t.reset();
    auto res = kmeans_fit(X.data(), N, D, cfg);
    double elapsed = t.ms();

    std::cout << "KMeans N=" << N << " D=" << D << " K=" << K
    << " iters=" << res.iters
    << " inertia=" << res.inertia
    << " time_ms=" << elapsed << std::endl;


    return 0;
}