#include "Filter.h"

#include <cassert>

void filter(std::vector<AdInfo *> &adInfos, Information &info, FilterHandle handle)
{
  assert(handle);
  std::vector<AdInfo *>::iterator iter; 
  for (iter = adInfos.begin(); iter != adInfos.end();) {
    bool ret = handle(*iter, info);
    if (ret) {
      adInfos.erase(iter);
      iter = adInfos.begin();
    } else {
      iter++;
    }
  }
}
