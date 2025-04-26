#include "ram-metric.h"

RAMMetric::RAMMetric(const std::initializer_list<std::string> stats) : needed_stats(stats) {}

RAMMetric::RAMMetric(const std::vector<std::string> &specs) : needed_stats(specs.begin(), specs.end()) {}

std::string RAMMetric::getName() const
{
    return "RAM";
}

std::unordered_map<std::string, std::string> RAMMetric::calculateMetric()
{
    std::unordered_map<std::string, std::string> stat2value;
    std::string line;
    std::string label;
    std::ifstream file("/proc/meminfo");
    while (std::getline(file, line)) {
        label = extractLabel(line);
        if (needed_stats.count(label) > 0) {
            stat2value[label] = extractStatistic(line);
        }
    }
    return stat2value;
}

std::string RAMMetric::extractLabel(const std::string &line)
{
    size_t colon_pos = line.find(':');
    if (colon_pos != std::string::npos) {
        return line.substr(0, colon_pos);
    }
    return "";
}

std::string RAMMetric::extractStatistic(const std::string& line)
{
    size_t space_pos = line.find(' ');
    if (space_pos != std::string::npos && space_pos + 1 < line.size()) {
        return trimLeadingWhitespace(line.substr(space_pos + 1));
    }
    return "";
}

std::string RAMMetric::trimLeadingWhitespace(const std::string& str) {
    auto first = std::find_if(str.begin(), str.end(), [](int ch) {
        return !std::isspace(ch);
    });
    return std::string(first, str.end());
}