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
#include <arpa/inet.h>

#include "lora_udp_pkt.h"
#include "lora_udp_rxpkt.h"

using namespace rapidjson;

race_app_server::race_app_server() {
	this->listen_port = -1;
	this->listen_socket = -1;
	this->listening_thread = NULL;
	this->verbose = false;
	this->is_thread_running = true;
}

void race_app_server::load_configuration(std::string configfile) {
	char buffer[65536];
	Document document;

	/* Open configuration file */
    FILE* p_file = fopen(configfile.c_str(), "r");
    if (p_file == NULL) {
    	throw std::runtime_error("Cannot open configuration file " + configfile);
    }
    /* Reads the file */
    FileReadStream fs(p_file, buffer, sizeof(buffer));
    /* Parse the stream */
    document.ParseStream(fs);
    /* Iterate on all members of the json file */
    for (Value::ConstMemberIterator fileIt = document.MemberBegin(); fileIt != document.MemberEnd(); ++fileIt) {
        /* Get member name */
    	std::string objectType(fileIt->name.GetString());
        if (objectType.compare("race_app_server_conf") == 0) {
            const Value& race_app_server_conf = fileIt->value;
            if (race_app_server_conf.IsObject()) {
            	/* Iterate on all members of race_app_server_conf */
                for (Value::ConstMemberIterator confIt = race_app_server_conf.MemberBegin(); confIt != race_app_server_conf.MemberEnd(); ++confIt) {
                    /* Get key name */
                	std::string key(confIt->name.GetString());
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
	std::cout << "Race Application Server version XX" << std::endl;
	std::cout << "Configuration:" << std::endl;
	std::cout << "Listen Port: " << listen_port << std::endl;
}

void race_app_server::process_datagram(uint8_t* data, int size) {
	/* This method processes the received UDP datagram from the packet forwarder
	 * It only cares about rxpkt datagrams. Others are discarded
	 * */
	lora_udp_pkt new_pkt;
	new_pkt.parse(data, size);
	this->log(INFO, new_pkt.get_string());
	if (new_pkt.get_pkt_type() == PUSH_DATA && new_pkt.get_json_obj_name() == "rxpk") {
		lora_udp_rxpkt new_rxpkt(new_pkt.get_rapidjson_doc());
		std::cout << new_rxpkt << std::endl;
	}




}

void race_app_server::listen() {
	int nb_bytes;
	struct sockaddr_storage dist_addr;
	socklen_t addr_len = sizeof dist_addr;
	uint8_t databuf[4096];
    char host_name[64];
    char port_name[64];

	this->log(INFO, "Start listening on port " + std::to_string(this->listen_port));

	while(this->is_thread_running) {
		nb_bytes = recvfrom(this->listen_socket, databuf, sizeof databuf, 0, (struct sockaddr *)&dist_addr, &addr_len);
		if (nb_bytes == -1) {
			/* Error during recvfrom */
			throw std::runtime_error("Error during data reception : " + std::string(strerror(errno)));
		}
		if (this->verbose) {
			getnameinfo((struct sockaddr *)&dist_addr, addr_len, host_name, sizeof host_name, port_name, sizeof port_name, NI_NUMERICHOST);
			this->log(DEBUG, "Received " + std::to_string(nb_bytes) + " bytes from " + host_name + ":" + port_name);
		}
		/* Process data */
		this->process_datagram(databuf, nb_bytes);
	}
}

void race_app_server::start() {
	/* Create socket and bind it to the address at the provided port */
	this->connect_listen();
	/* Create listening thread */
	this->listening_thread = new std::thread(&race_app_server::listen, this);
	this->listening_thread->join();
}

void race_app_server::log(log_levels level, std::string msg) {
	if (level == INFO || (level == DEBUG && this->verbose)) {
		std::cout << msg << std::endl;
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
    char host_name[64];
    char port_name[64];

    /* prepare hints to open network sockets */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; /* should handle IP v4 or v6 automatically */
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; /* will assign local IP automatically */

    /* look for address at provided port */
    i = getaddrinfo(NULL, std::to_string(this->listen_port).c_str(), &hints, &results);
    if (i != 0) {
    	throw std::runtime_error("Cannot get port information for port " + std::to_string(this->listen_port) + " : " + gai_strerror(i));
    }
    /* Iterate over all returned items */
    for (result=results; result!=NULL; result=result->ai_next) {
    	this->log(DEBUG, "Create socket: Family: " + std::to_string(result->ai_family) + " Socket type: " + std::to_string(result->ai_socktype) + " Protocol: " + std::to_string(result->ai_protocol));
		this->listen_socket = socket(result->ai_family, result->ai_socktype,result->ai_protocol);
		if (this->listen_socket == -1) {
			/* Socket creation failed, try next item */
			continue;
		} else {
	    	getnameinfo(result->ai_addr, result->ai_addrlen, host_name, sizeof host_name, port_name, sizeof port_name, NI_NUMERICHOST);
			i = bind(this->listen_socket, result->ai_addr, result->ai_addrlen);
			if (i == -1) {
				/* bind failed, try next field */
				this->log(DEBUG, "Binding socket to " + std::string(host_name) + ":" + std::string(port_name) + " failed");
				shutdown(this->listen_socket, SHUT_RDWR);
				continue;
			} else {
				/* success, get out of loop */
				this->log(INFO, "Binding socket to " + std::string(host_name) + ":" + std::string(port_name) + " success");
				break;
			}
		}
    }
    /* Check if we managed to create socket and bind it */
    if (result == NULL) {
    	throw std::runtime_error("Cannot create socket or bind to port " + std::to_string(this->listen_port));
    }
    freeaddrinfo(results);
}

void race_app_server::stop() {
	this->is_thread_running = false;
	this->listening_thread->join();
}
