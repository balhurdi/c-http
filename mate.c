#define MATE_IMPLEMENTATION
#include "mate.h/mate.h"

void build_server_lib() {
  StaticLib server_lib =
      CreateStaticLib((StaticLibOptions){.output = "libserver",
                                         .debug = FLAG_DEBUG,
                                         .warnings = FLAG_WARNINGS_VERBOSE});

  AddFile(server_lib, "./src/server.c");
  AddFile(server_lib, "./src/client.c");

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

void build_client_tests() {
  Executable executable =
      CreateExecutable((ExecutableOptions){.output = "client_tests",
                                           .debug = FLAG_DEBUG,
                                           .warnings = FLAG_WARNINGS_VERBOSE});
  AddFile(executable, "./src/tests/client_tests.c");
  AddLibraryPaths(executable, "./build");
  LinkSystemLibraries(executable, "server");

  InstallExecutable(executable);
  errno_t err = RunCommand(executable.outputPath);
  Assert(err == SUCCESS, "RunCommand: failed client tests "
                         "SUCCESS"); // Assert that we return SUCCESS == 0
}

i32 main(void) {
  StartBuild();
  {
    build_server_lib();
    build_client_tests();
    build_server_exec();
  }
  EndBuild();
}
