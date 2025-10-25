#include<kmeans.hpp>
#include<thread>
#include <random>
#include <algorithm>
#include <cmath>

// --- helpers ---
static inline float l2sq(const float* a, const float* b, std::size_t D) {
    float s = 0.f;
    for (std::size_t i = 0; i < D; ++i) {
        float d = a[i] - b[i];
        s += d * d;
    }
    return s;
}

static void kmeanspp_init(const float* data, std::size_t N, std::size_t D, int K, unsigned int seed, std::vector<float>& centroids){
    std::mt19937 rng(seed);
    std::uniform_int_distribution<std::size_t> uni(0, N-1);

    centroids.resize(static_cast<std::size_t>(K) * D);

    std::size_t first = uni(rng);
    std::memcpy(&centroids[0], &data[first*D], D * sizeof(float));

    std::vector<float> dist2(N, std::numeric_limits<float>::max());

    for (int c = 1; c < K; ++c){
        // update D(x)^2 to nearest chosen centroid
        for (std::size_t i = 0; i < N; ++i) {
        float d = l2sq(&data[i * D], &centroids[(c - 1) * D], D);
        if (d < dist2[i]) dist2[i] = d;
        }
        // pick next center with prob ~ D(x)^2
        double sum = 0.0;
        for (float v : dist2) sum += static_cast<double>(v);
        std::uniform_real_distribution<double> u(0.0, sum);
        double r = u(rng);
        double acc = 0.0;
        std::size_t idx = 0;
        for (; idx < N; ++idx) {
            acc += static_cast<double>(dist2[idx]);
            if (acc >= r) break;
        }
        if (idx >= N) idx = N - 1;
        std::memcpy(&centroids[c * D], &data[idx * D], D * sizeof(float));
    }
}

static unsigned hw_threads() {
    unsigned t = std::thread::hardware_concurrency();
    return t ? t : 4u;
}

KMeansResult kmeans_fit(const float* data, std::size_t N, std::size_t D, const KMeansConfig& cfg){
    const int K = cfg.k;
    const int max_iters = cfg.max_iters;
    const float tol = cfg.tol;
    const unsigned threads = cfg.threads ? cfg.threads : hw_threads();

    KMeansResult R;
    R.labels.assign(N, -1);
    R.centroids.clear();
    kmeanspp_init(data, N, D, K, cfg.seed, R.centroids);

    std::vector<float> newC((std::size_t)K * D, 0.0f);
    std::vector<int> counts(K, 0);

    float prev_shift = std::numeric_limits<float>::max();

    for (int it = 0; it < max_iters; ++it) {
        std::fill(newC.begin(), newC.end(), 0.0f);
        std::fill(counts.begin(), counts.end(), 0);

        unsigned T = threads;
        std::vector<std::thread> pool;
        std::vector<std::vector<float>> partialC(T, std::vector<float>((std::size_t)K * D, 0.0f));
        std::vector<std::vector<int>> partialN(T, std::vector<int>(K, 0));

        auto worker = [&](unsigned tid){
            std::size_t chunk = (N + T - 1) / T;
            std::size_t start = tid * chunk;
            std::size_t end = std::min(N, start + chunk);
            for (std::size_t i = start; i < end; ++i) {
                const float* x = &data[i * D];
                int best = 0; float bestd = std::numeric_limits<float>::max();
                for (int c = 0; c < K; ++c) {
                    float d = l2sq(x, &R.centroids[(std::size_t)c * D], D);
                    if (d < bestd) { bestd = d; best = c; }
                }
                R.labels[i] = best;
                partialN[tid][best] += 1;
                float* acc = &partialC[tid][(std::size_t)best * D];
                for (std::size_t j = 0; j < D; ++j) acc[j] += x[j];
            }
        };

        for (unsigned t = 0; t < T; ++t) pool.emplace_back(worker, t);
        for (auto& th : pool) th.join();

        for (unsigned t = 0; t < T; ++t) {
            for (int c = 0; c < K; ++c) {
                counts[c] += partialN[t][c];
                float* dst = &newC[(std::size_t)c * D];
                const float* src = &partialC[t][(std::size_t)c * D];
                for (std::size_t j = 0; j < D; ++j) dst[j] += src[j];
            }
        }

        float shift = 0.0f;
        for (int c = 0; c < K; ++c) {
            float* cent = &R.centroids[(std::size_t)c * D];
            if (counts[c] > 0) {
                for (std::size_t j = 0; j < D; ++j) {
                    float newv = newC[(std::size_t)c * D + j] / (float)counts[c];
                    float diff = cent[j] - newv;
                    shift += diff * diff;
                    cent[j] = newv;
                }
            }
        }
        R.iters = it + 1;
        if (prev_shift > 0 && shift / (prev_shift + 1e-12f) < tol) break;
        prev_shift = shift;
    }
    double inertia = 0.0;
    unsigned T = threads;
    std::vector<std::thread> pool;
    std::vector<double> partial(T, 0.0);


    auto worker = [&](unsigned tid){
        std::size_t chunk = (N + T - 1) / T;
        std::size_t start = tid * chunk;
        std::size_t end = std::min(N, start + chunk);
        double acc = 0.0;
        for (std::size_t i = start; i < end; ++i) {
            const float* x = &data[i * D];
            int c = R.labels[i];
            acc += (double)l2sq(x, &R.centroids[(std::size_t)c * D], D);
        }
        partial[tid] = acc;
    };


    for (unsigned t = 0; t < T; ++t) pool.emplace_back(worker, t);
    for (auto& th : pool) th.join();
    for (unsigned t = 0; t < T; ++t) inertia += partial[t];


    R.inertia = (float)inertia;
    return R;
}

KMeansResult kmeans_fit(const std::vector<std::vector<float>>& X, const KMeansConfig& cfg){
    std::size_t N = X.size();
    if(N == 0) return {};

    std::size_t D = X[0].size();
    std::vector<float> buf(N*D);

    for(std::size_t i = 0; i < N; ++i) {
        std::copy(X[i].begin(), X[i].end(), &buf[i*D]);
    }
    return kmeans_fit(buf.data(), N, D, cfg);
}

std::string summarize(const KMeansResult& result, std::size_t D){
    return std::format("iters={}, intertial={}", result.iters, result.inertia);
}