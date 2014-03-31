#ifndef __FILTER_H__
#define __FILTER_H__

#include <http/HttpRequest.h>
#include <http/HttpResponse.h>
#include "../Models/AdInfo.h"
#include "Information.h"

typedef bool(*FilterHandle)(AdInfo *adInfo, Information &infos);

void filter(std::vector<AdInfo *> &adInfos, Information &info, FilterHandle handle);

extern bool CookieFilter(AdInfo *adInfo, Information &infos);

#endif
