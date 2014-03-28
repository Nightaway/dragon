#include <cstdio>
#include <cstring>

#include "ShmMemCli.h"

Option option;

int main(int argc, char *argv[]) {
  if (!ParseOption(argc, argv)) return 1;
  if (option.ShowHelp) {
    printf("                   -h Show Help Info\n");
    printf("                  --SharedMemObjName $name $size \n");
    printf("                  --ShowTable List All Tables\n");
  } else {
    printf("Shared Memory Object Name:%s size:%d\n", 
           option.SharedMemObjName,
           option.SharedMemObjSize);
    dragon::NamedSemiSpace space(option.SharedMemObjName, option.SharedMemObjSize);
    space.Open();
    dragon::OffsetTable table;
    table.Open(1000, space);
    for (unsigned i=0; i<1000; i++)
    {
      printf("index:%d, offset:%d\n", i, table.Get(i));
    }

    space.Close();
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
        option.SharedMemObjSize = atoi(argv[++i]);
      }
    }
  }
  return true;
}
