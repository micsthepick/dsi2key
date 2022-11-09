#pragma once

//#define D2K_DUMP_CONFIGS
#define ELPP_DISABLE_DEBUG_LOGS
#define ELPP_DISABLE_VERBOSE_LOGS
#define ELPP_DISABLE_TRACE_LOGS
//ELPP_DISABLE_INFO_LOGS
//ELPP_DISABLE_WARNING_LOGS
//ELPP_DISABLE_ERROR_LOGS
//ELPP_DISABLE_FATAL_LOGS
#if 1
#include  <iostream>
#define TRACE
#define DEBUG
#define ERROR
//#define FATAL //TODO: conflicting definition for vjoy's public.h
#define LOG(a) std::cout
#define LOG_EVERY_N(a,b) std::cout
#define LOG_N_TIMES(a,b) std::cout
#define INITIALIZE_EASYLOGGINGPP
#else
#include "common/easylogging++.h"
#endif

namespace D2K {

void InitLogging(int argc, char* argv[]);
void DeInitLogging();

} // namespace D2K
