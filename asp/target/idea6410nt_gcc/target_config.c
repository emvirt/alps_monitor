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
 * ターゲット依存モジュール（idea6410NT用）
 */

#include "kernel_impl.h"
#include <sil.h>
#include "itron.h"
#include "mmu.h"
#include "idea6410nt.h"

/*
 *  ベクターテーブルの先頭番地
 */
extern void *vector_table;

/*
*  割込み属性が設定されているかを判別するための変数
*/
uint64_t	bitpat_cfgint;

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
    idea6410_mmu_init();

    /*
     * CACHE の初期化
     */
    idea6410_cache_init();

    /*
     * VICの初期化
     */
    vic_init();

    /*
     *  sys_putc が可能になるようにUARTを初期化
     */
//     idea6410nt_init_uart();

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
     *  開発環境依存の終了処理
     */
    idea6410nt_exit();

    while(1);
}

/*
 *  ターゲット依存の文字出力
 */
void
target_fput_log(char_t c)
{
    if (c == '\n') {
        idea6410nt_putc('\r');
    }
    idea6410nt_putc(c);
}

/*
 *  Configure an interrupt line
 */
void
x_config_int(INTNO intno, ATR intatr, PRI intpri)
{
    assert(VALID_INTNO(intno));
    assert(TMIN_INTPRI <= intpri && intpri <= TMAX_INTPRI);

    // store a bit to indicate that this interrupt was configured
    bitpat_cfgint |= INTNO_BITPAT(intno);

    // disable the interrupt in the controller
    x_disable_int(intno);

    // set the priority of the interrupt
    vic_set_priority(intno, INT_IPM(intpri));

    // enable the interrupt if it was set in the .cfg file
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
//     syslog_0(LOG_EMERG, "Unregistered Interrupt occurs.");
    target_exit();
}
#endif /* OMIT_DEFAULT_INT_HANDLER */
