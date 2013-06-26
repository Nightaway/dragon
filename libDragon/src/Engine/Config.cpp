/*
 *				作者: 杨健
 *				日期: 2012-12-18
 *				功能: 配置文件解析
 *				描述:	 解析应用程序路径下的App.config配置文件
*/
#include <map>
#include <string>
#include <sstream>
#include <boost/foreach.hpp>

#include "../Macro.h"
#include "../Error.h"

#include "Config.h"

#include <stdio.h>
#include <cassert>

USING_NS_DRAGON;

enum  {
	tok_eof = -1,
	tok_sname,
	tok_string,
	tok_assign,
	tok_integer,
	tok_null,
	tok_newline,
	tok_unkown
};

#define INC_IF_EOF_BREAK \
	inc();\
	if (eof) break;

#define INC_IF_EOF_RETURN \
	inc();\
	if (eof) return true;

#define INC_IF_EOF_ASSERT \
	inc();\
	if (eof)  assert(false);

class Tok {
public:
	int tag_;
	std::string str_;

public:
	Tok(int tag, std::string str) : tag_(tag), str_(str) {} 
	Tok() {}
};

class Lexi {
	FILE *file_;

	int   lastError;
	char lastChar;
	bool eof;

	size_t inc()
	{
		size_t size = fread((void *)&lastChar, sizeof(char), 1, file_);
		if (size <= 0) {
			if (feof(file_)) {
				eof = true;
			}
		}
		return size;
	}

	void skipSpaceAndTab()
	{
		while (lastChar == ' ' || lastChar == '		' ) {
			INC_IF_EOF_BREAK;
		}
	}

	bool section_name(std::string &name)
	{
		if (lastChar == '[')
		{
			INC_IF_EOF_RETURN;
			while (lastChar != ']') {
				name += lastChar;
				INC_IF_EOF_BREAK;
			}
			return true;
		} 
		return false;
	}

	bool String(std::string &str)
	{
		if (::isalpha(lastChar)) {
			str += lastChar;

			INC_IF_EOF_RETURN;
			while (::isalnum(lastChar) || lastChar == '_') { // || lastChar == '.' || lastChar == ':') {
				str += lastChar;
				INC_IF_EOF_RETURN;
			}
			::fseek(file_, -1, SEEK_CUR);

			return true;
		}
		return false;
	}

	bool QuoteString(std::string &str) 
	{
		if (lastChar == '"') {
			INC_IF_EOF_ASSERT;

			while (lastChar != '"') {
				str += lastChar;
				INC_IF_EOF_ASSERT;
			}
			return true;
		}
		return false;
	}

	bool Integer(std::string &integer) {
		if (::isdigit(lastChar)) {
			integer += lastChar;

			INC_IF_EOF_RETURN;
			while (::isdigit(lastChar)) {
				integer += lastChar;
				INC_IF_EOF_RETURN;
			}
			::fseek(file_, -1, SEEK_CUR);

			return true;
		}
		return false;
	}

	bool Assign() {
		if (lastChar == '=') {
			return true;
		}
		return false;
	}

	bool NewLine() {
		if (lastChar == 0x0a) {
			return true;
		}

		if (lastChar  == 0x0d) {
			INC_IF_EOF_ASSERT;
			if (lastChar == 0x0a) {
				return true;
			}
		}

		return false;
	}

	void ungetTok(int len)
	{
		::fseek(file_, -len, SEEK_CUR);
	}

public:
	Lexi(const char *path) : lastError(kErrNone), eof(false) {
		file_ = fopen(path, "rb");

		if (file_ == NULL) {
			lastError = kErrConfigFileOpen;
		}
	}

	~Lexi() {
		fclose(file_);
	}

	int getLastErr()
	{
		return lastError;
	}

	bool isEOF()
	{
		return eof;
	}

	Tok getNextTok(bool isPeek = false)
	{
		size_t size = inc();

		if (size == 0) {
			eof = true;
			Tok tagEOF(tok_eof, "eof");
			return tagEOF;
		}

		skipSpaceAndTab();

		{
			std::string name;
			if (section_name(name) == true) {
				Tok tok(tok_sname, name);
				if (isPeek) ungetTok(name.length() + 2);
				return tok;
			}
		}

		{
			std::string str;
			if (String(str) == true) {
				Tok tok(tok_string, str);
				if (isPeek) ungetTok(str.length());
				return tok;
			}

			if (QuoteString(str) == true) {
				Tok tok(tok_string, str);
				if (isPeek) ungetTok(str.length() + 2);
				return tok;
			}
		}

		{
			std::string  integer;
			if (Integer(integer) == true) {
				Tok tok(tok_integer, integer);
				if (isPeek) ungetTok(integer.length());
				return tok;
			}
		}

		{
			if (Assign() == true) {
				Tok tok(tok_assign, "=");
				if (isPeek) ungetTok(1);
				return tok;
			}
		}

		{
			if (NewLine() == true) {
				Tok tok(tok_newline, "\n");
				if (isPeek) ungetTok(1);
				return tok;
			}
		}

		std::string str;
		str += lastChar;
		Tok tok(tok_unkown, str);
		if (isPeek) ungetTok(str.length());
		return tok;
	}
};

void expected(const Tok &t, int tag)
{
	if (t.tag_ != tag)
		assert(false);
}

void expected(const Tok &t, int tag1, int tag2)
{
	if (t.tag_ != tag1 && t.tag_ !=tag2)
		assert(false);
}

void expected(const Tok &t, int tag1, int tag2, int tag3)
{
	if (t.tag_ != tag1 && t.tag_ !=tag2 &&  t.tag_ !=tag2)
		assert(false);
}

void Config::Parse(const char *path)
{
	Lexi lexi(path);

	if (lexi.getLastErr() == kErrConfigFileOpen) {
		fprintf(stderr, "Open Config File [%s] Failed.", path);
		exit(lexi.getLastErr());
	}

	while (1) {
		Tok tok_sn = lexi.getNextTok();
		if (tok_sn.tag_ == tok_sname) {
			Tok tok_n = lexi.getNextTok();
			expected(tok_n, tok_newline);

			kv_t kv;
			while (1) {
				Tok tok_key = lexi.getNextTok();
				if (lexi.isEOF()) break;
				expected(tok_key, tok_string, tok_newline);

				if (tok_key.tag_ == tok_newline)
					break;

				Tok tok_ass = lexi.getNextTok();
				expected(tok_ass, tok_assign);

				Tok prediate =  lexi.getNextTok(true);
				if (prediate.tag_ == tok_newline) 
				{
					kv[tok_key.str_] = "";
				} else
				{
						Tok tok_value = lexi.getNextTok();
						expected(tok_value, tok_string, tok_integer);
						kv[tok_key.str_] = tok_value.str_;
				}

				Tok tok_n = lexi.getNextTok();
				if (lexi.isEOF()) break;
				expected(tok_n, tok_newline);

				Tok peek;
				while (1) {
					peek = lexi.getNextTok(true);
					if (lexi.isEOF()) break;
					if (peek.tag_ == tok_newline) 
					{
						 lexi.getNextTok();
					}  else break;
				}
				if (peek.tag_ == tok_sname)
					break;
			}
			config[tok_sn.str_] = kv;
		} else {
			break;
		}
	}
}

