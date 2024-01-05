
#include "include/server_unix.h"
#include "include/functs.h"


void process_command(int newsockfd, char* command){
    
    FILE* fp;

    if ((fp = popen(command, "r")) == NULL) {

        perror("UNIX SERVER popen");

        exit(EXIT_FAILURE);

    }
     
    unsigned long crc = crc32(0L, Z_NULL, 0);

    unsigned char buf[SIZEUNIX] = {0};

    while(1){

        bzero(buf,SIZEUNIX);

        int len = (int)fread(buf, 1, (size_t)SIZEUNIX, fp);

        if(len <= SIZEUNIX){

            int n = (int)send(newsockfd, buf, (size_t) len, MSG_WAITALL);

            if( n < 0 ){
                perror("UNIX SERVER Error sending command result");
            }

            if(feof(fp)){

                crc = crc32(crc, (const Bytef *)buf, (uInt)len);

                break;

            }
        }
    }

    // printf("CRC32: %lx\n",crc);

	char checksum_str[SIZE];

	sprintf(checksum_str,"%lx",crc);

    usleep(500);

    send(newsockfd,checksum_str,SIZE,MSG_WAITALL);

    return;  
}

char* get_command(char* string){

    char* command;

    command = strchr(string,':');

    command = strchr(command,'"');

    strtok(command,"}");

    return command;
}


void server_unix(char* argv){

    int sockfd, newsockfd, n, pid;

    long unsigned int servlen;

    struct sockaddr_un  cli_addr, serv_addr;

    socklen_t clilen;

    char buffer[SIZE];

    long unsigned int backlog = get_max_connections();
    
  
    /* Se toma el nombre del socket de la línea de comandos */
    // if(argc != 2){

    //     printf( "Use: %s <socket_name>\n", argv[0] );

    //     exit(EXIT_FAILURE);

    // }

    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0))<0){

        perror("UNIX SERVER: Socket creation");

        exit(EXIT_FAILURE);

    }
  
    /* Remover el nombre de archivo si existe */
    // unlink(port);
    
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sun_family = AF_UNIX;

    strcpy(serv_addr.sun_path, argv);

    servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
    
    if( bind( sockfd,(struct sockaddr*)&serv_addr, (socklen_t)servlen ) < 0 ){

        perror("UNIX SERVER Bind"); 

        exit(EXIT_FAILURE);

    }
    printf("\n UNIX SERVER: Process %d - AVAILABLE SOCKET: %s\n", getpid(), serv_addr.sun_path);

    if(listen(sockfd, (int)backlog) < 0){

        perror("Listen");

    }

    clilen=sizeof(cli_addr);
  
    while(1){

        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if(newsockfd < 0 ){

            perror("UNIX SERVER Accept");
        }

        pid = fork();

        if(pid == 0){        

            if(close(sockfd) < 0 ){

                perror("UNIX SERVER Close");
            }

            while(1){ 

                fflush(stdin);

                bzero(buffer,SIZE);
                
                n = (int)read(newsockfd, buffer, SIZE);

                if (n < 0 ){
                    perror("UNIX SERVER: Error in server read");
                }

                if(strncmp(buffer, "end", 3) == 0){

			        printf("UNIX SERVER: The client has closed connection. Process %d saying good bye!\n",getpid());

                    close(sockfd);

			        exit(EXIT_SUCCESS);
		        }
                else{

                    process_command(newsockfd, buffer);

                    fflush(stdout);
                }
                   
            }
	    }
        else{

	        printf("UNIX SERVER: New client, process %d is in charge of serving it\n", pid);

	        close(newsockfd);
        }
    }

  exit(EXIT_SUCCESS);

}