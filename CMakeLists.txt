cmake_minimum_required(VERSION 3.21)
project(Kinetic! VERSION "0.0.1")

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(K2C_BT 1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(K2C_BT 2)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(K2C_BT 1)
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(K2C_BT 2)
else()
    MESSAGE( STATUS "CMAKE_BUILD_TYPE not set Building Debug ${CMAKE_BUILD_TYPE}" )
    set(K2C_BT 1)
endif()

add_compile_options(/utf-8)

# Add headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Include Config.h
configure_file(${PROJECT_SOURCE_DIR}/include/config.h.in ${PROJECT_SOURCE_DIR}/include/config.h)
include_directories(${PROJECT_BINARY_BIN}) 

# Include spdlog
include_directories(${PROJECT_SOURCE_DIR}/vendor/spdlog/include)

# Add children 
add_subdirectory(${PROJECT_SOURCE_DIR}/src/k2c/logging)

# Add Source
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Create Executable
add_executable(kinetic ${sources})

target_link_libraries(kinetic logging)

# Copy Models to Binary BuildS
# file(COPY models DESTINATION ${CMAKE_BINARY_DIR})
# file(COPY models DESTINATION ${CMAKE_BINARY_DIR}/Debug)
# file(COPY models DESTINATION ${CMAKE_BINARY_DIR}/Release)