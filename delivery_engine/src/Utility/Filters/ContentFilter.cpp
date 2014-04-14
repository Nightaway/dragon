#include "ContentFilter.h"

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <htmlcxx/html/ParserDom.h>
#include <boost/asio.hpp>
#include <boost/array.hpp>

#include <iconv.h>

// 代码转换操作类
class CodeConverter {
private:
	iconv_t cd;
public:
	// 构造
	CodeConverter(const char *from_charset,const char *to_charset)
	{
		cd = iconv_open(to_charset,from_charset);
	}

	// 析构
	~CodeConverter() 
	{
		iconv_close(cd);
	}

	// 转换输出
	int convert(char *inbuf,int inlen,char *outbuf,int outlen) {
		char **pin = &inbuf;
		char **pout = &outbuf;

		memset(outbuf,0,outlen);
		return iconv(cd,pin,(size_t *)&inlen,pout,(size_t *)&outlen);
	}
};

std::string cxx_iconv(std::string fromCharset,
	       						  std::string toCharset,
								  std::string string)
{
	char buf[4096];
	memset(buf, 0, 4096);
	size_t out_len = 4096;
	char *in_p  = &(string[0]);
	size_t in_len = string.length();
	CodeConverter cc = CodeConverter(fromCharset.c_str(), toCharset.c_str());
	cc.convert(in_p, in_len, buf, out_len);
	return std::string(buf);
}

using namespace std;
using namespace htmlcxx;

using boost::asio::ip::tcp;

std::string PageChoice_safeEncoding(std::string string,
				    std::string outEncoding = "UTF-8")
{
	std::string encoding  = "UTF-8";

	for (unsigned idx=0; idx<string.length(); ++idx)
	{
		unsigned char ch = string[idx];

		if (ch < 128) {
			continue;
		}

		if ((ch & 224) == 224) {
			char c = string[++idx];
			if ((c & 128) == 128) {
				c = string[++idx];
				 if ((c & 128) == 128) {
					 encoding = "UTF-8";
					 break;
				 }
			}
		}

		if ((ch & 192) == 192) {
			char c = string[++idx];
			if ((c &128) == 128) {
				encoding = "GB2312";
				break;
			}
		}

	}

	if (encoding == outEncoding) {
		return string;
	} else {
		std::string content = cxx_iconv(encoding, outEncoding, string);
		return content;
	}
}

std::string getContent(std::string loc)
{
	size_t pos = loc.find(':');
	std::string proto = std::string(loc, 0, pos);
	std::cout << proto << endl;

	pos = loc.find("//");
	std::string _host = std::string(loc, pos + strlen("//"));
	std::cout << _host << endl;

	size_t _pos =_host.find('/');
	std::string host = std::string(_host, 0, _pos);
	std::cout << host << endl;

	std::string path;
	if (_pos == std::string::npos) {
	//	std::cout << "no /" << endl;
		path = "/";
	} else {
		path = std::string(_host, _pos);
	}
	cout << "path:" << path << endl;

	try {
		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(host, proto);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		tcp::socket socket(io_service);
		boost::asio::connect(socket, endpoint_iterator);
		boost::system::error_code error;

		std::string requestStr ="GET " + path + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: Close\r\n\r\n";
		cout << requestStr;
		socket.write_some(boost::asio::buffer(requestStr), error);
		std::string content = "";
		 for (;;)
		{
		  boost::array<char, 2048> buf;
		  boost::system::error_code error;

		  size_t len = socket.read_some(boost::asio::buffer(buf), error);

		  if (error == boost::asio::error::eof)
			break; // Connection closed cleanly by peer.
		  else if (error)
			throw boost::system::system_error(error); // Some other error.

		  content += std::string(buf.data(), len);
		  size_t pos = content.find("</title>");
		  if (pos != std::string::npos) {
			  break;
		  }
		}

	  content =  PageChoice_safeEncoding(content);
	  cout << content << endl;
	
	  HTML::ParserDom parser;
  	  tree<HTML::Node> dom = parser.parseTree(content);

	  std::string arr;

	  //Dump all links in the tree
	  tree<HTML::Node>::iterator it = dom.begin();
	  tree<HTML::Node>::iterator end = dom.end();
	  for (; it != end; ++it)
	  {
	     if (strcasecmp(it->tagName().c_str(), "meta") == 0)
	     {
	        it->parseAttributes();
		if (it->attribute("name").second == "keywords")
		{
			cout << "keywords:" << it->attribute("content").second << endl;
			arr += it->attribute("content").second;
			arr += "||||";
		}

		if (it->attribute("name").second == "description")
		{
			cout << "description:" << it->attribute("content").second << endl;
			arr += it->attribute("content").second;
			arr += "||||";
		}
	     }

	     if (strcasecmp(it->tagName().c_str(), "title") == 0)
	     {
			std::string tt = (++it)->text();
				cout << "title:" << tt << endl;
			arr += tt;
			arr += "||||";
	     }
	  }
	cout << "arr:" <<arr;
	return arr;
		
  }
  catch (std::exception& e)
  {
   	std::cerr << e.what() << std::endl;
	std::cerr << "err" << std::endl;
	return std::string();
  }

}
