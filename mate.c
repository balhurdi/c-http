#define MATE_IMPLEMENTATION
#include "mate.h/mate.h"

i32 main(void) {
  StartBuild();
  {
    Executable executable = CreateExecutable(
        (ExecutableOptions){.output = "server", .flags = "-Wall -g"});

    AddFile(executable, "./src/main.c");
    AddFile(executable, "./src/server.c");

    InstallExecutable(executable);
  }
  EndBuild();
}
