//Cliente que não usa OpemMp para Ordenação
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
#include <sstream>

using namespace std;

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
	
	strcpy(buf,"Tempo Aplicacao Normal:");
	strcat(buf,aux);
	strcat(buf,"segundos");
	
	send(socketHandle, buf, strlen(buf)+1, 0);
		
	
}

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

vector<long> mergesort(vector<long>& vec)
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

    left = mergesort(left);
    right = mergesort(right);
    

    return merge(left, right);
}

int main()
{
	float t1, t2;
	char tfinal[10];

	vector<long> v(1000000);
	for (long i=0; i<1000000; ++i)
	v[i] = (i * i) % 1000000;

	t1 = (float) clock();

	v = mergesort(v);
	
	t2 = (float) clock();	
	
	sprintf(tfinal,"%.3f",((t2 - t1)/ CLOCKS_PER_SEC));
	
	connectServer(tfinal);
	
	return 0;
  
}
