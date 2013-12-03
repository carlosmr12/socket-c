 /*
  * Checking efficiency of openMp application over a non-openMp application
  * Universidade Estadual de Montes Claros - UNIMONTES 
  * Departamento de Ciências da Computação - DCC 
  * Curso de Sistemas de Informação - 7 Período
  *                 
  * Disciplinas: Cliente/Servidor e Sistemas Distribuídos I
  * Prof.        Rafael Moreno 
  * Acadêmicos:       
  *     Carlos Rodrigues
  *     Déborah Soares
  *     Fábio Vinícius
  *     Lino José
  *     Renan Teixeira
  *
*/

/*
 *
 *  Client application with the use of OpenMp
 *
 */
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


/*
 *
 *  connectServer function
 *
 * */
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
	
	strcpy(buf,"Tempo da ordenacao mergeSort com o uso do OpenMP:\t");
	strcat(buf,aux);
	strcat(buf," segundos");
	
	send(socketHandle, buf, strlen(buf)+1, 0);
}


/*
 *
 *  Merge Function to merge the right and left side of the vector
 *
 * */
vector<long> merge(const vector<long>& left, const vector<long>& right){
    vector<long> result;
    unsigned left_it = 0, right_it = 0;

    while(left_it < left.size() && right_it < right.size()){
        if(left[left_it] < right[right_it]){
            result.push_back(left[left_it]);
            left_it++;
        }
        else{
            result.push_back(right[right_it]);
            right_it++;
        }
    }

    //  Add the data from both vectors onto a result vector
    while(left_it < left.size()){
        result.push_back(left[left_it]);
        left_it++;
    }

    while(right_it < right.size()){
        result.push_back(right[right_it]);
        right_it++;
    }

    return result;
}

/*
 *  MergeSort function to sort elements using threads 
 *
 * */
vector<long> mergesort(vector<long>& vec, int threads){
    // Terminate if the list contains only a single element, which means that the list is completely sorted
    if(vec.size() == 1){
        return vec;
    }

    // Get the location of the middle element
    std::vector<long>::iterator middle = vec.begin() + (vec.size() / 2);

    vector<long> left(vec.begin(), middle);
    vector<long> right(middle, vec.end());

    // Perform the merge sort on the two sides of the vector separately
    if (threads > 1){
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
    else{
      left = mergesort(left, 1);
      right = mergesort(right, 1);
    }

    // Merge the two sides of the vector
    return merge(left, right);
}

/*
 *  Main Function
 *
 * */
int main(){
	float t1, t2;
	char tfinal[10];

	vector<long> v(1000000);

	for (long i=0; i<1000000; ++i)
        v[i] = (i * i) % 1000000;

	t1 = (float) clock();

	v = mergesort(v, 4); 
	
	t2 = (float) clock();
	
	sprintf(tfinal,"%.5f",((t2 - t1)/ CLOCKS_PER_SEC));
	
	connectServer(tfinal);
	
	return 0;
}
