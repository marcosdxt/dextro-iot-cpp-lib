cmake_minimum_required(VERSION 3.10)
project(dextro-iot-cpp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opções de Build
option(BUILD_EXAMPLES "Build example applications" ON)
option(BUILD_TESTS "Build unit tests" ON)

# Includes
include_directories(include)

# Inclui MQTT-C
add_definitions(-DMQTT_PAL_POSIX)
add_subdirectory(external/mqtt-c)
include_directories(external/mqtt-c/include)

# Biblioteca Principal
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SOURCES})

# Target de include para consumidores
target_include_directories(${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PRIVATE mqttc)

# Exemplos
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Testes
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
