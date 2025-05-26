
# IRC Server en C++98

Este proyecto es una implementación mínima de un servidor IRC en C++98 como parte del cursus de 42. El servidor permite a múltiples clientes conectarse, unirse a canales, enviar mensajes y más, cumpliendo con las especificaciones básicas del protocolo IRC.

Objetivos del proyecto:

- Aprender cómo funciona el protocolo IRC.
- Profundizar en programación de sockets y redes en C++.
- Trabajar con file descriptors no bloqueantes y `poll()`.
- Gestionar múltiples clientes concurrentemente.
- Seguir buenas prácticas de diseño en C++98.

## Compilación

### Clona el repositorio y ejecuta el Makefile:
make; 
El ejecutable se generará con el nombre ircserv

### Para compilar y ejecutar:
- make test                         → ejecuta ./ircserv 6667 password
- make test PORT=1234 PASS=hola123  → ejecuta ./ircserv 1234 hola123

## Ejecución

./ircserv <puerto> <contraseña>

## Estructura del código


## Comandos implementados


## Cosas por mejorar / pendientes

## Cliente pendiente de valorar

- hexchat
- irssi

### Conectarte a tu servidor con irssi

- 


