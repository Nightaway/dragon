#ifndef __STRING_H__
#define __STRING_H__

#include "Macro.h"
#include "Object.h"
#include "Space.h"

NS_DRAGON

class String : public Object {
public:
	String() : string_(NULL), length_(0) {}
	~String() {}

	static Ref<String> New(const char *str, unsigned len, Space &space)
	{
		char *addr = reinterpret_cast<char *>(space.Allocate(len));
		strncpy(addr, str, len);
		Ref<String> ret = Ref<String>::Cast(addr);
		ret->SetString(addr);
		ret->SetLength(len);
		return Ref<String>::Cast(addr);
	}

	void SetString(char *string)
	{
		string_ = string;
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

	virtual unsigned Size();

private:
	char *string_;
	unsigned length_;
};


NS_END

#endif
