/* File: main.cpp
 * Date: 
 * Author: Léonard Bise
 * Description: 
 */

#include <iostream>
#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <arpa/inet.h>

using namespace std;

#define LGBUF 100	
#define SERVEUR_HOST "127.0.0.1"
#define SERVEUR_PORT 65535

int main(int argc, char **argv) {
	int sockfd, res;

	cout << "RaceTracker Application Server\n";

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("ERROR opening socket");
		exit(EXIT_FAILURE);
	}
    
	struct sockaddr_in name;
	name.sin_family= AF_INET;
	name.sin_addr.s_addr = inet_addr(SERVEUR_HOST); /*0x7f000001*/
	name.sin_port = SERVEUR_PORT;

	res = connect(sockfd, (struct sockaddr *) &name, sizeof(name));
	if (res < 0) {
		perror("ERROR Cannot connect to socket");
		exit(EXIT_FAILURE);	
	}


	return 0;
}