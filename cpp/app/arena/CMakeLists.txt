add_executable(arena main.cpp)

target_link_libraries(arena PRIVATE 
                        window 
                        control 
                        imagesearch 
                        task)