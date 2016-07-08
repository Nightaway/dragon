project(BudgetControll)

cmake_minimum_required(VERSION 2.6)

include_directories(../libDragon/src)
include_directories(../delivery_engine/src)
include_directories(../v8/include)

set (src_header
	 src/config.h
	 src/maintenance_cache_engine.h
	 src/cache_task.h
)

set (src_list
	 src/maintenance_cache_engine.cpp
	 src/cache_task.cpp
)

configure_file (
	${PROJECT_SOURCE_DIR}/config.h.in
	${PROJECT_SOURCE_DIR}/src/config.h
)

add_executable(BudgetControll ${src_list})
target_link_libraries(BudgetControll Dragon DE rt pthread  boost_system boost_filesystem boost_date_time boost_regex v8)
install(TARGETS BudgetControll DESTINATION BudgetControll/sbin)
install(DIRECTORY logs DESTINATION BudgetControll)
install(DIRECTORY scripts DESTINATION BudgetControll)
install(FILES App.conf    DESTINATION BudgetControll/conf)
install(FILES ${src_header}  DESTINATION include/BudgetControll/)
