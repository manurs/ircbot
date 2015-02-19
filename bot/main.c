#include "funciones.h"
#include <stdio.h>
#include <string.h>
int main(){
	char msg[512];
	pthread_t h1,h2;
	connect_client(&h1, &h2);
	while(1){
		printf("mensaje:");
		fflush(stdout);
		read(0, msg, sizeof(msg));
		char* p;
		if((p = strchr(msg, '\n')) != NULL) *p = '\0';
		if(strcmp(msg, "QUIT") == 0) break;
		//gets(msg);
		sprintf(msg,"%s%c%c",msg,0X0d,0X0d);
		printf("\n");
		escribir(sockfd,msg);
	}
	pthread_cancel(h1);
	pthread_join(h1, NULL);
	pthread_cancel(h2);
	pthread_join(h2, NULL);

	return EXIT_SUCCESS;
}
