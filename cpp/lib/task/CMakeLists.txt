add_library(task command.cpp common.cpp stateful.cpp)

target_include_directories(task PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(
    task
    PUBLIC
    imagesearch control window logger macro
)