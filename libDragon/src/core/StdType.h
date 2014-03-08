#ifndef __STD_TYPE_H__
#define __STD_TYPE_H__

#include "Macro.h"
#include <string>
#include <map>

NS_DRAGON

typedef std::string section_t; 
typedef std::string key_t;
typedef std::string value_t;
typedef std::map<key_t, value_t> key_value_t;
typedef std::map<section_t, key_value_t> config_t;

NS_END


#endif
