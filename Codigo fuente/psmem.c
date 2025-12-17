#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  // Obtiene información básica del proceso actual
  int pid = getpid();
  int ticks = uptime();
  
  // Calcula tiempo en segundos (100 ticks por segundo en XV6)
  int segundos = ticks / 100;
  int minutos = segundos / 60;
  segundos = segundos % 60;
  
  // Muestra información del proceso actual
  printf(1, "\n===== INFORMACION DEL PROCESO ACTUAL =====\n");
  printf(1, "PID del proceso: %d\n", pid);
  printf(1, "Tiempo de sistema: %d ticks\n", ticks);
  printf(1, "Tiempo formateado: %d minutos %d segundos\n", minutos, segundos);
  printf(1, "==========================================\n\n");
  
  
  exit();
}