#include "easylogging++Wrapper.h"

namespace D2K {

extern bool canLogFile;

void InitLogging(int argc, char *argv[])
{
#ifdef _NDS
    std::string LOGGING_CONFIG = \
"TO_FILE=true \n \
FILENAME=\"ds2key/dsi2key.log\"\n \
* GLOBAL:\n \
FORMAT=\"%msg\"\n \
ENABLED=true\n \
TO_STANDARD_OUTPUT=true\n \
PERFORMANCE_TRACKING=false\n \
MAX_LOG_FILE_SIZE=12800##100KB\n \
LOG_FLUSH_THRESHOLD=1";
#else
	std::string LOGGING_CONFIG = \
"TO_FILE=true \n \
* GLOBAL:\n \
FORMAT=\"[%datetime{%H:%m:%s}] %msg\"\n \
FILENAME=\"ds2key/dsi2key.log\"\n \
ENABLED=true\n \
TO_STANDARD_OUTPUT=true\n \
MILLISECONDS_WIDTH=6\n \
PERFORMANCE_TRACKING=false\n \
MAX_LOG_FILE_SIZE=2097152##2MB\n \
LOG_FLUSH_THRESHOLD=100\n \
* INFO:\n \
FORMAT=\"%msg\"\n \
* FATAL:\n \
FORMAT=\"[%datetime{%H:%m:%s}]%levshort:%msg\"\n \
* ERROR:\n \
FORMAT=\"[%datetime{%H:%m:%s}]%levshort:%msg\"\n \
* TRACE:\n \
FORMAT=\"[%datetime{%H:%m:%s}]%levshort:%msg\"\n \
* DEBUG:\n \
FORMAT=\"[%datetime{%H:%m:%s}]%levshort:%msg\"\n";
#endif
	START_EASYLOGGINGPP(argc, argv);
	el::Configurations c;
	if (!canLogFile) {
		LOGGING_CONFIG[8] = 'f';
		LOGGING_CONFIG[9] = 'a';
		LOGGING_CONFIG[10] = 'l';
		LOGGING_CONFIG[11] = 's';
		LOGGING_CONFIG[12] = 'e';
	}
	c.parseFromText(LOGGING_CONFIG.c_str());
	el::Loggers::reconfigureLogger("default", c);
	el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
}

void DeInitLogging()
{
	el::Loggers::flushAll();
}

} // namespace D2K
