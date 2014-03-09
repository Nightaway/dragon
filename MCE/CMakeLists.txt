project(MCE)

include_directories(../libDragon/src)
include_directories(../v8/include)

add_executable(MCE src/maintenance_cache_engine.cpp)
