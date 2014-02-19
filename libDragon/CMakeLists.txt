project(libDragon)

include_directories(../v8/include)

set(header_list 
	src/dragon.h
	src/config.h
)

set(core_header_list 
	src/core/Macro.h
	src/core/Module.h
	src/core/Application.h
	src/core/ApplicationCache.h
	src/core/RuntimeInfo.h	
	src/core/Controller.h
	src/core/StringRef.h
	src/core/QueryString.h
	src/core/JSCompiler.h
	src/core/Log.h
	src/core/Modules/Console.h
)

set(core_src_list 
	src/core/Module.cpp
	src/core/Application.cpp
	src/core/ApplicationCache.cpp
	src/core/RuntimeInfo.cpp
	src/core/Controller.cpp
	src/core/StringRef.cpp
	src/core/QueryString.cpp
	src/core/JSCompiler.cpp
	src/core/Log.cpp
	src/core/Modules/Console.cpp
)

set(http_header_list
	src/http/UrlRewriting.h
	src/http/UrlRouting.h
	src/http/HttpRequest.h
	src/http/HttpResponse.h
	src/http/HttpProcess.h
)

set(http_src_list
	src/http/UrlRewriting.cpp
	src/http/UrlRouting.cpp
	src/http/HttpRequest.cpp
	src/http/HttpResponse.cpp
	src/http/HttpProcess.cpp
)

set(utility_header_list
	src/utility/Wrapper.h
)

set(utility_src_list
	src/utility/Wrapper.cpp
)

set(src_list
	${core_src_list}
	${http_src_list}
	${utility_src_list}
)

add_library(Dragon SHARED ${src_list})
install(TARGETS Dragon DESTINATION lib)
install(FILES ${header_list}    DESTINATION include/dragon/)
install(FILES ${core_header_list}    DESTINATION include/dragon/core)
install(FILES ${http_header_list}    DESTINATION include/dragon/http)
install(FILES ${utility_header_list} DESTINATION include/dragon/utility)


