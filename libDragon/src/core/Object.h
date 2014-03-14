#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "Macro.h"
#include "Space.h"
#include "StringRef.h"

#ifndef NULL
#define NULL 0
#endif

NS_DRAGON	

	inline size_t sizeOf(const std::string &str)
	{
		size_t s = 0;
		s += sizeof(size_t);
		s += str.size();
		return s;
	}

	inline unsigned sizeOf(const StringRef str)
	{
		unsigned s = 0;
		s += sizeof(unsigned);
		s += str.length();
		return s;
	}

	template<typename T>
	class Ref {
	public:
		Ref() : addr_(NULL) {}
		Ref(T *addr) : addr_(addr) {}
		~Ref() {}

		bool IsNull()
		{
			return addr_?true:false;
		}

		T *operator*() const 
		{
			return addr_;
		}

		T *operator->() const 
		{
			return addr_;
		}

		static Ref<T> Cast(void *addr)
		{
			return Ref<T>(reinterpret_cast<T *>(addr));
		} 

		static Ref<T> New(Space &space)
		{
			return Ref<T>(reinterpret_cast<T *>(space.Allocate(sizeof(T))));
		}

	private:
		T *addr_;
	};

	class Object {
	public:
		Object();
		virtual ~Object();

		virtual unsigned Size()
		{
			return size_;
		}

		virtual void Dump(Space &space) {}
		virtual void Stuff(Space &space) {}

	protected:
		unsigned size_;
	};

NS_END

#endif
