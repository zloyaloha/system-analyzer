#include <fstream>
#include <iostream>
#include <unordered_map>

class IOutputer {
public:
    virtual void setMetric(const std::string& name, const std::unordered_map<std::string, std::string>& value) = 0;
    virtual ~IOutputer() = default;
};

class ConsoleOutputer : public IOutputer {
public:
    ConsoleOutputer(std::ostream& os);
    void setMetric(const std::string& name, const std::unordered_map<std::string, std::string>& value);
private:
    std::ostream& stream;
};

class FileOutputer : public IOutputer {
public:
    FileOutputer(const std::string& file_path);
    void setMetric(const std::string& name, const std::unordered_map<std::string, std::string>& value);
    ~FileOutputer();
private:
    std::ofstream stream;
};