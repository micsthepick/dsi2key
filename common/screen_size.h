#pragma once

#include <stdint.h>

#if defined(_NDS)
const uint16_t SCREEN_HEIGHT_ = 192;
const uint16_t SCREEN_WIDTH_  = 256;
#define SCREEN_BYTES 2
#elif defined(__3DS__)
const uint16_t SCREEN_WIDTH_  = 320;
const uint16_t SCREEN_HEIGHT_ = 240;
#define SCREEN_BYTES 3
#endif

