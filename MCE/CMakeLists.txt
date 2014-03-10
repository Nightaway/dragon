project(MCE)

cmake_minimum_required(VERSION 2.6)

include_directories(../libDragon/src)
include_directories(../v8/include)

configure_file (
	${PROJECT_SOURCE_DIR}/config.h.in
	${PROJECT_SOURCE_DIR}/src/config.h
)

add_executable(MCE src/maintenance_cache_engine.cpp)
target_link_libraries(MCE Dragon DE pthread boost_system boost_filesystem boost_thread boost_date_time v8_base.x64.a v8_nosnapshot.x64.a)
install(TARGETS MCE DESTINATION MCE/sbin)
install(DIRECTORY logs DESTINATION MCE)
install(DIRECTORY scripts DESTINATION MCE)
install(FILES App.conf    DESTINATION MCE/conf)
