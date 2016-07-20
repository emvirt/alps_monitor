/*
 *	TOPPERS/FMP Kernel
 *		Toyohashi Open Platform for Embedded Real-Time Systems/
 *		Advanced Standard Profile Kernel
 *	
 *	Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
 *				Graduate School of Information Science, Nagoya Univ., JAPAN
 *	Copyright (C) 2009-2010 by Industrial Technology Institute,
 *								Miyagi Prefectural Government, JAPAN
 *	
 *	上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *	ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *	変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *	(1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *		権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *		スコード中に含まれていること．
 *	(2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *		用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *		者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *		の無保証規定を掲載すること．
 *	(3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *		用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *		と．
 *	  (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *		  作権表示，この利用条件および下記の無保証規定を掲載すること．
 *	  (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *		  報告すること．
 *	(4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *		害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *		また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *		由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *		免責すること．
 *	
 *	本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *	よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *	に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *	アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *	の責任を負わない．
 * 
 *  @(#) $Id$
 */

/*
 *	ターゲット依存モジュール（APSH2AD用）
 */

#include "kernel_impl.h"
#include <sil.h>
#include "pdic/sh/sh_scif.h"
#include "apsh2ad.h"
#include "target_syssvc.h"		/*  TNUM_PORT  */

/*
 *  str_ker() の実行前にマスタプロセッサのみ実行される初期化処理
 */
void
target_mprc_initialize(void)
{
	prc_mprc_initialize();

	/*
	 *	低消費電力モードの設定
	 */
		/*
		 *	CPU0のsleep命令の実行で、CPU0がスリープモードへ遷移
		 *	　・スリープエラー禁止
		 *	　・オーディオ用水晶発信子が無効
		 */
	sil_wrb_mem((void *)STBCR1, STBCR1_AXTALE);

		/*
		 *	　・H-UDIは動作
		 *	　・FPU0,FPU1は動作
		 *	　・UBC0,UBC1は動作
		 *	　
		 *	　備考
		 *	　　JTAGエミュレータを使用する場合は、
		 *	　　H-UDI、FPU、UBCを停止してはいけない。
		 */
	sil_wrb_mem((void *)STBCR2, 0x00U);

		/*
		 *	　・ATAPIは停止
		 *	　・MTU2は停止
		 *	　・ADCは停止
		 *	　・DACは停止
		 *	　・RTCは停止
		 */
	sil_wrb_mem((void *)STBCR3, 0xffU);

		/*
		 *	　・SCFI0は動作
		 *	　・SCFI1は動作
		 *	　・SCFI2は停止
		 *	　・SCFI3は停止
		 *	　・SCFI4は停止
		 *	　・SCFI5は停止
		 */
	sil_wrb_mem((void *)STBCR4, (uint8_t)~(STBCR4_SCIF0 | STBCR4_SCIF1));

		/*
		 *	　・IIC3_0は停止
		 *	　・IIC3_1は停止
		 *	　・IIC3_2は停止
		 *	　・IIC3_3は停止
		 *	　・RCAN0は停止
		 *	　・RCAN1は停止
		 */
	sil_wrb_mem((void *)STBCR5, 0xffU);

		/*
		 *	　・SSIF0は停止
		 *	　・SSIF1は停止
		 *	　・SSIF2は停止
		 *	　・SSIF3は停止
		 *	　・SSIF4は停止
		 *	　・SSIF5は停止
		 */
	sil_wrb_mem((void *)STBCR6, 0xffU);

		/*
		 *	　・CMT01は動作
		 *	　・CMT23は停止
		 *	　・FLCTLは停止
		 *	　・SSU0は停止
		 *	　・SSU1は停止
		 *	　・2DGは停止
		 *	　・USBは停止
		 */
	sil_wrb_mem((void *)STBCR7, (uint8_t)~STBCR7_CMT01);

	/*
	 *	SCIFのI/Oポートの設定	 要検討:PE4/SCK0端子の設定も必要？
	 */
	sil_wrh_mem((void *)PECRL1, 0x1111U);/* SCIF0:RTS0 CTS0 TxD0 RxD0 */
#if TNUM_PORT > 1
	sil_wrh_mem((void *)PECRL2, 0x0110U);/* SCIF1:TxD1 RxD1 */
#endif /* TNUM_PORT > 1 */
	
	/*
	 * バーナー出力用のシリアルコントローラの初期化
	 */
	sh_scif_init(1);
#if TNUM_PORT > 1
	sh_scif_init(2);
#endif /* TNUM_PORT > 1 */
}

/*
 *	ターゲット依存の初期化
 */
void
target_initialize(void)
{
	/*
	 *	プロセッサ依存の初期化
	 */
	prc_initialize();
}

/*
 *	ターゲット依存の終了処理
 */
void
target_exit(void)
{
	/*
	 *	プロセッサ依存の終了処理
	 */
	prc_exit();

	/*
	 *	開発環境依存の終了処理
	 */
	apsh2ad_exit();

	/*
	 * ここには来ない
	 */
	while(1);
}

/*
 *	システムログの低レベル出力のための文字出力
 */
void
target_fput_log(char_t c)
{
#if TNUM_PORT == 1
	ID siopid = TARGET_PUTC_PORTID;
#else	/* TNUM_PORT == 1 */
	ID siopid = sil_get_pindex() + 1;
#endif	/* TNUM_PORT == 1 */

	if (c == '\n') {
		sh_scif_pol_putc('\r', siopid);
	}
	sh_scif_pol_putc(c, siopid);
}


/*
 *	アサーションの失敗時の実行中断処理（T.B.D）
 */
void
TOPPERS_assert_abort(void)
{
	uint_t index = sil_get_pindex();
	
	syslog(LOG_EMERG, "prcindex = %d" ,index);
	while(1);
}

