#include "parser.h"

ConfigData JsonConfig::parseConfig(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open config file: " + path);
    }

    nlohmann::json json;
    file >> json;

    ConfigData config;

    if (json.contains("settings")) {
        const auto& settings = json["settings"];
        if (settings.contains("period")) {
            config.period = settings["period"].get<int>();
        }
    }

    if (json.contains("metrics")) {
        for (const auto& metric : json["metrics"]) {
            MetricConfig mc;
            mc.type = metric["type"].get<std::string>();
            mc.params = metric;
            config.metrics.push_back(mc);
        }
    }

    if (json.contains("outputs")) {
        for (const auto& output : json["outputs"]) {
            OutputConfig oc;
            oc.type = output["type"].get<std::string>();
            oc.params = output;
            config.outputs.push_back(oc);
        }
    }

    return config;
}

std::vector<int> ConfigData::getCpuIds() const {
    for (const auto& m : metrics) {
        if (m.type == "cpu") {
            return m.params["ids"].get<std::vector<int>>();
        }
    }
    return {};
}

std::vector<std::string> ConfigData::getMemorySpecs() const {
    for (const auto& m : metrics) {
        if (m.type == "memory") {
            return m.params["spec"].get<std::vector<std::string>>();
        }
    }
    return {};
}

std::string ConfigData::getLogPath() const {
    for (const auto& o : outputs) {
        if (o.type == "log") {
            return o.params["path"].get<std::string>();
        }
    }
    return "";
}

bool ConfigData::isConsoleNeeded() const {
    for (const auto& o : outputs) {
        if (o.type == "console") {
            return true;
        }
    }
    return false;
}

bool ConfigData::isCpuNeed() const
{
    return std::find_if(metrics.begin(), metrics.end(), [](const MetricConfig& m) { return m.type == "cpu"; }) != metrics.end();
}

bool ConfigData::isRamNeed() const
{
    return std::find_if(metrics.begin(), metrics.end(), [](const MetricConfig& m) { return m.type == "memory"; }) != metrics.end();
}
