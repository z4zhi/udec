Para compilar: $ g++ mini_shell.cpp
Al ejecutar el programa, aparecera un prompt, donde se pueden ingresar los comandos con los argumentos.
Para salir, ingresar "exit" o apretar CTRL+C

Existe el comando sig <tipo> <pid> para enviar una señal a algun proceso:
  tipo = 1: señal SIGUSR1
  tipo = 2: señal SIGUSR2
  pid: pid del proceso a enviar la señal
  
