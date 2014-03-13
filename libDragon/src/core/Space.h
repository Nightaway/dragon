#ifndef __SPACE_H__
#define __SPACE_H__

#include <string>
#include <memory.h>

#include "Macro.h"
#include "Allocation.h"

NS_DRAGON

class Object;
class Page;

class Space {
public:
	Space();
	virtual ~Space();

	void *Address()
	{
		return addr_;
	}

	void *CurrentAddress()
	{
		return reinterpret_cast<char *>(addr_) + pos_;
	} 

	unsigned Capacity()
	{
		return size_;
	}

	void *Allocate(Object &obj);

	template <typename T>
	void Put(const T e)
	{
		void *addr = reinterpret_cast<char *>(addr_) + pos_;
		memcpy(addr, &e, sizeof(e));
		pos_ += sizeof(e);
	}

	template <typename T>
	void Get(T &e)
	{
		char *addr = reinterpret_cast<char *>(addr_) + pos_;
		memcpy(&e, addr, sizeof(e));
		pos_ += sizeof(e);
	}

	void Put(const std::string e)
	{
		char *addr = reinterpret_cast<char *>(addr_) + pos_;
		size_t length = e.length();
		memcpy(addr, &length, sizeof(size_t));
		pos_ += sizeof(size_t);
		
		addr = reinterpret_cast<char *>(addr_) + pos_;
		memcpy(addr, e.c_str(), e.size());
		pos_ += e.size();
	}

	void Get(std::string &t)
	{
		void *addr = reinterpret_cast<char *>(addr_) + pos_;
		size_t length = 0;
		memcpy(&length, addr, sizeof(size_t));
		pos_ += sizeof(size_t);
		
		t.resize(length);
		addr = reinterpret_cast<char *>(addr_) + pos_;
		memcpy(const_cast<char *>(t.data()), addr, t.size());
		pos_ += t.size();
	}

	void SetPos(unsigned pos)
	{
		pos_ = pos;
	}

	unsigned GetPos()
	{
		return pos_;
	}
protected:
	void *addr_;
	unsigned size_;
	unsigned pos_;
};

class SemiSpace : public Space {
public:
	SemiSpace();
	virtual ~SemiSpace();

	void *HalfAddress()
	{
		return half_addr_;
	}

	void *HeadAddress()
	{
		return head_addr_;
	}

	void Switch()
	{
		if (is_head_used) {
			addr_ = half_addr_;	
			is_head_used = false;
			pos_ = 0;
		}
		else {
			addr_ = head_addr_;
			is_head_used = true;
			pos_ = 0;
		}
	}

protected:
	bool is_head_used;
	void *head_addr_;
	void *half_addr_;
};

class NamedSemiSpace : public SemiSpace {
public:
	NamedSemiSpace(const char *name, unsigned size);
	virtual ~NamedSemiSpace();

	const char *GetName()
	{
		return name_;
	}

	void Create();
	void Destroy();

protected:
	const char *name_;
	int fd_;
};

NS_END

#endif

