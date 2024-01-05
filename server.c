#include "include/server_inet4.h"
#include "include/client_inet4.h"
#include "include/server_inet6.h"
#include "include/client_inet6.h"
#include "include/server_unix.h"
#include "include/client_unix.h"

#define BUF_SIZE 512

int main(int argc, char* argv[]){
    
    
    if(argc != 4){

        printf("Use: <program_name> - <Unix port (1111 recommended)>, <Ipv4 port (1114 recommended)>, <Ipv4 port (1114 recommended)>\n");

        exit(EXIT_SUCCESS);
    }

    // uint16_t unix_port = (uint16_t)atoi(argv[1]);

    uint16_t ipv4_port = (uint16_t)atoi(argv[2]);
    
    uint16_t ipv6_port = (uint16_t)atoi(argv[3]);

    
    printf("\nEnter 'end' to close all servers\n");
    
    int pid1, pid2, pid3;

    pid1 = fork();

    if(pid1 == 0){

        printf("UNIX Server starting up...\n");

        server_unix(argv[1]);

        exit(EXIT_SUCCESS);

    }
    
    pid2 = fork();

    if(pid2 == 0){

        printf("INET IPV4 Server starting up...\n");

        server_inet4(ipv4_port);

        exit(EXIT_SUCCESS);

    }
    
    pid3 = fork();

    if(pid3 == 0){

        printf("INET IPV6 Server starting up...\n");

        server_inet6(ipv6_port);

        exit(EXIT_SUCCESS);

    }

    /*
 * Lo unico que hace el proceso padre es quedarse en este while
 * preguntando por si el usuario desea apagar el servidor.
 * En ese caso, se eliminan todos los canales de IPC creados.
*/
  char aux[BUF_SIZE];

  while(1){

    fgets(aux,BUF_SIZE,stdin);

    if(strncmp(aux, "end", 3) == 0){

      kill(pid1,SIGKILL);

      kill(pid2,SIGKILL);

      kill(pid3,SIGKILL);

      return(EXIT_SUCCESS);
    }
}

    

}