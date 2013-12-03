all:
	g++ server.cpp -o server -lpthread
	g++ client.cpp -o client
	g++ clientOpenMp.cpp -o clientOpenMp -fopenmp
