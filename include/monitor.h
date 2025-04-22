#pragma once
#include "metrics.h"
#include "output.h"
#include <chrono>
#include <thread>
#include <mutex>
#include <unordered_map>

class Monitor {
public:
    Monitor();
    ~Monitor();
    // Monitor(const Config& config);
    void run();
    void stop();
    void collectMetrics();

private:
    std::mutex mutex;
    std::vector<std::unique_ptr<IMetric>> metrics;
    std::vector<std::unique_ptr<IOutputer>> outputers;
    std::vector<std::thread> threads;
    std::thread printThread;
    std::unordered_map<std::string, std::string> last_metrics;
    std::chrono::milliseconds interval;
    bool running;
};