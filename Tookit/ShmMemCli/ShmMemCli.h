#ifndef __SHM_MEM_CLI_H__
#define __SHM_MEM_CLI_H__

#include <core/Space.h>
#include <core/Table.h>

struct Option {
  bool ShowHelp;
  char *SharedMemObjName;
  unsigned SharedMemObjSize;

  Option() {
    ShowHelp = false;
    SharedMemObjName = NULL;
    SharedMemObjSize = 0;
  }
};

bool ParseOption(int argc, char *argv[]);

#endif
