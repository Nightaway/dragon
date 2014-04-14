#ifndef __FILTER_H__
#define __FILTER_H__

#include <http/HttpRequest.h>
#include <http/HttpResponse.h>
#include <core/QueryString.h>
#include <core/modules/Config.h>
#include "../Models/AdInfo.h"
#include "Information.h"

typedef bool(*FilterHandle)(AdInfo *adInfo, Information &infos);

void filter(std::vector<AdInfo *> &adInfos, Information &info, FilterHandle handle);

extern bool VisitorFilter(AdInfo *adInfo, Information &infos);
extern bool StandardFilter(AdInfo *adInfo, Information &infos);
extern bool StandardFilterInit();

#endif
