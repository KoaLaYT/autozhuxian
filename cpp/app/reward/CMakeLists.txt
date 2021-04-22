add_executable(reward main.cpp)

target_link_libraries(reward PRIVATE 
                        window 
                        control 
                        imagesearch 
                        task)