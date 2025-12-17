#include "types.h"
#include "stat.h"
#include "user.h"

int syscount(int);

// Nombres de todas las syscalls para mostrar en el reporte
char *syscall_names[] = {
  "",          // 0 - no usado
  "fork",      // 1
  "exit",      // 2
  "wait",      // 3
  "pipe",      // 4
  "read",      // 5
  "kill",      // 6
  "exec",      // 7
  "fstat",     // 8
  "chdir",     // 9
  "dup",       // 10
  "getpid",    // 11
  "sbrk",      // 12
  "sleep",     // 13
  "uptime",    // 14
  "open",      // 15
  "write",     // 16
  "mknod",     // 17
  "unlink",    // 18
  "link",      // 19
  "mkdir",     // 20
  "close",     // 21
  "trace",     // 22
  "numprocs",  // 23
  "getmem",    // 24
  "syscount",  // 25
};

int
main(int argc, char *argv[])
{
  // Si se proporciona un argumento, mostrar el contador de una syscall especifica
  if(argc > 1) {
    int syscall_num = atoi(argv[1]);
    
    // Valida que el numero este en el rango valido
    if(syscall_num < 0 || syscall_num > 25) {
      printf(2, "Error: numero de syscall invalido (debe estar entre 0 y 25)\n");
      exit();
    }
    
    // Obtiene el contador para esa syscall
    int count = syscount(syscall_num);
    
    printf(1, "\n=== CONTADOR DE INVOCACIONES ===\n");
    printf(1, "Syscall: ");
    printf(1, syscall_names[syscall_num]);
    printf(1, " (ID: %d)\n", syscall_num);
    printf(1, "Invocaciones: %d\n", count);
    printf(1, "================================\n\n");
  } else {
    // Si no hay argumentos, mostrar un resumen de todas las syscalls
    printf(1, "\n====== RESUMEN DE INVOCACIONES DE SYSCALLS ======\n");
    printf(1, "ID | Nombre      | Invocaciones\n");
    printf(1, "---|-------------|-------------\n");
    
    // Muestra el contador para cada syscall
    for(int i = 1; i <= 25; i++) {
      int count = syscount(i);
      printf(1, " %d | ", i);
      printf(1, syscall_names[i]);
      printf(1, " | %d\n", count);
    }
    
    printf(1, "*****************************************\n\n");
  }
  
  exit();
}