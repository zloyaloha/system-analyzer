#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <map>
#include <memory>

class IMetric {
    public:
    virtual std::string getName() = 0;
    virtual std::string calculateMetric() = 0;
    virtual ~IMetric() = default;
};

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
    std::string getName() override;
    std::string calculateMetric() override;
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

    std::map<std::string, CpuData, CpuNameComparator> readCpuData();
    std::string calculateCpuUsage();
    double calculateCoreUsage(const CpuData &curr_data, const CpuData &prev_data);

    std::unordered_set<int> needed_cores;

    int iter;
private:
    bool checkCoreIsNeeded(const std::string coreNumber);
};

