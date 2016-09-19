project(testcxx)

cmake_minimum_required(VERSION 2.6)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(../libDragon/src)
include_directories(../DataCollection/src)
include_directories(../v8/include)

add_executable(test test.cpp)
target_link_libraries(test Dragon DataCollection rt pthread boost_system boost_filesystem boost_date_time boost_regex v8 aws-cpp-sdk-core aws-cpp-sdk-sqs)
