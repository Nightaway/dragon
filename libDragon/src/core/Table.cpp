#include "Table.h"

NS_USING_DRAGON

OffsetTable::OffsetTable() :
		capacity_(0), table_(NULL)
{

}

OffsetTable::~OffsetTable()
{
	
}

OffsetTable::OffsetTable(unsigned capacity, Space &space) : capacity_(capacity)
{
	unsigned bytes = capacity * sizeof(unsigned *); 
	table_ = reinterpret_cast<unsigned *>(space.Allocate(bytes));
	memset(table_, 0, bytes);
}

void OffsetTable::Open(Space &space)
{
	table_ = reinterpret_cast<unsigned *>(space.Address());
}
