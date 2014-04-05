#ifndef __COMMON_H
#define __COMMON_H

#include "stm32f0xx_it.h"
#define SystemCoreClock  8000000
typedef enum {FALSE = 0, TRUE = !FALSE} bool;
#define u32 uint32_t
#define s32 int32_t
#define u16 uint16_t
#define s16 int16_t
#define u8 uint8_t
#define s8 int8_t

#endif