#pragma once
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

struct MetricConfig {
    std::string type;
    nlohmann::json params;
};

struct OutputConfig {
    std::string type;
    nlohmann::json params;
};

struct ConfigData {
    int period = 30;
    std::vector<MetricConfig> metrics;
    std::vector<OutputConfig> outputs;

    std::vector<int> getCpuIds() const;
    std::vector<std::string> getMemorySpecs() const;
    std::string getLogPath() const;
    bool isConsoleNeeded() const;
    bool isCpuNeed() const;
    bool isRamNeed() const;
};

class IConfig {
public:
    virtual ~IConfig() = default;
    virtual ConfigData parseConfig(const std::string& path) = 0;
};

class JsonConfig : public IConfig {
public:
    ConfigData parseConfig(const std::string& path) override;
};