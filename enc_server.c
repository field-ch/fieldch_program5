#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX_SOCKETS 5 //maximum number of concurrent sockets
#define MAX_BUF 256 //maximum buffer size 
#define HOSTNAME "localhost"
#define DELIM ">" //delimeter for strtok

int num_sockets = 0; //socket counter gv

void plain2cipher(char **);

void plain2cipher(char **msg){

	char * token;
	char plaintext[MAX_BUF];
	char key[MAX_BUF];
	char ciphertext[MAX_BUF];
	char buffer[MAX_BUF];
	int mkmod[MAX_BUF];

	memset(plaintext, '\0', sizeof(plaintext));
	memset(key, '\0', sizeof(key));
	memset(ciphertext, '\0', sizeof(ciphertext));
	memset(buffer, '\0', sizeof(buffer));
	memset(mkmod, 0, sizeof(mkmod));

	token = strtok(*msg, DELIM);
	strcpy(plaintext, token);

	token = strtok(NULL, DELIM);
	strcpy(key, token);

	int i = 0;
	for(; i < strlen(plaintext); i++){
		
		if(plaintext[i] == ' '){
			plaintext[i] = 91; //replace space character with ASCII value right after Z
		}
		if(key[i] == ' '){
			key[i] = 91; //same as above. This is to keep the modular arithmetic simple
		}
		int val = plaintext[i]-64 + key[i]-64; //65 is the ASCII value of 'A' (we want the numeric value range to be from 0-27) so this normalizes 'A' to be 0 
		mkmod[i] = val%27 + 64; //need to add 65 back in order for the right ASCII character to be stored into the buffer
		ciphertext[i] = mkmod[i]; 
	}	
	memset(*msg, '\0', sizeof(msg)); //clear the original message
	strcpy(*msg, ciphertext); //copy the ciphertext to the original message
}

int main(int argc, char *argv[]){

	int connection, port, numChars, wstatus;
	char plaintext[MAX_BUF], key[MAX_BUF];
	struct hostent * host;
	struct sockaddr_in saddr, caddr;
	socklen_t csize = sizeof(caddr);

	char * buffer = (char*)calloc(MAX_BUF, sizeof(char));
	memset(buffer, '\0', sizeof(buffer));

	pid_t spawnpid;
	memset((char*) &saddr, '\0', sizeof(saddr)); //clear the socket address
	
	saddr.sin_family = AF_INET; //using IPv4 protocol
	port = atoi(argv[1]); //set the listening port

	saddr.sin_port = htons(port);

	host = gethostbyname(HOSTNAME);

	memcpy(&saddr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);

	int Listen = socket(AF_INET, SOCK_STREAM, 0);
	
	bind(Listen, (struct sockaddr *)&saddr, sizeof(saddr));
	
	listen(Listen, 5);

	while(1){

		//TODO: listen on the specified port.
		//Once a connection is accepted,
		//fork a new child process(up to MAX_SOCKETS)
		//which receives plaintext and key from enc_client
		//and sends back ciphertext to enc_client
		
		connection = accept(Listen, (struct sockaddr *)&caddr, &csize);
		num_sockets++;
		spawnpid = fork();
		switch(spawnpid){
			case -1: //error
				exit(1);
			case 0: //child process
				memset(buffer, '\0', MAX_BUF);
				memset(plaintext, '\0', MAX_BUF);
				memset(key, '\0', MAX_BUF);

				numChars = recv(connection, buffer, MAX_BUF-1, 0); // receive "plaintext>key" from client
				printf("SERVER: received message from client = %s\n", buffer);
	
				//TODO: call a function that tokenizes the "plaintext>key" string, handles the
				//encryption to ciphertext and stores the ciphertext back into the original buffer
				plain2cipher(&buffer);
					
				numChars = send(connection, buffer, strlen(buffer), 0); //send ciphertext back to enc_client
				break;
			default: //parent process(enc_server)
				while(num_sockets >= 5){ // if 5 concurrent connections are active
					
					spawnpid = wait(&wstatus); // wait for a child process (client connection) to end
					num_sockets--; //decrement active concurrent socket count
				}
				break;
		}
		close(connection);	
	}
	close(Listen);
	free(buffer);
	return 0;
}
