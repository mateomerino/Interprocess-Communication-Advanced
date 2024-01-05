
#include "include/server_inet6.h"
#include "include/functs.h"

char* tolowerstring(char* string){

    char* aux;

    aux=string;

    for(int i=0; i < (int)strlen(string); i++){

        string[i]=(char)tolower(aux[i]);
    }
    return string;
}


char* getfreememory(char* string){

    FILE* fp;

    char info[SIZE];

    char* aux;

    fp= fopen("/proc/meminfo", "r");    

    if(fp == NULL){

        printf("INET 6 SERVER No se pudo abrir el file /proc/meminfo");

        exit(EXIT_FAILURE);

    }
    //Read file
    while( feof(fp) == 0 ){

        if(fgets(info,100,fp)){

            if(strncmp("MemFree:", (char*)info, 8) == 0){

                break;

            }
        }
    }

    strtok(info,"\n");

    aux = strchr(info,' ');

    strtrim(aux);

    cJSON* object= cJSON_CreateObject();

    cJSON_AddStringToObject(object,"FREE MEMORY:",aux);

    string=cJSON_Print(object);

    cJSON_Delete(object);

    return string;
    
}

char* get_load(char* string){

    FILE* fp;

    char info[SIZE];

    fp= fopen("/proc/loadavg", "r");

    if(fp == NULL){

        printf("INET 6 SERVER No se pudo abrir el file /proc/meminfo");

        exit(EXIT_FAILURE);

    }

    fgets(info,100,fp);

    // printf("%s\n",info);

    strtrim(info);

    strtok(info,"\n");

    // printf("%s\n",info);

    cJSON* object=cJSON_CreateObject();

    cJSON_AddStringToObject(object,"LOAD AVG:",info);

    string=cJSON_Print(object);

    cJSON_Delete(object);

    return string;
}



void send_data(int newsockfd, char* string){

    char data[SIZE];

    strcpy(data, string);

    unsigned long crc = crc32(0L, Z_NULL, 0);

    int n = (int)send(newsockfd, data , SIZE, MSG_WAITALL);

    if(n < 0){

        perror("INET 6 SERVER Escritura en socket");

        exit(EXIT_FAILURE);

    }

    // return;

    crc = crc32(crc, (const Bytef *)data, (uInt)n);

    // printf("CRC32: %lx\n",crc);

	char checksum_str[SIZE];

	sprintf(checksum_str,"%lx",crc);

    usleep(500);

    send(newsockfd,checksum_str,SIZE,MSG_WAITALL);

    return; 
}


void server_inet6( uint16_t port ){

    int sockfd, newsockfd,  pid;

    // int puerto;

    unsigned int clilen;

	char buffer[SIZE];

    char str_addr[INET6_ADDRSTRLEN];

    // puerto=atoi(argv[1]);

    //char* address=argv[2];
    //char* interface=argv[3];
	struct sockaddr_in6 serv_addr, cli_addr;

	int n;

    int flag=1;

    int ret;

    long unsigned int backlog = get_max_connections();
    
	// if (argc<2){

    //     fprintf( stderr, "Uso: %s <puerto>\n", argv[0] );

	// 	exit(EXIT_FAILURE);
	// }

    sockfd=socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);

    if(sockfd==-1){

        perror("INET 6 SERVER Socket creation");

        exit(EXIT_FAILURE);

    }

    ret=setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag,sizeof(flag));

    if(ret==-1){

        perror("INET 6 SERVER Option Settings");

        exit(EXIT_FAILURE);

    }
    
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	
	serv_addr.sin6_family = AF_INET6;

    serv_addr.sin6_addr = in6addr_any;

    serv_addr.sin6_port = htons(port);
    
	if (bind( sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0){

		perror("INET 6 SERVER Bind");

		exit(EXIT_FAILURE);

	}

    printf( "\nINET 6 SERVER: Process %d - AVAILABLE SOCKET: %d\n", getpid(), ntohs(serv_addr.sin6_port) );

	listen(sockfd, (int)backlog);

	clilen = sizeof(cli_addr);

	while(1){

		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        inet_ntop(AF_INET6, &(cli_addr.sin6_addr),str_addr, sizeof(str_addr));

        char* buff;

        // char* getmemory="getmemory";

        // char* getload="getload";

		pid=fork(); 

		if(pid == 0){  // Proceso hijo

		    close(sockfd);

            char* json_memory;

            char* json_load;

		    while (1){

                bzero(buffer, SIZE);

                n= (int)recv(newsockfd, buffer, SIZE, 0);

                if(n < 0 ){

                    perror("INET 6 SERVER Lectura de socket");

                    exit(EXIT_FAILURE);

                }
                buff = tolowerstring(buffer);

                buff[strlen(buffer)-1] = '\0';
                
                if( !(strcmp(buff, "getmemory")) ){

                    json_memory = getfreememory(json_memory);
                    
                    send_data(newsockfd, json_memory);

                    continue;

                }else if(!strcmp( buff, "getload")){

                    json_load=get_load(json_load);

                    send_data(newsockfd, json_load);

                    continue;

                }
                else if(!strcmp( "end", buffer )){

                    printf( "INET 6 SERVER: Client finished connection. Process %d closing socket\n", getpid());

                    close(newsockfd);

                    exit(EXIT_SUCCESS);

                }
                else{

                    continue;
                }
		    }
		  }
		else{

		    printf( "INET 6 SERVER: New client, process %d is in charge of it.\n", pid );

		    close( newsockfd );
		}
	}

	// return EXIT_SUCCESS; 
} 