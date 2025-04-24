cmake_minimum_required(VERSION 3.10)
project(grayscale_service)

# Add cpp-httplib
include(FetchContent)
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.14.1
)
FetchContent_MakeAvailable(httplib)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(grayscale_service server.cpp)
target_link_libraries(grayscale_service PRIVATE
    ${OpenCV_LIBS}
    httplib::httplib
)
