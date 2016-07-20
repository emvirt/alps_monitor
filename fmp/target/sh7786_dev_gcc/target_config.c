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
 *  @(#) $Id: target_config.c 516 2010-02-03 05:57:37Z ertl-honda $
 */

/*
 *  ターゲット依存モジュール（SH7786_DEV用）
 */

#include "kernel_impl.h"
#include <sil.h>
#include "mp.h"
#include "sh7786_dev.h"
#include "../pdic/sh/sh_scif.h"

#ifdef ROM_BOOT
uint32_t __dummy = 1;
#endif

/*
 *  低レベル出力で使用するポート番号
 */
static const ID prc_fput_log_uart_table[2] = {
	1,
	2
};

/*
 *  自プロセッサが使用する低レベル出力ポートの取得
 */
Inline ID
get_my_prc_fput_log_uart(void)
{
	return (prc_fput_log_uart_table[x_prc_index()]);
}

/*
 * str_ker()の前でマスタプロセッサで行う初期化
 */
void
target_mprc_initialize(void)
{
	/*
	 *  SH4A依存部での初期化
	 */
	prc_mprc_initialize();

	/*
	 *  GPIOのレジスタを設定してSCIF1のピンを有効にする
	 */
	sil_wrh_mem((void *)GPIO_PDCR,
                (sil_reh_mem((void *)GPIO_PDCR) & GPIO_PDCR_SCIF1_ENABLE));
	/*
	 *  GPIOのレジスタを設定してSCIF0のピンを有効にする
	 */
	sil_wrh_mem((void *)GPIO_PHCR,
                (sil_reh_mem((void *)GPIO_PHCR) & GPIO_PHCR_SCIF0_ENABLE));

	/* スタンバイコントロールレジスタ　Light Sleepモードに */
	sil_wrw_mem((void *)C0STBCR,
                (sil_rew_mem((void *)C0STBCR) | CSTBCR_LIGHT_SLEEP));
	sil_wrw_mem((void *)C1STBCR,
                (sil_rew_mem((void *)C1STBCR) | CSTBCR_LIGHT_SLEEP));

	/*
	 *  SCIF0を有効に
	 */
	sil_wrh_mem((void *)SPCAR, 0x0044U);/* enable SCIF0 clock=1.842 MHz */
}

/*
 *  ターゲット依存の初期化
 */
void
target_initialize(void)
{
	/*
	 *  プロセッサ依存の初期化
	 */
	prc_initialize();

	/*
	 *  低レベル出力用のシリアルコントローラの初期化
	 */
	sh_scif_init(get_my_prc_fput_log_uart());
}

/*
 *  ターゲット依存の終了処理
 */
void
target_exit(void)
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

	/*
	 *  プロセッサ依存の終了処理
	 */
	prc_terminate();

	/*
	 *  開発環境依存の終了処理
	 */
	sh7786_dev_exit();

	/*
	 * ここには来ない
	 */
	while(1);
}

/*
 *  ターゲット依存の文字出力
 */
void
target_fput_log(char c)
{
	ID siopid = get_my_prc_fput_log_uart();

	if (c == '\n') {
		sh_scif_pol_putc('\r', siopid);
	}
	sh_scif_pol_putc(c, siopid);
}
