project(ngx)

cmake_minimum_required(VERSION 2.8)

include_directories(${DAP_SOURCE_DIR}/v8/include)
include_directories(${DAP_SOURCE_DIR}/libDragon/src)
include_directories(${DAP_SOURCE_DIR}/libDapmvc/src)

link_directories(${DAP_SOURCE_DIR}/v8/out/x64.release/obj.target/tools/gyp)

add_executable(ngx ngx.cpp)
target_link_libraries(ngx dapmvc Dragon boost_filesystem mysqlcppconn v8_base.x64 v8_nosnapshot.x64)
