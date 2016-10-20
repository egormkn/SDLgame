#ifndef LOGGER_INCLUDED
#define LOGGER_INCLUDED

// Logger class from Professional C++ programming
// Modified by EgorMKN

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <mutex>

// Definition of a thread-safe singleton logger class
class Logger {
public:
    enum Level {
        DEBUG,
        INFO,
        WARNING
    };

    // Returns a reference to the singleton Logger object
    static Logger &instance();

    template <typename T>
    friend Logger& operator<<(Logger& logger, const T& value) {
        std::lock_guard<std::mutex> guard(sMutex);
        std::string l;
        switch (logger.level) {
            case Level::DEBUG:
                l = "DEBUG";
                break;
            case Level::INFO:
                l = "INFO";
                break;
            default:
                l = "DEBUG";
        }
        logger.logHelper(value, l);
        return logger;
    }

    friend Logger& operator<<(Logger& logger, Level level) {
        std::lock_guard<std::mutex> guard(sMutex);
        logger.level = level;
        return logger;
    }

protected:
    // Static variable for the one-and-only instance
    static Logger *pInstance;

    Level level = Level::DEBUG;

    // Data member for the output stream
    std::ostream& stream = std::cout;

// Embedded class to make sure the single Logger
// instance gets deleted on program shutdown.
    friend class Cleanup;

    class Cleanup {
    public:
        ~Cleanup();
    };

    // Logs message. The thread should own a lock on sMutex
    // before calling this function.
    void logHelper(const std::string &inMessage,
                   const std::string &inLogLevel);

private:
    Logger();

    virtual ~Logger();

    Logger(const Logger &);

    Logger &operator=(const Logger &);

    static std::mutex sMutex;
};
#endif //LOGGER_INCLUDED