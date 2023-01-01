#ifndef _HANDLER_H
#define _HANDLER_H
#include "gz_api_types.h"

#define UNUSED(x) (void)(x)

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned long uintptr_t;

hvc_retval_t test_hvc(uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3, uint64_t x4, uint64_t x5, uint64_t x6, uint64_t x7);

hvc_retval_t test_smc(uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3, uint64_t x4, uint64_t x5, uint64_t x6, uint64_t x7);

uint32_t test_sync(struct cpu_user_regs *cpu_regs);

uint32_t test_sync_HA(struct cpu_user_regs *cpu_regs);

void CpuOn_cb(void);

void linuxlaunch_cb(void);

void cpuresume_cb(void);

void cpususpend_cb(void);

int smcfilter_cb(uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3, uint64_t x4, uint64_t x5, uint64_t x6, uint64_t x7);
#endif

