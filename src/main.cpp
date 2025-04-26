#include "monitor.h"
#include <memory>
#include <fstream>
#include "parser.h"


int main() {
    std::unique_ptr<Monitor> monitor = std::make_unique<Monitor>("../test.json");

    monitor->run();
    std::string q;
    std::cin >> q;

    if (q != "") {
        monitor->stop();
    }
}