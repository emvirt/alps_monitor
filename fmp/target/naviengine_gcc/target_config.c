/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 * 
 *  Copyright (C) 2007-2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_config.c 91 2009-01-25 16:50:43Z ertl-honda $
 */

/*
 * ターゲット依存モジュール（NaviEngine用）
 */
#include "kernel_impl.h"
#include <sil.h>

/*
 *  str_ker() の前でマスタプロセッサで行う初期化
 */
void
target_mprc_initialize(void)
{
	/* MPCore依存部での初期化 */
	chip_mprc_initialize();

	/* NaviEngine 内の MPCore Status の初期化 */
	sil_wrw_mem((void *)NAVIENGINE_MPCORE_STATUS, 0x00);
}

/*
 * メモリのベースアドレスとサイズ
 */
#define SDRAM_PADDR 0x80000000
#define SDRAM_SIZE  0x10000000 /* 256MB */

/*
 * メモリマップの初期化（chip依存部から呼び出される）
 * 同時にキャッシュも有効にする
 */
void
target_mmu_init(void)
{
	MEMORY_ATTRIBUTE m_attribute;

	/*
	 *  Enable caching for the SDRAM
	 */
	m_attribute.pa   = SDRAM_PADDR;
	m_attribute.va   = m_attribute.pa;
	m_attribute.size = SDRAM_SIZE;
	m_attribute.s    = 1;		  /* 共有		 */
	m_attribute.ap   = 3;		  /* フルアクセス */
	m_attribute.tex  = 1;		  /* Outer and Inner */
	m_attribute.c    = 1;		  /* Inner Write-Back, Write Allocate */ 
	m_attribute.b    = 1;

	mmu_map_memory(&m_attribute);

	/*
	 *  ベクターテーブルを0x0000に置くため，SDRAMを
	 *  0x0000から配置する 
	 */
	m_attribute.pa   = 0x81000000;
	m_attribute.va   = 0x00000000;   
	m_attribute.size = 0x10000000 - 0x01000000;
	m_attribute.ap   = 3;          /* フルアクセス */
	m_attribute.tex  = 1;          /* Outer and Inner */
	m_attribute.c    = 1;          /* Inner Write-Back, Write Allocate */ 
	m_attribute.b    = 1;

	mmu_map_memory(&m_attribute);
}

/*
 *  ターゲット依存の初期化
 */
void
target_initialize(void)
{
	/*
	 * チップ依存の初期化
	 */
	chip_initialize();
        
	/*
	 *  UARTを初期化
	 */
#ifdef G_SYSLOG
	if (x_sense_mprc()) {
		naviengine_uart_init();
	}
#else
	naviengine_uart_init();
#endif /* G_SYSLOG */
}

/*
 *  ターゲット依存の終了処理
 */
void
target_exit(void)
{
	/*
	 *  チップ依存の終了処理
	 */
	chip_exit();

	/*
	 *  開発環境依存の終了処理
	 */
	naviengine_exit();

	while(1);
}

/*
 *  システムログの低レベル出力のための文字出力
 */
void
target_fput_log(char_t c)
{
	if (c == '\n') {
		naviengine_uart_putc('\r');
	}
	naviengine_uart_putc(c);
}

/*
 *  実行時間分布集計モジュールで用いる測定用タイマの初期化関数
 */
void
perf_timer_initialize(intptr_t exinf)
{
	scu_counter_init();
}

/*
 *  実行時間分布集計モジュール用のカウンタによる計測
 */
void
perf_timer_get(uint32_t *p_time)
{
	*p_time = scu_counter_get_current();
}

/*
 *  実行時間分布集計モジュール用のカウンタによる測定結果の単位変換
 *  nsecに変換 
 */
uint32_t
perf_timer_conv_tim(uint32_t time)
{
	return (time * 1000) / NAVIENGINE_CORE_CLK;
}
