/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2006-2010 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  �嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *  �����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *  �ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ������Ȥ�
 *      ���դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 * 
 *  @(#) $Id: mpcore.c 553 2010-03-16 02:51:56Z ertl-honda $
 */

#include "kernel_impl.h"

/*
 *  DIC CPU���󥿥ե�������Ϣ
 */

/*
 *  CPU���󥿥ե������ν����
 */
void
dic_cpu_init(void)
{
	/* CPU���󥿥ե�������̵���� */
	sil_wrw_mem((void *)DIC_CPUIF_CR, 0);

	/* ����ͥ���٤����� */
	dic_cpu_set_priority(DIC_MIN_PRIORITY);

	/* �����ͥ���٤����ӥå�ͭ���� */
	dic_cpu_set_bp(DIC_CPUIF_BPR_ALL);

	/* �ڥ�ǥ��󥰤��Ƥ����ǽ��������Τǡ�EOI �ˤ�ꥯ�ꥢ */
	sil_wrw_mem((void *)DIC_CPUIF_EOI,
				sil_rew_mem((void *)DIC_CPUIF_IAR));
#ifdef NONTRUST
	/* CPU���󥿥ե�������ͭ���� */
	sil_wrw_mem((void *)DIC_CPUIF_CR, DIC_CPUIF_CR_EN);
#else

#ifdef TOPPERS_WITH_SAFEG
	/* CPU���󥿥ե�������ͭ���� */
	sil_wrw_mem((void *)DIC_CPUIF_CR, (DIC_CPUIF_CR_EN_S|DIC_CPUIF_CR_EN_NS|DIC_CPUIF_CR_FIQEN) );
#else
	/* CPU���󥿥ե�������ͭ���� */
	sil_wrw_mem((void *)DIC_CPUIF_CR, DIC_CPUIF_CR_EN);
#endif /* TOPPERS_WITH_SAFEG */

#endif /* NONTRUST */
}

/*
 *  CPU���󥿥ե������ν�λ
 */
void
dic_cpu_stop(void)
{
	/* CPU���󥿥ե�������̵���� */
	sil_wrw_mem((void *)(DIC_CPUIF_CR), 0);
}


/*
 * DIC DIS���󥿥ե�������Ϣ
 */

/*
 *  ����߶ػ�
 */
void
dic_disable_int(uint8_t id)
{
	uint16_t offset_addr;
	uint16_t offset_bit;

	offset_addr = (id / 32) * 4;
	offset_bit  = id % 32;

	sil_wrw_mem((void *)(DIC_CE + offset_addr), (1 << offset_bit));
}

/*
 *  ����ߵ���
 */
void
dic_enable_int(uint8_t id)
{
	uint16_t offset_addr;
	uint16_t offset_bit;

	offset_addr = (id / 32) * 4;
	offset_bit  = id % 32;

	sil_wrw_mem((void *)(DIC_SE + offset_addr), (1 << offset_bit));
}

/*
 * ����ߥڥ�ǥ��󥰥��ꥢ
 */
void
dic_clear_pending(uint8_t id)
{
	uint16_t offset_addr;
	uint16_t offset_bit;

	offset_addr = (id / 32) * 4;
	offset_bit  = id % 32;

	sil_wrw_mem((void *)(DIC_CP + offset_addr), (1 << offset_bit));
}

/*
 *  ����ߥڥ�ǥ��󥰥��å�
 */
void
dic_set_pending(uint8_t id){
	uint16_t offset_addr;
	uint16_t offset_bit;

	offset_addr = (id / 32) * 4;
	offset_bit  = id % 32;

	sil_wrw_mem((void *)(DIC_SP + offset_addr), (1 << offset_bit));
}

/*
 *  ������׵�Υ����å�
 */
bool_t
dic_probe_int(uint8_t id){
	uint32_t state;
	uint16_t offset_addr;
	uint16_t offset_bit;

	offset_addr = (id / 32) * 4;
	offset_bit  = id % 32;

	state = sil_rew_mem((void *)(DIC_SP + offset_addr));

	if ((state & (1 << offset_bit)) == (1 << offset_bit)) {
		return(true);
	}
	else {
		return(false);
	}
}

/*
 *  ����ߥ���ե�����졼���������
 */
void
dic_config(uint8_t id, uint8_t config){
	uint16_t offset_addr;
	uint16_t offset_bit;
	uint32_t cfgr_reg_val;
	SIL_PRE_LOC;

	offset_addr = (id / 16) * 4;
	offset_bit  = (id % 16) * 2;

	SIL_LOC_SPN();
	cfgr_reg_val  = sil_rew_mem((void *)(DIC_ICR + offset_addr));
	cfgr_reg_val &= ~(0x03U << offset_bit);
	cfgr_reg_val |= (0x03U & config) << offset_bit;
	sil_wrw_mem((void *)(DIC_ICR + offset_addr), cfgr_reg_val);
	SIL_UNL_SPN();
}

/*
 *  �����ͥ���٤Υ��å�
 *  ����ɽ�����Ϥ���
 */
void
dic_set_priority(uint8_t id, int pri){
	uint16_t offset_addr;
	uint16_t offset_bit;
	uint32_t pr_reg_val;
	SIL_PRE_LOC;

	offset_addr = (id / 4) * 4;
	offset_bit  = ((id % 4) * 8) + IPM_BIT_OFFSET;

	SIL_LOC_SPN();
	pr_reg_val  = sil_rew_mem((void *)(DIC_IPR + offset_addr));
	pr_reg_val &= ~(IPM_BIT_MASK << offset_bit);
	pr_reg_val |= (IPM_BIT_MASK & pri) << (offset_bit); 
	sil_wrw_mem((void *)(DIC_IPR + offset_addr), pr_reg_val);
	SIL_UNL_SPN();
}

/*
 *  DIC����ߥ������åȤ�����
 *  cpus�ϥ������åȤȤ���CPU�Υӥåȥѥ�����ǻ���
 *   CPU0 : 0x01
 *   CPU1 : 0x02
 *   CPU2 : 0x04
 *   CPU3 : 0x08
 */
void
dic_set_target(uint8_t id, uint8_t cpus)
{
	uint32_t offset_addr;
	uint32_t offset_bit;
	uint32_t itr_reg_val;
	SIL_PRE_LOC;

	offset_addr = (id / 4) * 4;
	offset_bit  = (id % 4) * 8;

	SIL_LOC_SPN();
	itr_reg_val  = sil_rew_mem((void *)(DIC_ITR + offset_addr));
	itr_reg_val &= ~(0xf << offset_bit);
	itr_reg_val |= (cpus << offset_bit);
	sil_wrw_mem((void *)(DIC_ITR + offset_addr), itr_reg_val);
	SIL_UNL_SPN();
}

/*
 * GIC�γ���ߥ������ƥ�����
 * ������ֹ�Ȥ��Υ������ƥ��ǻ���
 * ns(non secure bit) : 
 *     0 -> secure(FIQ) setting
 *     1 -> non-secure(IRQ) setting
 */
void dic_set_security(int intno, unsigned int ns){
    unsigned int reg = DIC_SEC + ((intno/32)*4);
    *((volatile unsigned int *)reg) |= (ns << (intno%32));
}


/*
 *  DIC���󥿥ե������ν����
 */
void
dic_init(void)
{
	int i;
	unsigned int add;

	/* DIC���󥿥ե�������̵���� */
	sil_wrw_mem((void *)(DIC_CR), 0);

	// all interrupts are secure setting
	for(add=0x1E001080; add<=0x1E0010FC; add+=4){
		*((volatile unsigned int *)add) = 0x0; //0xFFFFFFFF;
	}

#if __TARGET_ARCH_ARM == 7
#ifndef NONTRUST
    /* ����ߤΥ������ƥ����� */
    dic_set_security(54,1);
    dic_set_security(57,1);    
    dic_set_security(58,1);    
    dic_set_security(66,1);    
    dic_set_security(67,1);    
    dic_set_security(76,1);    
    dic_set_security(79,1);
	//non-trust timer (Test Chip��¢������SP804_0) non-secure setting(for Linux)
    dic_set_security(80,1);    
	//non-trust timer (Test Chip��¢������SP804_1) non-secure setting
    dic_set_security(81,1);    

	//non-trust UART non-secure setting
    dic_set_security(84,1);        

#endif
#endif /* __TARGET_ARCH_ARM == 7 */

	/* ����ߤ����ƶػ� */
	for(i = 0; i < DIC_TNUM_INT/32; i++){
		sil_wrw_mem((void *)(DIC_CE + (4 * i)), 0xffffffff);
	}

	/* �ڥ�ǥ��󥰤򥯥ꥢ */
	for(i = 0; i < DIC_TNUM_INT/32; i++){
		sil_wrw_mem((void *)(DIC_CP + (4 * i)), 0xffffffff);
	}

	/* ͥ���ٺ��������  */
	for(i = 0; i < DIC_TNUM_INT/4; i++){
		sil_wrw_mem((void *)(DIC_IPR + (4 * i)), 0xffffffff);
	}

	/* �������åȽ����������CPU0�ء� */
	for(i = TMIN_GLOBAL_INTNO/4; i < DIC_TNUM_INT/4; i++){
//	for(i = 0; i < DIC_TNUM_INT/4; i++){
		sil_wrw_mem((void *)(DIC_ITR + (4 * i)), 0x01010101);
	}

	/* �⡼�ɽ����(1-N Level) */
	for(i = 0; i < DIC_TNUM_INT/16; i++){
		sil_wrw_mem((void *)(DIC_ICR + (4 * i)), 0x55555555);
	}
#ifndef NONTRUST
	/* DIC���󥿥ե�������ͭ���� */
	sil_wrw_mem((void *)(DIC_CR), (DIC_CR_ENABLE|DIC_CR_NS_ENABLE));
#else
	/* DIC���󥿥ե�������ͭ���� */
	sil_wrw_mem((void *)(DIC_CR), DIC_CR_ENABLE);
#endif
}


/*
 * (trust only)
 * ��ǽ��(�ޥ����ץ��å��ʳ��Ǥ�¹Ԥ���)����ߥ������ƥ��ν����
 * SGI�γ�����ֹ�Υ������ƥ���������ץ��å��ǹԤ�ɬ�פ����롣
 */
void
dic_sgi_sec_init(void)
{
	/* DIC���󥿥ե�������̵���� */
	sil_wrw_mem((void *)(DIC_CR), 0);

    /* all SGI non-secure setting */
    //*((volatile unsigned int *)0x1E001080) = 0xFFFF;
    /* all PPI non-secure setting */
    //*((volatile unsigned int *)0x1E001080) |= 0xFFFF0000;

	/*
     * SGI non-secure setting 
	 * �ȥ饹��¦�Ǥϳ�����ֹ�2��3��ץ��å��ֳ�����ֹ�Ȥ��ƻȤ����Ȥˤ���
     * ( kzm_ca9����Linux(SMP)�Ǥϡ���������ꥳ���Υ֡��Ȥ�
     *   swi(�������ֹ�1)����Ѥ��뤿�� )
     */
    dic_set_security(0,1);    
    dic_set_security(1,1);    
    //dic_set_security(2,1);    
    //dic_set_security(3,1);    

	/* PPI non-secure setting */
    dic_set_security(29,1);        

	/* DIC���󥿥ե�������ͭ���� */
	sil_wrw_mem((void *)(DIC_CR), (DIC_CR_ENABLE|DIC_CR_NS_ENABLE));
}


/*
 *  DIC���󥿥ե������ν�λ
 */
void
dic_stop(void)
{
	/* DIC���󥿥ե�������̵���� */
	sil_wrw_mem((void *)(DIC_CR), 0);
}

/*
 *
 * SCU��Ϣ�δؿ�
 *
 */


#ifdef NONTRUST
#include "safeg_syscalls.h"
#endif


/*
 *  CP15��AUXILIARY�쥸������SA�ӥåȤ򥻥åȤ���SMP�⡼�ɤˤ���
 */
Inline void
enable_sa(void){
	uint32_t bits;

	CP15_AUXILIARY_READ(bits);
	bits |= CP15_AUXILIARY_SA_BIT;
#ifndef NONTRUST
	CP15_AUXILIARY_WRITE(bits);
#endif

//#ifdef NONTRUST
//	safeg_switch_to_t(T_SMC_SWITCH_RET, 0);
//#endif
}

Inline void
scu_enable(void){
	uint32_t scu_ctrl;

	/* SCU�� tag RAMS ��̵���� */
	sil_wrw_mem((void*)MPCORE_SCU_IALL, MPCORE_SCU_IALL_WAYS);

	/* SCU��ͭ���� */
	scu_ctrl  = sil_rew_mem((void *)MPCORE_SCU_CTRL);
	scu_ctrl |= MPCORE_SCU_CTRL_EN;
	sil_wrw_mem((void *)MPCORE_SCU_CTRL, scu_ctrl);
}

/*
 *  SCU�ν���������ʥޥ����ץ��å��Τ߼¹ԡ�
 */
void
scu_init(void){
	/* SCU ��ͭ���� */
	scu_enable();

	/* ���ƤΥ����Υ⡼�ɤ�NORMAL�Ȥ��� */
	sil_wrw_mem((void *)MPCORE_SCU_CPUST, 0x00U);
	
#ifndef NONTRUST
	/* �Υ�ȥ饹��¦�����A9��¢�����ޤؤΥ������������ */
	sil_wrw_mem((void *)MPCORE_SCU_SNSAC, 0xFFF); //��CPU�����
	//sil_wrw_mem((void *)MPCORE_SCU_SNSAC, 0x111);  //CPU0�Τߵ���
#endif
}


/*
 *  SMP�⡼�ɤ����ꤹ��
 */
void
mpcore_smp_mode_enable(void){
	uint32_t sr;

	/* ������߶ػ� */
	sr = current_sr();
	set_sr(sr|CPSR_IRQ_BIT|CPSR_FIQ_BIT);
	//set_sr(sr|CPSR_CPULOCK);
	
	/* ���㥷���̵�� */
	mpcore_dcache_clean_and_invalidate();
	mpcore_icache_invalidate();

	/* Data Synchronization Barrier */
	mpcore_data_sync_barrier();

	/* TLB�ν���� */
	mpcore_invalidate_unfied_tlb();

	/* CP15��SMP/nAMP bit �򥻥åȤ��� */
	enable_sa();

	/* ����߶ػ߾��֤򸵤��᤹ */
	set_sr(sr);
}

/*
 *  D����å���򳫻�
 */
void
mpcore_dcache_enable(void)
{
	uint32_t bits;

	CP15_CONTROL_READ(bits);

	/* ���Ǥ�ON�ʤ�꥿���� */
	if (bits & CP15_CONTROL_C_BIT){
		return;
	}

	mpcore_dcache_invalidate();

	bits |= CP15_CONTROL_C_BIT;
	CP15_CONTROL_WRITE(bits);
}

/*
 *  D����å������ߤ���̵���Ȥ��롥
 *  CA9�Ǥϡ�D����å��夬̵���ʾ��֤�Clean and Invalidate()��¹Ԥ���ȡ�
 *  ˽�����뤿�ᡤD����å���ξ��֤�Ƚ�Ǥ��ơ�̵���ʾ��ϡ�Invalidate
 *  �Τߤ�Ԥ��� 
 */
void
mpcore_dcache_disable(void)
{
	uint32_t bits;

	CP15_CONTROL_READ(bits);
	if( bits & CP15_CONTROL_C_BIT ){
		bits &= ~CP15_CONTROL_C_BIT;
		CP15_CONTROL_WRITE(bits);
		mpcore_dcache_clean_and_invalidate();
	}
	else{
		mpcore_dcache_invalidate();
	}
}

/*
 * I����å���γ���
 */
void mpcore_icache_enable (void)
{
	uint32_t bits;

	CP15_CONTROL_READ(bits);

	/*
	 *  ���Ǥ�ͭ���ʤ�꥿����
	 */
	if(bits & CP15_CONTROL_I_BIT){
		return;
	}

	mpcore_icache_invalidate();

	bits |= CP15_CONTROL_I_BIT;
	CP15_CONTROL_WRITE(bits);
}

/*
 *  I����å�������
 */
void
mpcore_icache_disable (void)
{
	uint32_t bits;

	CP15_CONTROL_READ(bits);
	bits &= ~CP15_CONTROL_I_BIT;
	CP15_CONTROL_WRITE(bits);

	mpcore_icache_invalidate();
}

/*
 *  I/D����å����ξ����ͭ����
 */
void
mpcore_cache_enable(void)
{
	mpcore_dcache_enable();
	mpcore_icache_enable();
}

/*
 *  I/D����å����ξ����̵����
 */
void
mpcore_cache_disable(void)
{
	mpcore_dcache_disable();
	mpcore_icache_disable();
}

/*
 *  MMU��Ϣ�Υɥ饤��
 */
#define DOMAIN_NO  1
#define PAGE_TABLE_SIZE  0x00004000 /* 4KB */

/*
 * �ڡ����ơ��֥�
 */
static uint8_t PageTable[TNUM_PRCID][PAGE_TABLE_SIZE]  __attribute__ ((aligned (0x4000)));

#define my_PageTable (PageTable[x_prc_index()])

/*
 * �ڡ����ơ��֥�Υ������������ꤹ��
 */
static void
mmu_set_section(uintptr_t va, uintptr_t pa,
                uint8_t s, uint8_t tex, uint8_t ap,
                uint8_t c, uint8_t b)
{
	uint32_t ttb;
	uint32_t *sptr;

	/*
	 * �ڡ����ơ��֥�Υ١������ɥ쥹�����
	 */
	CP15_TTB0_READ(ttb);

	/*
	 * VA���饻�������Υ��ɥ쥹�����
	 */
	sptr  = (uint32_t *)((ttb & 0xFFFFC000) | (((va & 0xFFF00000) >> 20) << 2));
	*sptr = pa | (s << 16) | (tex << 12) | (ap << 10) | (c << 3) | (b << 2) | (1 << 1);
	
}

#define SIZE_1M    0x00100000

/*
 * ����˽���,����Υޥåԥ󥰤�Ԥ�
 * �ޥåԥ󥰤�ñ�̤�1MB 
 */
void
mmu_map_memory(MEMORY_ATTRIBUTE *m_attribute)
{
	uint32_t size;
	uintptr_t va;
	uintptr_t pa;

	size = m_attribute->size;
	va   = m_attribute->va;
	pa   = m_attribute->pa;

	/* MBñ�̤ˤʤ�褦�˥����������� */
	size = (size + 0x000FFFFF) & ~0x000FFFFF;

	while(size > 0) {
		mmu_set_section(va, pa,
						m_attribute->s,
						m_attribute->tex,
						m_attribute->ap,
						m_attribute->c,
						m_attribute->b);

		va   += SIZE_1M;
		pa   += SIZE_1M;
		size -= SIZE_1M;
	}

	
}




/*
 *  MMU�ν����
 */
void
mpcore_mmu_init(void){
	MEMORY_ATTRIBUTE m_attribute;

	uint32_t bits = 0;

	/* �ץ�ե��å��Хåե��򥯥ꥢ */
	mpcore_pbuffer_flash();
	
	/*
	 * TTBR0���Ѥ����Ѥ˻���
	 */
	CP15_TTBCR_WRITE(0);


	/*
	 * �Ѵ��ơ��֥�(TT)�Ȥ��ơ�PageTable����Ѥ���
	 * Shared�ӥåȤ򥻥å� 
	 */
#if __TARGET_ARCH_ARM == 6
	CP15_TTB0_WRITE((((uint32_t)my_PageTable)|CP15_TTB0_RGN_S|CP15_TTB0_RGN_WBWA));
#else /* __TARGET_ARCH_ARM == 7 */
	CP15_TTB0_WRITE((((uint32_t)my_PageTable)|CP15_TTB0_RGN_S|CP15_TTB0_RGN_WBWA|CP15_TTB0_IRGN_WBWA));
#endif /* __TARGET_ARCH_ARM == 7 */

	/* �ץ�ե��å��Хåե��򥯥ꥢ */
	mpcore_pbuffer_flash();


	/*
	 * ������������ʪ�����ɥ쥹 = ���ۥ��ɥ쥹
	 * �Υ󥭥�å���֥롤�Υ�Хåե��֥�Ȥ��� 
	 */
	m_attribute.pa   = 0x00000000;
	m_attribute.va   = m_attribute.pa;
	m_attribute.size = 0x80000000;
	m_attribute.s    = 1;          /* ��ͭ         */
	m_attribute.ap   = 3;          /* �ե륢������ */
	m_attribute.tex  = 0;          /* Strongly Ordered */
	m_attribute.c    = 0;
	m_attribute.b    = 0;


	mmu_map_memory (&m_attribute);


	m_attribute.pa   = 0x80000000;
	m_attribute.va   = m_attribute.pa;

	mmu_map_memory (&m_attribute);

	/*
	 *  �������åȰ�¸���Ǥ�MMU�ν����
	 */
	target_mmu_init();


	/*
	 * �ɥᥤ���ֹ�򥻥å�
	 */
	CP15_DOMAINS_WRITE(DOMAIN_NO);

	/*
	 * CONTROL���ץ��å��� M�ӥåȡ�XP�ӥåȤ򥻥åȤ��ơ�
	 * MMU��ͭ���ˤ���
	 */
	CP15_CONTROL_READ(bits);
	bits |= CP15_CONTROL_M_BIT | CP15_CONTROL_XP_BIT;
	CP15_CONTROL_WRITE(bits);
}

#if __TARGET_ARCH_ARM == 6
/*
 * SCU�����󥿤ν������MN0����ѡ�
 */
void
scu_counter_init(void)
{
	/*
	 *  MN0�� Cycle Count ������
	 */
	sil_wrw_mem((void *)MPCORE_SCU_PM_EVT0,
				sil_rew_mem((void *)MPCORE_SCU_PM_EVT0) | MPCORE_SCU_PM_EVT_CYC_COUNT0_4);

	/*
	 *  �ꥻ�åȤȥ�������
	 */
	sil_wrw_mem((void *)MPCORE_SCU_PM_CTRL,
			sil_rew_mem((void *)MPCORE_SCU_PM_CTRL) | MPCORE_SCU_PM_CTRL_RST | MPCORE_SCU_PM_CTRL_ENA);
}
#endif /* __TARGET_ARCH_ARM == 6 */
