/*
 * Copyright (c) 2020 MediaTek Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "gz_api_table.h"
#include "gz_api_types.h"
#include "handler.h"

uint32_t clientID;

#ifndef NULL
#define NULL ((void*)0)
#endif

/* For BSS section */
static int rkp_bss_init_start __attribute__((section(".init.bss.start")));
static int rkp_bss_init_end __attribute__((section(".init.bss.end")));

/* For text section */
extern unsigned char* __bss_start;
extern unsigned char* __bss_end;
#define BSS_INIT_VAL (0x0)
void rkp_bss_init(void)
{
	unsigned char* bss_ptr;
	bss_ptr = (unsigned char*)&rkp_bss_init_start;
	while (bss_ptr != (unsigned char*)&rkp_bss_init_end) {
		*bss_ptr = BSS_INIT_VAL;
		bss_ptr++;
	}
}

static void rkp_keep_data(void)
{
	rkp_bss_init_start = 0xFFFFFFFF;
	rkp_bss_init_end = 0xFFFFFFFF;
}

int rkp_main(void* api_vector, uint32_t api_cnt, uint32_t is_ddr_reserve_mode)
{
	uint32_t ret;
	int rc;
	uint64_t rkp_ns_pa, rkp_ns_va;
	uint32_t rkp_ns_size;


	if (api_cnt < GZ_API_IDX_MAX)
		return (-1);

	initialize_gz_api_table((uint64_t *)api_vector);
	rkp_printf("Hello RKP!\n");
	rkp_printf(" is_ddr_reserve_mode = %u \n", is_ddr_reserve_mode);

	clientID = gz_sm_register(2, "rkp_test");
	rkp_printf("gz_sm_register, clientID = %u \n", clientID);
	ret = gz_eh_hvc_register(clientID, 6, test_hvc);
	rkp_printf("gz_eh_hvc_register ret = 0x%x\n", ret);
	ret = gz_eh_hvc_register(clientID, 9, test_hvc);
	rkp_printf("gz_eh_hvc_register ret = 0x%x\n", ret);

	ret = gz_eh_smc_register(clientID, 49, test_smc);
	rkp_printf("gz_eh_smc_register ret = 0x%x\n", ret);

	ret = gz_eh_smc_register(clientID, 6, test_smc);
	rkp_printf("gz_eh_smc_register ret = 0x%x\n", ret);

	/*negative test*/
	ret = gz_eh_sync_register(clientID, NULL);
	rkp_printf("gz_eh_sync_register ret = 0x%x\n", ret);

	ret = gz_eh_sync_register(clientID, (oem_sync_handler)test_sync);
	rkp_printf("gz_eh_sync_register ret = 0x%x\n", ret);

	/*register HA exception handler*/
	ret = gz_exception_cb_register(clientID, HA_EXCEPTION, (oem_sync_handler)test_sync_HA);
	rkp_printf("gz_exception_cb_register for HA, ret = 0x%x\n", ret);

	/*negative test*/
	ret = gz_exception_cb_register(clientID, HA_EXCEPTION, (oem_sync_handler)test_sync_HA);
	rkp_printf("gz_exception_cb_register for HA, ret = 0x%x\n", ret);

	rkp_printf("gz_dump_el2_map_tbl:\n");
	gz_dump_el2_map_tbl();

	ret = gz_cpuon_tick_register(clientID, CpuOn_cb);
	rkp_printf("register cpu ON handler, ret = 0x%x\n", ret);

	ret = gz_cpususpend_tick_register(clientID, cpususpend_cb);
	rkp_printf("register cpu SUSPEND handler, ret = 0x%x\n", ret);

	ret = gz_cpuresume_tick_register(clientID, cpuresume_cb);
	rkp_printf("register cpu resume handler, ret = 0x%x\n", ret);

	ret = gz_launchlinux_tick_register(clientID, linuxlaunch_cb);
	rkp_printf("register launch linux handler, ret = 0x%x\n", ret);

	rkp_printf("gz_dump_el1_map_tbl:\n");
	gz_dump_el1_map_tbl();

	rkp_printf("gz_heap_dump:\n");
	gz_heap_dump();

	ret = gz_smc_filter_register(clientID, smcfilter_cb);
	rkp_printf("register smc filter, ret = 0x%x\n", ret);

	rc = gz_get_rkp_ns_info(clientID, &rkp_ns_pa, &rkp_ns_va, &rkp_ns_size);
	if(rc)
		rkp_printf("ERROR: gz_get_rkp_ns_info failed, rc = 0x%x\n", rc);
	else
		rkp_printf("gz_get_rkp_ns_info, pa: 0x%llx,  va: 0x%llx, size: 0x%x,\n", rkp_ns_pa, rkp_ns_va, rkp_ns_size);

	rkp_printf("Hello RKP (init done)!\n");
	return 0;
}

int rkp_init(void* api_vector, uint32_t api_cnt, uint32_t is_ddr_reserve_mode)
        __attribute__((section(".text.boot")));
int rkp_init(void* api_vector, uint32_t api_cnt, uint32_t is_ddr_reserve_mode)
{
    /* keep symbol from removing by linker */
    rkp_keep_data();

    /* init bss data */
    rkp_bss_init();

   /* to rkp_main*/
   return rkp_main(api_vector, api_cnt, is_ddr_reserve_mode);
}
