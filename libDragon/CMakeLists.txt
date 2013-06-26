PROJECT(libDragon)

cmake_minimum_required(VERSION 2.8)

include_directories(${DAP_SOURCE_DIR}/v8/include)

SET(SRC_LIST
    src/RuntimeInfo/Memory.cpp
    src/RuntimeInfo/Object.cpp
    src/RuntimeInfo/Reflection.cpp
    src/RuntimeInfo/TypeMapping.cpp
    src/RuntimeInfo/RuntimeInfo.cpp
    src/Engine/Application.cpp
    src/Engine/Config.cpp
    src/Engine/Router.cpp
    src/Engine/View.cpp
    src/Engine/Internals/Compiler.cpp
    src/Web/HttpRequest.cpp
    src/Web/HttpResponse.cpp
    src/Web/HttpProcess.cpp
    src/DB/DBContext.cpp
)

add_library(Dragon SHARED ${SRC_LIST})

install(TARGETS Dragon
	LIBRARY DESTINATION lib)

