#include <iostream>
#include <cstdlib>


bool    isvalidPort(char *str_port)
{
    /* TODO: si le pasas 7643dsjfhd funciona ,
     no se si habria que caparlo mas, por el atoi*/
    int port = std::atoi(str_port);

    if(port <= 1024 || port > 65535)
    {
        std::cerr << "The port should be between 1025 and 65535." << std::endl;
        return false;
    }
    /* TODO: guardar el valor */
    return true;
}


int main(int argc, char** argv) 
{
    if (argc != 3)
    {
        std::cerr << "Not a good argument please enter ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    std::cout << "Número de argumentos: " << argc << std::endl;
    if (argv[1] && !isvalidPort(argv[1]))
        return 1;

    for (int i = 0; i < argc; ++i)
        std::cout << "argv[" << i << "]: " << argv[i] << std::endl;


    return 0;
}