/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2006-2010 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  @(#) $Id: mpcore.c 553 2010-03-16 02:51:56Z ertl-honda $
 */

#include "kernel_impl.h"

/*
 *  DIC CPUインタフェース関連
 */

/*
 *  CPUインタフェースの初期化
 */
void
dic_cpu_init(void)
{
	/* CPUインタフェースを無効に */
	sil_wrw_mem((void *)DIC_CPUIF_CR, 0);

	/* 最低優先度に設定 */
	dic_cpu_set_priority(DIC_MIN_PRIORITY);

	/* 割込み優先度の全ビット有効に */
	dic_cpu_set_bp(DIC_CPUIF_BPR_ALL);

	/* ペンディングしている可能性があるので，EOI によりクリア */
	sil_wrw_mem((void *)DIC_CPUIF_EOI,
				sil_rew_mem((void *)DIC_CPUIF_IAR));
#ifdef NONTRUST
	/* CPUインタフェースを有効に */
	sil_wrw_mem((void *)DIC_CPUIF_CR, DIC_CPUIF_CR_EN);
#else

#ifdef TOPPERS_WITH_SAFEG
	/* CPUインタフェースを有効に */
	sil_wrw_mem((void *)DIC_CPUIF_CR, (DIC_CPUIF_CR_EN_S|DIC_CPUIF_CR_EN_NS|DIC_CPUIF_CR_FIQEN) );
#else
	/* CPUインタフェースを有効に */
	sil_wrw_mem((void *)DIC_CPUIF_CR, DIC_CPUIF_CR_EN);
#endif /* TOPPERS_WITH_SAFEG */

#endif /* NONTRUST */
}

/*
 *  CPUインタフェースの終了
 */
void
dic_cpu_stop(void)
{
	/* CPUインタフェースを無効に */
	sil_wrw_mem((void *)(DIC_CPUIF_CR), 0);
}


/*
 * DIC DISインタフェース関連
 */

/*
 *  割込み禁止
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
 *  割込み許可
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
 * 割込みペンディングクリア
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
 *  割込みペンディングセット
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
 *  割込み要求のチェック
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
 *  割込みコンフィギュレーション設定
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
 *  割込み優先度のセット
 *  内部表現で渡す．
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
 *  DIC割込みターゲットの設定
 *  cpusはターゲットとするCPUのビットパターンで指定
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
 * GICの割込みセキュリティ設定
 * 割込み番号とそのセキュリティで指定
 * ns(non secure bit) : 
 *     0 -> secure(FIQ) setting
 *     1 -> non-secure(IRQ) setting
 */
void dic_set_security(int intno, unsigned int ns){
    unsigned int reg = DIC_SEC + ((intno/32)*4);
    *((volatile unsigned int *)reg) |= (ns << (intno%32));
}


/*
 *  DICインタフェースの初期化
 */
void
dic_init(void)
{
	int i;
	unsigned int add;

	/* DICインタフェースを無効に */
	sil_wrw_mem((void *)(DIC_CR), 0);

	// all interrupts are secure setting
	for(add=0x1E001080; add<=0x1E0010FC; add+=4){
		*((volatile unsigned int *)add) = 0x0; //0xFFFFFFFF;
	}

#if __TARGET_ARCH_ARM == 7
#ifndef NONTRUST
    /* 割込みのセキュリティ設定 */
    dic_set_security(54,1);
    dic_set_security(57,1);    
    dic_set_security(58,1);    
    dic_set_security(66,1);    
    dic_set_security(67,1);    
    dic_set_security(76,1);    
    dic_set_security(79,1);
	//non-trust timer (Test Chip内蔵タイマSP804_0) non-secure setting(for Linux)
    dic_set_security(80,1);    
	//non-trust timer (Test Chip内蔵タイマSP804_1) non-secure setting
    dic_set_security(81,1);    

	//non-trust UART non-secure setting
    dic_set_security(84,1);        

#endif
#endif /* __TARGET_ARCH_ARM == 7 */

	/* 割込みを全て禁止 */
	for(i = 0; i < DIC_TNUM_INT/32; i++){
		sil_wrw_mem((void *)(DIC_CE + (4 * i)), 0xffffffff);
	}

	/* ペンディングをクリア */
	for(i = 0; i < DIC_TNUM_INT/32; i++){
		sil_wrw_mem((void *)(DIC_CP + (4 * i)), 0xffffffff);
	}

	/* 優先度最低に設定  */
	for(i = 0; i < DIC_TNUM_INT/4; i++){
		sil_wrw_mem((void *)(DIC_IPR + (4 * i)), 0xffffffff);
	}

	/* ターゲット初期化（全てCPU0へ） */
	for(i = TMIN_GLOBAL_INTNO/4; i < DIC_TNUM_INT/4; i++){
//	for(i = 0; i < DIC_TNUM_INT/4; i++){
		sil_wrw_mem((void *)(DIC_ITR + (4 * i)), 0x01010101);
	}

	/* モード初期化(1-N Level) */
	for(i = 0; i < DIC_TNUM_INT/16; i++){
		sil_wrw_mem((void *)(DIC_ICR + (4 * i)), 0x55555555);
	}
#ifndef NONTRUST
	/* DICインタフェースを有効に */
	sil_wrw_mem((void *)(DIC_CR), (DIC_CR_ENABLE|DIC_CR_NS_ENABLE));
#else
	/* DICインタフェースを有効に */
	sil_wrw_mem((void *)(DIC_CR), DIC_CR_ENABLE);
#endif
}


/*
 * (trust only)
 * 機能：(マスタプロセッサ以外でも実行する)割込みセキュリティの初期化
 * SGIの割込み番号のセキュリティ設定は全プロセッサで行う必要がある。
 */
void
dic_sgi_sec_init(void)
{
	/* DICインタフェースを無効に */
	sil_wrw_mem((void *)(DIC_CR), 0);

    /* all SGI non-secure setting */
    //*((volatile unsigned int *)0x1E001080) = 0xFFFF;
    /* all PPI non-secure setting */
    //*((volatile unsigned int *)0x1E001080) |= 0xFFFF0000;

	/*
     * SGI non-secure setting 
	 * トラスト側では割込み番号2と3をプロセッサ間割込み番号として使うことにする
     * ( kzm_ca9向けLinux(SMP)では、セカンダリコアのブートに
     *   swi(割り込み番号1)を使用するため )
     */
    dic_set_security(0,1);    
    dic_set_security(1,1);    
    //dic_set_security(2,1);    
    //dic_set_security(3,1);    

	/* PPI non-secure setting */
    dic_set_security(29,1);        

	/* DICインタフェースを有効に */
	sil_wrw_mem((void *)(DIC_CR), (DIC_CR_ENABLE|DIC_CR_NS_ENABLE));
}


/*
 *  DICインタフェースの終了
 */
void
dic_stop(void)
{
	/* DICインタフェースを無効に */
	sil_wrw_mem((void *)(DIC_CR), 0);
}

/*
 *
 * SCU関連の関数
 *
 */


#ifdef NONTRUST
#include "safeg_syscalls.h"
#endif


/*
 *  CP15のAUXILIARYレジスタのSAビットをセットしてSMPモードにする
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

	/* SCUの tag RAMS を無効に */
	sil_wrw_mem((void*)MPCORE_SCU_IALL, MPCORE_SCU_IALL_WAYS);

	/* SCUを有効に */
	scu_ctrl  = sil_rew_mem((void *)MPCORE_SCU_CTRL);
	scu_ctrl |= MPCORE_SCU_CTRL_EN;
	sil_wrw_mem((void *)MPCORE_SCU_CTRL, scu_ctrl);
}

/*
 *  SCUの初期化処理（マスタプロセッサのみ実行）
 */
void
scu_init(void){
	/* SCU を有効に */
	scu_enable();

	/* 全てのコアのモードをNORMALとする */
	sil_wrw_mem((void *)MPCORE_SCU_CPUST, 0x00U);
	
#ifndef NONTRUST
	/* ノントラスト側からのA9内蔵タイマへのアクセスを許可 */
	sil_wrw_mem((void *)MPCORE_SCU_SNSAC, 0xFFF); //全CPUを許可
	//sil_wrw_mem((void *)MPCORE_SCU_SNSAC, 0x111);  //CPU0のみ許可
#endif
}


/*
 *  SMPモードに設定する
 */
void
mpcore_smp_mode_enable(void){
	uint32_t sr;

	/* 全割込み禁止 */
	sr = current_sr();
	set_sr(sr|CPSR_IRQ_BIT|CPSR_FIQ_BIT);
	//set_sr(sr|CPSR_CPULOCK);
	
	/* キャシュを無効 */
	mpcore_dcache_clean_and_invalidate();
	mpcore_icache_invalidate();

	/* Data Synchronization Barrier */
	mpcore_data_sync_barrier();

	/* TLBの初期化 */
	mpcore_invalidate_unfied_tlb();

	/* CP15のSMP/nAMP bit をセットする */
	enable_sa();

	/* 割込み禁止状態を元に戻す */
	set_sr(sr);
}

/*
 *  Dキャッシュを開始
 */
void
mpcore_dcache_enable(void)
{
	uint32_t bits;

	CP15_CONTROL_READ(bits);

	/* すでにONならリターン */
	if (bits & CP15_CONTROL_C_BIT){
		return;
	}

	mpcore_dcache_invalidate();

	bits |= CP15_CONTROL_C_BIT;
	CP15_CONTROL_WRITE(bits);
}

/*
 *  Dキャッシュを停止して無効とする．
 *  CA9では，Dキャッシュが無効な状態でClean and Invalidate()を実行すると，
 *  暴走するため，Dキャッシュの状態を判断して，無効な場合は，Invalidate
 *  のみを行う． 
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
 * Iキャッシュの開始
 */
void mpcore_icache_enable (void)
{
	uint32_t bits;

	CP15_CONTROL_READ(bits);

	/*
	 *  すでに有効ならリターン
	 */
	if(bits & CP15_CONTROL_I_BIT){
		return;
	}

	mpcore_icache_invalidate();

	bits |= CP15_CONTROL_I_BIT;
	CP15_CONTROL_WRITE(bits);
}

/*
 *  Iキャッシュを停止
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
 *  I/Dキャッシュを両方を有効に
 */
void
mpcore_cache_enable(void)
{
	mpcore_dcache_enable();
	mpcore_icache_enable();
}

/*
 *  I/Dキャッシュを両方を無効に
 */
void
mpcore_cache_disable(void)
{
	mpcore_dcache_disable();
	mpcore_icache_disable();
}

/*
 *  MMU関連のドライバ
 */
#define DOMAIN_NO  1
#define PAGE_TABLE_SIZE  0x00004000 /* 4KB */

/*
 * ページテーブル
 */
static uint8_t PageTable[TNUM_PRCID][PAGE_TABLE_SIZE]  __attribute__ ((aligned (0x4000)));

#define my_PageTable (PageTable[x_prc_index()])

/*
 * ページテーブルのセクションを設定する
 */
static void
mmu_set_section(uintptr_t va, uintptr_t pa,
                uint8_t s, uint8_t tex, uint8_t ap,
                uint8_t c, uint8_t b)
{
	uint32_t ttb;
	uint32_t *sptr;

	/*
	 * ページテーブルのベースアドレスを取得
	 */
	CP15_TTB0_READ(ttb);

	/*
	 * VAからセクションのアドレスを取得
	 */
	sptr  = (uint32_t *)((ttb & 0xFFFFC000) | (((va & 0xFFF00000) >> 20) << 2));
	*sptr = pa | (s << 16) | (tex << 12) | (ap << 10) | (c << 3) | (b << 2) | (1 << 1);
	
}

#define SIZE_1M    0x00100000

/*
 * 指定に従い,メモリのマッピングを行う
 * マッピングの単位は1MB 
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

	/* MB単位になるようにサイズを設定 */
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
 *  MMUの初期化
 */
void
mpcore_mmu_init(void){
	MEMORY_ATTRIBUTE m_attribute;

	uint32_t bits = 0;

	/* プリフェッチバッファをクリア */
	mpcore_pbuffer_flash();
	
	/*
	 * TTBR0を用いる用に指定
	 */
	CP15_TTBCR_WRITE(0);


	/*
	 * 変換テーブル(TT)として，PageTableを使用する
	 * Sharedビットをセット 
	 */
#if __TARGET_ARCH_ARM == 6
	CP15_TTB0_WRITE((((uint32_t)my_PageTable)|CP15_TTB0_RGN_S|CP15_TTB0_RGN_WBWA));
#else /* __TARGET_ARCH_ARM == 7 */
	CP15_TTB0_WRITE((((uint32_t)my_PageTable)|CP15_TTB0_RGN_S|CP15_TTB0_RGN_WBWA|CP15_TTB0_IRGN_WBWA));
#endif /* __TARGET_ARCH_ARM == 7 */

	/* プリフェッチバッファをクリア */
	mpcore_pbuffer_flash();


	/*
	 * 全セクションを物理アドレス = 仮想アドレス
	 * ノンキャッシャブル，ノンバッファブルとする 
	 */
	m_attribute.pa   = 0x00000000;
	m_attribute.va   = m_attribute.pa;
	m_attribute.size = 0x80000000;
	m_attribute.s    = 1;          /* 共有         */
	m_attribute.ap   = 3;          /* フルアクセス */
	m_attribute.tex  = 0;          /* Strongly Ordered */
	m_attribute.c    = 0;
	m_attribute.b    = 0;


	mmu_map_memory (&m_attribute);


	m_attribute.pa   = 0x80000000;
	m_attribute.va   = m_attribute.pa;

	mmu_map_memory (&m_attribute);

	/*
	 *  ターゲット依存部でのMMUの初期化
	 */
	target_mmu_init();


	/*
	 * ドメイン番号をセット
	 */
	CP15_DOMAINS_WRITE(DOMAIN_NO);

	/*
	 * CONTROLコプロセッサの Mビット，XPビットをセットして，
	 * MMUを有効にする
	 */
	CP15_CONTROL_READ(bits);
	bits |= CP15_CONTROL_M_BIT | CP15_CONTROL_XP_BIT;
	CP15_CONTROL_WRITE(bits);
}

#if __TARGET_ARCH_ARM == 6
/*
 * SCUカウンタの初期化（MN0を使用）
 */
void
scu_counter_init(void)
{
	/*
	 *  MN0を Cycle Count に設定
	 */
	sil_wrw_mem((void *)MPCORE_SCU_PM_EVT0,
				sil_rew_mem((void *)MPCORE_SCU_PM_EVT0) | MPCORE_SCU_PM_EVT_CYC_COUNT0_4);

	/*
	 *  リセットとスタート
	 */
	sil_wrw_mem((void *)MPCORE_SCU_PM_CTRL,
			sil_rew_mem((void *)MPCORE_SCU_PM_CTRL) | MPCORE_SCU_PM_CTRL_RST | MPCORE_SCU_PM_CTRL_ENA);
}
#endif /* __TARGET_ARCH_ARM == 6 */
