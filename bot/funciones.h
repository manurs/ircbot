#ifndef _SOCKET_H
#define _SOCKET_H
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void connect_client(pthread_t* h1, pthread_t* h2);
int abrirSocketTCP();
int abrirSocketUDP();
int abrirBind(int sockfd,const struct sockaddr_in *addr);
int aceptar(int sockfd, struct sockaddr_in ip4addr);
int abrirConnect(int sockfd, struct sockaddr ip4addr);
int abrirListen(int sockfd);
int recibir(int sockfd,char *buf);
int escribir(int sockfd,char *msg);
void * Ping(void *args);
void * servRecv(void *args);
int iscommand(char* s);


int sockfd;
#endif
