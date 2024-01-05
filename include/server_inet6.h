#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "cJSON.h"
#include <zlib.h>


char* tolowerstring(char* string);

char* getfreememory(char* string);

char* get_load(char* string);

void send_data(int newsockfd, char* string);

void server_inet6( uint16_t port );

