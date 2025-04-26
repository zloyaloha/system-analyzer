#include "monitor.h"

Monitor::Monitor() : interval(500)
{
    metrics.push_back(std::make_unique<CPUMetric>(std::initializer_list<int>{-1, 9, 10, 11}));
    metrics.push_back(std::make_unique<RAMMetric>(std::initializer_list<std::string>{"MemTotal", "MemFree", "MemAvailable"}));
    outputers.push_back(std::make_unique<ConsoleOutputer>(std::cout));
    // outputers.push_back(std::make_unique<FileOutputer>("/home/zloyaloha/programming/random_practice/system-analyzer/log"));
}

Monitor::~Monitor()
{
    running = false;
    for (auto& thread: threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    if (printThread.joinable()) {
        printThread.join();
    }
}

void Monitor::run()
{
    running = true;
    for (const auto& metric: metrics) {
        // вычисление метрик может быть трудоёмким, пускай считаются параллельно.
        threads.emplace_back([this, &metric]() { // по-хорошему здесь нужен пул потоков, но поскольку на данный момент метрики две, я решил ограничиться таким решением.
            while (running) {
                std::unordered_map<std::string, std::string> name2value = metric->calculateMetric();
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    last_metrics[metric->getName()] = name2value;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            }
        });
    }
    printThread = std::thread([this](){
        while (running) {
            for (const auto& metric: metrics) {
                std::lock_guard<std::mutex> lock(mutex);
                for (const auto& outputer: outputers) {
                    if (last_metrics.find(metric->getName()) != last_metrics.end()) {
                        outputer->setMetric(metric->getName(), last_metrics[metric->getName()]);
                    }
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

void Monitor::addMetric(std::unique_ptr<IMetric> metric)
{
    metrics.push_back(std::move(metric));
}

void Monitor::addOutputer(std::unique_ptr<IOutputer> outputer)
{
    outputers.push_back(std::move(outputer));
}
