#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
#include<stdio.h>
#include<netdb.h> //hostent
#include<netinet/in.h>
#include "parser.h"

int errors = 0;
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor

    int debug = 1;
    int sock = *(int*)socket_desc;
    int read_size;
	int new_socket, c;
    int i, j;
    char getRequest[40000], getHost[40000], url[100], client_message[40000], ip[100];
	char server_reply[50000];
	//For IP retrival
	int status;
	struct addrinfo hints, *res, *p;
	char ipstr[INET6_ADDRSTRLEN];
	//For Client Socket
	int client_socket_desc;
	struct sockaddr_in connect2server;

    //Receive a message from client
    while( (read_size = recv(sock , client_message , 40000 , 0)) > 0 )
    {
        //Send the message back to client
        if (debug != 0) {
            printf("Browser says:\n%s\n", client_message);
        }

		//get and format the first line from the browser request, the GET request
        for(i = 0; client_message[i] != '\n'; i++) {
            getRequest[i] = client_message[i];
        }

        char *endOfLine = "\n\0";
        strcat(getRequest,endOfLine);

		if (debug != 0) {
			printf("The get request is: %s", getRequest);
		}

		//Process GET Request
		char buffer[100];
		strcpy(buffer, getRequest);
		strtok(buffer, " ");
		getURL(getRequest, url);

		if (debug != 0) {
				printf("Destination URL: %s\n", url);
		}

		if ( (strcmp(buffer, "GET") == 0) && (strcmp(url, "pages.cpsc.ucalgary.ca") == 0) ) {
			puts("Get request detected");

			int getaddrinfo(const char *node,     // e.g. "www.example.com" or IP
				const char *service,  // e.g. "http" or port number
				const struct addrinfo *hints,
				struct addrinfo **res);

			memset(&hints, 0, sizeof hints); // make sure the struct is empty
			hints.ai_family = AF_INET;		 // ipv4
			hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

			// get ready to connect
			if ((status = getaddrinfo(url, NULL, &hints, &res)) != 0) {
				fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
				exit(1);
			}

			printf("IP addresses for %s:\n", url);

			for (p = res; p != NULL; p = p->ai_next) {
				void *addr;
				char *ipver;

				// get the pointer to the address itself,
				// different fields in IPv4 and IPv6:
				if (p->ai_family == AF_INET) { // IPv4
					struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
					addr = &(ipv4->sin_addr);
					ipver = "IPv4";
				}
				// convert the IP to a string and print it:
				inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
				printf("%s: %s\n", ipver, ipstr);
			}

			client_socket_desc = socket(AF_INET, SOCK_STREAM, 0);
			if (client_socket_desc == -1)
			{
				printf("Could not create socket");
			}

			connect2server.sin_addr.s_addr = inet_addr(ipstr);
			connect2server.sin_family = AF_INET;
			connect2server.sin_port = htons(80);

			if (connect(client_socket_desc, (struct sockaddr *)&connect2server, sizeof(connect2server)) < 0)
			{
				puts("connect error");
				return 1;
			}

			puts("Connected");

			//puts(client_message);
			if (debug != 0) {
            	printf("Browser says2:\n%s\n", client_message);
        	}
			char modMessage[40000];
			strcpy(modMessage, client_message);
			modifyGET(modMessage);
			if (debug != 0) {
            	printf("We say2:\n%s\n", modMessage);
        	}
			if (send(client_socket_desc, modMessage, strlen(modMessage), 0) < 0)
			{
				puts("Send failed");
				return 1;
			}
			printf("Request Sent to %s\n", ipstr);

			if (recv(client_socket_desc, server_reply, 40000, 0) < 0)
			{
				puts("recv failed");
			}
			puts("Reply received\n");

			if (debug != 0) {
				puts(server_reply);
			}

			puts("Connection accepted");

			char serverReplyContentType[3000];
			strcpy(serverReplyContentType, server_reply);
			getContentType(serverReplyContentType);
			if (debug != 0) {
				printf("The content type is: %s\n", serverReplyContentType);
			}
			if((strcmp(serverReplyContentType, "Content-Type: text/html") == 0)) {
				char buffer3[30000];
				strcpy(buffer3, server_reply);
				int txtLenght = getContentLenght(buffer3);
				
				if (debug != 0) {
					printf("text/html TROLLING UNDERWAY!\ntxtLenght is: %d", txtLenght);
					printf("text/html TROLLING UNDERWAY!\n");
					puts(server_reply);
				}
				findBody(server_reply, errors, txtLenght);
				write(sock, server_reply, strlen(server_reply));
			}
			if((strcmp(serverReplyContentType, "Content-Type: text/plain") == 0)) {
				char buffer3[30000];
				strcpy(buffer3, server_reply);
				int txtLenght = getContentLenght(buffer3);
				
				if (debug != 0) {
					printf("text/html TROLLING UNDERWAY!\ntxtLenght is: %d", txtLenght);
					puts(server_reply);
				}
				findTxt(server_reply, errors, txtLenght);
				puts(server_reply);

				write(sock, server_reply, strlen(server_reply));
			}
			else {
				write(sock, server_reply, sizeof(server_reply));
			}

			//puts(server_reply);
			//free(client_socket_desc);

			//freeaddrinfo(res); // free the linked-list

		}
		else {
			int getaddrinfo(const char *node,     // e.g. "www.example.com" or IP
				const char *service,  // e.g. "http" or port number
				const struct addrinfo *hints,
				struct addrinfo **res);

			memset(&hints, 0, sizeof hints); // make sure the struct is empty
			hints.ai_family = AF_INET;		 // ipv4
			hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

			// get ready to connect
			if ((status = getaddrinfo(url, NULL, &hints, &res)) != 0) {
				fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
				exit(1);
			}

			printf("IP addresses for %s:\n", url);

			for (p = res; p != NULL; p = p->ai_next) {
				void *addr;
				char *ipver;

				// get the pointer to the address itself,
				// different fields in IPv4 and IPv6:
				if (p->ai_family == AF_INET) { // IPv4
					struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
					addr = &(ipv4->sin_addr);
					ipver = "IPv4";
				}
				// convert the IP to a string and print it:
				inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
				printf("%s: %s\n", ipver, ipstr);
			}

			client_socket_desc = socket(AF_INET, SOCK_STREAM, 0);
			if (client_socket_desc == -1)
			{
				printf("Could not create socket");
			}

			connect2server.sin_addr.s_addr = inet_addr(ipstr);
			connect2server.sin_family = AF_INET;
			connect2server.sin_port = htons(80);

			if (connect(client_socket_desc, (struct sockaddr *)&connect2server, sizeof(connect2server)) < 0)
			{
				puts("connect error");
				return 1;
			}

			puts("Connected");

			//puts(client_message);
			if (debug != 0) {
            	printf("Browser says2:\n%s\n", client_message);
        	}
			char modMessage[40000];
			strcpy(modMessage, client_message);
			modifyGET(modMessage);
			if (debug != 0) {
            	printf("We say2:\n%s\n", modMessage);
        	}
			if (send(client_socket_desc, modMessage, strlen(modMessage), 0) < 0)
			{
				puts("Send failed");
				return 1;
			}
			printf("Request Sent to %s\n", ipstr);

			if (recv(client_socket_desc, server_reply, 40000, 0) < 0)
			{
				puts("recv failed");
			}
			puts("Reply received\n");

			if (debug != 0) {
				puts(server_reply);
			}

			puts("Connection accepted");

			write(sock, server_reply, strlen(server_reply));
		}
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    //Free the socket pointer
    free(socket_desc);

    return 0;
}

int main(int argc , char *argv[])
{
	errors = atoi(argv[1]);
    int debug = 0;
    int socket_desc , new_socket , c , *new_sock;
    struct sockaddr_in server , client;
    char *message;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( atoi(argv[2]) );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("bind failed");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");

        //Reply to the client
        message = "Connection recieved. Assigning handler\n";
        //write(new_socket , message , strlen(message));

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = new_socket;

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }

    if (new_socket<0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}