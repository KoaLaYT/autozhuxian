add_library(macro INTERFACE)

target_sources(
        "macro"
        INTERFACE
        ${CMAKE_SOURCE_DIR}/include/macro/assert.hpp
        ${CMAKE_SOURCE_DIR}/include/macro/expect.hpp
)

target_include_directories(macro INTERFACE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(macro INTERFACE fmt::fmt)