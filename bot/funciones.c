#include "funciones.h"
#include <pthread.h>
/*anadido return -1*/

char * posiciona(char crt, char *str){
	unsigned int i=0;
	if(str==NULL)
		return NULL;
	for(i=0;i<strlen(str);i++){
		if(str[i]==crt)
			break;
	}
	return &str[i+1];
}
int check_usr(char * usr){
	FILE *pFile = NULL;
	size_t size = 0;
	pFile = fopen ("usr.txt","r");
	char *buff = NULL;
	while(getline(&buff, &size, pFile) != -1){
		char*p = NULL;
		if((p = strchr(buff, '\n')) != NULL) *p = '\0';
		//wprintw(output_win, "buffer leido:[%s]usr:[%s]\n", buff, usr);
		//wrefresh(output_win);
		fflush(stdout);
		if(strcmp(usr, buff) == 0){
			free(buff);
			fclose(pFile);
			return 1;
		}
		free(buff);
		buff = NULL;
	}
	free(buff);
	fclose(pFile);
	return 0;
	
}
void * servRecv(void *args){
	char buf[512], aux2[512], ch[32], aux3[512];
	char *command = NULL, *usr = NULL, *trash = NULL;
	int aux;
	char* ultra_trash =  NULL;
	char maximum_trash[1024];
	char* p = NULL;
	while(1){
		command=NULL;
		usr=NULL;
		//ch=NULL;
		trash=NULL;
		if(0<(aux=recibir(sockfd, buf))){
			strcpy(aux3, buf);
			while((p = strchr(aux3, '\r')) != NULL) *p = '/';
			if (strstr(aux3, " PONG") == NULL) wprintw(output_win, "Recibido: %s", aux3);
			if (strstr(aux3, "raspberry@") != NULL) excptloro = 1;
			wrefresh(output_win);
			fprintf(plogf, "Recibido: %s", aux3); //hacer con hijos, enviar esto por pipe, leer en orden en padre
			usr = strtok (buf,"!");

			if(usr==NULL)
				continue;
			//strcpy(usr,&usr[1]);
			//wprintw(output_win, "usr = %s\n\n",usr);
			//wrefresh(output_win);
			trash = strtok (NULL," ");
			if(trash==NULL)
				continue;
			//wprintw(output_win, " %s",trash);
			command = strtok (NULL," ");
			if(command==NULL)
				continue;
			trash = strtok (NULL," ");
			//wprintw(output_win, "trash=%s\n",trash);
			//wrefresh(output_win);
			strcpy(ch, trash);
			//wprintw(output_win, "command = %s\n\n",command);
			//wrefresh(output_win);
			if(strcmp(command,"JOIN")==0){
				//wprintw(output_win, "%s\n",usr);
				//wrefresh(output_win);
				sprintf(aux2,"MODE #cosas +o %s%c%c",usr,0x0d,0x0a);
				wprintw(output_win, "Enviado: MODE #cosas +o %s\n",usr);
				wrefresh(output_win);
				escribir(sockfd, aux2);
				fprintf(plogf, "Enviado: MODE #cosas +o %s\n", usr);
				
			} else if(strcmp(command,"PRIVMSG")==0){

				trash = strtok (NULL,"");
				//wprintw(output_win, "trash=%s\n",trash);
				//wrefresh(output_win);
				ultra_trash = malloc(sizeof(char)*strlen(trash));
				
				//wprintw(output_win, "USR=[%s]", usr);
				//wrefresh(output_win);
				strcpy(ultra_trash, &trash[1]);
				//wprintw(output_win, "%lu\n",sizeof(trash));
				//wrefresh(output_win);
				//wprintw(output_win, "ultra_trash = %s\n", ultra_trash);
				//wrefresh(output_win);

				//maximum_trash = malloc(strlen(ultra_trash) + strlen(usr) + strlen("PRIVMSG"));
				maximum_trash[0] = '\0';
				//char dest[] = "dr_nick";

				//wprintw(output_win, "EXCPTLORO=%i\n", excptloro);
				wrefresh(output_win);
				//wprintw(output_win, "usr==%s\n", usr);
				//sprintf(maximum_trash, "PRIVMSG %s :<%s@%s> %s", dest, &usr[1], ch, ultra_trash);
				wrefresh(output_win);
				if(iscommand(ultra_trash) == 0 && loro == 1 && excptloro == 0){
					sprintf(maximum_trash, "PRIVMSG %s :%s", ch, ultra_trash);
					//wprintw(output_win, "maximum_trash:%s\n", maximum_trash);
					//wrefresh(output_win);
					sleep(1);
					escribir(sockfd, maximum_trash);
					aux3[0] = '\0';
					strcpy(aux3, maximum_trash);
					while((p = strchr(aux3, '\r')) != NULL) *p = '/';
					wprintw(output_win, "Enviado: %s", aux3);
					wrefresh(output_win);
					fprintf(plogf, "Enviado: %s", aux3);

				}else if(check_usr(&usr[1]) != 0){
					if(strncmp(ultra_trash,"SEND",strlen("SEND"))==0){
						sendv=1;
						wprintw(output_win, "SEND\n");
						wrefresh(output_win);
					}else if(strncmp(ultra_trash,"NSEND",strlen("NSEND"))==0){
						sendv=0;
						wprintw(output_win, "NSEND\n");
						wrefresh(output_win);
					} else if(strncmp(ultra_trash,"LORO",strlen("LORO"))==0){
						loro=1;
						wprintw(output_win, "LORO\n");
						wrefresh(output_win);
					} else if(strncmp(ultra_trash,"NLORO",strlen("NLORO"))==0){
						loro=0;
						wprintw(output_win, "NLORO\n");
						wrefresh(output_win);
					} else if (sendv ==1 && iscommand(ultra_trash) == 1){
						escribir(sockfd, ultra_trash);
						aux3[0] = '\0';
						strcpy(aux3, ultra_trash);
						while((p = strchr(aux3, '\r')) != NULL) *p = '/';
						wprintw(output_win, "Enviado: %s", aux3);
						wrefresh(output_win);
						fprintf(plogf, "Enviado: %s", aux3);
					}

					//wprintw(output_win, "send= %d\n",send);
					//wrefresh(output_win);
				}
				excptloro = 0;
				free(ultra_trash);
				ultra_trash = NULL;
				//free(ch);
	    	} /*else{
	    		if(strstr(usr, "raspberry") == NULL){
					maximum_trash[0] = '\0';
					char dest[] = "dr_nick";
					sprintf(maximum_trash, "PRIVMSG %s :<%s@%s> %s", dest, &usr[1], ch, command);
					char* p;
					while((p = strchr(maximum_trash, '\n')) != NULL) *p = '/';
					wprintw(output_win, "maximum_trash:%s\n", maximum_trash);
					fflush(stdout);
					escribir(sockfd, maximum_trash);
				}
			}*/
		}
	}
	wprintw(output_win, "Terminada la conexion\n");
	wrefresh(output_win);
}

int iscommand(char* s){
	if(strncmp(s,"PRIVMSG",strlen("PRIVMSG"))==0){
		return 1;
	} else if(strncmp(s,"JOIN",strlen("JOIN"))==0){
		return 1;
	} else if(strncmp(s,"NICK",strlen("NICK"))==0){
		return 1;
	} else if(strncmp(s,"SEND",strlen("SEND"))==0){
		return 1;
	} else if(strncmp(s,"MODE",strlen("MODE"))==0){
		return 1;
	} else if(strncmp(s,"NOTICE",strlen("NOTICE"))==0){
		return 1;
	} else if(strncmp(s,"QUIT",strlen("QUIT"))==0){
		return 1;
	} else if(strncmp(s,"INVITE",strlen("INVITE"))==0){
		return 1;
	} else if(strncmp(s,"TOPIC",strlen("TOPIC"))==0){
		return 1;
	} else if(strncmp(s,"PART",strlen("PART"))==0){
		return 1;
	} else if(strncmp(s,"LORO",strlen("LORO"))==0){
		return 1;
	} else if(strncmp(s,"NLORO",strlen("NLORO"))==0){
		return 1;
	}
	return 0;
}

void connect_client(pthread_t* h1, pthread_t* h2)
{
	struct addrinfo hints, *res = NULL;
	int nick;// user;
	//char port[20];
	char command[256];
	wprintw(output_win, "Conectando a %s:%s\n","metis.ii.uam.es","6667");
	wrefresh(output_win);
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	/*Comenzamos la conexion TCP*/
	wprintw(output_win, "Se obtiene iformacion\n");
	wrefresh(output_win);
	if(0!=getaddrinfo("metis.ii.uam.es", "6667", &hints, &res)){
		wprintw(output_win, "Error al onbtener informacion del servidor\n");
		wrefresh(output_win);
		return;
	}
	wprintw(output_win, "socket\n");
	wrefresh(output_win);
	sockfd=abrirSocketTCP();
	if(sockfd==-1){
		wprintw(output_win, "Error al crear el socket\n");
		wrefresh(output_win);
		return;
	}
	wprintw(output_win, "connect\n");
	wrefresh(output_win);
	if(-1==abrirConnect(sockfd, *(res->ai_addr))){
		wprintw(output_win, "Error al conectar\n");	
		wrefresh(output_win);
		return;
	}
	/*Conexion IRC*/
	wprintw(output_win, "IRC\n");
	wrefresh(output_win);
	/*wprintw(output_win, "los caracteres son estos:%c %c\n",0X0d,0X0a);
	wrefresh(output_win);*/
	pthread_create(h1,NULL, servRecv, (void *)NULL );
	//IRC_Nick(command, NULL, "raspberry");
	
	sprintf(command, "NICK raspberry%c%cUSER raspberry raspberry metis.ii.uam.es :raspberry%c%c",0X0d,0X0d,0X0d,0X0d);
	nick=escribir(sockfd,command);
	char* p = NULL;
	while((p = strchr(command, '\r')) != NULL) *p = '/';
	wprintw(output_win, "%s, %d\n",command,nick );
	wrefresh(output_win);
	/*strcpy(command,"USER as as metis.ii.uam.es :as");
	nick=escribir(sockfd,command);
	wprintw(output_win, "%s, %d\n",command,nick );
	*/
	sleep(1);
	//IRC_User(command, NULL, "guest", "w", "raspberry");
	//strcpy(command,"");
	//user=escribir(sockfd,command);
	//wprintw(output_win, "%s, %d\n",command,user );
	pthread_create(h2,NULL, Ping, (void *)NULL );
	//pthread_cancel(h1);
	//pthread_join(h1, NULL);
	
	free(res);
}
 void * Ping(void *args){
 	while(1){
 		char command[30];
		sleep(20);
		sprintf(command,"PING metis.ii.uam.es%c%c",0X0d,0X0d);
		escribir(sockfd,command);
		//wprintw(output_win, "%s", command);
	}
 }
int abrirSocketTCP(){
	int sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sockfd < 0){
		switch(errno){
			case EACCES:
				wprintw(output_win, "Permission to create a socket of the specified type and/or  protocol is denied.\n");	
			break;
		   	case EAFNOSUPPORT:
				wprintw(output_win, "The  implementation  does not support the specified address family.\n");
		    break;
		   	case EINVAL: 
		   		wprintw(output_win, "Unknown protocol, or protocol family not available. or Invalid flags in type.\n");
				 break;
		   	case EMFILE: 
		   		wprintw(output_win, "Process file table overflow.\n");
		   	break;
		   	case ENFILE: 
		   		wprintw(output_win, "The system limit on the total number  of  open  files  has  been reached.\n");
			break;
		   	case ENOBUFS:
		   		wprintw(output_win, "Insufficient  memory is available.  The socket cannot be created until sufficient resources are freed.\n");
		   	break; 
		   	case ENOMEM:
		        wprintw(output_win, "Insufficient  memory is available.  The socket cannot be created until sufficient resources are freed.\n");
		    break;
		   	case EPROTONOSUPPORT:
		    	wprintw(output_win, "The protocol type or the specified  protocol  is  not  supportedwithin this domain.\n" );
		   break;
		}
		wrefresh(output_win);
		return -1;
	}
	return sockfd;

}
/*anadido return -1*/
int abrirSocketUDP(){
	int sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(sockfd < 0){
		switch(errno){
			case EACCES:
				wprintw(output_win, "Permission to create a socket of the specified type and/or  protocol is denied.\n");
			break;
		   	case EAFNOSUPPORT:
		    	wprintw(output_win, "The  implementation  does not support the specified address family.\n");
		    break;
		   	case EINVAL: 
		   		wprintw(output_win, "Unknown protocol, or protocol family not available. or Invalid flags in type.\n");
			break;
		   	case EMFILE: 
		   		wprintw(output_win, "Process file table overflow.\n");
		   	break;
		   	case ENFILE: 
		   		wprintw(output_win, "The system limit on the total number  of  open  files  has  been reached.\n");
			break;
		   	case ENOBUFS:
		   		wprintw(output_win, "Insufficient  memory is available.  The socket cannot be created until sufficient resources are freed.\n");
		   	break; 
		   	case ENOMEM:
		        wprintw(output_win, "Insufficient  memory is available.  The socket cannot be created until sufficient resources are freed.\n");
		    break;
		   	case EPROTONOSUPPORT:
		    	wprintw(output_win, "The protocol type or the specified  protocol  is  not  supportedwithin this domain.\n" );
		   break;

		}
		wrefresh(output_win);
		return -1;
	}
	return sockfd;

}
int abrirConnect(int sockfd, struct sockaddr res){
	int cnct=0;
	cnct= connect(sockfd, &res, sizeof(res));
	if (cnct==-1){
		switch(errno){
			case EACCES:
		    	wprintw(output_win, "For UNIX domain sockets, which are identified by pathname: Write permission is denied on the socket file, or search permission is denied for one of the directories in the path prefix. (See also path_resolution(7).) \n");
				wprintw(output_win, "or The user tried to connect to a broadcast address without having the socket broadcast flag enabled or the connection request failed because of a local firewall rule.\n" );
						 		
				break;
		 	
		 	case EPERM:
		    		wprintw(output_win, "The user tried to connect to a broadcast address without having the socket broadcast flag enabled or the connection request failed because of a local firewall rule.\n"); 
				break;
			case EADDRINUSE:
		    	wprintw(output_win, "Local address is already in use. \n");
				break;
			case EAFNOSUPPORT:
		    	wprintw(output_win, "The passed address didn't have the correct address family in its sa_family field. \n");
				break;
			case EAGAIN:
			    wprintw(output_win, "No more free local ports or insufficient entries in the routing cache. For AF_INET see the description of /proc/sys/net/ipv4/ip_local_port_range ip(7) for information on how to increase the number of local ports. \n");
				break;
			case EALREADY:
			    wprintw(output_win, "The socket is nonblocking and a previous connection attempt has not yet been completed.\n"); 
				break;
			case EBADF:
			    wprintw(output_win, "The file descriptor is not a valid index in the descriptor table.\n"); 
				break;
			case ECONNREFUSED:
			    wprintw(output_win, "No-one listening on the remote address. \n");
				break;
			case EFAULT:
			    wprintw(output_win, "The socket structure address is outside the user's address space. \n");
				break;
			case EINPROGRESS:
			    wprintw(output_win, "The socket is nonblocking and the connection cannot be completed immediately. It is possible to select(2) or poll(2) for completion by selecting the socket for writing. After select(2) indicates writability, use getsockopt(2) to read the SO_ERROR option at level SOL_SOCKET to determine whether connect() completed successfully (SO_ERROR is zero) or unsuccessfully (SO_ERROR is one of the usual error codes listed here, explaining the reason for the failure). \n"); 
				break;
			case EINTR:
			    wprintw(output_win, "The system call was interrupted by a signal that was caught; see signal(7).\n"); 
				break;
			case EISCONN:
			    wprintw(output_win, "The socket is already connected.\n"); 
				break;
			case ENETUNREACH:
			    wprintw(output_win, "Network is unreachable. \n");
				break;
			case ENOTSOCK:
			    wprintw(output_win, "The file descriptor is not associated with a socket.\n"); 
				break;
			case ETIMEDOUT:
			    wprintw(output_win, "Timeout while attempting connection. The server may be too busy to accept new connections. Note that for IP sockets the timeout may be very long when syncookies are enabled on the server.\n");
			    break;
		}
		wrefresh(output_win);
		return -1;
	}
	return 0;
}

int recibir(int sockfd,char *buf){
	int aux=0;
	aux = recv(sockfd, buf, 1000, 0);
	buf[aux]='\0';
	if (aux==-1){
		switch(errno){
			case (EAGAIN || EWOULDBLOCK):
              printf ("The socket is marked nonblocking and the receive operation would block, or a receive timeout had been set and the timeout" 
              		  "expired before data was received.  POSIX.1-2001 allows either  error  to"
		              "be  returned for this case, and does not require these constants to have the same value, so a portable application  should  check"
		              "for both possibilities.\n");
			break;

	       	case EBADF:  
	       		printf ("The argument sockfd is an invalid descriptor.\n");
			break;
	       	case ECONNREFUSED:
	              printf ("A remote host refused to allow the network connection (typically because it is not running the requested service).\n");
			break;
	       	case EFAULT: 
	       		wprintw(output_win, "The  receive  buffer  pointer(s)  point  outside  the  process's address space.\n");
			break;
	       	case EINTR:  
	       		wprintw(output_win, "The  receive  was interrupted by delivery of a signal before any data were available; see signal(7).\n");
			break;
	       	case EINVAL: 
	       		wprintw(output_win, "Invalid argument passed.\n");
			break;
	       	case ENOMEM: 
	       		wprintw(output_win, "Could not allocate memory for recvmsg().\n");
			break;
	       	case ENOTCONN:
	            wprintw(output_win, "The socket is associated with a connection-oriented protocol and has not been connected (see connect(2) and accept(2)).\n");
			break;
	       	case ENOTSOCK:
	            wprintw(output_win, "The argument sockfd does not refer to a socket.\n");
			break;
		}
		wrefresh(output_win);
		return -1;
	}
	return aux;
}
int escribir(int sockfd,char *msg){
	int aux=send(sockfd,msg,strlen(msg),0);
	if(-1==aux){
		switch(errno){
		 	case EACCES: 
				wprintw(output_win, "(For UNIX domain sockets, which are identified by pathname) Write permission is denied on the destination socket file, or search permission  is"
			              "denied for one of the directories the path prefix.  (See path_resolution(7).)"

			              "(For UDP sockets) An attempt was made to send to a network/broadcast address as though it was a unicast address.\n");
				break;

	       case (EAGAIN || EWOULDBLOCK):
	             wprintw(output_win, " The  socket  is marked nonblocking and the requested operation would block.  POSIX.1-2001 allows either error to be returned for this case, and"
	              "does not require these constants to have the same value, so a portable application should check for both possibilities.\n");
	            break;
	       case EBADF:
	         	wprintw(output_win, "An invalid descriptor was specified.\n");
	         	break;
	       case ECONNRESET:
	            wprintw(output_win, "Connection reset by peer.\n");
	            break;
	       case EDESTADDRREQ:
	            wprintw(output_win, "The socket is not connection-mode, and no peer address is set.\n");
	            break;
	       case EFAULT:
	        	wprintw(output_win, "An invalid user space address was specified for an argument.\n");
	        	break;
	       case EINTR:
	         	wprintw(output_win, "A signal occurred before any data was transmitted; see signal(7).\n");
	         	break;
	       case EINVAL: 
	       		wprintw(output_win, "Invalid argument passed.\n");
	       		break;
	       case EISCONN:
	            wprintw(output_win, "The connection-mode socket was connected already but a recipient was specified.  (Now either this error is returned, or the recipient  specifi‐"
	              "cation is ignored.)\n");
	            break;
	       case EMSGSIZE:
	            wprintw(output_win, "The socket type requires that message be sent atomically, and the size of the message to be sent made this impossible.\n");
	            break;
	       case ENOBUFS:
	            wprintw(output_win, "The output queue for a network interface was full.  This generally indicates that the interface has stopped sending, but may be caused by tran‐"
	            "sient congestion.  (Normally, this does not occur in Linux.  Packets are just silently dropped when a device queue overflows.)\n");
	            break;
	       case ENOMEM: 
	       		wprintw(output_win, "No memory available.\n");
	       		break;
	       case ENOTCONN:
	            wprintw(output_win, "The socket is not connected, and no target has been given.\n");
	        case ENOTSOCK:
	            wprintw(output_win, "The argument sockfd is not a socket.\n");
	            break;
	       case EOPNOTSUPP:
	            wprintw(output_win, "Some bit in the flags argument is inappropriate for the socket type.\n");
	            break;
	       case EPIPE:
	         	wprintw(output_win, "The local end has been shut down on a connection oriented socket.  In this case the process will also receive a SIGPIPE unless MSG_NOSIGNAL  is set.\n");
	         	break;

		}
		wrefresh(output_win);
		return -1;
	}            
	return aux;  
}
/*int conexionPruebaCliente(char * direccion,char * puerto){
	struct addrinfo hints, *res;
	int sockfd, sock_server;
	char msg[1000];
    memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
	hints.ai_socktype = SOCK_STREAM;
	wprintw(output_win, "Abriendo socket\n");
	getaddrinfo("172.16.187.180", "2020", &hints, &res);
	sockfd = abrirSocketTCP();
	if (sockfd==-1){
		wprintw(output_win, "ERROR: socket\n");
		return -1;
	}
	
	wprintw(output_win, "Conectando\n");
	if(-1==(sock_server==abrirConnect(sockfd,*(res->ai_addr)))){
		wprintw(output_win, "ERROR: connect\n");
		return -1;
	}
	
	// convert to network byte order
	// send data normally:
	while (1){
		recibir(sockfd, msg);
		wprintw(output_win, "Recibido:%s\n", msg);
		strcpy(msg,"PONG");
		sleep(2);
		//send(sockfd,msg,strlen(msg),0);
		escribir(sockfd, msg);	
	}
	return 0;
}
*/

/*int conexionPruebaServidor(){
	int sockfd, socketClient, aux = 1;
	struct sockaddr_in ip4addr;
	char buf[1000];
	wprintw(output_win, "Abriendo socket\n");
	sockfd = abrirSocketTCP();
	if (sockfd==-1){
		wprintw(output_win, "ERROR: socket\n");
		return -1;
	}
	wprintw(output_win, "Abriedo bind\n");
	if( -1==abrirBind(sockfd)){
		wprintw(output_win, "ERROR: bind\n");
		return -1;	
	}
	wprintw(output_win, "Escuchando\n");
	if(-1==abrirListen(sockfd)){
		wprintw(output_win, "ERROR: listen\n");
		return -1;			
	}
	wprintw(output_win, "Esperando accept\n");
	socketClient = aceptar(sockfd, ip4addr);
	if(socketClient ==-1){
		wprintw(output_win, "ERROR: accept\n");
		return -1;	
	}

	wprintw(output_win, "sock_client:%d, sockfd:%d\n", socketClient,sockfd);
	while( aux > 0){
		sleep(2);
		strcpy(buf,"PING");
		write(socketClient, buf , strlen(buf));
		aux=recv(socketClient, buf,1000,0);
		if(aux == 0){
			wprintw(output_win, "fin de la conexion\n");
			return 0;
		}
		buf[aux]='\0';
		wprintw(output_win, "Recibido:%s\n",buf );
	}
	//byte_count = recv(sockfd, buf, sizeof buf, 0);
	//wprintw(output_win, "recv()'d %d bytes of data in buf\n", byte_count);
	
	return 0;
}*/
