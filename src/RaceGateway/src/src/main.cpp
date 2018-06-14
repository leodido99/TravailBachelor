/* File: main.cpp
 * Date: 
 * Author: Léonard Bise
 * Description: 
 */

#include "race_app_server.h"


#include <iostream>
#include <sys/socket.h>
#include <cstdio>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

#define LGBUF 100	
#define SERVEUR_HOST "127.0.0.1"
#define SERVEUR_PORT 65535

int main(int argc, char **argv) {
	race_app_server myAppServer;

	/* TODO Check arguments for :
	 * Config file
	 * verbose lvl */

	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		fprintf(stdout, "Current working dir: %s\n", cwd);

	/* Load configuration */
	myAppServer.load_configuration("global_conf.json");
	/* Print configuration */
	myAppServer.print_configuration();



//
//	int sockfd, res;
//
//
//
//
//
//
//
//	sockfd = socket(AF_INET, SOCK_STREAM, 0);
//	if (sockfd < 0) {
//		perror("ERROR opening socket");
//		exit(EXIT_FAILURE);
//	}
//
//	struct sockaddr_in name;
//	name.sin_family= AF_INET;
//	name.sin_addr.s_addr = inet_addr(SERVEUR_HOST); /*0x7f000001*/
//	name.sin_port = SERVEUR_PORT;
//
//	res = connect(sockfd, (struct sockaddr *) &name, sizeof(name));
//	if (res < 0) {
//		perror("ERROR Cannot connect to socket");
//		exit(EXIT_FAILURE);
//	}


	return 0;
}
