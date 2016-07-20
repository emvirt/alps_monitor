/*
 *	TOPPERS/FMP Kernel
 *		Toyohashi Open Platform for Embedded Real-Time Systems/
 *		Advanced Standard Profile Kernel
 *	
 *	Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *								Toyohashi Univ. of Technology, JAPAN
 *	Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
 *				Graduate School of Information Science, Nagoya Univ., JAPAN
 *	Copyright (C) 2007-2010 by Industrial Technology Institute,
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
 *	ターゲット依存モジュール アセンブリ言語部（APSH2AD）
 */

#define  TOPPERS_MACRO_ONLY
#define UINT_C(val)		(val)		/* uint_t型の定数を作るマクロ */
#define ULONG_C(val)	(val)		/* ulong_t型の定数を作るマクロ */
#include "kernel_impl.h"

/*
 *	ROM化時に，DATAセクションのサイズが0だと，BSSがTXTの直下に配置され
 *	るため，DATAセクションのサイズを0にしないためのダミー変数を定義．
 *	コンパイラの最適化により削除されるのを防ぐため、アセンブラで記述し
 *	ている。
 */
	.data
_dummy:
	.long 0x00000000

/*
 *	低レベルのターゲット依存の初期化
 *
 *	スタートアップモジュールの中で，メモリの初期化の前に呼び出される．
 *	以下の状態でここに分岐する。
 *	　r4:プロセッサINDEX
 *	　r5:テーブルのインデックス=プロセッサINDEX*4
 */

/*
 *	初期化時のシーケンス
 *	
 *	以下のシーケンスでプロセッサ間の同期を取る。
 *	
 *	　　　　　　　　　CPU0　　　　　　　　CPU1
 *	ターゲット依存（target_support.S）
 *	　　　　　　　　セマフォ制御レジスタSEM0をロック
 *	
 *	　　　　　　　　クロック設定		クロック設定
 *	　　　　　　　　FRQCR1以外　　　　　FRQCR1のみ
 *	
 *	　　　　　　　　高速内蔵RAM0へのアクセス設定（SYSCRn）
 *	
 *	　　　　　　　　　　　　　　　　　　高速内蔵RAM1へのアクセス設定（SYSCRn）
 *	　　　　　　　　　　　　　　　　　　高速内蔵RAM1上の変数cpu1_flag←0
 *	　　　　　　　　　　　　　　　　　　高速内蔵RAM1上に命令コードをコピー
 *	　　　　　　　　　　　　　　　　　　コピーした命令コードに分岐
 *	　　　　　　　　SEM0を待つ		←	SEM0をリリース...(1)
 *	
 *	　　　　　　　　外部メモリへのアクセス設定
 *	　　　　　　　　（バスステートコンローラの初期化）
 *										　
 *	アーキテクチャ依存（start.S）
 *	　　　　　　　　bss/dataセクションの初期化
 *	　　　　　　　　software_init_hook()の呼び出し
 *	　　　　　　　　target_mprc_initialize()の呼び出し
 *	
 *	　　　　　　　　cpu1_flag←1	→	cpu1_flag=1となるまで待つ...(2)
 *	　　　　　　　　　　　カーネル起動
 *	　　　　　　　　　　　（sta_ker()の呼び出し）
 *	
 *	
 *  　他プロセッサと同期を取るには、
 *  　　・sleep命令＋プロセッサ間割込み
 *  　　・セマフォレジスタを使った排他制御
 *  　　・内部RAMへのtest & set命令
 *  　の３つが考えられる。
 *  　
 *  　処理内容や使用状況により、以下の制限事項がある。
 *  　
 *  　・全割込みロック状態
 *  　　　プロセッサ間割込みは使用できない。
 *  　
 *  　・クロック周波数の設定
 *  　　　FRQCR1レジスタはCPU1からのみ設定可能
 *  　
 *  　・高速内蔵RAMへのアクセス設定（SYSCRnの初期化）
 *  　　　他プロセッサは高速内蔵メモリへのアクセス不可
 *  　　　（高速内蔵メモリに配置された命令への命令フェッチも不可）
 *  　
 *  　・外部メモリへのアクセス設定
 *  　　（バスステートコントローラの初期化）
 *  　　　他プロセッサは外部メモリへのアクセス不可
 *  　　　（外部メモリに配置された命令への命令フェッチも不可）
 *  　
 *  　ここでは、
 *	　　・CPU0が待つ場合：セマフォ制御レジスタ
 *	　　・CPU1が待つ場合：高速内蔵RAM1上のフラグ変数
 *  　を用いている。
 *	　フラグを用いるのは、
 *	　　・CPU1をCPUバスだけで動作させるため
 *	　　・target_mprc_initialize()と干渉しないため
 *	　　　　（内部でセマフォ制御レジスタを初期化している。）
 */
	.text
	.align 4
	.globl _hardware_init_hook
_hardware_init_hook:
	tst    r4, r4
	bt     _cpu0
	bra    _cpu1
	nop

    /*******************************
     *  マスタプロセッサの場合の処理
     ********************************/
_cpu0:
    /*
     *  セマフォレジスタSEMR0の初期化（ロック相当）
     *  　SEMR0を読み出すことにより、ビット0が0（資源使用中）に
     *  　初期化される。
     */
	mov.l  _semr0_p, r1
	mov.b  @r1, r2

	/* クロック発信器(CPG)設定 */
	/* I0Φ=200MHz,BΦ=66.666MHz,PΦ=33.333MHz */
	/*	　備考：CPU1の内部クロックI1ΦはFRQCR1で設定	*/
	mov.l _frqcr0_addr, r1
	movi20 #0x0104, r2
	mov.w r2, @r1

	/*
	 *	内部RAM0を有効にする
	 *
	 *	システムコントロールレジスタの設定の注意事項
	 *　　・高速内蔵RAM空間以外にプログラムを配置する。
	 *　　・SYSCRnへのライト命令の直後にリード命令を配置する。
	 */
	mov.l _sys_cr1_addr, r1	 /*  CPU0から高速内蔵RAM0へのアクセス有効  */
	mov   #0xff, r0
	mov.b r0, @r1
	mov.b @r1, r3
							/*  CPU0から高速内蔵RAM0へのライト有効  */
	mov.b r0, @(SYSCR2 - SYSCR1, r1)
	mov.b @(SYSCR2 - SYSCR1, r1), r3
							/*  CPU1から高速内蔵RAM0へのアクセス有効  */
	mov.b r0, @(SYSCR3 - SYSCR1, r1)
	mov.b @(SYSCR3 - SYSCR1, r1), r3
							/*  CPU1から高速内蔵RAM0へのライト有効  */
	mov.b r0, @(SYSCR4 - SYSCR1, r1)
	mov.b @(SYSCR4 - SYSCR1, r1), r0

	/*
	 *	CPU1との同期ポイント(1)
	 *　　CPU1が高速内部RAM1に分岐するまで待つ。
	 */
	mov.l  _semr0_p, r1
_wait_cpu1:
	mov.b  @r1, r2
			/*
			 *  SEMR0のSEMFビット（ビット0）以外は常に0が読み出されるので、
			 *  符号拡張の結果、上位バイトも0に設定される。
			 */
	tst    r2, r2
	bt     _wait_cpu1	/*  SEMR0が0の間、待つ  */

#ifdef ROM_BOOT
	/* バスコントローラの設定 */
		/* CS0モードレジスタ */
		/* PRMOD  0 =ノーマルアクセス互換モード */
		/* PWENB  0 =ページライトアクセス禁止 */
		/* PRENB  0 =ページリードアクセス禁止 */
		/* EWENB  0 =外部ウェイト禁止 */
		/* WRMOD  0 =バイトライトストローブモード */
	mov.l _csmod0_addr, r1
	mov.l _csmod0_val, r2
	mov.l r2, @r1

		/* CS0ウェイト制御レジスタ1 */
		/* CSRWAIT	00110=6ウェイトサイクル */
		/* CSWWAIT	00110=6ウェイトサイクル */
		/* CSPRWAIT   000=0ウェイトサイクル */
		/* CSPWWAIT   000=0ウェイトサイクル */
	mov.l _cs1wcnt0_addr, r1
	mov.l _cs1wcnt0_val, r2
	mov.l r2, @r1

		/* CS0ウェイト制御レジスタ2 */
		/* CSON   001=1ウェイトサイクル */
		/* WDON   010=2ウェイトサイクル */
		/* WRON   010=2ウェイトサイクル */
		/* RDON   001=1ウェイトサイクル */
		/* WDOOFF 000=0ウェイトサイクル */
		/* CSWOFF 001=1ウェイトサイクル */
		/* CSROFF 001=1ウェイトサイクル */
	mov.l _cs2wcnt0_addr, r1
	mov.l _cs2wcnt0_val, r2
	mov.l r2, @r1

	/* SDRAM 接続ポートの初期化 */
	mov.l _pacrl4_addr, r1		/* D31~16 */
	movi20 #0x1111, r2
	mov.w r2, @r1
	mov.l _pacrl3_addr, r1
	mov.w r2, @r1					/*	r2=0x1111  */
	mov.l _pacrl2_addr, r1
	mov.w r2, @r1					/*	r2=0x1111  */
	mov.l _pacrl1_addr, r1
	movi20 #0x1110, r2
	mov.w r2, @r1

	mov.l _pbcrh1_addr, r1		/* PB17 SDWE */
	movi20 #0x0001, r2
	mov.w r2, @r1
	mov.l _pbcrl4_addr, r1		/* CKE CAS RAS WE3 */
	movi20 #0x1111, r2
	mov.w r2, @r1
	mov.l _pbcrl3_addr, r1		/* WE2 WE1 WE0 CS5 */
	mov.w r2, @r1					/*	r2=0x1111  */
	mov.l _pbcrl2_addr, r1		/* SDCS0 CS3 CS2 CS1 */
	movi20 #0x2111, r3
	mov.w r3, @r1
	mov.l _pbcrl1_addr, r1		/* A22 A21 A1 A0 */
	mov.w r2, @r1					/*	r2=0x1111  */

	mov.l _pccrl3_addr, r1		/* PC10~0 */
	movi20 #0x0000, r0
	mov.w r0, @r1
	mov.l _pccrl2_addr, r1		/* PC10~0 */
	mov.w r0, @r1					/*	r0=0x0000  */
	mov.l _pccrl1_addr, r1		/* PC10~0 */
	mov.w r0, @r1					/*	r0=0x0000  */

								/*	要検討　必要？	*/
	mov.l _pciorl_addr, r1		/* OUT : PC10~0 */
	movi20 #0x07FF, r2
	mov.w r2, @r1

	mov.l _pdcrl1_addr, r1		/* A25 A24 A23 */
	movi20 #0x0222, r2
	mov.w r2, @r1

	mov.l _phiorl_addr, r1		/* PH7,PH8:USBHOST0 PH9,PH10:USBHOST1 PH6:WP*/
	movi20 #0x0280, r2			/*	 WP:FLCTL（フラッシュメモリ）のライトプロテクト  */
	mov.w r2, @r1

	mov.l _pjcrl4_addr, r1		/* PJ12~0 */
	movi20 #0x0000, r0
	mov.w r0, @r1
	mov.l _pjcrl3_addr, r1
	mov.w r0, @r1					/*	r0=0x0000  */
	mov.l _pjcrl2_addr, r1
	mov.w r0, @r1					/*	r0=0x0000  */
	mov.l _pjcrl1_addr, r1
	mov.w r0, @r1					/*	r0=0x0000  */

	mov.l _pjiorl_addr, r1		/* OUT : PJ12~0 */
	movi20 #0x1FFF, r2
	mov.w r2, @r1
	
		/* SDRAM初期化レジスタ0 */
			/* DPC	 初期化プリチャージサイクル(000=3サイクル) */
			/* DARFC 初期化オートリフレッシュ回数(1000=8回) */
			/* DARFI 初期化オートリフレッシュ間隔(0010=5サイクル) */
	mov.l _sdir0_addr, r1
	mov.l _sdir0_val, r2
	mov.l r2, @r1
		/* SDRAM初期化レジスタ1 */
			/* DINIRQ  初期化シーケンス開始 */
	mov.l _sdir1_addr, r1
	mov.l _sdir1_val, r2
	mov.l r2, @r1

		/*
		 * SDRAM初期化終了待ち 
		 *	 r1:SDIR1レジスタのアドレス
		 *	 r2:SDIR1レジスタの値
		 *	 r3:マスクデータ（0x00010000）
		 */
	mov.l _sdir1_mask, r3
	mov.l @r1, r2	/*	r2<-SDIR1レジスタの値  */
_wait_initial_sdram:
	tst   r3, r2
	bf/s  _wait_initial_sdram
	 mov.l @r1, r2	/*	r2<-SDIR1レジスタの値  */
	
		/*
		 * SDRAMステータスレジスタSDSTRの確認
		 *	 r1:SDSTRレジスタのアドレス
		 *	 r2:SDSTRレジスタの値
		 */
	mov.l _sdstr_addr, r1
	mov.l @r1, r2	/*	r2<-SDSTRレジスタの値  */
_check_sdstr:
	tst   r2, r2
	bf/s  _check_sdstr
	 mov.l @r1, r2	/*	r2<-SDSTRレジスタの値  */

		/* SDRAM0モードレジスタ */
			/* A9	ライトバースト長(0=Burst) */
			/* A8-7 テストモード(00=Mode Reister Set) */
			/* A6-4 CASレイテンシ(010=2サイクル) */
			/* A3	バーストタイプ(0=Sequential) */
			/* A2-1 バースト長(000=1) */
	mov.l _sd0mod_addr, r1
	movi20 #0x0020, r2
	mov.l r2, @r1

		/* SDRAM0タイミングレジスタ */
			/* DRAS ロウアクティブ期間(010=3サイクル) */
			/* DRCD ロウカラムレイテンシ期間(01=2サイクル) */
			/* DPCG ロウプリチャージ期間(001=2サイクル) */
			/* DWR	ライトリカバリ期間(1=2サイクル) */
			/* DCL	CASレイテンシ(010=2サイクル) */
	mov.l _sd0tr_addr, r1
	movi20 #0x21302, r2
	mov.l r2, @r1

		/* SDRAM0アドレスレジスタ */
			/* DDBW SDRAMデータバス幅(10=32ビット) */
			/* DSZ	チャネルサイズ(011=32Mバイト) */
	mov.l _sd0adr_addr, r1
	movi20 #0x0203, r2
	mov.l r2, @r1

		/* SDRAMリフレッシュ制御レジスタ1 */
			/* DRFEN オートリフレッシュ動作(1=有効) */
			/* DREFW オートリフレッシュサイクル数(0100=5サイクル) */
			/* DRFC  オートリフレッシュ要求間隔 */
			/* 64mS/4096=1.56us */
	mov.l _sdrfcnt1_addr, r1
	movi20 #0x14208, r2
	mov.l r2, @r1

		/* SDRAMC0制御レジスタ */
			/* BSIZE 外部バス幅(01=32ビットバス) */
			/* EXENB 動作許可(1=動作許可) */
	mov.l _sdc0cnt_addr, r1
	mov.l _sdc0cnt_val, r2
	mov.l r2, @r1

#endif /*  ROM_BOOT  */

	rts/n

    /************************************
     *  マスタプロセッサでない場合の処理
     ************************************/
_cpu1:
	/*
	 *	CPU1の内部クロックI1Φの分周率：×１倍
	 *	（FRQCR1には、CPU1からでないとアクセスできない。）
	 */
	mov.l _frqcr1_addr, r1
	mov    #00, r2
	mov.w  r2, @r1

	/*
	 *	内部RAM1を有効にする
	 *
	 *	システムコントロールレジスタの設定の注意事項
	 *　　・高速内蔵RAM空間以外にプログラムを配置する。
	 *　　・SYSCRnへのライト命令の直後にリード命令を配置する。
	 */
	mov.l _sys_cr7_addr, r1	 /*  CPU0から高速内蔵RAM1へのアクセス有効  */
	mov   #0xff, r0
	mov.b r0, @r1
	mov.b @r1, r3				/*  ダミーリード  */
							/*  CPU0から高速内蔵RAM1へのライト有効  */
	mov.b r0, @(SYSCR8 - SYSCR7, r1)
	mov.b @(SYSCR8 - SYSCR7, r1), r3	/*  ダミーリード  */
							/*  CPU1から高速内蔵RAM1へのアクセス有効  */
	mov.b r0, @(SYSCR9 - SYSCR7, r1)
	mov.b @(SYSCR9 - SYSCR7, r1), r3	/*  ダミーリード  */
							/*  CPU1から高速内蔵RAM1へのライト有効  */
	mov.b r0, @(SYSCR10 - SYSCR7, r1)
	mov.b @(SYSCR10 - SYSCR7, r1), r0	/*  ダミーリード  */

	/*
	 *	高速内蔵RAM1上に命令コードをコピー
	 */
	mov.l  _ram1_routine_start_p, r1
	mov.l  _ram1_routine_end_p, r2
	mov.l  _ram1_routine_dst, r3
_copy_ram1:
	mov.l  @r1+, r0
	cmp/hi r1, r2
	mov.l  r0, @r3
	add    #4, r3
	bt	   _copy_ram1
	/*
	 *	高速内蔵RAM1上にコピーした命令コードに分岐
	 */
	mov.l  _ram1_routine_dst, r1
	add    #4, r1		/*  cpu1_flagの分、4バイトスキップする  */
	jmp    @r1
	 nop

	/*
	 *	高速内蔵RAM1上に配置する変数と命令コード
	 */
	.align 2
_ram1_routine_start:	/*  RAM1にコピーする領域：ここから  */

_cpu1_flag:				/*  同期用のフラグ変数cpu1_flag  */
	.long 0x00			/*  　初期値：0  */

	/*
	 *	CPU0との同期ポイント(1)
	 *	　CPU0への通知
	 */
	mov.l  _semr0_p, r1
	mov    #SEMR_SEMF_BIT, r2
	mov.b  r2, @r1

	/*
	 *	CPU0との同期ポイント(2)
	 *	　cpu1_flag=1となるまで待つ。
	 */
	mov.l  _cpu1_flag_addr, r1
_wait_cpu0:
	mov.l  @r1, r0
	cmp/eq #1, r0
	bf     _wait_cpu0

	rts/n	/*  start.Sへリターン  */


	.align 2
_semr0_p:
	.long SEMR0
_cpu1_flag_addr:
	.long INNER_RAM1_PAGE0_ADDRESS_START
_ram1_routine_end:	/*  RAM1にコピーする領域：ここまで  */


_frqcr0_addr:
	.long FRQCR0
_frqcr1_addr:
	.long FRQCR1

_sys_cr1_addr:
	.long SYSCR1
_sys_cr7_addr:
	.long SYSCR7


#ifdef ROM_BOOT
_csmod0_addr:
	.long CSMOD0
_csmod0_val:
	.long 0x00000000
_cs1wcnt0_addr:
	.long CS1WCNT0
_cs1wcnt0_val:
	.long 0x07070000
_cs2wcnt0_addr:
	.long CS2WCNT0
_cs2wcnt0_val:
	.long 0x12210011

_pacrl4_addr:
	.long PACRL4
_pacrl3_addr:
	.long PACRL3
_pacrl2_addr:
	.long PACRL2
_pacrl1_addr:
	.long PACRL1

_pbcrh1_addr:
	.long PBCRH1
_pbcrl4_addr:
	.long PBCRL4
_pbcrl3_addr:
	.long PBCRL3
_pbcrl2_addr:
	.long PBCRL2
_pbcrl1_addr:
	.long PBCRL1

_pccrl3_addr:
	.long PCCRL3
_pccrl2_addr:
	.long PCCRL2
_pccrl1_addr:
	.long PCCRL1

_pciorl_addr:
	.long PCIORL

_pdcrl1_addr:
	.long PDCRL1

_phiorl_addr:
	.long PHIORL

_pjcrl4_addr:
	.long PJCRL4
_pjcrl3_addr:
	.long PJCRL3
_pjcrl2_addr:
	.long PJCRL2
_pjcrl1_addr:
	.long PJCRL1

_pjiorl_addr:
	.long PJIORL

_sdir0_addr:
	.long SDIR0
_sdir0_val:
	.long 0x00000082
_sdir1_addr:
	.long SDIR0
_sdir1_val:
	.long 0x00000001
_sdir1_mask:
	.long 0x00010000
_sdstr_addr:
	.long SDSTR
_sd0mod_addr:
	.long SD0MOD
_sd0tr_addr:
	.long SD0TR
_sd0adr_addr:
	.long SD0ADR
_sdrfcnt1_addr:
	.long SDRFCNT1
_sdc0cnt_addr:
	.long SDC0CNT
_sdc0cnt_val:
	.long 0x00110000

#endif /*  ROM_BOOT  */

_ram1_routine_start_p:
	.long _ram1_routine_start
_ram1_routine_end_p:
	.long _ram1_routine_end
_ram1_routine_dst:
	.long INNER_RAM1_PAGE0_ADDRESS_START

