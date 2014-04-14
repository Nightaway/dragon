project(libRedis)

cmake_minimum_required(VERSION 2.6)

set (REDIS_H_LIST
	redisclient.h
	anet.h
	fmacros.h
)

set (TESTS_H_LIST
	tests/functions.h
)

set (REDIS_SRC_LIST 
	anet.cpp
	test_client.cpp
	tests/functions.cpp
	tests/test_lists.cpp
	tests/test_sets.cpp
	tests/test_zsets.cpp
	tests/test_hashes.cpp
	tests/test_cluster.cpp
	tests/test_distributed_strings.cpp
	tests/test_distributed_ints.cpp 
	tests/test_distributed_mutexes.cpp
	tests/test_generic.cpp
	tests/benchmark.cpp
)

add_library(redis SHARED ${REDIS_SRC_LIST})
install(TARGETS redis
	LIBRARY DESTINATION lib)
