
#include "include/client_unix.h"
#include "include/functs.h"

bool command_validation(char* buf){

	if(strncmp(buf,"journalctl -n",13)==0){

		return true;

	}else{

		return false;
	}

}

void recv_result(int sockfd){

	char buffer[SIZEUNIX];

	bzero(buffer,SIZEUNIX);
	
	int n; 

	unsigned long crc = crc32(0L, Z_NULL, 0);

	n = (int)recv(sockfd, buffer, SIZEUNIX, 0);
	
	printf(GREEN);

	printf("%s\n",buffer);

	printf(WHITE);

	crc = crc32(crc, (const Bytef *)buffer, (uInt)n);

	// printf("CRC32: %lx\n",crc);


	char aux[SIZE];

	bzero(aux,SIZE);

	char checksum_str[SIZE];

	recv(sockfd,aux,SIZE,0);

	sprintf(checksum_str,"%lx",crc);

	if(strcmp(checksum_str, aux) == 0){

		printf("Checksum passed!\n");

		return;
	}
	else{

		printf("Checksum error\n");

		return;
	}

}

void add_json_flag(char* buffer){

	strcat(buffer," -o json-pretty");

}

int main( int argc, char *argv[] ) {

    int sockfd;

	long unsigned int servlen;

	struct sockaddr_un serv_addr;

	char buffer[SIZE];

    if (argc<2){

        fprintf(stderr,"Use %s socket-name\n",argv[0]);

        exit(EXIT_SUCCESS);

    }

	memset((char*)&serv_addr, '\0', sizeof(serv_addr));

	serv_addr.sun_family = AF_UNIX;

	strcpy(serv_addr.sun_path, argv[1]);

	servlen = strlen( serv_addr.sun_path) + sizeof(serv_addr.sun_family);

	if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 ){

		perror("Socket creation");

		exit(EXIT_FAILURE);

	}

    if(connect(sockfd, (struct sockaddr*)&serv_addr, (socklen_t)servlen) < 0 ){

		perror("Client connection");

		exit(EXIT_FAILURE);
	}

	while(1) {

		bzero(buffer,SIZE);

		fflush(stdin);

		printf("Enter a 'journalctl -n ' <number> command: \n");

		fgets(buffer, SIZE-1, stdin);

		if(strncmp(buffer,"end",3)==0){

			send(sockfd, buffer, strlen(buffer),MSG_WAITALL);

			printf("Goodbye!\n");

			close(sockfd);

			exit(EXIT_SUCCESS);
		}

		strtok(buffer,"\n");

		if(command_validation(buffer)){

			add_json_flag(buffer);

			send(sockfd, buffer, strlen(buffer),0);

			recv_result(sockfd);


		}else{

			printf("Invalid command\n");

			continue;
		}

		fflush(stdout);
		
	}
	return 0;
}