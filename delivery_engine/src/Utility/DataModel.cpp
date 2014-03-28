#include "DataModel.h"

#include <core/Table.h>

NS_USING_DRAGON

ZoneInfo *dragon::GetZoneInfoById(int id, NamedSemiSpace &space)
{
  space.SetPos(0);
  OffsetTable table;
  table.Open(1000, space);
  unsigned offset = table.Get(id);
  if (offset == 0)
    return NULL;
  space.SetPos(offset);
  ZoneInfo *zoneInfo = new ZoneInfo();
  zoneInfo->Stuff(space);
  return zoneInfo;
}

AdInfo *dragon::GetAdInfoById(int id, NamedSemiSpace &space)
{
  space.SetPos(0);
  OffsetTable table1;
  OffsetTable table;

  table1.Open(1000, space);
  table.Open(1000, space);

  unsigned offset = table.Get(id);
  if (offset == 0)
    return NULL;
  space.SetPos(offset);
  AdInfo *adInfo = new AdInfo();
  adInfo->Stuff(space);
  return adInfo;
}
