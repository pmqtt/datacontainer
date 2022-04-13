cmake_minimum_required(VERSION 3.16)
project(datacontainer)

set(CMAKE_CXX_STANDARD 17)


find_package(Boost REQUIRED COMPONENTS system program_options log thread date_time chrono unit_test_framework fiber log_setup)
add_definitions(-DBOOST_LOG_DYN_LINK)

include_directories(${Boost_INCLUDE_DIRS} $ENV{SOC_API})
link_directories($ENV{SOC_API}/lib )

file(GLOB ALL_OBJ "*.cpp" "*.h")

list(REMOVE_ITEM ALL_OBJ ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_library(OBJECT_LIB OBJECT ${ALL_OBJ} )

add_executable(datacontainer main.cpp $<TARGET_OBJECTS:OBJECT_LIB>)

target_link_libraries(datacontainer soc  ${Boost_LIBRARIES} pthread paho-mqttpp3 paho-mqtt3a paho-mqtt3as)
set_target_properties(datacontainer PROPERTIES RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")