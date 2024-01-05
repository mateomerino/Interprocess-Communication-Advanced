#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fcntl.h>
#include "cJSON.h"
#include <zlib.h>

#define TAM 1024
// #define SIZE 20480
#define WHITE  "\033[037m"
#define GREEN "\033[032m"



bool command_validation(char* buf);

void recv_result(int sockfd);

void add_json_flag(char* buffer);
