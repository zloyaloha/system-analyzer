#include "output.h"

ConsoleOutputer::ConsoleOutputer(std::ostream &os = std::cout) : stream(os) {}

void ConsoleOutputer::setMetric(const std::string &name, const std::string &value)
{
    if (value != "") {
        stream << name << '\n' << value << '\n';
    }
}

FileOutputer::FileOutputer(const std::string &file_path) : stream(file_path)
{
    if (!stream.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + file_path);
    }
}

void FileOutputer::setMetric(const std::string &name, const std::string &value)
{
    if (value != "") {
        stream << name << '\n' << value << '\n';
    }
}

FileOutputer::~FileOutputer()
{
    stream << std::flush;
}
