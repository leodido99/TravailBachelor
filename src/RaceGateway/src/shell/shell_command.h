/** 
 * @file shell_command.h
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jun 25, 2018
 */
#ifndef SRC_SHELL_SHELL_COMMAND_H_
#define SRC_SHELL_SHELL_COMMAND_H_

#include <list>
#include <string>

class shell_command {
protected:
	std::string name;
public:
	/**
	 * Destructor
	 */
	virtual ~shell_command() {}

	/**
	 * Executes the command
	 * @param
	 */
	virtual void exec(std::list<std::string>* parameters) = 0;

	/**
	 * Sets the command name
	 * @return
	 */
	const std::string& getName() const {
		return name;
	}

	/**
	 * Returns the command name
	 * @param name
	 */
	void setName(const std::string& name) {
		this->name = name;
	}

	friend inline bool operator==(const std::string& lhs, const shell_command& rhs);
	friend inline bool operator==(const shell_command& lhs, const std::string& rhs);
};

/**
 * == Operator overload
 * @param other
 * @return
 */
inline bool operator==(const std::string& lhs, const shell_command& rhs) {
	return rhs.name.compare(lhs) == 0;
}

inline bool operator==(const shell_command& lhs, const std::string& rhs) {
	return lhs.name.compare(rhs) == 0;
}

#endif /* SRC_SHELL_SHELL_COMMAND_H_ */
