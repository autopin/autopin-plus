
#include "spdlog/spdlog.h"


int main(int, char* [])
{
    int howmany = 1000000;
    namespace spd = spdlog;
    ///Create a file rotating logger with 5mb size max and 3 rotated files
    auto logger = spd::rotating_logger_st("file_logger", "logs/spd-sample", 10 *1024 * 1024 , 5);

    logger->set_pattern("[%Y-%b-%d %T.%e]: %v");
    for(int i  = 0 ; i < howmany; ++i)
        logger->info() << "spdlog message #" << i << ": This is some text for your pleasure";
    return 0;
}
