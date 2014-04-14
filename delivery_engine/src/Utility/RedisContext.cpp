#include "RedisContext.h"

#include <utility/Crc.h>
#include <boost/lexical_cast.hpp>

void RedisContext::Init()
{
		// 初始化Redis服务器
		std::string redis_ip[6] = {"172.16.213.79","172.16.213.80","172.16.213.81","172.16.213.82","172.16.213.83","172.16.213.84"};
		std::string redis_port[4] = {":6379",":6380",":6381",":6382"};
		std::string redis_server;

		for(int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				redis_server = redis_ip[i] + redis_port[j];

				for (int k = 0; k < 64 ; k++)
				{
					char ip[50] = "";
					std::string redis_server_idx = redis_server + boost::lexical_cast<std::string>(k);
					strcpy(ip, redis_server_idx.c_str());	
					redisPositionMap[dragon::Crc::crc32(ip, strlen(ip))] = redis_server;
				}
			}
		}
}

void RedisContext::Dispose()
{

}

std::string RedisContext::findRedisServer(std::string pcid)
{
	char ip[128] = "";
	strcpy(ip, pcid.c_str());

	int pos = dragon::Crc::crc32(ip, strlen(ip));

	RedisPositionMap::iterator end = redisPositionMap.end();
	for (RedisPositionMap::iterator iter = redisPositionMap.begin(); iter != end; ++iter)
	{
		if(iter->first > pos) 
		{
			return  iter->second;		
		}
	}
	return redisPositionMap.begin()->second;
}

redis::client *RedisContext::Connect(std::string pcid)
{
	std::string server = findRedisServer(pcid);
	std::string ip;
	int port;
	int index = server.find(":", 0);
	ip = server.substr(0, index);
	port = atoi(server.substr(index + 1, server.length()).c_str());
	int dbindex = 0;

	return new redis::client(ip, port, dbindex);
}

