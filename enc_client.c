#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX_SOCKETS 5 //maximum number of concurrent sockets
#define MAX_BUF 256 //maximum buffer size 
#define HOSTNAME "localhost"


int main(int argc, char *argv[]){

	int csocket, port, chars_w, chars_r;
	struct sockaddr_in saddr;
	char buffer[MAX_BUF];
	char buf[MAX_BUF];
	struct hostent * host;
	FILE * plaintext;
	FILE * key;

	memset((char*)&saddr, '\0', sizeof(saddr));
	saddr.sin_family = AF_INET;
	port = atoi(argv[3]);

	saddr.sin_port = htons(port);
	host = gethostbyname(HOSTNAME);
	memcpy(&saddr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);

	csocket = socket(AF_INET, SOCK_STREAM, 0);
	connect(csocket, (struct sockaddr *)&saddr, sizeof(saddr));

	memset(buffer, '\0', sizeof(buffer));
	memset(buf, '\0', sizeof(buf));

	plaintext = fopen(argv[1], "r"); 
	fgets(buffer, sizeof(buffer)-1, plaintext);
	fclose(plaintext);

	key = fopen(argv[2], "r");
	fgets(buf, sizeof(buf)-1, key);
	fclose(key);

	buffer[strcspn(buffer, "\n")] = '\0';
	buf[strcspn(buf, "\n")] = '\0';
	
	strcat(buffer, ">"); //append delimeter 
	strcat(buffer, buf); //append key

	chars_w = send(csocket, buffer, strlen(buffer), 0); //send "plaintext>key" which can be tokenized using ">" as the delimeter character

	memset(buffer, '\0', sizeof(buffer));

	chars_r = recv(csocket, buffer, sizeof(buffer)-1, 0);
	printf("%s\n", buffer);

	close(csocket);	
	return 0;
}
