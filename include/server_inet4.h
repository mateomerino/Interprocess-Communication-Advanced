#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <syslog.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <zlib.h>
#include <fcntl.h> 
#include "cJSON.h"
#include <time.h>
//#define TAM 256

#define CHUNK 512
#define CHECKSUM 100

//#define BACKLOG 9223372036854775807

void get_zip_name(char* output, int client_n);

void get_txt_name(char* message,int client_n);

void write_in_txt(char* message,char* buffer);

void write_in_zip(char* output,char* message, char*buffer);

void send_zip(char* output,int newsockfd);

char* data_to_json(char* log, char* clientname);

void server_inet4(int port);
