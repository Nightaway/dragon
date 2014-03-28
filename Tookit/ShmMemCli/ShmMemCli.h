#ifndef __SHM_MEM_CLI_H__
#define __SHM_MEM_CLI_H__

#include <core/Space.h>
#include <core/Table.h>

struct Option {
  bool ShowHelp;
  char *SharedMemObjName;

  Option() {
    ShowHelp = false;
    SharedMemObjName = NULL;
  }
};

bool ParseOption(int argc, char *argv[]);

#endif
