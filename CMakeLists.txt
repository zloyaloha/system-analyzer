cmake_minimum_required(VERSION 3.12)
project("system-analyzer")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS "-fsanitize=address")

add_compile_options(-Wall -Wextra -Wpedantic -Werror)
find_package(nlohmann_json 3.2.0 REQUIRED)

set(SOURCES
    src/metrics/cpu-metric.cpp
    src/metrics/ram-metric.cpp
    src/monitor.cpp
    src/main.cpp
    src/output.cpp
    src/parser.cpp
)

set(HEADERS
    include/metrics/metric.h
    include/metrics/cpu-metric.h
    include/metrics/ram-metric.h
    include/monitor.h
    include/output.h
    include/parser.h
)

add_executable(linux_monitor ${SOURCES} ${HEADERS})

target_include_directories(linux_monitor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/include/metrics
)

target_link_libraries(linux_monitor PRIVATE
    nlohmann_json::nlohmann_json
)