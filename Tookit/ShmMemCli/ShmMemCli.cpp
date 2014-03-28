#include <cstdio>
#include <cstring>

#include "ShmMemCli.h"

Option option;

int main(int argc, char *argv[]) {
  if (!ParseOption(argc, argv)) return 1;
  if (option.ShowHelp) {
    printf("                   -h Show Help Info\n");
    printf("                  --SharedMemObjName Specify Shared Memory Object Name\n");
    printf("                  --ShowTable List All Tables\n");
  } else {
    printf("Shared Memory Object Name:%s\n", option.SharedMemObjName);
    dragon::NamedSemiSpace space(option.SharedMemObjName, 1024);
  }
  return 0;
}

bool ParseOption(int argc, char *argv[])
{
  if (argc == 1) {
    option.ShowHelp = true;
    return true;
  }

  for (int i=1; i<argc; ++i) {
    if (strcmp(argv[i], "-h") == 0) { 
      option.ShowHelp = true;
      return true;
    } else {
      if (strcmp(argv[i], "--ShareMemObjName")) {
        option.SharedMemObjName = argv[++i];
      }
    }
  }
  return true;
}
