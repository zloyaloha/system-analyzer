#pragma once
#include "metric.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <memory>

struct CpuNameComparator {
    bool operator()(const std::string& a, const std::string& b) const {
        if (a.size() < b.size()) {
            return true;
        } else if (a.size() > b.size()) {
            return false;
        } else {
            if (a.size() == 3) {
                return false;
            }
            auto extractA = std::stoi(a.substr(3));
            auto extractB = std::stoi(b.substr(3));
            return extractA < extractB;
        }
    }
};


class CPUMetric: public IMetric {
public:
    CPUMetric(const std::initializer_list<int> &ids);
    CPUMetric(const std::vector<int>& cores);
    std::string getName() const override;
    std::unordered_map<std::string, std::string> calculateMetric() override;
private:
    struct CpuData {
        unsigned long user;
        unsigned long nice;
        unsigned long system;
        unsigned long idle;
        unsigned long iowait;
        unsigned long irq;
        unsigned long softirq;
        unsigned long steal;
    };

    std::map<std::string, CpuData, CpuNameComparator> curr_val;
    std::map<std::string, CpuData, CpuNameComparator> prev_val;

    std::unordered_set<int> needed_cores;
    private:
    bool checkCoreIsNeeded(const std::string coreNumber) const;
    std::map<std::string, CpuData, CpuNameComparator> readCpuData() const;
    std::unordered_map<std::string, std::string> calculateCpuUsage() const;
    std::string calculateCoreUsage(const CpuData &curr_data, const CpuData &prev_data) const;
};