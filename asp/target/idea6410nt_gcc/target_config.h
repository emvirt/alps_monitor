/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2007 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_config.h 369 2007-07-18 12:48:50Z honda $
 */

/*
 *  ターゲット依存モジュール（idea6410NT用）
 *
 *  カーネルのターゲット依存部のインクルードファイル．kernel_impl.hのター
 *  ゲット依存部の位置付けとなる．
 */

#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H

/*
 *  ターゲットシステムのハードウェア資源の定義
 */
#include "idea6410nt.h"

/*
 *  トレースログに関する設定
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  ベクタールーチンをカーネルで持つかの定義
 */
#define VECTOR_KERNEL

/*
 *  ベクタールーチンを持たない場合のベクターアドレスの先頭番地
 */
#define VECTOR_START  0x00

/*
 *  データセクションの初期化をしない
 */
// #define TOPPERS_OMIT_DATA_INIT

/*
 *  デフォルトの非タスクコンテキスト用のスタック領域の定義
 */
#define DEFAULT_ISTKSZ      0x2000U
// TODO: check about DEFAULT_ISTK??

/*
 *  微少時間待ちのための定義（本来はSILのターゲット依存部）
 */
#define SIL_DLY_TIM1    20
#define SIL_DLY_TIM2    10

/*
 *  割込みハンドラ番号に関する定義
 */
#define TMIN_INHNO  VIC_TMIN_INTNO
#define TMAX_INHNO  VIC_TMAX_INTNO
#define TNUM_INH    VIC_TNUM_INT

#define TMIN_INTNO  VIC_TMIN_INTNO
#define TMAX_INTNO  VIC_TMAX_INTNO
#define TNUM_INT    VIC_TNUM_INT

#ifndef TOPPERS_MACRO_ONLY

/*
 *  割込み番号の範囲の判定
 *
 *  ビットパターンを求めるのを容易にするために，8は欠番になっている．
 */
#define VALID_INTNO(intno) (TMIN_INTNO <= (intno) && (intno) <= TMAX_INTNO)
#define VALID_INTNO_DISINT(intno)	VALID_INTNO(intno)
#define VALID_INTNO_CFGINT(intno)	VALID_INTNO(intno)
#define VALID_INTNO_ATTISR(intno)   VALID_INTNO(intno)

/*
 *  割込みハンドラの登録用テーブル
 *   実態はコンフィギュレータで生成する
 */
extern const FP inh_tbl[TNUM_INH];

/*
 *  割込みハンドラの設定
 */
Inline void
x_define_inh(INHNO inhno, FP inthdr)
{
	vic_set_vector(inhno, inthdr);
}

/*
 *  割込みハンドラの出入口処理の生成マクロ
 *
 */
#define INT_ENTRY(inhno, inthdr)    inthdr
#define INTHDR_ENTRY(inhno, inhno_num, inthdr) extern void inthdr(void);

/*
 * VIC configuration functions
 */

/*
 * Functions to change external <--> internal representation of priority
 * masks:
 * 
 *   - External=PRI=ipm:     -15 ~ 0
 *   - Internal=uint8_t=iipm: 0 ~ 15
 *
 * This is because in uITRON interrupt priorities are represented with
 * negative numbers (so they dont conflict with task priorities)
 */

#define EXT_IPM(iipm)   ((PRI)(iipm - 0x0fU))     // to external (iipm=0~15)
#define INT_IPM(ipm)    ((uint8_t)(ipm + 0x0fU))  // to internal (ipm=-15~0)

/*
 * Macro defining the internal IPM to enable all interrupts. That is, set the 
 * IPM to the lowest priority (15 = 0 + 0x0fU)
 */
#define IIPM_ENAALL (INT_IPM(TIPM_ENAALL)) // e.g.: IIPM_ENAALL=15

/*
 *  Set the interrupt priority mask IPM
 */
Inline void set_iipm(uint8_t iipm)
{
	vic_cpu_set_priority(iipm);
}

Inline void x_set_ipm(PRI intpri)
{
	set_iipm(INT_IPM(intpri));
}

#define t_set_ipm(intpri) x_set_ipm(intpri)
#define i_set_ipm(intpri) x_set_ipm(intpri)

/*
 *  Get the current interrupt priority mask IPM
 */
Inline uint8_t current_iipm(void)
{
	return(vic_cpu_current_priority());  // 0~15
}

Inline PRI x_get_ipm(void)
{
	return(EXT_IPM(current_iipm())); // -15 ~ 0
}

#define t_get_ipm() x_get_ipm()
#define i_get_ipm() x_get_ipm()

/*
 *  Disable/Enable interrupts functions
 */

extern uint64_t	bitpat_cfgint;

#define SUPPORT_DIS_INT /* dis_int supported */
#define SUPPORT_ENA_INT /* ena_int is supported */

Inline bool_t x_disable_int(INTNO intno)
{
	if ((bitpat_cfgint & INTNO_BITPAT(intno)) == 0U) {
		return(false);
	}
	vic_disable_int(intno);
// 	idf |= INTNO_BITPAT(intno);
    return(true);
}

Inline bool_t x_enable_int(INTNO intno)
{
	if ((bitpat_cfgint & INTNO_BITPAT(intno)) == 0U) {
		return(false);
	}

	vic_enable_int(intno);
// 	idf &= ~INTNO_BITPAT(intno);
	return(true);
}

#define t_disable_int(intno)  x_disable_int(intno)
#define i_disable_int(intno)  t_disable_int(intno)
#define t_enable_int(intno)   x_enable_int(intno)
#define i_enable_int(intno)   x_enable_int(intno)

/*
 *  Clear pending interrupts
 */
Inline void x_clear_int(INTNO intno)
{
    vic_clear_pending(intno);
}

#define t_clear_int(intno) x_clear_int(intno)
#define i_clear_int(intno) x_clear_int(intno)

/*
 *  Check if a interrupt is active (polling)
 */
Inline bool_t x_probe_int(INTNO intno)
{
    return(vic_probe_int(intno));
}

#define t_probe_int(intno) x_probe_int(intno)
#define i_probe_int(intno) x_probe_int(intno)

/*
 *  割込み要求ラインの属性の設定
 */
extern void x_config_int(INTNO intno, ATR intatr, PRI intpri);

Inline void i_begin_int(INTNO intno){}

Inline void i_end_int(INTNO intno)
{
	vic_cpu_end_int(intno);
}

/*
 *  ターゲットシステム依存の初期化
 */
extern void target_initialize(void);

/*
 *  ターゲットシステムの終了
 *
 *  システムを終了する時に使う．
 */
extern void target_exit(void) NoReturn;

/*
 *  atexitの処理とデストラクタの実行
 */
Inline void
call_atexit(void)
{
    extern void    software_term_hook(void);
    void (*volatile fp)(void) = software_term_hook;

    /*
     *  software_term_hookへのポインタを，一旦volatile指定のあるfpに代
     *  入してから使うのは，0との比較が最適化で削除されないようにするた
     *  めである．
     */
    if (fp != 0) {
        (*fp)();
    }
}

/*
 *  割込みハンドラ（target_support.S）
 */
extern void interrupt_handler(void);

/*
 *  未定義の割込みが入った場合の処理
 */
extern void default_int_handler(void);


#endif /* TOPPERS_MACRO_ONLY */

/*
 *  コア依存モジュール（ARM用）
 */
#include "arm_gcc/common/core_config.h"

/*
 *  トレースログに関する設定
 */
// #include "nulltrace/trace_config.h"

#endif /* TOPPERS_TARGET_CONFIG_H */