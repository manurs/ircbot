#include "funciones.h"
#include <stdio.h>
#include <string.h>
void main(){
	char msg[512];
	connect_client();
	while(1){
		printf("mensaje:");
		gets(msg);
		sprintf(msg,"%s%c%c",msg,0X0d,0X0d);
		printf("\n");
		escribir(sockfd,msg);
	}
}
