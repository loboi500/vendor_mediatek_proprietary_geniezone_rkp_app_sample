#include "handler.h"

hvc_retval_t test_hvc(uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3, uint64_t x4, uint64_t x5, uint64_t x6, uint64_t x7)
{
	hvc_retval_t ret;
	rkp_printf("[%s]%d:       %llu, %llu, %llu, %llu, %llu, %llu, %llu, %llu \n",__func__,__LINE__, x0,x1,x2,x3,x4,x5,x6,x7);
	ret.x[0] = 5566;
	ret.x[1] = 7788;
	ret.x[2] = 2222;
	ret.x[3] = 3333;

	return ret;
}

hvc_retval_t test_smc(uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3, uint64_t x4, uint64_t x5, uint64_t x6, uint64_t x7)
{
	hvc_retval_t ret;

	rkp_printf("[RKP_TEST][%s]%d: 0x%llx, 0x%llx, 0x%llx, 0x%llx \n",__func__,__LINE__, x0,x1,x2,x3);
	rkp_printf("[RKP_TEST][%s]%d: 0x%llx, 0x%llx, 0x%llx, 0x%llx \n",__func__,__LINE__, x4,x5,x6,x7);

	ret.x[0] = 0x55;
	ret.x[1] = 0x66;
	ret.x[2] = 0x77;
	ret.x[3] = 0x8888;

	return ret;
}

uint32_t test_sync(struct cpu_user_regs *cpu_regs)
{
	rkp_printf("[RKP_TEST]cpu_regs->x1 = %llu \n", cpu_regs->x0);
	return GZ_EXCEPTION_NO_ERROR;
}

uint32_t test_sync_HA(struct cpu_user_regs *cpu_regs)
{
	rkp_printf("[RKP_TEST][HA abort handler~~~]cpu_regs->x1 = %llu \n", cpu_regs->x0);
	return GZ_EXCEPTION_NO_ERROR;
}

void CpuOn_cb(void)
{rkp_printf("[RKP_TEST] CPU ON!! \n");}


void linuxlaunch_cb(void)
{rkp_printf("[RKP_TEST] L!I!N!U!X!! \n");}

//void systemresume_cb(void)
//{rkp_printf("[RKP_TEST] system wakeup!! \n");}

void cpuresume_cb(void)
{
	//rkp_printf("===>RKP2: cpu wakeup!! \n");
}

void cpususpend_cb(void)
{
	//rkp_printf("====>RKP2: cpu sleep!! \n");
}

int smcfilter_cb(uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3, uint64_t x4, uint64_t x5, uint64_t x6, uint64_t x7)
{
	//rkp_printf("====>RKP_TEST: smc filter: 0x%llx 0x%llx 0x%llx 0x%llx 0x%llx 0x%llx 0x%llx 0x%llx\n ", x0, x1,x2,x3,x4,x5,x6,x7);
	UNUSED(x0);
	UNUSED(x1);
	UNUSED(x2);
	UNUSED(x3);
	UNUSED(x4);
	UNUSED(x5);
	UNUSED(x6);
	UNUSED(x7);
	return 0;
}
