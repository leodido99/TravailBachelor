/* File: race_app_server.cpp
 * Date: 
 * Author: Léonard Bise
 * Description: 
 * Implementation of the Race Application Server class
 *
 * Semtech's packet forwarder (https://github.com/Lora-net/packet_forwarder)
 * was used as a base to develop this application, in particular the util_sink application.
 *
 * @hallard Single channel gateway (https://github.com/hallard/single_chan_pkt_fwd) implementation was also
 * used as a base (in particular the rapidjson usage for the configuration)
 */

#include "race_app_server.h"

#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

using namespace rapidjson;

race_app_server::race_app_server() {
	this->listen_port = -1;
	this->verbose = false;
}

void race_app_server::load_configuration(string configfile) {
	char buffer[65536];
	Document document;

	/* Open configuration file */
    FILE* p_file = fopen(configfile.c_str(), "r");
    if (p_file == NULL) {
    	throw runtime_error("Cannot open configuration file " + configfile);
    }
    /* Reads the file */
    FileReadStream fs(p_file, buffer, sizeof(buffer));
    /* Parse the stream */
    document.ParseStream(fs);
    /* Iterate on all members of the json file */
    for (Value::ConstMemberIterator fileIt = document.MemberBegin(); fileIt != document.MemberEnd(); ++fileIt) {
        /* Get member name */
    	string objectType(fileIt->name.GetString());
        if (objectType.compare("race_app_server_conf") == 0) {
            const Value& race_app_server_conf = fileIt->value;
            if (race_app_server_conf.IsObject()) {
            	/* Iterate on all members of race_app_server_conf */
                for (Value::ConstMemberIterator confIt = race_app_server_conf.MemberBegin(); confIt != race_app_server_conf.MemberEnd(); ++confIt) {
                    /* Get key name */
                	string key(confIt->name.GetString());
                    if (key.compare("listen_port") == 0) {
                        listen_port = confIt->value.GetInt();
                    }
                }
            }
        }
    }
}

void race_app_server::print_configuration() {
	/* TODO Setup version */
	cout << "Race Application Server version XX" << endl;
	cout << "Configuration:" << endl;
	cout << "Listen Port: " << listen_port << endl;
}

void race_app_server::start() {

	this->connect_listen();




}

void race_app_server::log(string msg) {
	if (this->verbose) {
		cout << msg << endl;
	}
}

void race_app_server::set_verbose(bool verbose) {
	this->verbose = verbose;
}

void race_app_server::connect_listen() {
	struct addrinfo hints;
	struct addrinfo *results;
	struct addrinfo *result;
	int i;

    /* prepare hints to open network sockets */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; /* should handle IP v4 or v6 automatically */
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; /* will assign local IP automatically */

    /* look for address */
    i = getaddrinfo(NULL, to_string(this->listen_port).c_str(), &hints, &results);
    if (i != 0) {
    	throw runtime_error("Cannot get port information for port " + to_string(this->listen_port) + " : " + gai_strerror(i));
    }
    /* Iterate over all returned items */
    for (result=results; result!=NULL; result=result->ai_next) {
    	this->log("Create socket: Family: " + to_string(result->ai_family) + " Socket type: " + to_string(result->ai_socktype) + " Protocol: " + to_string(result->ai_protocol));
		this->listen_socket = socket(result->ai_family, result->ai_socktype,result->ai_protocol);
		if (this->listen_socket == -1) {

		}

    }


}
