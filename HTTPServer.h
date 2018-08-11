#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

const char LOCALHOST[] = "127.0.0.1";

void server(int port)
{
	// Initialize socket
	int servSock = socket(AF_INET, SOCK_STREAM, 0);
	if(servSock == -1)
	{
		perror("Socket could not be created.\n");
		exit(EXIT_FAILURE);
	}

	// Define server address
	struct sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);
	servAddr.sin_addr.s_addr = inet_addr(LOCALHOST);

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
	char buffer[1024];
	read(newSock, buffer, 1024);
	printf("Request from client: \n%s\n", buffer);

	// Send homepage if requested
	if( !strncmp("GET / HTTP/1.1", buffer, strlen("GET / HTTP/1.1")) )
	{
		printf("Sending home page...\n");
		
		// Initialize response
		char body[] = "<HTML>\n<HEAD>\n<TITLE>Home Page</TITLE>\n</HEAD>\n<BODY>\n<h1>HOME PAGE</h1>\n<img src=\"scr.jpg\"></img>\n</BODY>\n</HTML>\n";
		char header[128];
		sprintf(header, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n", strlen(body));

		// Send HTTP response header
		send(newSock, header, strlen(header), 0);
		
		// Send response body
		send(newSock, body, strlen(body), 0);
		printf("Send response:\n%s\n", body);
	}

	// Send screenshot image if requested
	read(newSock, buffer, 1024);
	printf("Request from client: \n%s\n", buffer);
	if( !strncmp("GET /scr.jpg", buffer, strlen("GET /scr.jpg")) )
	{
		printf("Sending scr.jpg...\n");

		// Initialize response
		char header[] = "HTTP/1.1 200 OK\nContent-Type: image/jpeg\n\n";

		// Send HTTP response header
		send(newSock, header, strlen(header), 0);

		// Send response body
		// Read from image
		char buffer[256];
		FILE *ptr;
		ptr = fopen("scr.jpg", "rb");
		size_t bytes;
		while( (bytes = fread(buffer, 1, sizeof(buffer), ptr)) > 0) 
		{
			// Send to client
			send(newSock, buffer, bytes, 0);
		}
	}

	shutdown(newSock, 0);
}