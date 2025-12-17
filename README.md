# Modificaciones al Sistema Operativo XV6

##  Información del Proyecto

**Universidad Nacional Jorge Basadre Grohmann**  
Escuela Profesional de Ingeniería en Informática y Sistemas

- **Equipo:** E15
- **Curso:** Sistemas Operativos
- **Docente:** Ing. Hugo Manuel Barraza Vizcarra

### Integrantes
- Edu Rubinho Puma Ccama - 2023-119053
- Jorge Enrique Obando Huallpa - 2017-130045
- Vladimir Roger Ticona Mamani - 2023-119063

---

##  Descripción

Proyecto de extensión del sistema operativo educativo **XV6** (MIT), implementando tres funcionalidades principales:
1. Rastreo de llamadas al sistema en tiempo real
2. Comandos de monitoreo del sistema (uptime, psmem)
3. Contador de invocaciones de syscalls

**XV6** es una reimplementación simplificada de Unix V6, diseñada para enseñanza de sistemas operativos.

---

##  Entregables

### 1 Rastreo de Llamadas al Sistema (Syscall Tracing)

**¿Qué hace?**  
Permite visualizar en tiempo real todas las syscalls que ejecuta el sistema, mostrando el nombre y parámetros de cada una.

**Conceptos clave:**
- **System Call (Syscall):** Interfaz entre programas de usuario y el kernel
- **Modo Usuario vs Kernel:** Las syscalls son el "puente" seguro entre ambos modos
- Cada syscall tiene un número único y nombre (fork, exec, read, write, etc.)

**Implementación:**
- Variable `syscall_trace` como interruptor ON/OFF
- Arreglo de nombres de syscalls
- Interceptación y registro de cada invocación

**Archivos modificados:**
- `syscall.c` - Lógica de rastreo
- `sysproc.c` - Nueva syscall `trace()`
- `trace.c` - Comando de usuario
- `syscall.h`, `user.h`, `usys.S`

**Uso:**
```bash
$ trace 1           # Activar rastreo
$ ls                # Ejecutar comando
[TRACE] PID 5: fork
[TRACE] PID 5: exec
[TRACE] PID 5: write
$ trace 0           # Desactivar
```

---

### 2 Comandos de Monitoreo del Sistema

#### Comando: uptime

**Funcionalidad:**
- Muestra tiempo de ejecución del sistema (ticks, segundos, minutos)
- Indica número de procesos activos

**Nuevas syscalls:**
- `numprocs()` - Cuenta procesos activos
- Usa `uptime()` existente

**Ejemplo:**
```bash
$ uptime
Sistema: 12345 ticks (123 seg, 2 min)
Procesos activos: 3
```

#### Comando: psmem

**Funcionalidad:**
- Muestra PID del proceso actual
- Memoria asignada (bytes y KB)
- Tiempo del sistema

**Nuevas syscalls:**
- `getmem()` - Obtiene memoria del proceso

**Ejemplo:**
```bash
$ psmem
PID: 5
Memoria: 12288 bytes (12 KB)
Sistema: 15000 ticks
```

**Archivos creados/modificados:**
- `uptime.c`, `psmem.c` - Comandos
- `sysproc.c` - Implementación syscalls
- `Makefile` - Compilación

---

### 3 Contador de Invocaciones

**¿Qué hace?**  
Registra estadísticas de cuántas veces se invoca cada syscall durante la ejecución del sistema.

**Implementación:**
- Arreglo `syscall_count[26]` en el kernel
- Incremento automático en cada invocación
- Nueva syscall `syscount()` para consultar

**Comando syscountcmd:**

```bash
# Ver todas las syscalls
$ syscountcmd
ID  | Syscall  | Invocaciones
----+----------+-------------
1   | fork     | 45
5   | read     | 156
16  | write    | 2145
21  | close    | 90

# Consultar específica
$ syscountcmd 5
La syscall 5 (read) ha sido invocada 156 veces
```

**Archivos modificados:**
- `syscall.c` - Arreglo de contadores
- `syscountcmd.c` - Comando de usuario
- `sysproc.c` - Implementación `sys_syscount()`

---

##  Compilación y Ejecución

### Compilar XV6
```bash
cd "Codigo fuente"
make
```

### Ejecutar
```bash
make qemu          # Con interfaz gráfica
make qemu-nox      # Sin interfaz
```

### Salir
- Presiona `Ctrl + A`, luego `X`

### Probar funcionalidades
```bash
# Rastreo
$ trace 1
$ ls
$ trace 0

# Monitoreo
$ uptime
$ psmem

# Estadísticas
$ syscountcmd
$ syscountcmd 16
```

---

##  Tabla de System Calls

| ID | Nombre   | Descripción                          |
|----|----------|--------------------------------------|
| 1  | fork     | Crea un nuevo proceso                |
| 2  | exit     | Termina el proceso actual            |
| 5  | read     | Lee datos de un archivo              |
| 7  | exec     | Ejecuta un programa                  |
| 11 | getpid   | Obtiene ID del proceso               |
| 14 | uptime   | Obtiene tiempo del sistema           |
| 15 | open     | Abre un archivo                      |
| 16 | write    | Escribe datos a un archivo           |
| 21 | close    | Cierra un descriptor de archivo      |
| 22 | trace    | Activa/desactiva rastreo *(NUEVA)*   |
| 23 | numprocs | Cuenta procesos activos *(NUEVA)*    |
| 24 | getmem   | Obtiene memoria del proceso *(NUEVA)*|
| 25 | syscount | Obtiene contador de syscall *(NUEVA)*|


