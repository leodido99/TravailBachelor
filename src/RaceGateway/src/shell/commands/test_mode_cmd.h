/** 
 * @file test_mode_cmd.h
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jun 25, 2018
 */
#ifndef SRC_SHELL_COMMANDS_TEST_MODE_CMD_H_
#define SRC_SHELL_COMMANDS_TEST_MODE_CMD_H_

#include "shell_command.h"
#include "test_mode_handler.h"

class test_mode_cmd : public shell_command {
private:
	test_mode_handler* handler;
	int cnt;

	std::string get_curr_time();
public:
	test_mode_cmd(test_mode_handler* handler);
	virtual ~test_mode_cmd();

	/**
	 * Executes the command
	 * @param
	 */
	void exec(std::list<std::string>* parameters);
};

#endif /* SRC_SHELL_COMMANDS_TEST_MODE_CMD_H_ */
