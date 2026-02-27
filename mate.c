#define MATE_IMPLEMENTATION
#include "mate.h/mate.h"

void build_server_lib() {
  StaticLib server_lib =
      CreateStaticLib((StaticLibOptions){.output = "libserver",
                                         .debug = FLAG_DEBUG,
                                         .warnings = FLAG_WARNINGS_VERBOSE});

  AddFile(server_lib, "./src/server.c");
  InstallStaticLib(server_lib);
}

void build_server_exec() {
  Executable executable =
      CreateExecutable((ExecutableOptions){.output = "main",
                                           .debug = FLAG_DEBUG,
                                           .warnings = FLAG_WARNINGS_VERBOSE});
  AddFile(executable, "./src/main.c");
  AddLibraryPaths(executable, "./build");
  LinkSystemLibraries(executable, "server");

  InstallExecutable(executable);
}

i32 main(void) {
  StartBuild();
  {
    build_server_lib();
    build_server_exec();
  }
  EndBuild();
}
