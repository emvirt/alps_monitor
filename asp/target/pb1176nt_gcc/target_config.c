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
 *  @(#) $Id: target_config.c 369 2007-07-18 12:48:50Z honda $
 */

/*
 * ターゲット依存モジュール（SOC1176NT用）
 */

#include "kernel_impl.h"
#include <sil.h>
#include "itron.h"
#include "mmu.h"

/*
 *  ベクターテーブルの先頭番地
 */
extern void *vector_table;

/*
 *  ターゲット依存の初期化
 */
void
target_initialize(void)
{
    /*
     *  ARM依存の初期化
     */
    core_initialize();

    /*
     * MMUの初期化
     */
    pb1176_mmu_init();

    /*
     * CACHE の初期化
     */
    pb1176_cache_init();

    /*
     * GICの初期化
     */
    gic_init();

    /*
     * GICのCPUインタフェース初期化
     */
    gic_cpu_init();

    /*
     *  sys_putc が可能になるようにUARTを初期化
     */
    pb1176nt_init_uart();

    /*
     * 割込みベクタテーブルを VECTOR_TABLE_BASE レジスタに設定する
     */
    CP15_SET_VBAR((uint32_t) &vector_table);
}

/*
 *  ターゲット依存の終了処理
 */
void
target_exit(void)
{
    /*
     *  ARM依存の終了処理
     */
    core_terminate();

    /*
     *  GICのCPUインタフェースを停止
     */
    gic_cpu_stop();

    /*
     *  GICのDISインタフェースを停止
     */
    gic_stop();

    /*
     *  開発環境依存の終了処理
     */
    pb1176nt_exit();

    while(1);
}

/*
 *  ターゲット依存の文字出力
 */
void
target_fput_log(char_t c)
{
    if (c == '\n') {
        pb1176nt_putc('\r');
    }
    pb1176nt_putc(c);
}

/*
 *  割込み要求ラインの属性の設定
 *
 *  ASPカーネルでの利用を想定して，パラメータエラーはアサーションでチェッ
 *  クしている．FI4カーネルに利用する場合には，エラーを返すようにすべき
 *  であろう．
 *
 * VICは，全てレベルトリガのため，属性で，
 * トリガの指定はできない
 */
void
x_config_int(INTNO intno, ATR intatr, PRI intpri)
{
    assert(VALID_INTNO(intno));
    assert(TMIN_INTPRI <= intpri && intpri <= TMAX_INTPRI);

    /*
     *  割込み要求のマスク
     *
     *  割込みを受け付けたまま，レベルトリガ／エッジトリガの設定や，割
     *  込み優先度の設定を行うのは危険なため，割込み属性にかかわらず，
     *  一旦マスクする．
     */
    x_disable_int(intno);

    /*
     * 属性を設定
     */
    if ((intatr & TA_EDGE) != 0U) {
        gic_config(intno, GIC_ICR_EDGE);
        x_clear_int(intno);
    }
    else {
        gic_config(intno, GIC_ICR_LEVEL);
    }

    /*
     * 割込み優先度マスクの設定
     */
    gic_set_priority(intno, INT_IPM(intpri));
    gic_set_target(intno, 1 << 0x00U);

    /*
     * 割込みを許可
     */
    if ((intatr & TA_ENAINT) != 0U){
        (void)x_enable_int(intno);
    }
}

#ifndef OMIT_DEFAULT_INT_HANDLER
/*
 * 未定義の割込みが入った場合の処理
 */
void
default_int_handler(void){
    syslog_0(LOG_EMERG, "Unregistered Interrupt occurs.");
    target_exit();
}
#endif /* OMIT_DEFAULT_INT_HANDLER */

#ifdef TOPPERS_SUPPORT_GET_UTM
#ifdef OMIT_GET_UTM

#ifndef LOG_GET_UTM_ENTER
#define LOG_GET_UTM_ENTER(p_sysutm)
#endif /* LOG_GET_UTM_ENTER */

#ifndef LOG_GET_UTM_LEAVE
#define LOG_GET_UTM_LEAVE(ercd, sysutm)
#endif /* LOG_GET_UTM_LEAVE */

ER get_utm(SYSUTM *p_sysutm)
{
	LOG_GET_UTM_ENTER(p_sysutm);
	
	*p_sysutm = (*((volatile uint32_t *)0x1000005C) / 24);
	
	LOG_GET_UTM_LEAVE(E_OK, *p_sysutm);
	return(E_OK);
}
#endif /* OMIT_GET_UTM */
#endif /* TOPPERS_SUPPORT_GET_UTM */
