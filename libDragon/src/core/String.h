#ifndef __STRING_H__
#define __STRING_H__

#include "Macro.h"
#include "Object.h"
#include "Space.h"

#include <stdio.h>

NS_DRAGON

class String : public Object {
public:
	String() : string_(NULL), length_(0) {}
	~String() {}

	static Ref<String> New(const char *str, unsigned len, Space &space)
	{
		char *addr = reinterpret_cast<char *>(space.Allocate(sizeof(String)));
		Ref<String> ret = Ref<String>::Cast(addr);
		memset(addr, 0, sizeof(unsigned));
		//memset(addr, 0, sizeof(unsigned));
		//memset(addr, 0, sizeof(char *));

		printf("size:%d\n", ret->Size());
		
		return ret;	
	}

	char *GetString()
	{
		return string_;
	}

	void SetLength(unsigned len)
	{
		length_ = len;
	}

	unsigned GetLength()
	{
		return length_;
	}

	String &operator=(const String &that)
	{
		this->string_ = that.string_;
		this->length_ = that.length_;
	}

	unsigned Size();

private:
	unsigned length_;
	char *string_;
};


NS_END

#endif
