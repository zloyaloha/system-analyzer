cmake_minimum_required(VERSION 3.12)
project("system-analyzer")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fsanitize=address")

add_compile_options(-Wall -Wextra -Wpedantic -Werror)
find_package(nlohmann_json 3.2.0 REQUIRED)

set(SOURCES
    src/monitor.cpp
    src/metrics.cpp
    src/main.cpp
    src/output.cpp
)

include_directories(include)

set(HEADERS
    include/metrics.h
    include/monitor.h
    include/output.h
)

add_executable(linux_monitor ${SOURCES} ${HEADERS})

target_link_libraries(linux_monitor PRIVATE
    nlohmann_json::nlohmann_json
)