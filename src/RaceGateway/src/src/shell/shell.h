/**
 * @file shell.h
 * @brief Shell
 *
 * The application shell provides a way for the user
 * to enter commands to perform various operations on the race_app_server.
 *
 * @author Léonard Bise
 * @date   Jul 9, 2018
 */

#ifndef SRC_SHELL_H_
#define SRC_SHELL_H_

#include <thread>

class shell {
private:
	std::thread* shell_thread;
	bool shell_thread_running;

	/**
	 * Shell thread method
	 */
	void shell_handler();
public:
	/**
	 * Constructor
	 */
	shell();

	/**
	 * Starts the shell
	 */
	void start();
};

#endif /* SRC_SHELL_H_ */
