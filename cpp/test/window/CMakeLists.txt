add_executable(window_test window_test.cpp)

target_link_libraries(window_test PRIVATE "command" window imagesearch util GTest::gtest GTest::gtest_main)