find_package(OpenCV REQUIRED)

add_executable(window_api_test window_api_test.cpp)

target_link_libraries(window_api_test PRIVATE GTest::gtest GTest::gtest_main)

if(OpenCV_FOUND)
  # Additional Include Directories
  include_directories(${OpenCV_INCLUDE_DIRS})

  # Additional Library Directories
  link_directories(${OpenCV_LIB_DIR})

  # Additional Dependencies
  target_link_libraries(window_api_test PRIVATE ${OpenCV_LIBS})
endif()