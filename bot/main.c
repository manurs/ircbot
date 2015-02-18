#include "funciones.h"
#include <stdio.h>
#include <string.h>
int main(){
	char msg[512];
	connect_client();
	while(1){
		printf("mensaje:");
		read(0, msg, sizeof(msg));
		char* p;
		if((p = strchr(msg, '\n')) != NULL) *p = '\0';
		//gets(msg);
		sprintf(msg,"%s%c%c",msg,0X0d,0X0d);
		printf("\n");
		escribir(sockfd,msg);
	}
	return EXIT_SUCCESS;
}
