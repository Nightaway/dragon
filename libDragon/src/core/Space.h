#ifndef __SPACE_H__
#define __SPACE_H__

#include "Macro.h"
#include "Allocation.h"

#include <string>
#include <memory.h>

NS_DRAGON

class Space {
public:
	Space();
	virtual ~Space();

	void *Address()
	{
		return addr_;
	}

	unsigned Capacity()
	{
		return size_;
	}

	template <typename T>
	void Put(const T &t)
	{
		void *addr = reinterpret_cast<char *>(addr_) + pos_;
		memcpy(addr, &t, sizeof(T));
		pos_ += sizeof(T);
	}

	template <typename T>
	void Get(const T &t)
	{
		char *addr = reinterpret_cast<char *>(addr_) + pos_;
		memcpy(&t, addr, sizeof(T));
		pos_ -= sizeof(T);
	}

	void Put(const std::string &t)
	{
		char *addr = reinterpret_cast<char *>(addr_) + pos_;
		size_t size = t.size();
		memcpy(addr, &size, sizeof(size_t));
		pos_ += sizeof(size_t);

		addr = reinterpret_cast<char *>(addr_) + pos_;
		memcpy(addr, t.c_str(), t.size());
		pos_ += t.size();
	}

	void Get(std::string &t)
	{
		void *addr = reinterpret_cast<char *>(addr_) + pos_;
		size_t size = 0;
		memcpy(&size, addr, sizeof(size_t));
		pos_ -= sizeof(size_t);

		t.resize(size);
		addr = reinterpret_cast<char *>(addr_) + pos_;
		memcpy(const_cast<char *>(t.data()), addr, t.size());
		pos_ -= t.size();
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
		}
		else {
			addr_ = head_addr_;
			is_head_used = true;
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

