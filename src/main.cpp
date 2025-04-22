#include "monitor.h"
#include <memory>

int main() {
    std::unique_ptr<Monitor> monitor = std::make_unique<Monitor>();

    monitor->run();
    std::string q;
    std::cin >> q;

    if (q != "") {
        monitor->stop();
    }
}