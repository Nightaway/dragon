#ifndef __REDIS_CONTEXT_H__
#define __REDIS_CONTEXT_H__

#include <string>
#include <map>
#include "libRedis/redisclient.h"


typedef std::map<unsigned int , std::string>RedisPositionMap;

class RedisContext {
	RedisPositionMap redisPositionMap;
	std::string findRedisServer(std::string pcid);
	
public:
		void Init();
		void Dispose();
		
		redis::client *Connect(std::string pcid);
};


#endif