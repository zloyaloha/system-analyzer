#include "monitor.h"

Monitor::Monitor() : interval(25)
{
    metrics.push_back(std::make_unique<CPUMetric>(std::initializer_list<int>{-1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}));
    outputers.push_back(std::make_unique<ConsoleOutputer>(std::cout));
    outputers.push_back(std::make_unique<FileOutputer>("/home/zloyaloha/programming/random_practice/system-analyzer/log"));
}

Monitor::~Monitor()
{
    running = false;
    for (auto& thread: threads) {
        thread.join();
    }
    if (printThread.joinable()) {
        printThread.join();
    }
}

void Monitor::run()
{
    running = true;
    for (const auto& metric: metrics) {
        threads.emplace_back([this, &metric]() {
            while (running) {
                std::string metric_value = metric->calculateMetric();
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    last_metrics[metric->getName()] = metric_value;
                }
                std::this_thread::sleep_for(interval);
            }
        });
    }
    printThread = std::thread([this](){
        while (running) {
            for (const auto& metric: metrics) {
                std::lock_guard<std::mutex> lock(mutex);
                for (const auto& outputer: outputers) {
                    outputer->setMetric(metric->getName(), last_metrics[metric->getName()]);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    });
}

void Monitor::stop()
{
    running = false;
}