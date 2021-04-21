add_library("command" command.cpp)

target_include_directories("command" PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(
    "command" 
    PUBLIC
    imagesearch control window logger
)