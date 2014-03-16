project(libDragon)

include_directories(../v8/include)

set(header_list 
	src/dragon.h
	src/config.h
)

set(core_header_list 
	src/core/Macro.h
	src/core/Module.h
	src/core/Allocation.h
	src/core/Space.h
	src/core/Object.h
	src/core/Heap.h
	src/core/Table.h
	src/core/String.h
	src/core/Application.h
	src/core/ApplicationCache.h
	src/core/RuntimeInfo.h	
	src/core/Controller.h
	src/core/StringRef.h
	src/core/QueryString.h
	src/core/JSCompiler.h
	src/core/Modules/Config.h
	src/core/Modules/Log.h
	src/core/Modules/Console.h
)

set(core_src_list 
	src/core/Module.cpp
	src/core/Allocation.cpp
	src/core/Space.cpp
	src/core/Object.cpp
	src/core/Heap.cpp
	src/core/Table.cpp
	src/core/String.cpp
	src/core/Application.cpp
	src/core/ApplicationCache.cpp
	src/core/RuntimeInfo.cpp
	src/core/Controller.cpp
	src/core/StringRef.cpp
	src/core/QueryString.cpp
	src/core/JSCompiler.cpp
	src/core/Modules/Config.cpp
	src/core/Modules/Log.cpp
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

set(wrapper_header_list
	src/wrapper/Wrapper.h
)

set(wrapper_src_list
	src/wrapper/Wrapper.cpp
)

set(maintenance_header_list 
	src/maintenance/Task.h
	src/maintenance/Process.h
)

set(maintenance_src_list 
	src/maintenance/Task.cpp
	src/maintenance/Process.cpp
)

set(src_list
	${core_src_list}
	${http_src_list}
	${wrapper_src_list}
	${maintenance_src_list}
)

add_library(Dragon SHARED ${src_list})
install(TARGETS Dragon DESTINATION lib)
install(FILES ${header_list}    DESTINATION include/dragon/)
install(FILES ${core_header_list}    DESTINATION include/dragon/core)
install(FILES ${http_header_list}    DESTINATION include/dragon/http)
install(FILES ${wrapper_header_list} DESTINATION include/dragon/wrapper)
install(FILES ${maintenance_header_list} DESTINATION include/dragon/maintenance)
