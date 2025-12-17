#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "syscall.h"

// Usuario hace una llamada al sistema con INT T_SYSCALL.
// El número de la syscall está en %eax.
// Los argumentos están en el stack

// VARIABLE GLOBAL PARA ACTIVAR/DESACTIVAR RASTREO
int syscall_trace = 0;

// Obtiene un entero de la dirección en el proceso actual
int
fetchint(uint addr, int *ip)
{
  struct proc *curproc = myproc();
  if(addr >= curproc->sz || addr+4 > curproc->sz)
    return -1;
  *ip = *(int*)(addr);
  return 0;
}

// Obtiene una cadena terminada en nulo de la dirección en el proceso actual
int
fetchstr(uint addr, char **pp)
{
  char *s, *ep;
  struct proc *curproc = myproc();

  if(addr >= curproc->sz)
    return -1;
  *pp = (char*)addr;
  ep = (char*)curproc->sz;
  for(s = *pp; s < ep; s++){
    if(*s == 0)
      return s - *pp;
  }
  return -1;
}

// Obtiene el argumento n-ésimo de la syscall como un entero
int
argint(int n, int *ip)
{
  return fetchint((myproc()->tf->esp) + 4 + 4*n, ip);
}

// Obtiene el argumento n-ésimo como un puntero
int
argptr(int n, char **pp, int size)
{
  int i;
  struct proc *curproc = myproc();
  if(argint(n, &i) < 0)
    return -1;
  if(size < 0 || (uint)i >= curproc->sz || (uint)i+size > curproc->sz)
    return -1;
  *pp = (char*)i;
  return 0;
}

// Obtiene el argumento n-ésimo como una cadena
int
argstr(int n, char **pp)
{
  int addr;
  if(argint(n, &addr) < 0)
    return -1;
  return fetchstr(addr, pp);
}

// Declaraciones externas de las implementaciones de syscalls
extern int sys_chdir(void);
extern int sys_close(void);
extern int sys_dup(void);
extern int sys_exec(void);
extern int sys_exit(void);
extern int sys_fork(void);
extern int sys_fstat(void);
extern int sys_getpid(void);
extern int sys_kill(void);
extern int sys_link(void);
extern int sys_mkdir(void);
extern int sys_mknod(void);
extern int sys_open(void);
extern int sys_pipe(void);
extern int sys_read(void);
extern int sys_sbrk(void);
extern int sys_sleep(void);
extern int sys_unlink(void);
extern int sys_wait(void);
extern int sys_write(void);
extern int sys_uptime(void);
extern int sys_trace(void);
// NUEVAS SYSCALLS PARA ENTREGABLE 2
extern int sys_numprocs(void);
extern int sys_getmem(void);

// ARREGLO CON LOS NOMBRES DE TODAS LAS SYSCALLS (para rastreo)
char *syscallnames[] = {
  [SYS_fork]    "fork",
  [SYS_exit]    "exit",
  [SYS_wait]    "wait",
  [SYS_pipe]    "pipe",
  [SYS_read]    "read",
  [SYS_kill]    "kill",
  [SYS_exec]    "exec",
  [SYS_fstat]   "fstat",
  [SYS_chdir]   "chdir",
  [SYS_dup]     "dup",
  [SYS_getpid]  "getpid",
  [SYS_sbrk]    "sbrk",
  [SYS_sleep]   "sleep",
  [SYS_uptime]  "uptime",
  [SYS_open]    "open",
  [SYS_write]   "write",
  [SYS_mknod]   "mknod",
  [SYS_unlink]  "unlink",
  [SYS_link]    "link",
  [SYS_mkdir]   "mkdir",
  [SYS_close]   "close",
  [SYS_trace]   "trace",
  [SYS_numprocs] "numprocs",
  [SYS_getmem]   "getmem",
};

// Tabla de punteros a las funciones de syscalls
static int (*syscalls[])(void) = {
  [SYS_fork]    sys_fork,
  [SYS_exit]    sys_exit,
  [SYS_wait]    sys_wait,
  [SYS_pipe]    sys_pipe,
  [SYS_read]    sys_read,
  [SYS_kill]    sys_kill,
  [SYS_exec]    sys_exec,
  [SYS_fstat]   sys_fstat,
  [SYS_chdir]   sys_chdir,
  [SYS_dup]     sys_dup,
  [SYS_getpid]  sys_getpid,
  [SYS_sbrk]    sys_sbrk,
  [SYS_sleep]   sys_sleep,
  [SYS_uptime]  sys_uptime,
  [SYS_open]    sys_open,
  [SYS_write]   sys_write,
  [SYS_mknod]   sys_mknod,
  [SYS_unlink]  sys_unlink,
  [SYS_link]    sys_link,
  [SYS_mkdir]   sys_mkdir,
  [SYS_close]   sys_close,
  [SYS_trace]   sys_trace,
  // NUEVAS SYSCALLS REGISTRADAS
  [SYS_numprocs] sys_numprocs,
  [SYS_getmem]   sys_getmem,
};

// Manejador principal de las llamadas al sistema
void
syscall(void)
{
  int num;
  struct proc *curproc = myproc();

  // Obtiene el número de syscall del registro eax
  num = curproc->tf->eax;
  
  // Ejecuta la syscall si es válida
  if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
    curproc->tf->eax = syscalls[num]();
    
    // SI EL RASTREO ESTÁ ACTIVADO, MOSTRAR INFORMACIÓN DESPUÉS DE EJECUTAR
    if(syscall_trace && num > 0 && num < NELEM(syscallnames) && syscallnames[num]) {
      cprintf("[TRACE] PID %d: %s\n", curproc->pid, syscallnames[num]);
    }
  } else {
    cprintf("%d %s: unknown sys call %d\n",
            curproc->pid, curproc->name, num);
    curproc->tf->eax = -1;
  }
}