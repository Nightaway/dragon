#ifndef __ADINFO_H__
#define __ADINFO_H__

#include <core/Space.h>
#include <core/Object.h>
#include <core/StringRef.h>
#include <core/String.h>
#include <string>

class AdInfo : public dragon::Object {
public:
	unsigned Size();
	void Dump(dragon::Space &space);
	void Stuff(dragon::Space &space);
public:
	int id;
	std::string name;
	unsigned width;
	unsigned height;
};

#endif
