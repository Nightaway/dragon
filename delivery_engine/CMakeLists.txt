project(delivery_engine)

cmake_minimum_required(VERSION 2.6)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/Utility/libRedis)

include_directories(../libDragon/src)
include_directories(../v8/include)
include_directories(${PROJECT_SOURCE_DIR}/src/Utility/libRedis)

set(app_header_list 
	src/DEApplication.h
)

set(app_src_list 
	src/DEApplication.cpp
)

set(controller_header_list
	src/Controllers/deliveryController.h
)

set(controller_src_list
	src/Controllers/deliveryController.cpp
)

set(model_header_list
	src/Models/AdInfo.h
	src/Models/ZoneInfo.h
	src/Models/VisitorInfo.h
)

set(model_src_list
	src/Models/AdInfo.cpp
	src/Models/ZoneInfo.cpp
	src/Models/VisitorInfo.cpp
)

set(util_header_list
	src/Utility/DataModel.h
	src/Utility/Filter.h
	src/Utility/IPLocator.h
	src/Utility/RedisContext.h
)

set(util_src_list
	src/Utility/DataModel.cpp
	src/Utility/Filter.cpp
	src/Utility/IPLocator.cpp
	src/Utility/RedisContext.cpp
)

set(filter_src_list
	src/Utility/Filters/VisitorFilter.cpp
	src/Utility/Filters/StandardFilter.cpp
	src/Utility/Filters/CookieRedirect.cpp
	src/Utility/Filters/MotuFilter.cpp
	src/Utility/Filters/ContentFilter.cpp
)

set(src_list
	${controller_src_list}
	${app_src_list}
	${model_src_list}
	${util_src_list}
	${filter_src_list}
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
install(FILES ${app_header_list}        DESTINATION include/DE/)
install(FILES ${controller_header_list} DESTINATION include/DE/Controllers)
