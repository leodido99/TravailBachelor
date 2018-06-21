/**
 * @file logger.h
 * @brief A way to log to the console
 *
 * This class provides a way to easily log to the console
 * any events happening during the execution. The user can
 * specify severity for the log message and select up to which
 * severity the message should be displayed
 * Found at https://stackoverflow.com/questions/6168107/how-to-implement-a-good-debug-logging-feature-in-a-project
 *
 *
 * @author Robert S. Barnes
 * @date   May 29, 2011
 */

#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <iostream>
#include <sstream>

/* consider adding boost thread id since we'll want to know whose writting and
 * won't want to repeat it for every single call */

/* consider adding policy class to allow users to redirect logging to specific
 * files via the command line
 */

enum loglevel_e
    {logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4};

class logIt
{
public:
    logIt(loglevel_e _loglevel = logERROR) {
        _buffer << _loglevel << " :"
            << std::string(
                _loglevel > logDEBUG
                ? (_loglevel - logDEBUG) * 4
                : 1
                , ' ');
    }

    template <typename T>
    logIt & operator<<(T const & value)
    {
        _buffer << value;
        return *this;
    }

    ~logIt()
    {
        _buffer << std::endl;
        // This is atomic according to the POSIX standard
        // http://www.gnu.org/s/libc/manual/html_node/Streams-and-Threads.html
        std::cerr << _buffer.str();
    }

private:
    std::ostringstream _buffer;
};

extern loglevel_e loglevel;

#define log(level) \
if (level > loglevel) ; \
else logIt(level)

#endif
