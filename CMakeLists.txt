# CMakeLists.txt

cmake_minimum_required(VERSION 3.15)
project(ImageFilter)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

# optimization and profiling flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -Wall -Wextra")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
set(CMAKE_CXX_FLAGS_DEBUG "-O2 -DBENCHMARK -g ")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Find OpenCV
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} include)
aux_source_directory(src DIR_SRC)

add_executable(img_filter ${DIR_SRC})

target_link_libraries(img_filter ${OpenCV_LIBS})
