find_package(spdlog CONFIG REQUIRED)

add_library(logger INTERFACE)

target_sources(
    logger
    INTERFACE
    ${CMAKE_SOURCE_DIR}/include/logger/logger.hpp)

target_include_directories(logger INTERFACE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(logger INTERFACE spdlog::spdlog spdlog::spdlog_header_only)