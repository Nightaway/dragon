project(testcxx)

cmake_minimum_required(VERSION 2.6)

include_directories(../libDragon/src)
include_directories(../delivery_engine/src)
include_directories(../v8/include)

add_executable(test test.cpp)
target_link_libraries(test Dragon DE bz2 rt pthread boost_system boost_filesystem boost_date_time boost_regex v8_base.x64.a v8_snapshot.a)
