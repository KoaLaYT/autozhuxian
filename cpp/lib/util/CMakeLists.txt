add_library(util common_task.cpp)

target_include_directories(util PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(util PUBLIC imagesearch "command" control window)