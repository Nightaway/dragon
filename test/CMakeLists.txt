PROJECT(test)

cmake_minimum_required(VERSION 2.8)

include_directories(${DAP_SOURCE_DIR}/gtest/include)
include_directories(${DAP_SOURCE_DIR}/libDragon/src)

link_directories(${DAP_SOURCE_DIR}/v8/out/x64.release/obj.target/tools/gyp)

add_executable(test_main test_main.cc ClassABC_Test.cpp Class/ClassABC.cpp)
target_link_libraries(test_main gtest Dragon boost_filesystem mysqlcppconn v8_base.x64 v8_nosnapshot.x64)
