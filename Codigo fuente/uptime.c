#include "types.h"
#include "stat.h"
#include "user.h"

// Declaraciones explícitas de las nuevas funciones
int numprocs(void);

int
main(int argc, char *argv[])
{
  // Obtiene los ticks del reloj del sistema
  int ticks = uptime();
  
  // Obtiene el número de procesos activos en el sistema
  int num_procesos = numprocs();
  
  // Calcula el tiempo en segundos (asumiendo 100 ticks por segundo en XV6)
  int segundos = ticks / 100;
  int minutos = segundos / 60;
  segundos = segundos % 60;
  
  // Muestra la información del sistema
  printf(1, "=== INFORMACION DEL SISTEMA XV6 ===\n");
  printf(1, "Tiempo de ejecucion: %d ticks\n", ticks);
  printf(1, "Tiempo formateado: %d minutos %d segundos\n", minutos, segundos);
  printf(1, "Numero de procesos activos: %d\n", num_procesos);
  printf(1, "=====================================\n");
  
  exit();
}
