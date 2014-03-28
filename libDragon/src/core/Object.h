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
			return (addr_ == NULL)?true:false;
		}

		T operator*() const 
		{
			return *addr_;
		}

		T *operator->() const 
		{
			return addr_;
		}

                void *Address()
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
                
                static Ref<T> New(T value, Space &space)
                {
                  void *addr = space.Allocate(sizeof(T));
                  memcpy(addr, &value, sizeof(T));
                  return Ref<T>::Cast(addr);
                }

	private:
		T *addr_;
	};

  enum ObjectTag{
    TAG_OBJECT = 0,
    TAG_INTEGER,
    TAG_DOUBLE,
    TAG_STRING,
    TAG_TABLE
  };

   class Object {
     public:
       Object();
       ~Object();

       unsigned Size()
       {
         return sizeof(Object);
       }
   };

  class Integer : public Object  {
   public:
    Integer();
    ~Integer();
    
    int Value();
    static Ref<Integer> New(int value, Space &space);

  private:
    void *addr_;
  };

NS_END

#endif
