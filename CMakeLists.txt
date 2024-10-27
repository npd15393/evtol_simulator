cmake_minimum_required(VERSION 3.14)
project(evtol_sim)

set(CMAKE_CXX_STANDARD 11)

include_directories(include src thirdparty)

add_library(
  ${PROJECT_NAME}
  SHARED
  include/simulator.hpp
  include/vehicle_type.hpp
  src/simulator.cpp
  src/vehicle_type.cpp)

add_executable(${PROJECT_NAME}_exe src/main.cpp)

## Testing ##
if(BUILD_TESTING)
    # Fetch Google Test
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2  # Use a specific GTest version
    )
    # Makes GTest available in this build
    FetchContent_MakeAvailable(googletest)

    # Create the test executable
    add_executable(${PROJECT_NAME}_tests test/test_vehicle_type.cpp)

    # Link GTest and the main project library (if applicable)
    target_link_libraries(${PROJECT_NAME}_tests PRIVATE gtest_main "${PROJECT_NAME}")

    # Add Google Test's test discovery for CMake
    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}_tests)
endif()
