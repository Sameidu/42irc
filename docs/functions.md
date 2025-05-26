# FUNCIONES PERMITIDAS

## socket
> include <sys/socket.h>

Crea un nuevo descriptor de socket, que permite establecer una comunicación entre procesos a través de redes. Este descriptor se comporta como un archivo y puede usarse para leer, escribir y cerrar conexiones.

## close
> include <unistd.h>

Cierra un descriptor de archivo o socket abierto, liberando los recursos asociados. Debe usarse siempre al terminar comunicación o producirse un error.

## setsockopt
> include <sys/socket.h>

Permite establecer opciones de configuración en un socket, como permitir la reutilización de direcciones, establecer tiempos de espera o activar opciones de bajo nivel del protocolo.

- Ajustar tamaño de buffers
- Configurar keep-alive
- Habilitar opciones TCP especificas

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

## connect

## listen

## accept

## htons

## htonl

## ntohs

## ntohl

## inet_addr

## inet_ntoa

## send

## recv

## signal

## sigaction

## lseek

## fstat

## fcntl

## poll

## select()

## kqueue()

## epoll()