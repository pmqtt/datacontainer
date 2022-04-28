cmake_minimum_required(VERSION 3.16)
project(datacontainer)

set(CMAKE_CXX_STANDARD 17)


find_package(Boost REQUIRED COMPONENTS system program_options log thread date_time chrono unit_test_framework log_setup)
find_package( yaml-cpp REQUIRED )
find_package(cpprestsdk REQUIRED)
#add_definitions(-DBOOST_LOG_DYN_LINK)

include_directories(${Boost_INCLUDE_DIRS})

file(GLOB ALL_OBJ "src/*.cpp" "src/*.h")

list(REMOVE_ITEM ALL_OBJ ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_library(OBJECT_LIB OBJECT ${ALL_OBJ} )

add_executable(datacontainer src/main.cpp $<TARGET_OBJECTS:OBJECT_LIB>)

target_link_libraries(datacontainer ${Boost} pthread ${YAML_CPP_LIBRARIES} cpprestsdk::cpprest )
set_target_properties(datacontainer PROPERTIES RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")



enable_testing()
file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp )

foreach(testSrc ${TEST_SRCS})
        #Extract the filename without an extension (NAME_WE)

        get_filename_component(testName ${testSrc} NAME_WE)
        #Add compile target
        add_executable(${testName} ${testSrc} $<TARGET_OBJECTS:OBJECT_LIB>)
        message(${Boost_LIBRARIES})
        target_link_libraries(${testName} pthread ${Boost_LIBRARIES} ${YAML_CPP_LIBRARIES} )
        #I like to move testing binaries into a testBin directory
        set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/test/bin)

        #Finally add it to test execution -
        #Notice the WORKING_DIRECTORY and COMMAND
        add_test(NAME ${testName}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test/bin
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/bin/${testName} )

endforeach(testSrc)









