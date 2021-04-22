add_executable(login main.cpp task.cpp)

target_link_libraries(login PRIVATE window control imagesearch task logger)