#ifndef _SAFEG_SYSCALLS_H_
#define _SAFEG_SYSCALLS_H_

/*
 * SAFEG System Monitor Calls 
 *
 * T_: Trust calls 
 * NT_: Non-Trust calls
 *
 * template: safeg_syscall(int num, int *arg0, int *arg1)
 *
 * indentation:
 *      num
 *          arg0
 *              arg1
 */
#define T_SMC_SWITCH  0 /* switch to NT */
#define     T_SMC_SWITCH_OK      0 /* no error */
#define     T_SMC_SWITCH_ERR     1 /* error, return to T */
#define     T_SMC_SWITCH_HANDLER 2 /* NT returns, raise safeg handler*/
#define     T_SMC_SWITCH_RET     3 /* NT returns */
#define     T_SMC_SWITCH_SHVAR   4 /* NT returns, arg1 = shared variable */

#define T_SMC_REBOOT  1 /* reboot NT */
#define     T_SMC_REBOOT_OK      0 /* no error */
#define     T_SMC_REBOOT_ERR     1 /* error, return to T */

#define NT_SMC_SWITCH 0 /* switch to T */
#define     NT_SMC_SWITCH_OK     0 /* no error */
#define     NT_SMC_SWITCH_ERR    1 /* error, return to NT */

#define NT_SMC_INV_CACHE 1 /* invalidate cache and return to NT */
#define     NT_SMC_INV_CACHE_OK   0 /* no error */
#define     NT_SMC_INV_CACHE_ERR  1 /* error, return to T */
#define     NT_SMC_INV_CACHE_D    2 /* invalidate data cache */
#define     NT_SMC_INV_CACHE_I    3 /* invalidate instruction cache */
#define     NT_SMC_INV_CACHE_ALL  4 /* invalidate both caches */

#ifndef __ASSEMBLY__

static inline
void safeg_switch_to_t(int arg0, int arg1)
{
    Asm("mov r0, %0\n\t"
        "mov r1, %1\n\t"
        "mov r2, %2\n\t"
        "smc 0\n\t"
        : /* no output */
        : "I" (NT_SMC_SWITCH), "r" (arg0), "r" (arg1)
        : "r0", "r1", "r2", "memory");
}

#endif /* __ASSEMBLY__ */

#endif /* _SAFEG_SYSCALLS_H_ */
