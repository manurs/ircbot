#include "funciones.h"
#include <pthread.h>
/*anadido return -1*/


void * servRecv(void *args){
	char buf[512], aux2[512];
	char *command, *usr, /* *ch,*/*trash;
	int aux; 
	while(1){
		command=NULL;
		usr=NULL;
		//ch=NULL;
		trash=NULL;
		if(0<(aux=recibir(sockfd, buf))){
			printf("%drecibido:%s\n\n\t",aux,buf);
			usr = strtok (buf,"!");
			if(usr==NULL)
				continue;
			strcpy(usr,&usr[1]);
			printf(" %s",usr);
			trash = strtok (NULL," ");
			if(trash==NULL)
				continue;
			//printf(" %s",trash);
			command = strtok (NULL," ");
			if(command==NULL)
				continue;
			printf(" %s\n",command);
			if(strcmp(command,"JOIN")==0){
				printf("%s\n",usr);
				sprintf(aux2,"MODE #cosas +o %s%c%c",usr,0x0d,0x0a);
				printf(" %s",aux2);
				escribir(sockfd, aux2);
				
			}
			int send = 1;
			char msg[1024] = "\0";
			if(strcmp(command,"PRIVMSG")==0){
		    	trash = strtok (NULL," ");
				trash = strtok (NULL,":");
				char* ultra_trash = malloc(sizeof(trash));
				strcpy(ultra_trash, trash);
				trash = strtok(NULL, ":");
				if (trash != NULL){
					sprintf(msg, "%s:%s", ultra_trash, trash);
				} else{
					//poner aqui un if que si lee JOIN en ultra_trash ponga send a 1
					send = 0;
				}
				fprintf(stderr, "trash=%s\n",trash);
				fprintf(stderr, "ultra_trash = %s\n", ultra_trash);
				fflush(stderr);
				if(strstr(msg,"SEND") != NULL){
					send=1;
					printf("SEND\n");
				} else if(strstr(msg,"NSEND") != 0){
					send=0;
					printf("NSEND\n");
				}
				/*if(msg!=NULL){
				printf("msg=%s\n",msg);
				strcpy(msg,trash);
				sprintf(aux2,"%s:%s",aux2,msg);
				}*/
				printf("send %d\n",send);
				if(send ==1){
					char* p = msg;
					p += strlen("SEND ");
					printf ("se envia=%s",p);
					escribir(sockfd, p);

				}
	    	}
		}
	}
	printf("Terminada la conexion\n");
}
void connect_client(void)
{
	struct addrinfo hints, *res;
	int nick;// user;
	pthread_t h1,h2;
	//char port[20];
	char command[256];
	printf("Conectando a %s:%s\n","metis.ii.uam.es","6667"  );
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	/*Comenzamos la conexion TCP*/
	printf("se obtiene iformacion\n");
	if(0!=getaddrinfo("metis.ii.uam.es", "6667", &hints, &res)){
		printf("Error al onbtener informacion del servidor\n");
		return;
	}
	printf("socket\n");
	sockfd=abrirSocketTCP();
	if(sockfd==-1){
		printf("Error al crear el socket\n");
		return;
	}
	printf("connect\n");
	if(-1==abrirConnect(sockfd, *(res->ai_addr))){
		printf("Error al conectar\n");	
		return;
	}
	/*Conexion IRC*/
	printf("IRC\n");
	printf("los caracteres son estos:%c %c",0X0d,0X0a);
	pthread_create(&h1,NULL, servRecv, (void *)NULL );
	//IRC_Nick(command, NULL, "raspberry");
	sprintf(command, "NICK raspberry%c%cUSER raspberry raspberry metis.ii.uam.es :raspberry%c%c",0X0d,0X0d,0X0d,0X0d);
	nick=escribir(sockfd,command);
	printf("%s, %d\n",command,nick );
	
	/*strcpy(command,"USER as as metis.ii.uam.es :as");
	nick=escribir(sockfd,command);
	printf("%s, %d\n",command,nick );
	*/
	sleep(1);
	//IRC_User(command, NULL, "guest", "w", "raspberry");
	//strcpy(command,"");
	//user=escribir(sockfd,command);
	//printf("%s, %d\n",command,user );
	pthread_create(&h2,NULL, Ping, (void *)NULL );
	
}
 void * Ping(void *args){
 	while(1){
 		char command[30];
 		int user;
		sleep(20);
		sprintf(command,"PING metis.ii.uam.es%c%c",0X0d,0X0d);
		user=escribir(sockfd,command);
		printf("%s", command);
	}
 }
int abrirSocketTCP(){
	int sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sockfd < 0){
		switch(errno){
			case EACCES:
				printf("Permission to create a socket of the specified type and/or  protocol is denied.\n");
			break;
		   	case EAFNOSUPPORT:
				printf("The  implementation  does not support the specified address family.\n");
		    break;
		   	case EINVAL: 
		   		printf("Unknown protocol, or protocol family not available. or Invalid flags in type.\n");
				 break;
		   	case EMFILE: 
		   		printf("Process file table overflow.\n");
		   	break;
		   	case ENFILE: 
		   		printf("The system limit on the total number  of  open  files  has  been reached.\n");
			break;
		   	case ENOBUFS:
		   		printf("Insufficient  memory is available.  The socket cannot be created until sufficient resources are freed.\n");
		   	break; 
		   	case ENOMEM:
		        printf("Insufficient  memory is available.  The socket cannot be created until sufficient resources are freed.\n");
		    break;
		   	case EPROTONOSUPPORT:
		    	printf("The protocol type or the specified  protocol  is  not  supportedwithin this domain.\n" );
		   break;
		}
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
				printf("Permission to create a socket of the specified type and/or  protocol is denied.\n");
			break;
		   	case EAFNOSUPPORT:
		    	printf("The  implementation  does not support the specified address family.\n");
		    break;
		   	case EINVAL: 
		   		printf("Unknown protocol, or protocol family not available. or Invalid flags in type.\n");
			break;
		   	case EMFILE: 
		   		printf("Process file table overflow.\n");
		   	break;
		   	case ENFILE: 
		   		printf("The system limit on the total number  of  open  files  has  been reached.\n");
			break;
		   	case ENOBUFS:
		   		printf("Insufficient  memory is available.  The socket cannot be created until sufficient resources are freed.\n");
		   	break; 
		   	case ENOMEM:
		        printf("Insufficient  memory is available.  The socket cannot be created until sufficient resources are freed.\n");
		    break;
		   	case EPROTONOSUPPORT:
		    	printf("The protocol type or the specified  protocol  is  not  supportedwithin this domain.\n" );
		   break;

		}
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
		    	printf("For UNIX domain sockets, which are identified by pathname: Write permission is denied on the socket file, or search permission is denied for one of the directories in the path prefix. (See also path_resolution(7).) \n");
				printf("or The user tried to connect to a broadcast address without having the socket broadcast flag enabled or the connection request failed because of a local firewall rule.\n" );
						 		
				break;
		 	
		 	case EPERM:
		    		printf("The user tried to connect to a broadcast address without having the socket broadcast flag enabled or the connection request failed because of a local firewall rule.\n"); 
				break;
			case EADDRINUSE:
		    	printf("Local address is already in use. \n");
				break;
			case EAFNOSUPPORT:
		    	printf("The passed address didn't have the correct address family in its sa_family field. \n");
				break;
			case EAGAIN:
			    printf("No more free local ports or insufficient entries in the routing cache. For AF_INET see the description of /proc/sys/net/ipv4/ip_local_port_range ip(7) for information on how to increase the number of local ports. \n");
				break;
			case EALREADY:
			    printf("The socket is nonblocking and a previous connection attempt has not yet been completed.\n"); 
				break;
			case EBADF:
			    printf("The file descriptor is not a valid index in the descriptor table.\n"); 
				break;
			case ECONNREFUSED:
			    printf("No-one listening on the remote address. \n");
				break;
			case EFAULT:
			    printf("The socket structure address is outside the user's address space. \n");
				break;
			case EINPROGRESS:
			    printf("The socket is nonblocking and the connection cannot be completed immediately. It is possible to select(2) or poll(2) for completion by selecting the socket for writing. After select(2) indicates writability, use getsockopt(2) to read the SO_ERROR option at level SOL_SOCKET to determine whether connect() completed successfully (SO_ERROR is zero) or unsuccessfully (SO_ERROR is one of the usual error codes listed here, explaining the reason for the failure). \n"); 
				break;
			case EINTR:
			    printf("The system call was interrupted by a signal that was caught; see signal(7).\n"); 
				break;
			case EISCONN:
			    printf("The socket is already connected.\n"); 
				break;
			case ENETUNREACH:
			    printf("Network is unreachable. \n");
				break;
			case ENOTSOCK:
			    printf("The file descriptor is not associated with a socket.\n"); 
				break;
			case ETIMEDOUT:
			    printf("Timeout while attempting connection. The server may be too busy to accept new connections. Note that for IP sockets the timeout may be very long when syncookies are enabled on the server.\n");
			    break;
		}
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
	       		printf("The  receive  buffer  pointer(s)  point  outside  the  process's address space.\n");
			break;
	       	case EINTR:  
	       		printf("The  receive  was interrupted by delivery of a signal before any data were available; see signal(7).\n");
			break;
	       	case EINVAL: 
	       		printf("Invalid argument passed.\n");
			break;
	       	case ENOMEM: 
	       		printf("Could not allocate memory for recvmsg().\n");
			break;
	       	case ENOTCONN:
	            printf("The socket is associated with a connection-oriented protocol and has not been connected (see connect(2) and accept(2)).\n");
			break;
	       	case ENOTSOCK:
	            printf("The argument sockfd does not refer to a socket.\n");
			break;
		}
		return -1;
	}
	return aux;
}
int escribir(int sockfd,char *msg){
	int aux=send(sockfd,msg,strlen(msg),0);
	if(-1==aux){
		switch(errno){
		 	case EACCES: 
				printf("(For UNIX domain sockets, which are identified by pathname) Write permission is denied on the destination socket file, or search permission  is"
			              "denied for one of the directories the path prefix.  (See path_resolution(7).)"

			              "(For UDP sockets) An attempt was made to send to a network/broadcast address as though it was a unicast address.\n");
				break;

	       case (EAGAIN || EWOULDBLOCK):
	             printf(" The  socket  is marked nonblocking and the requested operation would block.  POSIX.1-2001 allows either error to be returned for this case, and"
	              "does not require these constants to have the same value, so a portable application should check for both possibilities.\n");
	            break;
	       case EBADF:
	         	printf("An invalid descriptor was specified.\n");
	         	break;
	       case ECONNRESET:
	            printf("Connection reset by peer.\n");
	            break;
	       case EDESTADDRREQ:
	            printf("The socket is not connection-mode, and no peer address is set.\n");
	            break;
	       case EFAULT:
	        	printf("An invalid user space address was specified for an argument.\n");
	        	break;
	       case EINTR:
	         	printf("A signal occurred before any data was transmitted; see signal(7).\n");
	         	break;
	       case EINVAL: 
	       		printf("Invalid argument passed.\n");
	       		break;
	       case EISCONN:
	            printf("The connection-mode socket was connected already but a recipient was specified.  (Now either this error is returned, or the recipient  specifi‐"
	              "cation is ignored.)\n");
	            break;
	       case EMSGSIZE:
	            printf("The socket type requires that message be sent atomically, and the size of the message to be sent made this impossible.\n");
	            break;
	       case ENOBUFS:
	            printf("The output queue for a network interface was full.  This generally indicates that the interface has stopped sending, but may be caused by tran‐"
	            "sient congestion.  (Normally, this does not occur in Linux.  Packets are just silently dropped when a device queue overflows.)\n");
	            break;
	       case ENOMEM: 
	       		printf("No memory available.\n");
	       		break;
	       case ENOTCONN:
	            printf("The socket is not connected, and no target has been given.\n");
	        case ENOTSOCK:
	            printf("The argument sockfd is not a socket.\n");
	            break;
	       case EOPNOTSUPP:
	            printf("Some bit in the flags argument is inappropriate for the socket type.\n");
	            break;
	       case EPIPE:
	         	printf("The local end has been shut down on a connection oriented socket.  In this case the process will also receive a SIGPIPE unless MSG_NOSIGNAL  is set.\n");
	         	break;

		}
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
	printf("Abriendo socket\n");
	getaddrinfo("172.16.187.180", "2020", &hints, &res);
	sockfd = abrirSocketTCP();
	if (sockfd==-1){
		printf("ERROR: socket\n");
		return -1;
	}
	
	printf("Conectando\n");
	if(-1==(sock_server==abrirConnect(sockfd,*(res->ai_addr)))){
		printf("ERROR: connect\n");
		return -1;
	}
	
	// convert to network byte order
	// send data normally:
	while (1){
		recibir(sockfd, msg);
		printf("Recibido:%s\n", msg);
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
	printf("Abriendo socket\n");
	sockfd = abrirSocketTCP();
	if (sockfd==-1){
		printf("ERROR: socket\n");
		return -1;
	}
	printf("Abriedo bind\n");
	if( -1==abrirBind(sockfd)){
		printf("ERROR: bind\n");
		return -1;	
	}
	printf("Escuchando\n");
	if(-1==abrirListen(sockfd)){
		printf("ERROR: listen\n");
		return -1;			
	}
	printf("Esperando accept\n");
	socketClient = aceptar(sockfd, ip4addr);
	if(socketClient ==-1){
		printf("ERROR: accept\n");
		return -1;	
	}

	printf("sock_client:%d, sockfd:%d\n", socketClient,sockfd);
	while( aux > 0){
		sleep(2);
		strcpy(buf,"PING");
		write(socketClient, buf , strlen(buf));
		aux=recv(socketClient, buf,1000,0);
		if(aux == 0){
			printf("fin de la conexion\n");
			return 0;
		}
		buf[aux]='\0';
		printf("Recibido:%s\n",buf );
	}
	//byte_count = recv(sockfd, buf, sizeof buf, 0);
	//printf("recv()'d %d bytes of data in buf\n", byte_count);
	
	return 0;
}*/
