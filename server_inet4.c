
#include "include/server_inet4.h"
#include "include/functs.h"

char* data_to_json(char* log, char* clientname){

	char* info;

	time_t t;   

    time(&t);

	info = strcat(strtok(ctime(&t),"\n"), clientname);

	cJSON* object= cJSON_CreateObject();

	cJSON_AddStringToObject(object,info,log);

	char* string=cJSON_Print(object);

	return string;

}

void get_zip_name(char* output,int client_n){

	char aux[50];

	sprintf(aux,"%d",client_n);

	strcat(output,aux);

	char zip[5]=".zip";

	strncat(output,zip,5);
}

void get_txt_name(char* message,int client_n){

	char aux[50];

	sprintf(aux,"%d",client_n);

	strcat(message,aux);

	char txt[5]=".txt";

	strncat(message,txt,5);
}


void write_in_txt(char* message,char* buffer){

	FILE *fp;

	if ((fp = fopen(message, "a")) == NULL) {

		perror("INET 4 SERVER fopen");

		exit(EXIT_FAILURE);
	}
	// 2) Escribir el string recibido en el archivo
	if (fprintf(fp, "%s\n", buffer) < 0) {

		perror("INET 4 SERVER fprintf");

		exit(EXIT_FAILURE);
	}
	// 3) Cerrar el archivo
	if (fclose(fp) != 0){

		perror("INET 4 SERVER fclose");

		exit(EXIT_FAILURE);
	}
}

/*
*Esta funcion genera un ZIP. Escribe lo de message en output
*/

void write_in_zip(char* output,char* message, char*buffer){

	FILE* fp;

	gzFile zip_file;

	long unsigned int lent;

	//Creo el .ZIP
	if ((zip_file = gzopen(output, "wb")) == NULL) {

		perror("INET 4 SERVER gzopen");

		exit(EXIT_FAILURE);

		}
	// 5) Leer el archivo y escribirlo en el archivo comprimido
	if ((fp = fopen(message, "r")) == NULL) {

		perror("INET 4 SERVER fopen");

		exit(EXIT_FAILURE);

	}
	while ((lent = fread(buffer, 1, sizeof(buffer), fp)) > 0) {

		if (gzwrite(zip_file, buffer,(unsigned int)lent) == 0) {

			perror("INET 4 SERVER gzwrite");

			exit(EXIT_FAILURE);
		}
	}

	gzclose(zip_file);

	fclose(fp);
}


void send_zip(char* output,int newsockfd){

	FILE *fp;

	// printf("VOy a enviar el zip %s\n",output);
	
	if (!(fp = fopen(output, "rb"))){
		perror("INET 4 SERVER Failed opening file for transfer");
	}
        
	// int bytes_final = 0;
	
	unsigned long crc = crc32(0L, Z_NULL, 0);

	unsigned char chunk[CHUNK] = {0};

	while (1)
    {
		bzero(chunk,CHUNK);

        int bytes_read = (int)fread(chunk, 1, (size_t) CHUNK, fp);

        if (bytes_read > 0){

			printf("INET 4 SERVER: Process %d Sending %d bytes\n",getpid(),bytes_read);

			int n = (int)send(newsockfd, chunk, (size_t)bytes_read,	MSG_WAITALL);

			if(n < 0){
				perror("INET 4 SERVER Error sending file");
			}
		}
            
        if (bytes_read < CHUNK){

            if (feof(fp)){
				crc = crc32(crc, (const Bytef *)chunk, (uInt)bytes_read);
				// bytes_final = bytes_read;
				break;
			}   
            if (ferror(fp)){

				perror("INET 4 SERVER Failed transfering file");

				exit(EXIT_FAILURE);
			}    
        }
    }

	usleep(500);

	char checksum_str[CHECKSUM];

	sprintf(checksum_str,"%lx",crc);

	send(newsockfd,checksum_str, CHECKSUM, MSG_WAITALL);

	return;    
}





void server_inet4( int port ) {

	int sockfd, newsockfd, pid;

	// int puerto;

	unsigned int clilen;

	char buffer[SIZE];

	struct sockaddr_in serv_addr, cli_addr;

	int client_n=0;
	
	long unsigned int backlog = get_max_connections();

	// if (argc<2){

	// 	fprintf(stderr, "Uso: %s <puerto>\n", argv[0]);

	// 	exit(EXIT_FAILURE);
	// }

	sockfd=socket(AF_INET, SOCK_STREAM, 0);
	
	memset((char *) &serv_addr, 0, sizeof(serv_addr));

	// puerto=atoi(argv[1]);

	serv_addr.sin_family = AF_INET;

	serv_addr.sin_addr.s_addr = INADDR_ANY;

	serv_addr.sin_port=htons((uint16_t)port);


	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){

		perror("INET 4 SERVER Bind");

		exit(EXIT_FAILURE);
	}

	printf("\n INET 4 SERVER : Proceso %d - AVAILABLE SOCKET: %d\n", getpid(), ntohs(serv_addr.sin_port));

	listen(sockfd, (int)backlog);
	
	clilen=sizeof(cli_addr);

	while(1){
		newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr, &clilen);

		client_n++; 

		pid = fork(); 

		if (pid == 0){  // Proceso hijo

		    close(sockfd);

			char message[50]="messages_";
			
			char output[50]="output_";

			get_txt_name(message,client_n);

			get_zip_name(output,client_n);

			char log[SIZE];

			char clientname[SIZE];

			// char date[20];

			char* json_str;

			while (1)
			{
			
				//EMPIEZA TODO

				memset(buffer,'\0', SIZE);

				 //PRIMER RECEIVE

				if(recv(newsockfd, buffer, SIZE, 0) < 0){
					perror("INET 4 SERVER Lectura de socket");
					exit(EXIT_FAILURE);

				}
				//PRIMERO VERIFICO SI TENGO QUE TERMINAR

				if(!strncmp("end", buffer,3)){
					
					printf("INET 4 SERVER: Process %d. I received 'end'. Finishing connection and sending .ZIP\n\n", getpid());
					
					write_in_zip(output,message,buffer);

					send_zip(output,newsockfd);

					close(sockfd);

					remove(output);

					remove(message);

					fflush(stdout);

					exit(EXIT_SUCCESS);
					
				}

					strcpy(log, buffer);

					bzero(buffer, SIZE);

					if(recv(newsockfd, buffer, SIZE, 0) < 0){

					perror("INET 4 SERVER Lectura de socket");

					exit(EXIT_FAILURE);

					}

					strcpy(clientname, buffer);

					json_str = data_to_json(log, clientname);

					// printf("%s\n\n",json_str);

					openlog(clientname, LOG_PID, LOG_USER);

					syslog(LOG_INFO, "%s", log);
				
					write_in_txt(message,json_str);
	
		    }
		}
		else{

			printf("INET 4 SERVER: Nuevo cliente, que atiende el proceso hijo: %d\n", pid);

		  	close(newsockfd);
		}
	}
	// return 0; 
}