#pragma once
#include <fstream>
#include <ctime>

/**
 * @brief dont watch this :)
 * even i don't know WHAT it is
 * 
 */
struct Logger {
private:
    std::ofstream logger;

public:
    Logger() {
        if (!logger.is_open()) {
            std::time_t currentSeconds = time(NULL);
            std::tm* date = localtime(&currentSeconds);
            char* stringDate;
            strftime(stringDate, 8, "%x%X", date);
            std::cout << stringDate << endl;

            logger.open(stringDate, std::ios::ate);
        }
    }

    Logger(const Logger original) {
        this->logger = original.logger;
    }

    ~Logger() {
        if (logger.is_open())
            logger.close();
    }

    Logger& operator = (const Logger& source) {
        if (this == &source)
            return *this;

        if (this->logger.is_open()) {
            this->logger.close();
        }

        this->logger = source.logger;
        
        return *this;
    }
};