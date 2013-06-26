#ifndef __URL_H__
#define __URL_H__

#include <string>

namespace cxxmvc {

	inline unsigned char toHex(const unsigned char &x)
	{
		return x > 9 ? x + 55: x + 48;
	}

	inline unsigned char fromHex(const unsigned char &x)
	{
		return isdigit(x) ? x-'0' : x-'A' + 10;
	}

	class URL {
	public:
		static std::string encode(std::string url)
		{
			 std::string sOut;
			 for(size_t ix = 0; ix < url.size(); ix++ )
			 {
				 unsigned char buf[4];
				 memset(buf, 0, 4 );
				 if( isalnum( (unsigned char)url[ix] ) )
				 {
					 buf[0] = url[ix];
				 }
				 else if ( isspace( (unsigned char)url[ix] ) )
				 {
					 buf[0] = '+';
				 }
				 else
				 {
					 buf[0] = '%';
					 buf[1] = toHex( (unsigned char)url[ix] >> 4 );
					 buf[2] = toHex( (unsigned char)url[ix] % 16);
				 }
				 sOut += (char *)buf;
			 }
			 return sOut;
		}

		static std::string decode(std::string url)
		{
			 std::string sOut;
			 for(size_t ix = 0; ix < url.size(); ix++ )
			 {
				 unsigned char ch = 0;
				 
				if(url[ix] == '%')
				{
					ch = (fromHex(url[ix+1]) <<4);
					ch |= fromHex(url[ix+2]);
					ix += 2;
				}
				else if (url[ix] == '+')
				{
					ch = ' ';
				}
				else
				{
					ch = url[ix];
				}

				sOut += (char) ch;
			 }
			 return sOut;
		}
	};
};


#endif