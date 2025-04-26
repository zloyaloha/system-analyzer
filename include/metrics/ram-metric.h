#include "metric.h"
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

class RAMMetric : public IMetric {
public:
    RAMMetric(const std::initializer_list<std::string> needed_stats);
    std::string getName() const override;
    std::unordered_map<std::string, std::string> calculateMetric() override;
private:
    std::unordered_set<std::string> stats;
    std::unordered_map<std::string, uint64_t> name2stat;
private:
    static std::string extractLabel(const std::string& label);
    static std::string extractStatistic(const std::string &line);
    static std::string trimLeadingWhitespace(const std::string &str);
};
