#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>

const char HOMEPAGE[] = "home.html";
const char IMAGE[] = "scr.jpg";
const int BUFF_SIZE = 1024;

void server(int port, double delay, int verbose)
{
	// Initialize socket
	int servSock = socket(AF_INET, SOCK_STREAM, 0);
	int option = 1;
	setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	if(servSock == -1)
	{
		perror("Socket could not be created.\n");
		exit(EXIT_FAILURE);
	}

	// Define server address
	struct sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);
	servAddr.sin_addr.s_addr = INADDR_ANY;

	// Bind socket to server address
	if(bind(servSock, &servAddr, sizeof(servAddr)) == -1)
	{
		perror("Could not bind socket.\n");
		exit(EXIT_FAILURE);
	}

	// Listen to given port
	if(listen(servSock, 1) == -1)
	{
		perror("Could not listen to given port.\n");
		return(EXIT_FAILURE);
	}
	if(verbose)
		printf("Listening to port %d...\n", port);

	// Initialize and accept a new incoming connection
	struct sockaddr_in cliAddr;
	int cliAddrLen = sizeof(cliAddr);
	int newSock = accept(servSock, &cliAddr, &cliAddrLen);
	if(newSock == -1)
	{
		perror("Could not accept incoming connection.\n");
		return(EXIT_FAILURE);
	}
	// Recieve request text
	char *rbuffer = (char *)malloc(BUFF_SIZE*sizeof(char));
	read(newSock, rbuffer, BUFF_SIZE - 1);
	rbuffer[BUFF_SIZE - 1] = '\0';

	// Send homepage if requested
	if( !strncmp("GET / HTTP/1.1", rbuffer, strlen("GET / HTTP/1.1")) )
	{
		if(verbose)
			printf("Sending home page...\n");
		
		// Initialize response
		char buffer[256];
		char body[1024] = "";
		FILE *hptr = fopen(HOMEPAGE, "r");
		size_t bytes;
		while( (bytes = fread(buffer, 1, sizeof(buffer), hptr)) > 0) 
		{
			// Append to body
			strncat(body, buffer, bytes);
		}
		// Set delay in web page
		sprintf(body, body, delay, delay);
		char header[128];
		sprintf(header, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n", strlen(body));

		// Send HTTP response header
		send(newSock, header, strlen(header), 0);
		
		// Send response body
		send(newSock, body, strlen(body), 0);
	}

	// Free and rellocate read buffer memory
	free(rbuffer);
	rbuffer = (char *)malloc(BUFF_SIZE*sizeof(char));

	// Send screenshot image if requested
	read(newSock, rbuffer, BUFF_SIZE - 1);
	rbuffer[BUFF_SIZE - 1] = '\0';

	if( !strncmp("GET /scr.jpg", rbuffer, strlen("GET /scr.jpg")) )
	{
		if(verbose)
			printf("Sending scr.jpg...\n");

		// Initialize response
		char header[] = "HTTP/1.1 200 OK\nContent-Type: image/jpeg\n\n";

		// Send HTTP response header
		send(newSock, header, strlen(header), 0);

		// Send response body
		// Read from image
		char buffer[4096];
		bzero(buffer, sizeof(buffer));
		FILE *iptr = fopen(IMAGE, "rb");
		size_t bytes;
		while( (bytes = fread(buffer, 1, sizeof(buffer), iptr)) > 0) 
		{
			// Send to client
			send(newSock, buffer, bytes, 0);
		}
	}

	// Free read buffer memory
	free(rbuffer);

	// Close all conections
	close(newSock);
	close(servSock);
}