#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fcntl.h>
#include "cJSON.h"
#include <zlib.h>
#include <time.h>


#define CHUNK 512
#define CHECKSUM 100


void recv_zip(int sockfd);

void get_client_name(char* clientname);
