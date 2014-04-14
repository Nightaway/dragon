#ifndef __STRING_ALOGRITHMS_H__
#define __STRING_ALOGRITHMS_H__

#include <vector>
#include <cmath>
#include <cstring>
#include <string>
#include <algorithm>
#include "../core/Macro.h"

NS_DRAGON

class StringAlogrithms {
	private:
		static void tolower(char &s)
		{
			s = ::tolower(s);
		}

	public:
		static std::string ReplaceAll(std::string str,
									  std::string sub1, 
									  std::string sub2)
		{
			size_t index = 0;
			index = str.find(sub1, 0);
			while(( index != std::string::npos))
			{
				str.replace(index, sub1.size(), sub2);  
				index = str.find(sub1, 0);
			}
			return str;
		}

		static std::string ToLower(std::string str)
		{
			std::for_each(str.begin(), str.end(), StringAlogrithms::tolower);
			return str;
		}

		static std::vector<std::string> explode(const std::string &delim, 
												std::string str)
		{
			std::vector<std::string>strings;
			size_t index = 0;

			while((index = str.find(delim, 0)) != std::string::npos)
			{
				if (!str.empty())
					strings.push_back(str.substr(0, index));

				str = str.substr(index+delim.length(), str.length());	
			}
			if (!str.empty())
				strings.push_back(str.substr(0, index));
			else
				strings.push_back("");
			return strings;
		}

	};

NS_END

#endif