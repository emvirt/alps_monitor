/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: prc_config.c 512 2010-02-01 05:33:27Z ertl-honda $
 */

/*
 *		プロセッサ依存モジュール（SH4A用）
 */
#include "kernel_impl.h"
#include "check.h"
#include "task.h"

/*
 *  SILのスピンロック用変数
 */
uint32_t TOPPERS_spn_var;

/*
 *  ロック取得時に使用するダミー変数
 */
uint32_t dangling_dummy;

/*
 *  各割込み番号毎のIPRの情報管理テーブル
 *  内容の定義は，プロセッサ型番毎に異なるのため，
 *  プロセッサ型番毎の定義ファイルで定義する
 */
const IPR_INFO ipr_info_tbl[TNUM_INH] = {
    IPR_INFO_TBL_DATA 
};

/*
 *  各割込み番号毎のMSKCLRの情報管理テーブル
 *  内容の定義は，プロセッサ型番毎に異なるのため，
 *  プロセッサ型番毎の定義ファイルで定義する
 */
const MSK_INFO mskclr_info_tbl[TNUM_INH] = {
    MSKCLR_INFO_TBL_DATA
};

/*
 *  カーネルのベクターベース
 */
extern void base_vec(void);

/*
 *  start.S での同期用
 */
volatile uint_t   start_sync;

/*
 *  sta_ker 内でのバリア同期用変数
 */
static volatile bool_t   prc_init[TNUM_PRCID];
static volatile uint_t   sys_start;

/*
 *  同期のためのマクロ
 */
#define MAGIC_START 0x87654321U

/*
 *  sta_ker 内でのバリア同期
 */
void
sta_ker_barrier_sync(void){
	volatile uint_t i, j;
	volatile uint_t flag;

	prc_init[x_prc_index()] = true;

	if (x_sense_mprc()) {
		do{
			flag = 0;
			for(i = 0; i < TNUM_PRCID; i++){
				if(prc_init[i] == true){
					flag++;
				}
			}
			for(j = 0; j < 100; j++);
		}while (flag < TNUM_PRCID);
		sys_start = MAGIC_START;
		start_sync = 0;

      
	}
	else {
		while(sys_start != MAGIC_START){
			for(j = 0; j < 100; j++);
		}
	}
}

/*
 *  str_ker()の前でマスタプロセッサで行う初期化
 */
void
prc_mprc_initialize(void)
{
	/*
	 *  SILのスピンロック用変数の初期化
	 */
	TOPPERS_spn_var = 0U;

	/*
	 * 割込み優先度レジスタの初期化
	 */
	init_int2pri();

	/*
	 *  割込みコントローラの初期化
	 */
	init_irc();
}


/*
 *  プロセッサ依存の初期化
 */
void
prc_initialize(void)
{
	TPCB *p_tpcb = get_my_p_tpcb();

	/*
	 *  CPUロックフラグ実現のための変数の初期化
	 */
	p_tpcb->lock_flag = true;
	p_tpcb->saved_iipm = IIPM_ENAALL;

	set_vbr(base_vec);
}

/*
 *  プロセッサ依存の終了処理
 */
void
prc_terminate(void)
{

}

/*
 *  割込み要求ライン属性の設定
 */
void
x_config_int(INTNO intno, ATR intatr, PRI intpri, uint_t affinity_mask)
{
	assert(VALID_INTNO_CFGINT(intno));
	assert(-15 <= intpri && intpri <= TMAX_INTPRI);

	/* 
	 *  一旦割込みを禁止する
	 */    
	(void)x_disable_int(intno) ;

	/*
	 *  レベルトリガ/エッジトリガの設定
	 *  IRQのみサポートする
	 */
	if(INTNO_IRQ(INTNO_MASK(intno)) != 0U){
		uint16_t icr1_val = sil_rew_mem((void*)ICR1);
		uint16_t offset   = INTNO_IRQ_OFFSET(INTNO_MASK(intno)) * 2U;

		icr1_val &=  ~(0x03U << offset);
        
		if((intatr & TA_POSEDGE) != 0U) {
		/*
		 *  ポジティブエッジ
		 */
			icr1_val |= IRQ_POSEDGE << offset;            
        }else if((intatr & TA_NEGEDGE) != 0U) {
		 /*
		  *  ネガティブエッジ
		 */
			icr1_val |= IRQ_NEGEDGE << offset;                        
        }else {
		/*
		 *  ローレベルトリガ
		 */
			icr1_val |= IRQ_LOWLEVEL << offset;
		}
        
		sil_wrw_mem((void*)ICR1, icr1_val);
	}

	/*
	 *  割込み要求マスク解除(必要な場合)
	 *  同時に割込み優先度もセットされる
	 */

	if ((intatr & TA_ENAINT) != 0U) {
		(void) x_enable_int(intno);
	}

	/*
	 *　割込みマスククリアレジスタセット
	 */
	set_target_irc(intno,x_prc_index());

}


#ifndef OMIT_DEFAULT_EXC_HANDLER
/*
 *  Trapa以外の例外で登録されていない例外が発生すると呼び出される
 */
void
default_exc_handler(void *p_excinf)
{
	uint32_t expevt = *(((uint32_t*)p_excinf) + P_EXCINF_OFFSET_EXPEVT);
	uint32_t spc    = *(((uint32_t*)p_excinf) + P_EXCINF_OFFSET_SPC);
	uint32_t ssr    = *(((uint32_t*)p_excinf) + P_EXCINF_OFFSET_SSR);
	uint32_t pr     = *(((uint32_t*)p_excinf) + P_EXCINF_OFFSET_PR);
    
	syslog(LOG_EMERG, "Unregistered Expevt error occurs.");
	syslog(LOG_EMERG, "Expevt = %08x SPC = %08x SR = %08x PR=%08X",
           expevt, spc, ssr, pr);
	target_exit();
}
#endif /* OMIT_DEFAULT_EXC_HANDLER */

#ifndef OMIT_DEFAULT_INT_HANDLER
/*
 *  未登録の割込みが発生した場合に呼び出される
 */
void
default_int_handler(void *p_excinf)
{
	uint32_t spc     = *(((uint32_t*)p_excinf) + 13U);
	uint32_t ssr     = *(((uint32_t*)p_excinf) + 11U);
	uint32_t pr      = *(((uint32_t*)p_excinf) + 12U);
	uint32_t intevt  = sil_rew_mem((void*)INTEVT);
#ifdef USE_INTEVT2    
	uint32_t intevt2 = sil_rew_mem((void*)INTEVT2);
#endif /* USE_INTEVT2 */      
	syslog(LOG_EMERG, "Unregistered Interrupt occurs.");
#ifdef USE_INTEVT2    
	syslog(LOG_EMERG, "INTEVT = %08x INTEVT2 = %08x SPC = %08x SR = %08x PR = %08x ",
           intevt, intevt2, spc, ssr, pr);
#else
	syslog(LOG_EMERG, "INTEVT = %08x SPC = %08x SR = %08x PR = %08x ",
           intevt, spc, ssr, pr);    
#endif /* USE_INTEVT2 */      
	target_exit();
}
#endif /* OMIT_DEFAULT_INT_HANDLER */
