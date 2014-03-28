project(ShmMemCli)

include_directories(../../libDragon/src)

add_executable(ShmMemCli ShmMemCli.cpp)
target_link_libraries(ShmMemCli Dragon pthread rt boost_system boost_filesystem boost_thread boost_date_time v8_base.x64.a v8_nosnapshot.x64.a)
