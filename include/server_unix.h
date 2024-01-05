#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <syslog.h>
#include "cJSON.h"
#include <zlib.h>



void process_command(int newsockfd, char* command);

char* get_command(char* string);

void server_unix(char* argv);


