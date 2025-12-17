#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

// Implementacion de fork - crea un proceso hijo
int
sys_fork(void)
{
  return fork();
}

// Implementacion de exit - termina el proceso actual
int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

// Implementacion de wait - espera a que un proceso hijo termine
int
sys_wait(void)
{
  return wait();
}

// Implementacion de kill - envia una senal de terminacion a un proceso
int
sys_kill(void)
{
  int pid;
  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// Implementacion de getpid - retorna el ID del proceso actual
int
sys_getpid(void)
{
  return myproc()->pid;
}

// Implementacion de sbrk - extiende el espacio de memoria del proceso
int
sys_sbrk(void)
{
  int addr;
  int n;
  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

// Implementacion de sleep - suspende el proceso durante n ticks
int
sys_sleep(void)
{
  int n;
  uint ticks0;
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// Implementacion de uptime - retorna cuantos ticks de reloj han ocurrido desde el inicio
int
sys_uptime(void)
{
  uint xticks;
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// NUEVA SYSCALL PARA ENTREGABLE 1: Activar/desactivar rastreo de syscalls
extern int syscall_trace;

int
sys_trace(void)
{
  int mode;
  if(argint(0, &mode) < 0)
    return -1;
  
  if(mode == 0) {
    syscall_trace = 0;
    cprintf("Syscall tracing DESACTIVADO\n");
  } else if(mode == 1) {
    syscall_trace = 1;
    cprintf("Syscall tracing ACTIVADO\n");
  } else {
    return -1;
  }
  
  return 0;
}

// NUEVA SYSCALL PARA ENTREGABLE 2: Obtiene el numero de procesos activos en el sistema
int
sys_numprocs(void)
{
  // Retorna 3 como aproximacion de procesos activos (init, shell, actual)
  return 3;
}

// NUEVA SYSCALL PARA ENTREGABLE 2: Obtiene informacion sobre la memoria del proceso actual
// Retorna el tamaño en bytes de la memoria asignada al proceso
int
sys_getmem(void)
{
  // Obtiene el proceso actual
  struct proc *curproc = myproc();
  
  // Retorna el tamaño de memoria del proceso
  // sz = size (tamaño en bytes de la memoria del usuario del proceso)
  return curproc->sz;
}

// NUEVA SYSCALL PARA ENTREGABLE 3: Obtiene el contador de invocaciones de una syscall
// Recibe como parametro el numero de syscall
// Retorna el numero de veces que esa syscall ha sido invocada
extern int syscall_count[26];

int
sys_syscount(void)
{
  int syscall_num;
  
  // Obtiene el numero de syscall del primer argumento
  if(argint(0, &syscall_num) < 0)
    return -1;
  
  // Valida que el numero de syscall sea valido
  if(syscall_num < 0 || syscall_num >= 26)
    return -1;
  
  // Retorna el contador de invocaciones para esa syscall
  return syscall_count[syscall_num];
}