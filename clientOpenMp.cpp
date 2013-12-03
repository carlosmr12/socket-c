//Cliente que usa OpemMp para Ordenação
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <omp.h>
#include <sstream>

using namespace std;


//Função Cliente Socket que conecta ao server e envia o tempo de ordenação

void connectServer(char *t){
	
	struct sockaddr_in remoteSocketInfo;
	struct hostent *hPtr;
	int socketHandle;
	char* remoteHost = "localhost";
	int portNumber = 1101;
	int rc = 0;  
	char buf[512];
	char aux[10];
	
	strcpy(aux, t);
	
	bzero(&remoteSocketInfo, sizeof(sockaddr_in));  // Clear structure memory
	hPtr = gethostbyname(remoteHost);
	socketHandle = socket(AF_INET, SOCK_STREAM, 0);

	memcpy((char *)&remoteSocketInfo.sin_addr, hPtr->h_addr, hPtr->h_length);
	remoteSocketInfo.sin_family = AF_INET;
	remoteSocketInfo.sin_port = htons((u_short)portNumber);      
	connect(socketHandle, (struct sockaddr *)&remoteSocketInfo, sizeof(sockaddr_in));
	
	strcpy(buf,"Tempo Aplicacao OpenMP:");
	strcat(buf,aux);
	strcat(buf,"segundos");
	
	send(socketHandle, buf, strlen(buf)+1, 0);
		
	
}


//Função de Ordenação usando MargeSort

vector<long> merge(const vector<long>& left, const vector<long>& right)
{
    vector<long> result;
    unsigned left_it = 0, right_it = 0;

    while(left_it < left.size() && right_it < right.size())
    {
        if(left[left_it] < right[right_it])
        {
            result.push_back(left[left_it]);
            left_it++;
        }
        else					
        {
            result.push_back(right[right_it]);
            right_it++;
        }
    }

    // Push the remaining data from both vectors onto the resultant
    while(left_it < left.size())
    {
        result.push_back(left[left_it]);
        left_it++;
    }

    while(right_it < right.size())
    {
        result.push_back(right[right_it]);
        right_it++;
    }

    return result;
}

vector<long> mergesort(vector<long>& vec, int threads)
{
    // Termination condition: List is completely sorted if it
    // only contains a single element.
    if(vec.size() == 1)
    {
        return vec;
    }

    // Determine the location of the middle element in the vector
    std::vector<long>::iterator middle = vec.begin() + (vec.size() / 2);

    vector<long> left(vec.begin(), middle);
    vector<long> right(middle, vec.end());

    // Perform a merge sort on the two smaller vectors
    
    //Parte do código que irá executar de forma paralela
    if (threads > 1)
    {
      #pragma omp parallel sections
      {
        #pragma omp section
        {
          left = mergesort(left, threads/2);
        }
        #pragma omp section
        {
          right = mergesort(right, threads - threads/2);
        }
      }
    }
    else
    {
      left = mergesort(left, 1);
      right = mergesort(right, 1);
    }

    return merge(left, right);
}

//Fim da Função de Ordenação

int main()
{
	float t1, t2;
	char tfinal[10];

	vector<long> v(1000000);
	for (long i=0; i<1000000; ++i)// Cria o vetor a ser Ordenado
	v[i] = (i * i) % 1000000;

	t1 = (float) clock(); // Coleta o tempo antes da ordenação

	v = mergesort(v, 4); // Chama função de Ordenação
	
	t2 = (float) clock();	// Coleta tempo após função de ordenação
	
	sprintf(tfinal,"%.3f",((t2 - t1)/ CLOCKS_PER_SEC)); // Calcula o tempo gasto para a ordenação
	
	connectServer(tfinal); // Chamada da Função para enviar o tempo de ordenação
	
	return 0;
  
}
