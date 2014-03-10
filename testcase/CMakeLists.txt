project(dragon_test)

cmake_minimum_required(VERSION 2.6)

include_directories(../gtest-1.6.0/include)
include_directories(../libDragon/src)
include_directories(../delivery_engine/src)
include_directories(../v8/include)

add_executable(dragon_test testmain.cpp testApp1.cpp testParseConfig.cpp testJavaScriptCompiler.cpp)

target_link_libraries(dragon_test gtest Dragon DE pthread boost_system boost_filesystem boost_thread boost_date_time v8_base.x64.a v8_nosnapshot.x64.a)


