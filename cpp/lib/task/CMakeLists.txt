add_library(task task.cpp)

target_include_directories(task PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(
    task 
    PUBLIC
    imagesearch control window
)