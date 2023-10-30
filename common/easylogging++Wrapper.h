#pragma once

#define D2K_DUMP_CONFIGS
//#define ELPP_DISABLE_ERROR_LOGS
//#define ELPP_DISABLE_FATAL_LOGS
#include  <iostream>
#if defined(_NDS)
#define ELPP_DISABLE_DEFAULT_CRASH_HANDLING
#define ELPP_DISABLE_ASSERT
#define ELPP_DISABLE_PERFORMANCE_TRACKING
#define ELPP_DISABLE_CUSTOM_FORMAT_SPECIFIERS
#define ELPP_DISABLE_LOG_FILE_FROM_ARG
#define ELPP_DISABLE_LOGGING_FLAGS_FROM_ARG
#endif
#define ELPP_DEFAULT_LOG_FILE "ds2key/ds2key.log"
#define ELPP_FEATURE_CRASH_LOG

#include "easylogging++.h"

namespace D2K {

void InitLogging(int argc, char* argv[]);
void DeInitLogging();

} // namespace D2K
