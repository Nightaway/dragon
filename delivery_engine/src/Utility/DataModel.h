#ifndef __DATA_MODEL_H__
#define __DATA_MODEL_H__

#include <core/Macro.h>
#include "../Models/AdInfo.h"
#include "../Models/ZoneInfo.h"

NS_DRAGON

ZoneInfo *GetZoneInfoById(int id, NamedSemiSpace &space);
AdInfo   *GetAdInfoById(int id, NamedSemiSpace &space);

NS_END


#endif
