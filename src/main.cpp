#include "monitor.h"
#include <memory>
#include <fstream>
#include "parser.h"


int main() {
    try {
        std::unique_ptr<IConfig> parser = std::make_unique<JsonConfig>();
        auto config = parser->parseConfig("/home/zloyaloha/programming/random_practice/system-analyzer/test.json");

        std::cout << "Monitoring period: " << config.period << " sec\n";

        for (const auto& cpu_id : config.getCpuIds()) {
            std::cout << "Monitoring CPU core: " << cpu_id << "\n";
        }

        for (const auto& name : config.getMemorySpecs()) {
            std::cout << "Monitoring RAM stats: " << name << "\n";
        }

        if (!config.getLogPath().empty()) {
            std::cout << "Logging to: " << config.getLogPath() << "\n";
        }

        if (config.isConsoleNeeded()) {
            std::cout << "Console Need" << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // std::unique_ptr<Monitor> monitor = std::make_unique<Monitor>();

    // monitor->run();
    // std::string q;
    // std::cin >> q;

    // if (q != "") {
    //     monitor->stop();
    // }
}