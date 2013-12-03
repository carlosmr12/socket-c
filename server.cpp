//Aplicação que recebe o tempo de ordenação das aplicações clientes

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>

using namespace std;

//Função que trata dos clientes
void* trataSocket(void* lp){
    	
    	int *csock = (int*)lp;
	char buffer[1024];
	int buffer_len = 1024;
	int bytecount;

	memset(buffer, 0, buffer_len);
	bytecount = recv(*csock, buffer, buffer_len, 0);
	cout <<  buffer <<endl;


	FINISH:
	free(csock);
	return 0;
}

int main(int argv, char** argc){

	int host_port= 1101;
	int hsock;
	int * p_int ;
	int err;
	int* csock;
	
	struct sockaddr_in my_addr;
	socklen_t addr_size = 0;
	sockaddr_in sadr;
	pthread_t thread_id=0;


	hsock = socket(AF_INET, SOCK_STREAM, 0);
	p_int = (int*)malloc(sizeof(int));
	*p_int = 1;
	setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int));
	free(p_int);

	my_addr.sin_family = AF_INET ;
	my_addr.sin_port = htons(host_port);
	
	memset(&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = INADDR_ANY ;
	
	bind( hsock, (sockaddr*)&my_addr, sizeof(my_addr));
	listen( hsock, 10);

	

	addr_size = sizeof(sockaddr_in);
	cout <<"Esperando aplicacao cliente"<< endl;
	
	while(true){
		csock = (int*)malloc(sizeof(int));
		
		if((*csock = accept( hsock, (sockaddr*)&sadr, &addr_size))!= -1){
			pthread_create(&thread_id,0,&trataSocket, (void*)csock );
			pthread_detach(thread_id);
		}
		else{
			cout <<"Erro ao aceitar conexao" << errno <<endl;
		}
	}
	
	FINISH:
	return 0;
}


