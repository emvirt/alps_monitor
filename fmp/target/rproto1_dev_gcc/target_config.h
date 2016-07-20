/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_config.h 564 2010-04-14 00:53:34Z ertl-honda $
 */

/*
 *  ターゲット依存モジュール（RPROTO1_DEV用）
 *
 *  カーネルのターゲット依存部のインクルードファイル．kernel_impl.hのター
 *  ゲット依存部の位置付けとなる．
 */

#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H

/*
 *  ターゲットシステムのハードウェア資源の定義
 */
#include "rproto1_dev.h"

/*
 *  get_utmをサポートするかどうかの定義
 */
#define SUPPORT_GET_UTM

/*
 *  FMPカーネル動作時のメモリマップと関連する定義
 *       
 *      0x08000000 -  コード領域
 *      0x08050000 -  データ領域
 */

/*
 *  デフォルトの非タスクコンテキスト用のスタック領域の定義
 */
#define DEFAULT_PRC1_ISTKSZ  (1024 * 8)  /* 8KB */
#define DEFAULT_PRC2_ISTKSZ  (1024 * 8)  /* 8KB */
#define DEFAULT_PRC3_ISTKSZ  (1024 * 8)  /* 8KB */
#define DEFAULT_PRC4_ISTKSZ  (1024 * 8)  /* 8KB */

/*
 *  キャッシュのライトバックを有効化
 */
#define ENABLE_CACHE_WRITE_BACK

/*
 *  キャッシュのコヒーレンシを有効化
 */
#define ENABLE_CACHE_COHERENCE

/*
 *  ディスパッチ時にFPUのバンクレジスタを保存を有効化
 */
//#define FPU_SAVE_BANKED_REG

/*
 *  ロックの方式
 */
//#define TTYPE_KLOCK G_KLOCK
#define TTYPE_KLOCK P_KLOCK
//#define TTYPE_KLOCK F_KLOCK

/*
 *  スピンロック機能の方式
 */
#define TTYPE_SPN NATIVE_SPN
//#define TTYPE_SPN EMULATE_SPN

/*
 *  ネイティブスピン方式の場合のスピンロックの最大数
 */
#define TMAX_NATIVE_SPN  256

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ロックアルゴリズムにFIFOロックを使用
 */
#define ENABLE_FIFO_LOCK

/*
 *  str_ker()の前でマスタプロセッサで行う初期化
 */
extern void target_mprc_initialize(void);

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
 *  ターゲットシステムの文字出力
 *
 *  システムの低レベルの文字出力ルーチン．ROMモニタ／GDB STUB呼出しで実
 *  現する．
 */
extern void target_putc(char c);

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  微少時間待ちのための定義（本来はSILのターゲット依存部）
 */
#define SIL_DLY_TIM1     11
#define SIL_DLY_TIM2     9


/*
 *  プロセッサ依存モジュール（SH4A用）
 */
#include "sh4a_gcc/rproto1_config.h"

/*
 *  トレースログに関する設定
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */
     
#endif  /* TOPPERS_TARGET_CONFIG_H */
