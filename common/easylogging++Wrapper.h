#pragma once

//#define D2K_DUMP_CONFIGS
//#define ELPP_DISABLE_DEBUG_LOGS
#define ELPP_DISABLE_VERBOSE_LOGS
#define ELPP_DISABLE_TRACE_LOGS
//ELPP_DISABLE_INFO_LOGS
//ELPP_DISABLE_WARNING_LOGS
//ELPP_DISABLE_ERROR_LOGS
//ELPP_DISABLE_FATAL_LOGS
#include  <iostream>
#define TRACE
#define DEBUG
#define ERROR
#define FATAL
#define ELPP_DEFAULT_LOG_FILE "ds2key/ds2key.log"
#if defined(_NDS) || defined(__3DS__)
#define ELPP_DISABLE_DEFAULT_CRASH_HANDLING
#define ELPP_DISABLE_ASSERT
#endif
#include "easylogging++.h"

namespace D2K {

void InitLogging(int argc, char* argv[]);
void DeInitLogging();

} // namespace D2K
