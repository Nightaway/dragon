#ifndef __MEMORY_H__
#define __MEMORY_H__
	
#include <vector>
#include <fstream>
#include <string>
#include <memory.h>

NS_DRAGON
	NS_CORE

enum {
	SERI_IN,
	SERI_OUT
};

class Memory {
	char io_bit;
	int idx;
	std::vector<char> mem;

	public:
	Memory(char io, size_t size) : io_bit(io), idx(0), mem(size) {}
	Memory(char io) : io_bit(io), idx(0) {}

	void clear()
	{
		idx = 0;
	}

	std::vector<char> &data()
	{	
		return mem;
	}

	void write(const char *fname)
	{
		std::ofstream f(fname, std::ios_base::binary|std::ios_base::out);
		std::size_t size = mem.size();

		f.write((const char *)&size, sizeof(size_t));
		f.write(&mem[0], mem.size());
		f.close();
	}

	void read(const char *fname)
	{
		std::ifstream f(fname, std::ios_base::binary|std::ios_base::in);
		std::size_t size;
		f.read((char *)&size, sizeof(size_t));
		mem.resize(size);
		f.read(&mem[0], mem.size());
		f.close();
	}

	char get_io_bit()
	{
		return io_bit;
	}

	template <class T>
	inline void operator<<(const T v)
	{
		memcpy(&mem[idx], &v, sizeof(v));
		idx += sizeof(v);
	}

	template <class T>
	inline void operator>>(T &v)
	{
		memcpy(&v, &mem[idx], sizeof(v));
		idx += sizeof(v);
	}

	inline void operator<<(const std::string &v)
	{
		operator<< (v.length());
		if (v.length() > 0)
		{
			memcpy(&mem[idx], &v[0], v.length());
			idx += v.length();
		}
	}

	inline void operator>>(std::string &v)
	{
		size_t size;
		operator>>(size);
		if (size > 0)
		{
			v.resize(size);
			memcpy(&v[0], &mem[idx], size);
			idx += v.length();
		}
	}

	template<class T>
	inline void operator<<(const std::vector<T> &v)
	{
		operator<< (v.size());
		if (v.size() > 0)
		{
			for (typename std::vector<T>::const_iterator iter=v.begin(); iter!=v.end(); ++iter)
			{
				operator<<(*iter);
			}
		}
	}

	template<class T>
	inline void operator>>(std::vector<T> &v)
	{
		size_t size;
		operator>>(size);
		for (size_t i=0; i<size; ++i)
		{
			T instance;
			operator>>(instance);
			v.push_back(instance);
		}
	}

};

	NS_END
NS_END

#endif