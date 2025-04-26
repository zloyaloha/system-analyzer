#pragma once
#include "cpu-metric.h"
#include "ram-metric.h"
#include "output.h"
#include <chrono>
#include <thread>
#include <mutex>
#include <unordered_map>

class Monitor {
public:
    Monitor();
    ~Monitor();
    void run();
    void stop();
    void addMetric(std::unique_ptr<IMetric> metric);
    void addOutputer(std::unique_ptr<IOutputer> output);
private:
    std::mutex mutex;
    std::vector<std::unique_ptr<IMetric>> metrics;
    std::vector<std::unique_ptr<IOutputer>> outputers;
    std::vector<std::thread> threads;
    std::thread printThread;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> last_metrics;
    std::chrono::milliseconds interval;
    bool running;
};