project(libdapmvc)

cmake_minimum_required(VERSION 2.8)

include_directories(${DAP_SOURCE_DIR}/v8/include)
include_directories(${DAP_SOURCE_DIR}/libDragon/src)

link_directories(${DAP_SOURCE_DIR}/v8/out/x64.release/obj.target/tools/gyp)

SET(SRC_LIST
    src/Applications/DapApplication.cpp
)

add_library(dapmvc SHARED ${SRC_LIST})
target_link_libraries(dapmvc Dragon)

install(TARGETS dapmvc
	LIBRARY DESTINATION lib)

install(FILES src/App.config DESTINATION etc)
install(FILES src/Controllers/AdController.js DESTINATION Controllers)
