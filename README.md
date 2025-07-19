
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
- Si usas esta regla tienes que cambiar una funcion del main para que no pete
- Cambiar runSignals(), por este:

void	runSignals() {
	signal(SIGINT, handleSignal);
	signal(SIGQUIT, handleSignal);
	//signal(SIGTSTP, handleSignal);
	signal(SIGTERM, handleSignal);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
}

## Ejecución

./ircserv <puerto> <contraseña>

## Comandos implementados

### Verificacion del usuario
- PASS <Password>
- NICK <Nickname>
- USER <Username> 0 * :<Realname>

### Comandos de canal
- INVITE <nickname> <channel>
- JOIN <channel>{,<channel>} [<key>{,<key>}]
- KICk <channel> <user> *( "," <user> ) [<comment>]
- MODE <target> [<modestring> [<mode arguments>...]]
- PART <channel>{,<channel>} [<reason>]
- TOPIC <channel> [<topic>]
- NAMES <channel>{,<channel>}
- LIST [<channel>{,<channel>}] [<elistcond>{,<elistcond>}]

### Comandos de envio de mensajes
- NOTICE <target>{,<target>} <text to be sent>
- PRIVMSG <target>{,<target>} <text to be sent>

### Comando desconexion del server
- QUIT <reason>

### Comandos no implementados pero reconocidos
- CAP <subcommand> [:<capabilities>]
- WHO <mask>

## Cliente 

- Hexchat

### Conectarte a tu servidor con hexchat

- Instala Hexchat en tu ordenador
- Escribe hexchat en la terminal, o abre hexchat a traves del acceso directo
- Configurar , nickname, username, password, puerto y ip del server.
- Hexchat manda un msg por default que ontiene
- CAP <subcommand> [:<capabilities>]
- PASS <Password>
- NICK <Nickname>
- USER <Username> 0 * :<Realname>
- Si todo esta correcto , y el cliente se ha podido conectar y verificar con exito llegara un msg RPL_WELCOME, y se te unira a un canal general
- Ya puedes usar el server IRC

## Documentacion utilizada

- https://modern.ircdocs.horse/
- https://datatracker.ietf.org/doc/html/rfc2812#section-3.2.1
