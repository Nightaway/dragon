#include "Table.h"

NS_USING_DRAGON

OffsetTable::OffsetTable() :
		capacity_(0), offset_(NULL)
{

}

OffsetTable::~OffsetTable()
{
	
}

OffsetTable::OffsetTable(unsigned capacity, Space &space)
{
	unsigned bytes = capacity * sizeof(unsigned *); 
	offset_ = reinterpret_cast<unsigned *>(space.Allocate(bytes));
	memset(offset_, 0, bytes);
}
