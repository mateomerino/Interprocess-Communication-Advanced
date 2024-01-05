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

#include "include/functs.h"
#include "include/cJSON.h"

char* string_to_json(char* buf){

	cJSON* object= cJSON_CreateObject();

	char client_query[100];

	sprintf(client_query, "Client_PID_%d", getpid());

	cJSON_AddStringToObject(object,client_query,buf);

	char* string=cJSON_Print(object);

	cJSON_Delete(object);

	return string;
}

char *strtrim(char *str)
{
    while ((*str == ' ') || (*str == '\t'))
        str++;

    if(*str == 0)
        return str;

    char *end = str + strlen(str) - 1;

    while((end > str) && ((*end == ' ') || (*end == '\t')))
        end--;

    end[1] = '\0';

    return str;
}

long unsigned int get_max_connections(){

	char buf[100];

    char* maxconn;

    long unsigned int backlog;

	FILE* pipe;

    if ((pipe = popen("sysctl fs.file-max", "r")) == NULL) {

        perror("popen");

        exit(EXIT_FAILURE);

    }
	
    fgets(buf,100,pipe);
   
    maxconn=strrchr(buf,' ');
   
    maxconn=strtrim(maxconn);
    
    backlog=(long unsigned int)atoll(maxconn);
    
	return backlog;
}
