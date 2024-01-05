#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <net/if.h>
#include <errno.h>
#include <zlib.h>

//#define TAM 256


#define WHITE  "\033[037m"
#define GREEN "\033[032m"

void send_command(int sockfd, char* command);

void recv_data(int sockfd);


