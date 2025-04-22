#include <fstream>
#include <iostream>

class IOutputer {
public:
    virtual void setMetric(const std::string& name, const std::string& value) = 0;
    virtual ~IOutputer() = default;
};

class ConsoleOutputer : public IOutputer {
public:
    ConsoleOutputer(std::ostream& os);
    void setMetric(const std::string& name, const std::string& value);
private:
    std::ostream& stream;
};

class FileOutputer : public IOutputer {
public:
    FileOutputer(const std::string& file_path);
    void setMetric(const std::string& name, const std::string& value);
    ~FileOutputer();
private:
    std::ofstream stream;
};