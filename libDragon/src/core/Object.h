#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "Macro.h"
#include "Space.h"

#ifndef NULL
#define NULL 0
#endif

NS_DRAGON	

	template<typename T>
	class Ref {
	public:
		Ref() : Addr_(NULL) {}
		~Ref() {}

		bool IsNull()
		{
			return Addr_?true:false;
		}
	private:
		T *Addr_;
	};

	class Object {
	public:
		Object();
		virtual ~Object();

		unsigned Size()
		{
			return size_;
		}

		virtual void Dump(Space &space) {}

	protected:
		unsigned size_;
	};

NS_END

#endif
