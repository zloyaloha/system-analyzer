#include "output.h"

ConsoleOutputer::ConsoleOutputer(std::ostream &os = std::cout) : stream(os) {}

void ConsoleOutputer::setMetric(const std::string &name, const std::unordered_map<std::string, std::string>& stat2value)
{
    stream << name << '\n';
    for (const auto& [stat_name, value]: stat2value) {
        stream << stat_name << ": " << value << "   ";
    }
    stream << "\n\n";
}

FileOutputer::FileOutputer(const std::string &file_path) : stream(file_path)
{
    if (!stream.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + file_path);
    }
}

void FileOutputer::setMetric(const std::string &name, const std::unordered_map<std::string, std::string>& stat2value)
{
    stream << name << '\n';
    for (const auto& [stat_name, value]: stat2value) {
        stream << stat_name << ": " << value << "   ";
    }
    stream << "\n\n";
}

FileOutputer::~FileOutputer()
{
    stream << std::flush;
}
