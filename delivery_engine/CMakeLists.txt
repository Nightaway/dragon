project(delivery_engine)

cmake_minimum_required(VERSION 2.6)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(../libDragon/src)
include_directories(../v8/include)

find_package(aws-sdk-cpp)
add_definitions(-DUSE_IMPORT_EXPORT)

set(app_header_list 
	src/pbsApplication.h
)

set(app_src_list 
	src/pbsApplication.cpp
)

set(controller_header_list
	src/Controllers/deliveryController.h
)

set(controller_src_list
	src/Controllers/deliveryController.cpp
)

set(src_list
	${controller_src_list}
	${app_src_list}
)

configure_file (
	${PROJECT_SOURCE_DIR}/config.h.in
	${PROJECT_SOURCE_DIR}/src/config.h
)

add_library(DE SHARED ${src_list})
install(TARGETS DE DESTINATION lib)
install(DIRECTORY logs DESTINATION DE)
install(DIRECTORY scripts DESTINATION DE)
install(FILES App.conf       DESTINATION DE/conf)
install(FILES ipLib.data       DESTINATION DE/data)
install(FILES ${app_header_list}        DESTINATION include/DE/)
install(FILES ${controller_header_list} DESTINATION include/DE/Controllers)
