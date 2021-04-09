add_library(window window.cpp)

target_include_directories(window PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(window PUBLIC task imagesearch control)
