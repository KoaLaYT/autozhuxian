add_library(imagesearch screenshot.cpp)

target_include_directories(imagesearch PUBLIC ${CMAKE_SOURCE_DIR}/include)

if(OpenCV_FOUND)
  # Additional Library Directories
  link_directories(${OpenCV_LIB_DIR})

  # Additional Dependencies
  target_link_libraries(imagesearch PRIVATE ${OpenCV_LIBS})
endif()