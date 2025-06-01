# FUNCIONES PERMITIDAS

-> https://hyjae.gitbooks.io/socket-programming/content/

## socket
> include <sys/socket.h>

Crea un nuevo descriptor de socket, que permite establecer una comunicación entre procesos a través de redes. Este descriptor se comporta como un archivo y puede usarse para leer, escribir y cerrar conexiones.

- Estructura de direccion del socket:

struct sockaddr_in {
    short sin_family;
    unsigned short sin_port; // Port Number
    struct in_addr sin_addr; // IP Address
    char sin_zero[8];
}

## close
> include <unistd.h>

Cierra un descriptor de archivo o socket abierto, liberando los recursos asociados. Debe usarse siempre al terminar comunicación o producirse un error.

## setsockopt 
https://pubs.opengroup.org/onlinepubs/000095399/functions/setsockopt.html
> include <sys/socket.h>

Permite establecer opciones de configuración en un socket, como permitir la reutilización de direcciones, establecer tiempos de espera o activar opciones de bajo nivel del protocolo.

- Ajustar tamaño de buffers
- Configurar keep-alive
- Habilitar opciones TCP especificas

Cuando cierras un socket servidor, el sistema lo deja en un estado llamado TIME_WAIT durante un tiempo (unos minutos), para asegurarse de que no haya paquetes retrasados pululando por la red.
Durante ese tiempo, si intentas hacer un nuevo bind() al mismo puerto, el sistema te dirá: "no puedes, ya está en uso"
Si vuelves ha hacer bind(), te fallara por que el sistema dira que ya esta en uso ese puerto

Con la funcion setsockopt y la opcion SO_REUSEADDR le dice al sistema operativo que le deje usar ese puerto aun que este ocupado, y que te haces cargo de ello
Si ejecutas el server lo detienes y lo vuelves a lanzar , o aun hay clientes conectados a ese puerto fallara

int opt = 1;
setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

- The level argument specifies the protocol level at which the option resides. 
To set options at the socket level, specify the level argument as SOL_SOCKET.
- SO_REUSEADDR: Specifies that the rules used in validating addresses supplied to bind() should allow reuse of local addresses, 
if this is supported by the protocol. This option takes an int value. This is a Boolean option.

## getsockname
> include <sys/socket.h>
> include <netinet/in.h>

Permite obtener la dirección local (IP y puerto) asociada a un socket enlazado.

## getprotobyname
> include <netdb.h>

Permite obtener información sobre un protocolo de red a partir de su nombre. Devuelve una estructura <ins>*protoent*</ins> que contiene el número de protocolo, necesario para ciertas llamadas como <ins>*socket*</ins>. La estructura *protoent* contiene:

- *p_name*: nombre oficial del protocolo.
- *p_proto*: número del protocolo.
- *p_aliases*: lista de nombres alternativos.

## gethostbyname
> include <netdb.h>

**Obsoleta, se utiliza la función <ins>*getaddrinfo*</ins>**

Realiza la resolución de un nombre de host (como "localhost" o "www.example.com") y devuelve su dirección IP asociada en una estructura <ins>hostent</ins>.

- *h_name*: nombre oficial del host.
- *h_addrtype*: tipo de dirección.
- *h_length*: longitud de la dirección.
- *h_addr_list*: lista de direcciones.

## getaddrinfo
> include <netdb.h>

Realiza la resolución de nombres de dominio y servicios en direcciones IP y crea una lista de estructuras <ins>sockaddr</ins>. Reemplaza a funciones obsoletas como gethostbyname y getservbyname, y es compatible con IPv4 e IPv6.

- *ai_flags*: Opciones de configuración.
- *ai_family*: Familia de direcciones.
- *ai_socktype*: Tipo de socket.
- *ai_protocol*: Protocolo a usar.
- *ai_addrlen*: Tamaño en bytes de la estructura *ai_addr*.
- *ai_addr*: Puntero a la dirección.
- *ai_canonname*: Nombre canónico del host.
- *ai_next*: Siguiente nodo de la lista.

## freeaddrinfo
> include <netdb.h>

Libera la memoria dinámica asignada. Es obligatorio llamarla una vez que hayas terminado de usar la lista de resultados devuelta por <ins>*getaddrinfo*</ins>, para evitar fugas de memoria. No cierra sockets ni libera ningún otro recurso.

## bind
https://pubs.opengroup.org/onlinepubs/009695399/functions/bind.html
> include <sys/socket.h>
> include <netinet/in.h>

Asocia un socket con una dirección local (IP y puerto). Es necesario antes de escuchar conexiones entrantes en un servidor.

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

- sockfd: el descriptor de fichero del socket.
- addr: puntero a una estructura que contiene la dirección (como IP y puerto).
- addrlen: el tamaño de esa estructura.

## connect
> include <sys/socket.h>
> include <netinet/in.h>

Establece una conexión con un socket remoto.

## listen
https://pubs.opengroup.org/onlinepubs/009695099/functions/listen.html
> include <sys/socket.h>

Marca un socket como pasivo, es decir, preparado para aceptar conexiones entrantes. Se usa después de <ins>*bind()*</ins>.

int listen(int sockfd, int backlog);

- backlog es el número máximo de conexiones pendientes que el sistema puede mantener en cola mientras tu servidor aún no las acepta con accept()
no significa que sean esos los usuarios que se puedan conectar significa que puede haber hasta x conexiones esperando a ser atendidas por acept()

## accept
> include <>

## htons
https://www.gta.ufrj.br/ensino/eel878/sockets/htonsman.html
> include <netinet/in.h>

Como cada pc tiene un orden diferente de bytes estas funciones sirven para convertir el orden de bytes de tu host (maquina) a el orden de bytes de la network

htons - de host a red corta

## htonl
https://www.gta.ufrj.br/ensino/eel878/sockets/htonsman.html
> include <netinet/in.h>

htonl - de host a red larga

## ntohs
https://www.gta.ufrj.br/ensino/eel878/sockets/htonsman.html
> include <netinet/in.h>
ntohs - de red a host larga

## ntohl
https://www.gta.ufrj.br/ensino/eel878/sockets/htonsman.html
> include <netinet/in.h>

ntohl - de red a host corta

## inet_addr
> include <>

## inet_ntoa
> include <>

## send
> include <>

## recv
> include <>

## signal
> include <>

## sigaction
> include <>

## lseek
> include <>

## fstat
> include <>

## fcntl
https://pubs.opengroup.org/onlinepubs/007904975/functions/fcntl.html
> include <fcntl.h>

En nuestro caso lo usaremos para modificar las flags del archivo a O_NONBLOCK
con F_GETFD para conseguirlas y F_SETFD para modificarlas

Sirve para:
- Manipular descriptores de archivos (como sockets o archivos abiertos).
- Cambiar o leer opciones y propiedades asociadas a esos descriptores.

int fcntl(int fd, int cmd, ... /* arg */ );
- fd: el descriptor de archivo (ej: un socket).
- cmd: qué operación quieres hacer.
- arg: argumento adicional (depende del comando).

## poll
https://pubs.opengroup.org/onlinepubs/009696799/functions/poll.html
> include <poll.h>
poll(), es una funcion del sistema que te permite monitorizar multiples descriptores de archivos como socketspara saber si puedes leer o escribir en ellos sin bloquear tu programa
Necesitamos poder escuchar muchas conexiones a la vez sin que uan sola conexion bloquee las demas

int poll(struct pollfd fds[], nfds_t nfds, int timeout);
    - fds[]: Array de estructuras pollfd, que indican qué descriptores de archivo quieres   observar y qué eventos te interesan (lectura, escritura, errores...).
    - nfds: Número de elementos en el array fds[].
    - timeout:
        - 0: No espera, devuelve de inmediato (modo no bloqueante).
        - -1: Espera indefinidamente hasta que haya algo que hacer.
        - 0: Espera ese número de milisegundos como máximo.

struct pollfd {
    int   fd;         // File descriptor que quieres vigilar
    short events;     // Qué eventos te interesan (ej: POLLIN, POLLOUT)
    short revents;    // Qué eventos han ocurrido (rellenado por poll())
};

POLLIN	- Hay datos para leer
POLLOUT	- Puedes escribir sin bloquear
POLLERR	- Error en el descriptor
POLLHUP	- El otro extremo ha cerrado la conexión

Con poll(), se puede hacer:
- Escuchar nuevas conexiones entrantes.
- Leer mensajes de clientes conectados.
- Enviar mensajes cuando estén listos para recibirlos.
- Detectar errores o desconexiones.

## select
https://pubs.opengroup.org/onlinepubs/7908799/xsh/select.html
> include <sys/time.h>

Sirve para lo mismo que poll(), pero select() tiene un limite fijo de descriptores , que depende de FD_SETSIZE 

## kqueue
> include <>

## epoll
> include <>