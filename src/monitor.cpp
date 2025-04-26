#include "monitor.h"

Monitor::Monitor(const std::string &pathToConfig)
{
    try {
        std::unique_ptr<IConfig> parser = std::make_unique<JsonConfig>();
        auto config = parser->parseConfig(pathToConfig);
        if (config.isCpuNeed()) {
            std::unique_ptr<IMetric> cpu_metric = std::make_unique<CPUMetric>(config.getCpuIds());
            metrics.push_back(std::move(cpu_metric));
        }
        if (config.isRamNeed()) {
            std::unique_ptr<IMetric> ram_metric = std::make_unique<RAMMetric>(config.getMemorySpecs());
            metrics.push_back(std::move(ram_metric));
        }
        if (config.isConsoleNeeded()) {
            std::unique_ptr<IOutputer> console_outputer = std::make_unique<ConsoleOutputer>(std::cout);
            outputers.push_back(std::move(console_outputer));
        }
        std::string log_path = config.getLogPath();
        if (!log_path.empty()) {
            std::unique_ptr<IOutputer> file_outputer = std::make_unique<FileOutputer>(config.getLogPath());
            outputers.push_back(std::move(file_outputer));
        }
        interval = std::chrono::milliseconds(config.period);
    } catch (std::exception& e) {
        std::cout << "Error " << e.what();
    }
}

Monitor::Monitor(const ConfigData& config)
{
    if (config.isCpuNeed()) {
        std::unique_ptr<IMetric> cpu_metric = std::make_unique<CPUMetric>(config.getCpuIds());
        metrics.push_back(std::move(cpu_metric));
    }

    if (config.isRamNeed()) {
        std::unique_ptr<IMetric> ram_metric = std::make_unique<RAMMetric>(config.getMemorySpecs());
        metrics.push_back(std::move(ram_metric));
    }

    if (config.isConsoleNeeded()) {
        std::unique_ptr<IOutputer> console_outputer = std::make_unique<ConsoleOutputer>(std::cout);
        outputers.push_back(std::move(console_outputer));
    }

    std::string log_path = config.getLogPath();

    if (!log_path.empty()) {
        std::unique_ptr<IOutputer> file_outputer = std::make_unique<FileOutputer>(config.getLogPath());
        outputers.push_back(std::move(file_outputer));
    }
    interval = std::chrono::milliseconds(config.period);
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
