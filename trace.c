#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
  if(argc < 2) {
    printf(2, "Uso: trace [0|1]\n");
    printf(2, "  trace 1 - Activar rastreo de syscalls\n");
    printf(2, "  trace 0 - Desactivar rastreo de syscalls\n");
    exit();
  }

  int mode = atoi(argv[1]);

  if(mode != 0 && mode != 1) {
    printf(2, "Error: use 0 para desactivar o 1 para activar\n");
    exit();
  }

  if(trace(mode) < 0) {
    printf(2, "trace: syscall failed\n");
    exit();
  }

  exit();
}
