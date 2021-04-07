cmake_minimum_required(VERSION 3.18)

set(ENV{VCPKG_ROOT} "C:\\Users\\koalayt\\Documents")
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "")

project(cpp-tests)

# add_definitions(-DPSAPI_VERSION=1)

# find_library (PSAPI Psapi)

# target_link_libraries(main -lpsapi)

find_package(OpenCV CONFIG REQUIRED)
if(OpenCV_FOUND)
  # Additional Include Directories
  include_directories(${OpenCV_INCLUDE_DIRS})
endif()

add_subdirectory(app)
add_subdirectory(lib)
add_subdirectory(test)