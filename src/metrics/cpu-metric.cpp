#include "cpu-metric.h"

using ull = unsigned long long;

CPUMetric::CPUMetric(const std::initializer_list<int>& ids) : needed_cores(ids)
{
    curr_val = readCpuData();
}

std::string CPUMetric::getName() const
{
    return "CPU";
}

std::unordered_map<std::string, std::string> CPUMetric::calculateMetric()
{
    prev_val = curr_val;
    curr_val = readCpuData();
    return calculateCpuUsage();
}

std::map<std::string, CPUMetric::CpuData, CpuNameComparator> CPUMetric::readCpuData()
{
    std::ifstream file("/proc/stat");
    std::string line;
    std::map<std::string, CPUMetric::CpuData, CpuNameComparator> cpu_data;
    while (std::getline(file, line)) {
        if (line.substr(0, 3) == "cpu") {
            std::istringstream iss(line);
            CpuData data;
            std::string label;

            iss >> label >> data.user >> data.nice >> data.system >> data.idle >> data.iowait >> data.irq >> data.softirq >> data.steal;

            if (checkCoreIsNeeded(label)) {
                cpu_data[label] = std::move(data);
            }

        }
    }
    return cpu_data;
}

std::unordered_map<std::string, std::string> CPUMetric::calculateCpuUsage()
{
    std::stringstream ss;
    std::unordered_map<std::string, std::string> core2stat;
    for (const auto& [core_name, data]: prev_val) {
        core2stat[core_name] = calculateCoreUsage(curr_val.at(core_name), data);
    }
    return core2stat;
}


std::string CPUMetric::calculateCoreUsage(const CpuData& curr_data, const CpuData& prev_data)
{
    const ull Ud = curr_data.user - prev_data.user;
    const ull Nd = curr_data.nice - prev_data.nice;
    const ull Sd = curr_data.system - prev_data.system;
    const ull Id = curr_data.idle - prev_data.idle;
    const ull Iwd = curr_data.iowait - prev_data.iowait;
    const ull Irqd = curr_data.irq - prev_data.irq;
    const ull Sirqd = curr_data.softirq - prev_data.softirq;
    const ull Std = curr_data.steal - prev_data.steal;

    const ull i = Id + Iwd;
    const ull b = Ud + Nd + Sd + Irqd + Sirqd + Std;
    const ull total = std::max(i + b, 1ULL);

    if (total == 0) return 0;

    return std::to_string(100.0 * static_cast<double>(b) / total) + "%";
}

bool CPUMetric::checkCoreIsNeeded(const std::string coreLabel)
{
    if (coreLabel == "cpu") {
        return needed_cores.count(-1) > 0;
    } else {
        std::string number = coreLabel.substr(3);
        return needed_cores.count(std::stoi(number)) > 0;
    }
}
