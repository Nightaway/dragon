#ifndef __SELECT_H__
#define __SELECT_H__

#include <vector>
#include <core/modules/Config.h>
#include <http/HttpRequest.h>
#include <http/HttpResponse.h>
#include <core/QueryString.h>
#include "../Models/AdInfo.h"
#include "Information.h"

AdInfo *SelectOne(std::vector<AdInfo *> &adInfos);

#endif
