/*
* libevent echo server example.
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <sys/stat.h>

#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <event.h>
#include <log.h>

#pragma comment(lib,"ws2_32")
#pragma comment(lib,"mswsock")
#pragma comment(lib,"advapi32")

/* Port to listen on. */
#define SERVER_PORT 5555

/**
* A struct for client specific data, in this simple case the only
* client specific data is the read event.
*/
struct client {
	struct event ev_read;
	struct event ev_write;
	char buffer[ 8192 ];
	int total, wlen;
};

/**
* Set a socket to non-blocking mode.
*/
int
setnonblock(int fd)
{
	unsigned long nonblocking = 1;
	ioctlsocket( fd, FIONBIO, (unsigned long*) &nonblocking );
	return 0;
}

void
close_client(struct client *client, int fd)
{
	closesocket(fd);
	free(client);
}

/**
* This function will be called by libevent when the client socket is
* ready for reading.
*/
void
on_read(int fd, short ev, void *arg)
{
	struct client *client = (struct client *)arg;
	int len;
	
	len = recv(fd, client->buffer, sizeof( client->buffer ) - 1, 0);
	if (len == 0) {
		printf("Client(%d) disconnected.\n",fd);
		close_client(client, fd);
		return;
	}
	else if (len < 0 && WSAEWOULDBLOCK != WSAGetLastError() ) {
		printf("recv(%d) fail, len %d, errno %d\n", fd, len, WSAGetLastError() );

	}
	
	if( len > 0 ) {
		client->total = len;
		client->wlen = 0;
		if(0 != event_add( &( client->ev_write ), NULL ) ) {
			close_client( client, fd );
		}
	} else {
		if( 0!= event_add( &( client->ev_read ), NULL ) ) {
			close_client( client, fd );
		}
	}
}

/**
* This function will be called by libevent when the client socket is
* ready for writing.
*/
void
on_write(int fd, short ev, void *arg)
{
	struct client *client = (struct client *)arg;
	int len;
	
	len = send(fd, client->buffer + client->wlen, client->total - client->wlen, 0);
	if (len == 0) {
		printf("Client(%d) disconnected.\n",fd);
		close_client(client, fd);
		return;
	}
	else if (len < 0 && WSAEWOULDBLOCK != WSAGetLastError() ) {
		printf("send(%d) fail, len %d, errno %d\n", fd, len, WSAGetLastError() );
		close_client( client, fd );
		return;
	}

	if( len > 0 ) client->wlen += len;
	
	if( client->wlen >= client->total ) {
		client->buffer[ client->total ] = '\0';
		if( NULL != strstr( client->buffer, "quit" ) ) {
			//printf("Client(%d) quit.\n",fd);
			shutdown( fd, SD_BOTH );
			close_client( client, fd );
			return;
		}
		if( 0 != event_add( &( client->ev_read ), NULL ) ) {
			close_client( client, fd );
		}
	} else {
		if( 0 != event_add( &( client->ev_write ), NULL ) ) {
			close_client( client, fd );
		}
	}
}

/**
* This function will be called by libevent when there is a connection
* ready to be accepted.
*/
void
on_accept(int fd, short ev, void *arg)
{
	int client_fd;
	struct sockaddr_in client_addr;
	int client_len = sizeof(client_addr);
	struct client *client;
	static maxfd = 30000;
	
	client_fd = accept(fd, (struct sockaddr *)&client_addr, &client_len);
	if (client_fd == -1) {
		event_warn("accept failed, errno %d", WSAGetLastError());
		return;
	}

	if( client_fd > maxfd + 1000 ) {
		maxfd = client_fd;
		printf( "maxfd %d\n", maxfd );
	}
	
	setsockopt( client_fd, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *)&fd, sizeof(fd) );
	
	/* Set the client socket to non-blocking mode. */
	if (setnonblock(client_fd) < 0)
		event_warn("failed to set client socket non-blocking");
	
	/* We¡¯ve accepted a new client, allocate a client object to
	* maintain the state of this client. */
	client = (struct client*)calloc(1, sizeof(*client));
	if (client == NULL)
		event_err(1, "malloc failed");
	
	event_set(&client->ev_read, client_fd, EV_READ, on_read, client );
	event_set(&client->ev_write, client_fd, EV_WRITE, on_write, client );
	
	/* Setting up the event does not activate, add the event so it
	* becomes active. */
	if( 0 != event_add(&client->ev_read, NULL) ) {
		close_client( client, client_fd );
	}
	
	//printf("Accepted connection(%d) from %s\n", client_fd, inet_ntoa(client_addr.sin_addr));
}

int
main(int argc, char **argv)
{
	int listen_fd, nZero = 0;
	struct sockaddr_in listen_addr;
	int reuseaddr_on = 1;
	
	/* The socket accept event. */
	struct event ev_accept;
	struct event_base * evbase = NULL;
	
	WSADATA wsaData;
	
	int err = WSAStartup( MAKEWORD( 2, 0 ), &wsaData );
	if ( err != 0 ) {
		printf( "Couldn't find a useable winsock.dll.\n" );
		return -1;
	}
	
	/* Initialize libevent. */
	evbase = event_init();
	printf( "method : %s\n", event_base_get_method (evbase) );
	
	/* Create our listening socket. This is largely boiler plate
	* code that I¡¯ll abstract away in the future. */
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd < 0)
		event_err(1, "listen failed");
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseaddr_on,
		sizeof(reuseaddr_on)) == -1)
		event_err(1, "setsockopt failed");
	memset(&listen_addr, 0, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = INADDR_ANY;
	listen_addr.sin_port = htons(SERVER_PORT);
	if (bind(listen_fd, (struct sockaddr *)&listen_addr,
		sizeof(listen_addr)) < 0)
		event_err(1, "bind failed");
	if (listen(listen_fd, 1024) < 0)
		event_err(1, "listen failed");
	
        /* Set the socket to non-blocking, this is essential in event
	* based programming with libevent. */
	if (setnonblock(listen_fd) < 0)
		event_err(1, "failed to set server socket to non-blocking");
	
    // Disable send buffering on the socket.  Setting SO_SNDBUF
    // to 0 causes winsock to stop bufferring sends and perform
    // sends directly from our buffers, thereby reducing CPU usage.
    nZero = 0;
    if (SOCKET_ERROR == setsockopt(listen_fd, SOL_SOCKET,
			SO_SNDBUF, (char *)&nZero, sizeof(nZero))) {
		printf("setsockopt(SNDBUF): %d\n", WSAGetLastError());
		return -1;
	}

    // Disable receive buffering on the socket.  Setting SO_RCVBUF 
    // to 0 causes winsock to stop bufferring receive and perform
    // receives directly from our buffers, thereby reducing CPU usage.
    nZero = 0;
    if (SOCKET_ERROR == setsockopt(listen_fd, SOL_SOCKET,
			SO_RCVBUF, (char *)&nZero, sizeof(nZero))) {
		printf("setsockopt(SO_RCVBUF): %d\n", WSAGetLastError());
		return(FALSE);
	}

	/* We now have a listening socket, we create a read event to
	* be notified when a client connects. */
	event_set(&ev_accept, listen_fd, EV_ACCEPT|EV_READ|EV_PERSIST, on_accept, NULL);
	event_add(&ev_accept, NULL);
	
	/* Start the libevent event loop. */
	event_dispatch();
	
	return 0;
}
