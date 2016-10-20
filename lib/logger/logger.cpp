#include <stdexcept>
#include "Logger.h"

using namespace std;

Logger *Logger::pInstance = nullptr;

mutex Logger::sMutex;

Logger &Logger::instance() {
    static Cleanup cleanup;

    lock_guard<mutex> guard(sMutex);
    if(pInstance == nullptr) {
        pInstance = new Logger();
    }
    return *pInstance;
}

Logger::Cleanup::~Cleanup() {
    lock_guard<mutex> guard(Logger::sMutex);
    delete Logger::pInstance;
    Logger::pInstance = nullptr;
}

Logger::~Logger() {
}

Logger::Logger() {
    cout << endl << "New logger created" << endl;
}

void Logger::logHelper(const std::string &inMessage, const std::string &inLogLevel) {
    stream << inLogLevel << ": " << inMessage << endl;
}