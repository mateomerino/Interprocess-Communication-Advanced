
#include "include/client_inet6.h"

#include "include/functs.h"

void send_command(int sockfd, char* command){

	char data[SIZE];

	strcpy(data, command);

	int n = (int)send(sockfd, data , SIZE, MSG_WAITALL);

	if(n < 0){

		perror("Socket writing");

		exit(EXIT_FAILURE);

	}
}

void recv_data(int sockfd){

	char data[SIZE];

	int n = (int)recv(sockfd, data, SIZE, 0);

	unsigned long crc = crc32(0L, Z_NULL, 0);

	if(n < 0){

		perror("Socket reading");

		exit(EXIT_FAILURE);

	}

	printf(GREEN);	

	printf("\n%s\n",data);

	printf(WHITE);

	crc = crc32(crc, (const Bytef *)data, (uInt)n);

	char aux[SIZE];

	bzero(aux,SIZE);

	char checksum_str[SIZE];

	recv(sockfd,aux,SIZE,0);

	sprintf(checksum_str,"%lx",crc);

	if(strcmp(checksum_str, aux) == 0){

		printf("\nChecksum passed!\n\n");

		return;
	}
	else{

		printf("Checksum error\n");

		return;
	}

}

int main(int argc, char *argv[]){

	if (argc<2){

        fprintf( stderr, "Uso: %s <puerto>\n", argv[0] );

		exit(EXIT_FAILURE);
	}

	int sockfd, n;

	uint16_t puerto;

	struct sockaddr_in6 serv_addr;

    puerto = (uint16_t)atoi(argv[1]);

	char buffer[SIZE];

    sockfd=socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);

    if(sockfd==-1){

        perror("Socket Creation");

    }

	memset((char*) &serv_addr, '0', sizeof(serv_addr));

    //bcopy((char *)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);

	serv_addr.sin6_family = AF_INET6;

	serv_addr.sin6_port = htons(puerto);

	inet_pton(AF_INET6,"::1",&serv_addr.sin6_addr);

	//serv_addr.sin6_scope_id = if_nametoindex(interface);
    

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0){

		perror("CLIENT Connection");

		exit(EXIT_FAILURE);

	}
	while(1){

		printf("Enter <getmemory> or <getload> to obtain system information\n");

		bzero(buffer,SIZE);

		fgets(buffer, SIZE, stdin);


		if(strncmp("end", buffer, 3) == 0){

			printf("Client %d finishing connection\n",getpid());

			n=(int)write(sockfd, buffer, strlen(buffer));

			if(n < 0){

				perror("Error sending command");

			}

			close(sockfd);

			exit(EXIT_SUCCESS);

		}else if(strncmp("getmemory", buffer, 9) == 0){

			send_command(sockfd, buffer);

			recv_data(sockfd);

			continue;

		}else if(strncmp("getload", buffer, 7) == 0){

			send_command(sockfd, buffer);

			recv_data(sockfd);

			continue;

		}else{

			printf("Invalid command. Please try again\n\n");

			continue;

		}

	}
	return 0;
}