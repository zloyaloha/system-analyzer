#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <memory>

class IMetric {
    public:
    virtual std::string getName() const = 0;
    virtual std::unordered_map<std::string, std::string> calculateMetric() = 0;
    virtual ~IMetric() = default;
};

