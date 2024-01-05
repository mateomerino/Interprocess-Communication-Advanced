
#include "include/client_inet4.h"
#include "include/functs.h"

void get_client_name(char* clientname){

	sprintf(clientname, " Client_PID_%d", getpid());

}



void recv_zip(int sockfd){
	
	char client_zip[30];

	sprintf(client_zip, "Client_PID_%d.zip", getpid());

	unsigned long crc = crc32(0L, Z_NULL, 0);

	FILE *fp;
    
    fp = fopen(client_zip, "wb");
    if (fp == NULL) {
        perror("Failed to open file for writing");
        exit(EXIT_FAILURE);
    }
	
	
	int bytes_received = 0;

    char recv_buff[CHUNK];

    while ((bytes_received = (int)recv(sockfd, recv_buff, CHUNK,0)) > 0)
    {
		printf("Bytes received: %d\n",bytes_received);

        fwrite(recv_buff, 1, (size_t)bytes_received, fp);

        if (bytes_received < CHUNK){

			crc = crc32(crc, (const Bytef *)recv_buff, (uInt)bytes_received);

			break;
		}
            
    }

    if (bytes_received == -1){

		perror("Error in bytes received");

		exit(EXIT_FAILURE);
	}

	// printf("Bytes final %d\n",bytes_received);

	// printf("CRC32: %lx\n",crc);

	char aux[CHECKSUM];

	char checksum_str[CHECKSUM];

	recv(sockfd,aux,CHECKSUM,0);

	// printf("CRC32 Received: %s\n",aux);

	sprintf(checksum_str,"%lx",crc);

	if(strcmp(checksum_str,aux)==0){

		printf("Checksum passed! File received successfully: %s\n",client_zip);

	}else{

		perror("Checksum error:");
	}

	return;
}

int main( int argc, char *argv[] ) {

	int sockfd;

	uint16_t puerto;
	
	struct sockaddr_in serv_addr;

	struct hostent *server;


	char buffer[SIZE];

	// char* buffer_json;

	char client_name[SIZE];

	if (argc<3) {

		fprintf( stderr, "Uso %s host puerto\n", argv[0]);

		exit(EXIT_SUCCESS);
	}

	puerto = (uint16_t)atoi(argv[2]);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0){

        perror("Socket creation:");

    }

	server = gethostbyname(argv[1]);

	if(server==NULL){

        perror("Host Name");

    }
    
	memset((char*) &serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;

	bcopy((char *)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, (size_t)server->h_length);

	serv_addr.sin_port = htons(puerto);
	
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){

		perror("Connection");

		exit(EXIT_FAILURE);
	}
	
	while(1){

		fflush(stdin);

		bzero(buffer,SIZE);
		
		printf("Enter the message you want to LOG in Journalctl LOGS:\n ");

		fgets(buffer, SIZE, stdin);

		strtok(buffer,"\n");

		if(!strncmp("end", buffer,3)){

			if(send(sockfd, buffer, strlen(buffer),0) < 0){	
			
				perror("Escritura en socket");
			}	

			memset(buffer, '\0', SIZE);

			recv_zip(sockfd);

			fflush(stdout);

			close(sockfd);

			exit(EXIT_SUCCESS);

		}else{

			
			if( send(sockfd, buffer, strlen(buffer), MSG_WAITALL) < 1){ 
			
				perror("Client send");
			
				exit(EXIT_FAILURE);
			}

			usleep(500);

			get_client_name(client_name);

			if( send(sockfd, client_name, strlen(client_name), MSG_WAITALL) < 1){ 
			
				perror("Client send");
			
				exit(EXIT_FAILURE);
			}

		}
				
	}
	return 0;
} 
	        