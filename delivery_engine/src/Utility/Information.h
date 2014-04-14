#ifndef __INFORMATION_H__
#define __INFORMATION_H__

struct Information { 
  dragon::HttpRequest     *req;
  dragon::HttpResponse    *res;
  dragon::QueryString	  *qs;
  dragon::Config		  *config;
};

#endif
