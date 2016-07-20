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
 *  ターゲット依存モジュール（idea6410T用）
 *
 *  カーネルのターゲット依存部のインクルードファイル．kernel_impl.hのター
 *  ゲット依存部の位置付けとなる．
 */

#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H

/*
 *  ターゲットシステムのハードウェア資源の定義
 */
#include "idea6410t.h"

/*
 *  トレースログに関する設定
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  SafeGを使用する場合の処理の定義
 */
#ifdef TOPPERS_WITH_SAFEG

// #define TOPPERS_CUSTOM_IDLE

#endif /* TOPPERS_WITH_SAFEG */

/*
 *  CPUロックでセットするビット
 */
#define CPSR_CPULOCK     (CPSR_FIQ_BIT|CPSR_IRQ_BIT)

/*
 *  CPSRに常にセットするパターン
 */
#define CPSR_ALWAYS_SET CPSR_IRQ_BIT

/*
 *  support overrun handlers (can also be enabled in CFLAGS)
 */
// #define TOPPERS_SUPPORT_OVRHDR

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
 *  IRQとFIQハンドラの入り口処理をプロセッサ依存部で持つか，
 *  ターゲット依存部で持つかの指定
 */
//#define TARGET_IRQ_HANDLER
#define TARGET_FIQ_HANDLER

/*
 *  ASPカーネル動作時のメモリマップと関連する定義
 *
 *    10800000 - 1080ffff       コード領域
 *    10810000 -                データ領域
 *             - 11000000       デフォルトの非タスクコンテキスト用のスタック領域
 */

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
#define TMIN_INHNO 0U
#define TMAX_INHNO 63U
#define TNUM_INH   64U

/*
 *  割込み番号に関する定義
 */
#define TMIN_INTNO 0U
#define TMAX_INTNO 63U
#define TNUM_INT   64U


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
}

/*
 *  割込みハンドラの出入口処理の生成マクロ
 *
 */
#define INT_ENTRY(inhno, inthdr)    inthdr
#define INTHDR_ENTRY(inhno, inhno_num, inthdr) extern void inthdr(void);

/*
 *  TOPPERS標準割込み処理モデルの実現
 */

/*
 *  割込みのハンドラの割込み優先度テーブル
 *
 *  TZICは割込みハンドラ要因毎に優先度を設定できないため，ソフトウ
 *  ェア的に実現する
 */
extern const PRI inh_ipm_tbl[TNUM_INH];

/*
 *  各割込みの割込み要求禁止フラグの状態
 */
extern uint64_t idf;

/*
 *  割込み優先度マスク操作ライブラリ
 *
 *  CM922Tは割込み優先度マスクをハードウェア的に持たないため，割込み
 *  要求禁止フラグにより割込み優先度マスクを実現する
 */

/*
 *  現在の割込み優先度マスクの値
 */
extern PRI ipm;

/*
 *  割込み優先度マスク毎にセットする，割込み要求禁止フラグの値
 *  のテーブル
 */
extern const uint64_t ipm_mask_tbl[(TMAX_INTPRI - TMIN_INTPRI + 1)];

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  IPMをimp_mask_tblのインデックスに変換するマクロ
 */
#define INDEX_IPM(ipm)  (-(ipm)-1)

#ifndef TOPPERS_MACRO_ONLY

/*
 *  (モデル上の)割込み優先度マスクの設定
 *
 *  指定された優先度に設定された割込み要求禁止フラグのテーブルの値と（モデ
 *  ル上の）各割込みの割込み要求禁止フラグの状態を保持した変数の値との
 *  ORをIRCの割込み要求禁止フラグにセットし，設定した優先度を内部変数
 *  ipmに保存する．
 */
Inline void
x_set_ipm(PRI intpri)
{
    uint64_t ipm_mask;

    if (intpri == TIPM_ENAALL) {
        ipm_mask = 0U;
    } else {
        ipm_mask = ipm_mask_tbl[INDEX_IPM(intpri)];
    }

    /*
     *  CM922Tの割込みコントローラはイネーブルレジスタと
     *  クリアーレジスタがあるため，一旦全ての割込みを禁止してから，
     *  特定の割込みのみ許可する必要がある
     */
    /* 全割込み禁止 */
    tzic_disable_int(~0U);

    /* マスク指定されていない割込みのみ許可 */
    tzic_enable_int(~(ipm_mask|idf));

    ipm = intpri;
}

#define t_set_ipm(intpri) x_set_ipm(intpri)
#define i_set_ipm(intpri) x_set_ipm(intpri)

/*
 *  (モデル上の)割込み優先度マスクの参照
 *
 *  ipmの値を返す
 */
Inline PRI
x_get_ipm(void)
{
    return(ipm);
}

#define t_get_ipm() x_get_ipm()
#define i_get_ipm() x_get_ipm()

/*
 *  割込み属性が設定されているかを判別するための変数
 */
extern uint64_t	bitpat_cfgint;

/*
 * （モデル上の）割込み要求禁止フラグのセット
 *
 *  指定された，割込み番号の割込み要求禁止フラグのセットして，割込みを
 *  禁止する．また，（モデル上の）割込み要求禁止フラグを管理するidfの対
 *  応するビットををセットする．
 *  割込み要求をマスクする機能をサポートしていない場合には，falseを返す
 */
#define SUPPORT_DIS_INT /* dis_intをサポートする */

Inline bool_t
x_disable_int(INTNO intno)
{
    if ((bitpat_cfgint & INTNO_BITPAT(intno)) == 0U) {
        return(false);
    }
    tzic_disable_int(INTNO_BITPAT(intno));
    idf |= INTNO_BITPAT(intno);
    return(true);
}

#define t_disable_int(intno)  x_disable_int(intno)
#define i_disable_int(intno)  t_disable_int(intno)

/*
 *  (モデル上の)割り要求禁止フラグの解除
 *
 *  指定された，割込み番号の割込み要求禁止フラグのクリアして，割込みを
 *  許可する．また，（モデル上の）割込み要求禁止フラグを管理するidfの対
 *  応するビットををクリアする．
 *  割込み要求をマスクする機能をサポートしていない場合には，falseを返す
 */
#define SUPPORT_ENA_INT /* ena_intをサポートする */

Inline bool_t
x_enable_int(INTNO intno)
{
    if ((bitpat_cfgint & INTNO_BITPAT(intno)) == 0U) {
        return(false);
    }
    tzic_enable_int(INTNO_BITPAT(intno));
    idf &= ~INTNO_BITPAT(intno);
    return(true);
}

#define t_enable_int(intno) x_enable_int(intno)
#define i_enable_int(intno) x_enable_int(intno)

/*
 *  割込み要求のクリア
 */
Inline void
x_clear_int(INTNO intno)
{
    tzic_clear_int(INTNO_BITPAT(intno));
}

#define t_clear_int(intno) x_clear_int(intno)
#define i_clear_int(intno) x_clear_int(intno)


/*
 *  割込み要求のチェック
 */
Inline bool_t
x_probe_int(INTNO intno)
{
    return(tzic_probe_int(INTNO_BITPAT(intno)));
}

#define t_probe_int(intno) x_probe_int(intno)
#define i_probe_int(intno) x_probe_int(intno)

/*
 *  割込み要求ラインの属性の設定
 *
 *  TZICは，全てレベルトリガのため，属性で，トリガの指定はでき
 *  ない
 */
extern void    x_config_int(INTNO intno, ATR intatr, PRI intpri);

/*
 *  割込みハンドラの入り口で必要なIRC操作
 *
 *  CM922Tでは，必要な処理はない
 */
Inline void
i_begin_int(INTNO intno)
{
}

/*
 *  割込みハンドラの出口で必要なIRC操作
 *
 *  TZICでは，必要な処理はない
 */
Inline void
i_end_int(INTNO intno)
{
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
