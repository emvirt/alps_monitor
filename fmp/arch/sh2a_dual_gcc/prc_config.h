/*
 *	TOPPERS/FMP Kernel
 *		Toyohashi Open Platform for Embedded Real-Time Systems/
 *		Advanced Standard Profile Kernel
 *	
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
 *	@(#) $Id$
 */

/*
 *		プロセッサ依存モジュール（SH2A-DUAL用）
 *
 *	このインクルードファイルは，target_config.h（または，そこからインク
 *	ルードされるファイル）のみからインクルードされる．他のファイルから
 *	直接インクルードしてはならない．
 */

#ifndef TOPPERS_PRC_CONFIG_H
#define TOPPERS_PRC_CONFIG_H

#ifndef TOPPERS_MACRO_ONLY

/*
 *	プロセッサの特殊命令のインライン関数定義
 */
#include "prc_insn.h"

/*
 *	エントリ(start.S)
 */
extern void start(void);

/*
 *	未登録の割込み用のエントリ
 */
extern void default_int_handler_entry(void);

/*
 *	非タスクコンテキスト用のスタック初期値
 */
#define TOPPERS_ISTKPT(istk, istksz) ((STK_T *)((char_t *)(istk) + (istksz)))

/*
 *	タスクコンテキストブロックの定義
 */
typedef struct task_context_block {
	void	*sp;		/* スタックポインタ */
	FP		pc;			/* プログラムカウンタ */
} CTXB;


/*
 *  マスタープロセッサかを返す
 */
Inline bool_t
x_sense_mprc(void)
{
	if (x_prc_index() == (TOPPERS_MASTER_PRCID - 1)) {
		return true;
	}
	else {
		return false;
	}
}

#endif /* TOPPERS_MACRO_ONLY */

/*
 *	割込み優先度マスク操作ライブラリ
 *	
 *	SH2A-DUALでは，ステータスレジスタ（SR）の4〜7ビット目の4ビットに割込み
 *	優先度マスク（ハードウェアの割込み優先度マスク，IPM）が置かれてい
 *	る．IPMを保存しておくために，割込み優先度の外部表現（-1から連続し
 *	た負の値）を使うことも可能であるが，余計な左右ビットシフトと符号反
 *	転が必要になる．これを避けるために，IPMを保存する場合には，SRの4〜7
 *	ビット目を取り出した値を使うことにする．この値を割込み優先度マスク
 *	の内部表現と呼び，IIPMと書くことにする．
 */

/*
 *	割込み優先度マスクの外部表現と内部表現の変換
 *
 *	アセンブリ言語のソースファイルからインクルードする場合のために，CASTを使用
 */
#define EXT_IPM(iipm)	 (-CAST(PRI,(iipm) >> 4))		/* 内部表現を外部表現に */
#define INT_IPM(ipm)	 (CAST(uint32_t, -(ipm)) << 4)	/* 外部表現を内部表現に */

/*
 *	CPUロック状態での割込み優先度マスク
 *
 *	TIPM_LOCKは，CPUロック状態での割込み優先度マスク，すなわち，カーネ
 *	ル管理外のものを除くすべての割込みをマスクする値に定義する．
 */
#define TIPM_LOCK	 TMIN_INTPRI

/*
 *	カーネル管理外の割込みをサポートするか否かの定義（未完成）
 */
#if TIPM_LOCK != -15
#define TOPPERS_SUPPORT_NONKERNEL_INT
#endif

/*
 *	CPUロック状態での割込み優先度マスクの内部表現
 */
#define IIPM_LOCK	 INT_IPM(TIPM_LOCK)

/*
 *	割込み優先度マスク全解除（TIPM_ENAALL）の内部表現
 */
#define IIPM_ENAALL  INT_IPM(TIPM_ENAALL)

/*
 *	全割込みロックの内部表現
 */
#define IIPM_DISALL  UINT_C(0xf0)


#ifndef TOPPERS_MACRO_ONLY

/*
 *	IPM（ハードウェアの割込み優先度マスク，内部表現）の現在値の読出し
 */
Inline uint32_t
current_iipm(void)
{
	return((uint32_t)(current_sr() & 0xF0U));
}

/*
 *	IPM（ハードウェアの割込み優先度マスク，内部表現）の現在値の設定
 */
Inline void
set_iipm(uint32_t iipm)
{
	set_sr((current_sr() & ~0xF0U) | iipm);
}

/*
 *	TOPPERS標準割込み処理モデルの実現
 *
 *	SH2A-DUALはステータスレジスタ(SR)内に割込み優先度マスク(IPM)を持っている．
 *	CPUロックフラグに相当機能を持たない．そのため，擬似的にCPUロックフ
 *	ラグを実現する．
 *
 *	まず，CPUロック状態を管理すための変数(lock_flag)を用意する．
 *
 *	CPUロックフラグがクリアされている間は，IPMをモデル上の割込み優先度
 *	マスクの値に設定する．この間は，モデル上の割込み優先度マスクは，
 *	IPMを用いる．
 *	
 *	それに対してCPUロックフラグがセットされいる間は，IPMを，カーネル管
 *	理外のものを除くすべての割込み要求をマスクする値(TIPM_LOCK)と，モデ
 *	ル上の割込み優先度マスクとの高い方に設定する．この間のモデル上の割
 *	込み優先度マスクは，そのための変数(saved_iipm, 内部表現で保持)を用
 *	意して保持する．
 *
 *	それに対してCPUロックフラグがセットされいる間は，
 *	 　・IPMを，カーネル管理外のものを除くすべての割込み要求をマスク
 *	 　　する値(TIPM_LOCK)
 *	 　・モデル上の割込み優先度マスク
 *	の２つの内、高い方に設定する．この間のモデル上の割込み優先度マスク
 *	は，そのための変数(saved_iipm, 内部表現で保持)を用意して保持する．
 *	タスクコンテキストでは，CPUロック状態以外では，saved_iipmは常にIPM
 *	と同じ値になるようにする．
 */

/*
 *	プロセッサ状態参照の際の前処理と後処理
 *	　マルチプロセッサでは、プロセッサ状態参照の際にマイグレーションを
 *	　考慮して、CPUロック相当にする必要がある。
 *	
 *　　同じパターンが頻繁に現れるため、前処理と後処理をインライン関数
 *	　pre_lock()とpost_lock()として、まとめた。
 *	
 *	　使用方法
 *	　　uint32_t iimp = pre_lock():
 *	　　シングルプロセッサ相当の処理
 *	　　post_lock();
 *	
 */

/*
 *	プロセッサ状態参照の際の前処理
 */
Inline uint32_t
pre_lock(void)
{
	uint32_t sr, iipm;
	/*
	 *	マイグレーションを考慮して、CPUロック相当にする。
	 */
	sr = current_sr();
	iipm = sr & 0xF0U;
#ifndef TOPPERS_SUPPORT_NONKERNEL_INT
						/*  カーネル管理外の割込みをサポートしない場合  */
	set_sr(sr | 0xF0U);		/*  全割込み禁止  */
#else /* TOPPERS_SUPPORT_NONKERNEL_INT */
						/*  カーネル管理外の割込みをサポートする場合  */
	/*
	 *  元がカーネル管理割込み禁止でなければ、
	 *  カーネル管理割込みのみ、禁止する。
	 */
	if (IIPM_LOCK > iipm) {
		set_iipm(IIPM_LOCK);
	}
#endif /* TOPPERS_SUPPORT_NONKERNEL_INT */
	/* クリティカルセクションの前後でメモリが書き換わる可能性がある */
	Asm("":::"memory");
	return(iipm);
}


/*
 *	プロセッサ状態参照の際の後処理
 */
Inline void
post_lock(uint32_t iipm)
{
	/* クリティカルセクションの前後でメモリが書き換わる可能性がある */
	Asm("":::"memory");
	set_iipm(iipm);		/*  元のCPUロック状態に戻す。  */
}


/*
 *	コンテキストの参照
 *
 *	SH2A-DUALでは，コンテキストの判定に割込み/CPU例外のネスト回数を使用する
 *	
 */
Inline bool_t
sense_context(void)
{
	uint32_t iipm;
	PCB *my_p_pcb;
	bool_t ret;
	
	iipm = pre_lock();		/*  CPUロック相当  */
	my_p_pcb = get_my_p_pcb();
	if ((my_p_pcb->target_pcb.excnest_count) != 0U) {
		ret = true;			/*  非タスクコンテキスト  */
	} else {
		ret = false;		/*  非タスクコンテキスト  */
	}
	post_lock(iipm);		/*  元のCPUロック状態に戻す。  */

	return(ret);
}

/*
 *	CPUロック状態への移行
 *
 *	IPM（ハードウェアの割込み優先度マスク）を，saved_iipmに保存し，カー
 *	ネル管理外のものを除くすべての割込みをマスクする値（TIPM_LOCK）に設
 *	定する．また，lock_flagをtrueにする．
 *
 *	IPMが，最初からTIPM_LOCKと同じかそれより高い場合には，それを
 *	saved_iipmに保存するのみで，TIPM_LOCKには設定しない．これは，モデル
 *	上の割込み優先度マスクが，TIPM_LOCKと同じかそれより高いレベルに設定
 *	されている状態にあたる．
 *
 *	この関数は，CPUロック状態（lock_flagがtrueの状態）で呼ばれることは
 *	ないものと想定している．
 */
Inline void
x_lock_cpu(void)
{
	uint32_t iipm;
	PCB *my_p_pcb;

	/*
	 *	pre_lock()の返り値を直接saved_iipmに保存せず，一時変数iipmを
	 *	用いているのは，以下の理由による。
	 *	
	 *	１．シングルプロセッサと同じ理由
	 *	　pre_lock()を呼んだ直後に割込みが発生し，起動された割込み処理
	 *	　でPCB内のsaved_iipmが変更される可能性があるため
	 *	２．マルチプロセッサ固有の理由
	 *	　pre_lock()した後でないと、my_p_pcbにアクセスできない。
	 *	　（pre_lock()の戻り値を保存しておく必要がある。）
	 */
	iipm = pre_lock();	/*  要検討：本来の目的と違う使用方法  */

	my_p_pcb = get_my_p_pcb();
	my_p_pcb->target_pcb.saved_iipm = iipm;
	my_p_pcb->target_pcb.lock_flag = true;
	/* クリティカルセクションの前後でメモリが書き換わる可能性がある */
	Asm("":::"memory");
}

#define t_lock_cpu()	x_lock_cpu()
#define i_lock_cpu()	x_lock_cpu()

/*
 *	CPUロック状態の解除
 *
 *	lock_flagをfalseにし，IPM（ハードウェアの割込み優先度マスク）を，
 *	saved_iipmに保存した値に戻す．
 *
 *	この関数は，CPUロック状態（lock_flagがtrueの状態）でのみ呼ばれるも
 *	のと想定している．
 */
Inline void
x_unlock_cpu(void)
{
	PCB *my_p_pcb;
	uint32_t saved_iipm;

	my_p_pcb = get_my_p_pcb();
	my_p_pcb->target_pcb.lock_flag = false;
	saved_iipm = my_p_pcb->target_pcb.saved_iipm;

	/* クリティカルセクションの前後でメモリが書き換わる可能性がある */
	Asm("":::"memory");
	set_iipm(saved_iipm);
}

#define t_unlock_cpu()	  x_unlock_cpu()
#define i_unlock_cpu()	  x_unlock_cpu()

/*
 *	CPUロック状態の参照
 */
Inline bool_t
x_sense_lock(void)
{
	uint32_t iipm;
	PCB *my_p_pcb;
	bool_t lock_flag;

	iipm = pre_lock();		/*  CPUロック相当  */
	my_p_pcb = get_my_p_pcb();
	lock_flag = my_p_pcb->target_pcb.lock_flag;
	post_lock(iipm);		/*  元のCPUロック状態に戻す。  */

	return(lock_flag);
}

#define t_sense_lock()	  x_sense_lock()
#define i_sense_lock()	  x_sense_lock()

/*
 *  タスクコンテキスト かつ CPUロック解除状態なら true
 *  
 *  (!sense_context() && !t_sense_lock())でも判定できるが
 *  割込み禁止／許可のオーバーヘッドを減らすため、
 *  １つにまとめている。
 */
Inline bool_t
sense_tskctx_unl(void)
{
	uint32_t iipm;
	PCB *my_p_pcb;
	bool_t ret;

	iipm = pre_lock();		/*  CPUロック相当  */
	my_p_pcb = get_my_p_pcb();
	/*
	 *  CPUロック区間を短くするため、
	 *  このif文のロジックをCPUロック区間の外に移動することを
	 *  検討したが、非タスクコンテキストの場合もlock_flagを
	 *  読み出す必要があり、効率が悪いため、そのままとしている。
	 */
	if ((my_p_pcb->target_pcb.excnest_count == 0)	/*  タスクコンテキスト  */
		&& !(my_p_pcb->target_pcb.lock_flag)) {		/*  CPUロック解除状態  */
		ret = true;
	} else {
		ret = false;
	}
	post_lock(iipm);		/*  元のCPUロック状態に戻す。  */

	return(ret);
}

/*
 *  非タスクコンテキスト かつ CPUロック解除状態なら true
 *  
 *  (sense_context() && !i_sense_lock())でも判定できるが
 *  割込み禁止／許可のオーバーヘッドを減らすため、
 *  １つにまとめている。
 */
Inline bool_t
sense_intctx_unl(void)
{
	uint32_t iipm;
	PCB *my_p_pcb;
	bool_t ret;

	iipm = pre_lock();		/*  CPUロック相当  */
	my_p_pcb = get_my_p_pcb();
	/*
	 *  CPUロック区間を短くするため、
	 *  このif文のロジックをCPUロック区間の外に移動することを
	 *  検討したが、タスクコンテキストの場合もlock_flagを
	 *  読み出す必要があり、効率が悪いため、そのままとしている。
	 */
	if ((my_p_pcb->target_pcb.excnest_count != 0)	/*  非タスクコンテキスト  */
		&& !(my_p_pcb->target_pcb.lock_flag)) {		/*  CPUロック解除状態  */
		ret = true;
	} else {
		ret = false;
	}
	post_lock(iipm);		/*  元のCPUロック状態に戻す。  */

	return(ret);
}


/*
 *	chg_ipmで有効な割込み優先度の範囲の判定
 *
 *	TMIN_INTPRIの値によらず，chg_ipmでは，-15〜TIPM_ENAALL（＝0）の範囲
 *	に設定できることとする（ターゲット定義の拡張）．
 */
#define VALID_INTPRI_CHGIPM(intpri) \
				((-15 <= (intpri)) && ((intpri) <= TIPM_ENAALL))

/*
 * （モデル上の）割込み優先度マスクの設定
 *
 *	CPUロックフラグがクリアされている時は，ハードウェアの割込み優先度マ
 *	スクを設定する．
 *	
 *	CPUロックフラグがセットされている時は，ハードウェアの割込み優先度マ
 *	スクを，設定しようとした（モデル上の）割込み優先度マスクとTIPM_LOCK
 *	の高い方に設定する．
 *
 */
Inline void
x_set_ipm(PRI intpri)
{
	PCB *my_p_pcb;
	uint32_t saved_iipm;
	uint32_t iipm = INT_IPM(intpri);

	saved_iipm = pre_lock();		/*  CPUロック相当  */
	my_p_pcb = get_my_p_pcb();
	
	if (!(my_p_pcb->target_pcb.lock_flag)) {
		/*  元がCPUロック解除状態  */
		set_iipm(iipm);
	}
	else {
		/*
		 *	元がCPUロック状態なので、カーネル管理の割込みは入らない。
		 *	（target_pcb.saved_iipmの整合性は問題ない。）
		 */
		my_p_pcb->target_pcb.saved_iipm = iipm;
#ifndef TOPPERS_SUPPORT_NONKERNEL_INT
						/*  カーネル管理外の割込みをサポートしない場合  */
		/*
		 *	この時点でハードウェアの割込み優先度マスクが必ず15に
		 *	設定されているため，設定し直す必要がない．
		 */
#else /* TOPPERS_SUPPORT_NONKERNEL_INT */
						/*  カーネル管理外の割込みをサポートする場合  */
		set_iipm((iipm > IIPM_LOCK) ? iipm : IIPM_LOCK);
#endif /* TOPPERS_SUPPORT_NONKERNEL_INT */
	}
	/* IPMを書き換える前後でメモリが書き換わる可能性がある */
	Asm("":::"memory");
}

#define t_set_ipm(intpri)	 x_set_ipm(intpri)
#define i_set_ipm(intpri)	 x_set_ipm(intpri)

/*
 * （モデル上の）割込み優先度マスクの参照
 *
 *	CPUロックフラグがクリアされている時はハードウェアの割込み優先度マ
 *	スクを，セットされている時はPCB内のsaved_iipmを参照する．
 */
Inline PRI
x_get_ipm(void)
{
	PCB *my_p_pcb;
	uint32_t saved_iipm;
	uint32_t iipm;

	saved_iipm = pre_lock();		/*  CPUロック相当  */
	my_p_pcb = get_my_p_pcb();
	if (!(my_p_pcb->target_pcb.lock_flag)) {
		/*  元がCPUロック解除状態  */
		iipm = saved_iipm;
	}
	else {
		/*  元がCPUロック状態  */
		iipm = my_p_pcb->target_pcb.saved_iipm;
	}
	post_lock(saved_iipm);			/*  元のCPUロック状態に戻す。  */
	return(EXT_IPM(iipm));
}

#define t_get_ipm()    x_get_ipm()
#define i_get_ipm()    x_get_ipm()

/*
 *	最高優先順位タスクへのディスパッチ（prc_support.S）
 *
 *	dispatchは，タスクコンテキストから呼び出されたサービスコール処理か
 *	ら呼び出すべきもので，
 *	 ・タスクコンテキスト
 *	 ・CPUロック状態
 *	 ・ディスパッチ許可状態
 *	 ・（モデル上の）割込み優先度マスク全解除状態
 *	で呼び出さなければならない．
 */
extern void dispatch(void);

/*
 *  自タスクのマイグレーション（prc_support.S）
 */
extern void dispatch_and_migrate(ID prcid);

/*
 *  現在のコンテキストを捨てマイグレーション（prc_support.S）
 */
extern void exit_and_migrate(ID prcid);

/*
 *	ディスパッチャの動作開始（prc_support.S）
 *
 *	start_dispatchは，カーネル起動時に呼び出すべきもので，すべての割込
 *	みを禁止した状態（全割込みロック状態と同等の状態）で呼び出さなければ
 *	ならない．
 */
extern void start_dispatch(void) NoReturn;

/*
 *	現在のコンテキストを捨ててディスパッチ（prc_support.S）
 *
 *	exit_and_dispatchは，ext_tskから呼び出すべきもので，
 *	 ・タスクコンテキスト
 *	 ・CPUロック状態
 *	 ・ディスパッチ許可状態
 *	 ・（モデル上の）割込み優先度マスク全解除状態
 *	で呼び出さなければならない．
 */
extern void exit_and_dispatch(void) NoReturn;

/*
 *	カーネルの終了処理の呼出し（prc_support.S）
 *
 *	call_exit_kernelは，カーネルの終了時に呼び出すべきもので，非タスク
 *	コンテキストに切り換えて，カーネルの終了処理（exit_kernel）を呼び出
 *	す．
 */
extern void call_exit_kernel(void) NoReturn;

/*
 *	タスクコンテキストの初期化
 *
 *	タスクが休止状態から実行できる状態に移行する時に呼ばれる．この時点
 *	でスタック領域を使ってはならない．
 *
 *	activate_contextを，インライン関数ではなくマクロ定義としているのは，
 *	この時点ではTCBが定義されていないためである．
 */
extern void    start_r(void);

#define activate_context(p_tcb) 										\
{																		\
	(p_tcb)->tskctxb.sp = (void *)((char_t *)((p_tcb)->p_tinib->stk)	\
										+ (p_tcb)->p_tinib->stksz); 	\
	(p_tcb)->tskctxb.pc = (FP)start_r;									\
}

/*
 *	calltexは使用しない
 */
#define OMIT_CALLTEX

/*
 * ターゲット非依存部に含まれる標準の例外管理機能の初期化処理を用いない
 */
#define OMIT_INITIALIZE_EXCEPTION

/*
 * CPU例外ハンドラの初期化
 */
Inline void
initialize_exception(void)
{
}

/*
 *	SH2A-DUALの割込みアーキテクチャと割込み処理モデルの実現
 */

/*
 *	割込み優先度
 *
 *	各割込みの割込み優先度は割込み優先レベル設定レジスタ(IPRXX)によっ
 *	て設定する．そのため，各割込み要求ライン毎にどの割込み優先レベル設
 *	定レジスタのどのフィールドに対応しているかの情報（IPR_INFO型）の
 *	テーブルipr_info_tbl[]を用意する．
 *	割込み番号は連続していないため，サポートしていない番号には，
 *	addressエントリに0を設定し、これにより割込み番号の妥当性を判別でき
 *	る。
 *	（サポートする割込み番号はアプリケーションによらず、プロセッサの
 *	ハードウェア仕様によって決まるので、定数データとして保持する。）
 *	このテーブルのインデックスには、割込み番号の内部表現を用いる。
 */

/*
 *	各割込み番号毎の設定するIPRの情報管理のための構造体
 *	
 *	　サポートしない割込み番号の場合は、addressエントリに0を設定する。
 *	
 *	　メンバoffsetは8ビットあれば十分だが、下記の理由で32ビットにして
 *	　いる。
 *	　　・この構造体を配列にすると隙間ができるだけで、メモリの節約に
 *	　　　ならない。
 *	　　・SHでは32ビットデータでないと無駄な命令が生成される。
 */
typedef struct {
	/* 割込み優先レベル設定レジスタのアドレス */
	uint32_t address;
	/* オフセット */
	uint32_t  offset;
} IPR_INFO;

/*
 *	各割込み番号毎のIPRの情報管理テーブル
 */
extern const IPR_INFO ipr_info_tbl[TNUM_INT];

/*
 *	割込み番号・割込みハンドラ番号
 *
 *	割込みハンドラ番号(inhno)と割込み番号(intno)は，ベクタ番号を用いる．
 *
 *	ベクタ番号はTMIN_INTNO(=64)から始まるため，そのままの値を優先度管
 *	理等のテーブルのインデックスに用いると，無駄な領域が発生する．
 *	そのため，カーネル内部では，TMIN_INTNOを差し引いた値（内部表現）
 *	を用いる．
 *
 *	内部表現の名前は，iintno,iinhnoとする．
 */

/*
 * 割込みハンドラ番号の内部・外部表現相互変換
 */
											/* 内部表現を外部表現に */
#define EXT_INHNO(iintno) (CAST(uint32_t, (iinhno) + TMIN_INHNO))
											/* 外部表現を内部表現に */
#define INT_INHNO(intno)  (CAST(uint32_t, (inhno) - TMIN_INHNO))

/*
 * 割込み番号の内部・外部表現相互変換
 */
											/* 内部表現を外部表現に */
#define EXT_INTNO(iintno)	(CAST(uint32_t, (iintno) + TMIN_INTNO))
											/* 外部表現を内部表現に */
#define INT_INTNO(intno)	(CAST(uint32_t, (intno) - TMIN_INTNO))


/*
 *	割込み番号の範囲の判定
 *	
 *	TMIN_INTNO〜TMAX_INTNOの範囲であっても有効な番号でない場合があるた
 *	め，ipr_info_tbl[]で有効な番号かをチェックする
 */
/*  割り付け可能プロセッサのチェック 要検討：プロセッサ数依存 */
#define VALID_AFFINITY_MASK(intno)							\
		(((intno) & ~LOCAL_INTNO_MASK) <= 0x20000U)	

/*  IRQまたは内蔵周辺モジュールの割込み番号の判定  */
#define VALID_LOCAL_INTNO_IRQ_OR_PERIPHERAL(local_intno)	\
		((TMIN_INTNO <= (local_intno)) 						\
		&& ((local_intno) <= TMAX_INTNO) 					\
		&& (ipr_info_tbl[INT_INTNO(local_intno)].address != 0U))

/*  プロセッサ間割込みの割込み番号の判定  */
#define VALID_LOCAL_INTNO_IPI(local_intno) 					\
		LOCAL_INTNO_IS_IPI(local_intno)

#define VALID_LOCAL_INTNO(local_intno) 					\
		(VALID_LOCAL_INTNO_IRQ_OR_PERIPHERAL(local_intno)	\
		 || VALID_LOCAL_INTNO_IPI(local_intno))

#define VALID_INTNO(intno) 									\
		(VALID_AFFINITY_MASK(intno) && VALID_LOCAL_INTNO(LOCAL_INTNO(intno)))
#define VALID_INTNO_DISINT(intno)	VALID_INTNO(intno)
#define VALID_INTNO_CFGINT(intno)	VALID_INTNO(intno)

/*
 *	CPU例外ハンドラ番号
 *
 *	CPU例外ハンドラ番号としては，ベクタ番号を用いる．
 *	サポートするCPU例外ハンドラ数はプロセッサの型番毎に異なるので，プロ
 *	セッサの型番毎の定義ファイルに定義する．
 *	
 *	TMIN_EXCNO〜TMAX_EXCNOの範囲であっても有効な番号でない場合があるが、
 *	チェックを省略している。
 *
 */
#define VALID_LOCAL_EXCNO_DEFEXC(local_excno) \
		((TMIN_EXCNO <= (local_excno)) && ((local_excno) <= TMAX_EXCNO))
#define VALID_EXCNO_DEFEXC(excno) 	\
		(VALID_AFFINITY_MASK(excno)	\
		&& VALID_LOCAL_EXCNO_DEFEXC(LOCAL_EXCNO(excno)))

/*
 *	割込みハンドラの設定
 *
 *	割込みハンドラ番号inhnoの割込みハンドラの起動番地int_entryに
 *	設定する．
 */
Inline void
x_define_inh(INHNO inhno, FP int_entry, uint_t affinity_mask)
{
#ifndef NDEBUG
	uint_t prcid_in_inhno = inhno >> 16U;
	uint_t bitptn = 1U << (prcid_in_inhno - 1U);

	assert((bitptn & affinity_mask) != 0);
#endif /* NDEBUG */
}

/*
 *	割込みハンドラの出入口処理の生成マクロ
 *
 */
#define INT_ENTRY(inhno, inthdr)	 _kernel_##inthdr##_##inhno
#define INTHDR_ENTRY(inhno, inhno_num, inthdr)					\
		extern void _kernel_##inthdr##_##inhno(void);

/*
 *	割込み要求禁止フラグ
 *
 *	SH2A-DUALのIRCは割込み要求禁止フラグを持たない．割込みを個別に禁止する
 *	場合は，割込み優先レベル設定レジスタ(IPRXX)を0に設定する必要がある．
 *	割込み優先レベル設定レジスタを0にすることで割込み要求禁止フラグを
 *	実現すると、その間、モデル上の割込み優先度を記憶しておく領域が必要
 *	となる．コンフィギュレータでこのためのuint8_t型のテーブル
 *	int_iipm_tbl[]を用意する．
 *	割込み優先度が設定されているかどうかはビット7で保持する。
 *	（アプリケーションに依存した情報）
 *	
 *	ビット割り当てが割込み優先度の外部表現とも内部表現とも異なるので、
 *	注意すること。（符号を反転しているが、ビットシフトしていない。）
 *	
 *	テーブルint_iipm_tbl[][]に格納する値のビット割り当て：
 *	　　　ビット0〜3：IPRXXに設定するビットパターン
 *	　　　　　　　　　（割込み優先レベル）
 *	　　　ビット7：NOT_CFG_INT_BIT
 *	　　　　　　　　0:CFG_INTで割込み優先度が設定されている。
 *	　　　　　　　　1:CFG_INTで割込み優先度が設定されていない。
 */
extern const uint8_t int_iipm_tbl[TNUM_PRCID][TNUM_INT];

/*	CFG_INTで割込み優先度が設定されていないことを示すビット  */
#define NOT_CFG_INT_BIT		0x80U

/*
 * 割込み優先レベル設定レジスタの設定
 *	 INTNO local_intno：プロセッサ内でのローカルな割込み番号
 *	 uint32_t val：IPRレジスタに設定する値
 *	 uint_t prcindex：プロセッサ・インデックス（0オリジン）
 */
Inline void
irc_set_ipr(INTNO local_intno, uint32_t val, uint_t prcindex)
{
	uint32_t ipr_val;
	uint32_t iintno  	= INT_INTNO(local_intno);
	uint32_t address 	= ipr_info_tbl[iintno].address;
	uint32_t offset  	= ipr_info_tbl[iintno].offset;

   	/*	prcindex=1の場合はIPRのアドレスにオフセット0x100を加える  */
   	/*	　要検討：プロセッサ数に依存  */
	if (prcindex != 0) {
		address += PRC_IPR_OFFSET;
	}
	ipr_val = sil_reh_reg((uint16_t *)address);
	ipr_val &= ~(0x0FU << offset);
	ipr_val |= (val << offset);
	sil_wrh_reg((uint16_t *)address, ipr_val);
}

/*
 *	割込み要求禁止フラグのセット
 *
 *	割込み要因毎に割込み禁止の方法が異なる。
 *
 *	IRQ割込み
 *	　割込み禁止フラグがないため、割込み優先レベル設定レジスタ(IPRx)を
 *	　0に設定する．
 *
 *	内蔵周辺モジュール
 *	　IDCNTnレジスタのINTENビットをクリアする。
 *
 *	PINT割込み
 *	　CnPINTERレジスタのPINTnEビットをクリアする。
 *
 *	プロセッサ間割込み
 *	　CnIPERレジスタのCIPEnビットをクリアする。
 *
 *	要検討
 *	　将来的には、マクロ化してsh2a_dual_config.hに移動する。
 */

/*
 *	IRQ割込みの禁止
 *	 INTNO local_intno：プロセッサ内でのローカルな割込み番号
 *	 uint_t prcindex：プロセッサ・インデックス（0オリジン）
 *
 *	　割込み禁止フラグがないため、割込み優先レベル設定レジスタ(IPRx)を
 *	　0に設定する．
 */
Inline void
x_disable_int_irq(INTNO local_intno, uint_t prcindex)
{
	/*
	 *	割込み番号に対応したIPRの値を0にする．
	 */
	irc_set_ipr(local_intno, 0x00, prcindex);
}

/*
 *	内蔵周辺モジュール割込みの禁止
 *	 INTNO local_intno：プロセッサ内でのローカルな割込み番号
 *	 uint_t prcindex：プロセッサ・インデックス（0オリジン）
 *
 *	　IDCNTnレジスタのINTENビットをクリアする。
 */
Inline void
x_disable_int_peripheral(INTNO local_intno, uint_t prcindex)
{
	int_t index 		= LOCAL_INTNO_TO_IDCNT(local_intno);
	uint16_t *p_idcnt 	= idcnt_tbl[index];
	
	sil_anh_reg(p_idcnt, ~IDCNT_INTEN);
}
	
/*
 *	PINT割込みの禁止
 *	 INTNO local_intno：プロセッサ内でのローカルな割込み番号
 *	 uint_t prcindex：プロセッサ・インデックス（0オリジン）
 *
 *	　CnPINTERレジスタのPINTnEビットをクリアする。
 */
Inline void
x_disable_int_pint(INTNO local_intno, uint_t prcindex)
{
	uint_t pintno 		= LOCAL_INTNO_TO_PINTNO(local_intno);
	uint16_t *p_pinter 	= irc_reg_tbl[prcindex].pinter;
	
	sil_clrh_bit(p_pinter, pintno);
}

/*
 *	プロセッサ間割込みの禁止
 *	 INTNO local_intno：プロセッサ内でのローカルな割込み番号
 *	 uint_t prcindex：プロセッサ・インデックス（0オリジン）
 *
 *	　CnIPERレジスタのCIPEnビットをクリアする。
 */
Inline void
x_disable_int_ip(INTNO local_intno, uint_t prcindex)
{
	uint_t intlevel		= LOCAL_INTNO_TO_IP_INT_LEVEL(local_intno);
	uint16_t *p_iper 	= irc_reg_tbl[prcindex].iper;
	
	sil_clrh_bit(p_iper, intlevel);
}

/*
 *	割込み要求禁止フラグのセット
 *
 *	割込み属性が設定されていない割込み要求ラインに対して割込み要求禁止
 *	フラグをクリアしようとした場合には，falseを返す．
 *
 *	割込み要因毎に割込み禁止の方法が異なる。
 *
 *	IRQ割込み
 *	　割込み禁止フラグがないため、割込み優先レベル設定レジスタ(IPRx)を
 *	　0に設定する．
 *
 *	内蔵周辺モジュール
 *	　IDCNTnレジスタのINTENビットをクリアする。
 *
 *	PINT割込み
 *	　CnPINTERレジスタのPINTnEビットをクリアする。
 *
 *	プロセッサ間割込み
 *	　CnIPERレジスタのCIPEnビットをクリアする。
 */
Inline bool_t
x_disable_int(INTNO intno)
{
	INTNO iintno;
	uint32_t iipm;
	INTNO local_intno  	= LOCAL_INTNO(intno);
	uint_t prcindex		= x_prc_index();

	/*
	 *	プロセッサ間割込みの場合
	 *	　割込み優先レベルが固定でテーブルint_iipm_tblに含まれないので、
	 *	　他の割込み要因より先にチェックする。
	 */
	if (LOCAL_INTNO_IS_IPI(local_intno)) {
		x_disable_int_ip(local_intno, prcindex);
		return(true);
	}

	iintno = INT_INTNO(local_intno);
	iipm = int_iipm_tbl[prcindex][iintno];

	/*
	 *	割込み属性が設定されていない場合
	 */
	if ((iipm & NOT_CFG_INT_BIT) != 0) {
		return(false);
	}
	
	/*
	 *	残りの割込み要因毎に処理を振り分ける。
	 */
	if (LOCAL_INTNO_IS_IRQ(local_intno)) {
		/*	IRQの場合  */
		x_disable_int_irq(local_intno, prcindex);
	}
	else if (LOCAL_INTNO_IS_PERIPHERAL(local_intno)) {
		/*	内蔵周辺モジュールの場合  */
		x_disable_int_peripheral(local_intno, prcindex);
	}
	else if (LOCAL_INTNO_IS_PINT(local_intno)) {
		/*	PINT割込みの場合  */
		x_disable_int_pint(local_intno, prcindex);
	}
	else {
		return(false);
	}

	return(true);
}

#define t_disable_int(intno) x_disable_int(intno)
#define i_disable_int(intno) x_disable_int(intno)

/*
 *	割込み要求禁止フラグの解除
 *
 *	割込み要因毎に割込み許可の方法が異なる。
 *
 *	IRQ割込み
 *	　割込み禁止フラグがないため、割込み優先レベル設定レジスタ(IPRx)の
 *	　値を元に戻す。
 *
 *	内蔵周辺モジュール
 *	　IDCNTnレジスタのINTENビットをセットする。
 *
 *	PINT割込み
 *	　CnPINTERレジスタのPINTnEビットをセットする。
 *
 *	プロセッサ間割込み
 *	　CnIPERレジスタのCIPEnビットをセットする。
 *
 *	要検討
 *	　将来的には、マクロ化してsh2a_dual_config.hに移動する。
 */

/*
 *	IRQ割込みの許可
 *	 INTNO local_intno：プロセッサ内でのローカルな割込み番号
 *	 uint_t prcindex：プロセッサ・インデックス（0オリジン）
 *	 uint32_t ipr_val：IPRxレジスタに設定する値
 *
 *	　割込み禁止フラグがないため、割込み優先レベル設定レジスタ(IPRx)の
 *	　値を元に戻す。
 */
Inline void
x_enable_int_irq(INTNO local_intno, uint_t prcindex, uint32_t ipr_val)
{
	/*
	 *	割込み番号に対応したIPRの値をipr_valにする．
	 */
	irc_set_ipr(local_intno, ipr_val, prcindex);
}

/*
 *	内蔵周辺モジュール割込みの許可
 *	 INTNO local_intno：プロセッサ内でのローカルな割込み番号
 *	 uint_t prcindex：プロセッサ・インデックス（0オリジン）
 *
 *	　IDCNTnレジスタのINTENビットをセットする。
 */
Inline void
x_enable_int_peripheral(INTNO local_intno, uint_t prcindex)
{
	int_t index 		= LOCAL_INTNO_TO_IDCNT(local_intno);
	uint16_t *p_idcnt 	= idcnt_tbl[index];
	
	sil_orh_reg(p_idcnt, IDCNT_INTEN);
}
	
/*
 *	PINT割込みの許可
 *	 INTNO local_intno：プロセッサ内でのローカルな割込み番号
 *	 uint_t prcindex：プロセッサ・インデックス（0オリジン）
 *
 *	　CnPINTERレジスタのPINTnEビットをセットする。
 */
Inline void
x_enable_int_pint(INTNO local_intno, uint_t prcindex)
{
	uint_t pintno 		= LOCAL_INTNO_TO_PINTNO(local_intno);
	uint16_t *p_pinter 	= irc_reg_tbl[prcindex].pinter;
	
	sil_seth_bit(p_pinter, pintno);
}

/*
 *	プロセッサ間割込みの許可
 *	 INTNO local_intno：プロセッサ内でのローカルな割込み番号
 *	 uint_t prcindex：プロセッサ・インデックス（0オリジン）
 *
 *	　CnIPERレジスタのCIPEnビットをセットする。
 */
Inline void
x_enable_int_ip(INTNO local_intno, uint_t prcindex)
{
	uint_t intlevel		= LOCAL_INTNO_TO_IP_INT_LEVEL(local_intno);
	uint16_t *p_iper 	= irc_reg_tbl[prcindex].iper;
	
	sil_seth_bit(p_iper, intlevel);
}

/*
 *	割込み要求禁止フラグの解除
 *
 *	割込み属性が設定されていない割込み要求ラインに対して割込み要求禁止
 *	フラグをクリアしようとした場合には，falseを返す．
 *
 *	割込み要因毎に割込み許可の方法が異なる。
 *
 *	IRQ割込み
 *	　割込み禁止フラグがないため、割込み優先レベル設定レジスタ(IPRx)の
 *	　値を元に戻す。
 *
 *	内蔵周辺モジュール
 *	　IDCNTnレジスタのINTENビットをセットする。
 *
 *	PINT割込み
 *	　CnPINTERレジスタのPINTnEビットをセットする。
 *
 *	プロセッサ間割込み
 *	　CnIPERレジスタのCIPEnビットをセットする。
 */
Inline bool_t
x_enable_int(INTNO intno)
{
	INTNO iintno;
	uint32_t iipm;
	INTNO local_intno  	= LOCAL_INTNO(intno);
	uint_t prcindex		= x_prc_index();

	/*
	 *	プロセッサ間割込みの場合
	 *	　割込み優先レベルが固定でテーブルint_iipm_tblに含まれないので、
	 *	　他の割込み要因より先にチェックする。
	 */
	if (LOCAL_INTNO_IS_IPI(local_intno)) {
		x_enable_int_ip(local_intno, prcindex);
		return(true);
	}

	iintno = INT_INTNO(local_intno);
	iipm = int_iipm_tbl[prcindex][iintno];

	/*
	 *	割込み属性が設定されていない場合
	 */
	if ((iipm & NOT_CFG_INT_BIT) != 0) {
		return(false);
	}
	
	/*
	 *	残りの割込み要因毎に処理を振り分ける。
	 */
	if (LOCAL_INTNO_IS_IRQ(local_intno)) {
		/*	IRQの場合  */
		x_enable_int_irq(local_intno, prcindex, iipm);
	}
	else if (LOCAL_INTNO_IS_PERIPHERAL(local_intno)) {
		/*	内蔵周辺モジュールの場合  */
		x_enable_int_peripheral(local_intno, prcindex);
	}
	else if (LOCAL_INTNO_IS_PINT(local_intno)) {
		/*	PINT割込みの場合  */
		x_enable_int_pint(local_intno, prcindex);
	}
	else {
		return(false);
	}

	return(true);
}

#define t_enable_int(intno) x_enable_int(intno)
#define i_enable_int(intno) x_enable_int(intno)

/*
 *	割込み要求ラインの属性の設定
 *
 *	SH2A-DUALでは，カーネルで扱える割込み優先度は16段階であるため，intpri
 *	として与えることができる値は-15〜-1が標準である．
 *	
 *	SH2A-DUALの割込みは，内蔵周辺モジュール割込み，IRQ割込みに分類でき，そ
 *	れぞれ扱いが異なる．そのため，割込み番号から，どの割込みか分類する
 *	ためのマクロINTNO_IS_IRQ(intno)をコア依存部で用意する．
 *	本来はこのファイルで定義すべきだが、コア依存部ののshx_config.h中の
 *	x_config_intatr()で使用するため、shx_config.hに記述している。
 */
extern void x_config_int(INTNO intno, ATR intatr, PRI intpri, uint_t affinity_mask);

/*
 *	割込みハンドラの出口で必要なIRC操作
 *	　入口で必要なIRC操作i_begin_int()は、コア依存部で定義
 */
Inline void
i_end_int(INTNO intno)
{
}
#endif /* TOPPERS_MACRO_ONLY */

/*
 *	CPU例外ハンドラ関係
 */

/*
 *	例外ハンドラ引数 p_excinf から各種情報を取り出すためのマクロ
 */
#define P_EXCINF_OFFSET_VECTOR UINT_C( 0)
#define P_EXCINF_OFFSET_PR	   UINT_C( 1)
#define P_EXCINF_OFFSET_PC	   UINT_C(12)
#define P_EXCINF_OFFSET_SR	   UINT_C(13)

#define P_EXCINF_OFFSET_R0	   UINT_C( 4)

/*	CPU例外発生時のスタックポインタの値とp_excinfの差分  */
#define P_EXCINF_OFFSET_SP	   (P_EXCINF_OFFSET_SR + 1)


#ifndef TOPPERS_MACRO_ONLY
/*
 *	CPU例外ハンドラの設定
 */
Inline void
x_define_exc(EXCNO excno, FP exc_entry)
{
	assert(VALID_EXCNO_DEFEXC(excno));
}

/*
 *	CPU例外ハンドラの入口処理の生成マクロ
 */
#define EXC_ENTRY(excno, exchdr)	 _kernel_##exchdr##_##excno
#define EXCHDR_ENTRY(excno, excno_num, exchdr)					\
		extern void _kernel_##exchdr##_##excno(void *sp);

/*
 *	CPU例外が発生した時のコンテキストの参照
 *
 *	CPU例外が発生した時のコンテキストが，タスクコンテキストの時にfalse，
 *	そうでない時にtrueを返す．
 *	CPU例外ハンドラから呼ばれることを想定しているので、途中で
 *	マイグレーションされることは想定していない。
 */
Inline bool_t
exc_sense_context(void *p_excinf)
{
	PCB *my_p_pcb = get_my_p_pcb();
	
	return(my_p_pcb->target_pcb.excnest_count > 1U);
}

/*
 *	CPU例外が発生した時のIPM（ハードウェアの割込み優先度マスク，内部表
 *	現）の参照
 *	CPU例外ハンドラから呼ばれることを想定しているので、途中で
 *	マイグレーションされることは想定していない。
 */
Inline uint32_t
exc_get_iipm(void *p_excinf)
{
	uint32_t *sp = (uint32_t *)p_excinf;
	uint32_t sr = *(sp + P_EXCINF_OFFSET_SR);
	uint32_t iimp = sr & 0x00F0U;
	return(iimp);
}

/*
 *	CPU例外が発生した時のコンテキストと割込みのマスク状態の参照
 *
 *	CPU例外が発生した時のシステム状態が，
 *	　・カーネル実行中でなく，
 *	　・タスクコンテキストであり，
 *	　・全割込みロック状態でなく，
 *	　・CPUロック状態でなく，
 *	　・（モデル上の）割込み優先度マスク全解除状態
 *	である時にtrue，そうでない時にfalseを返す（CPU例外がカーネル管理外
 *	の割込み処理中で発生した場合にもfalseを返す）．
 *
 *	SH2A-DUALでは，CPU例外が発生した時のIPM（ハードウェアの割込み優先度マ
 *	スク）がすべての割込みを許可する状態であることをチェックすることで，
 *	　・カーネル実行中でないこと，
 *	　・全割込みロック状態でないこと，
 *	　・CPUロック状態でないこと，
 *	　・（モデル上の）割込み優先度マスク全解除状態であること
 *	の4つの条件をチェックすることができる（CPU例外が発生した時の
 *	lock_flagを参照する必要はない）．
 */
Inline bool_t
exc_sense_intmask(void *p_excinf)
{
	return(!exc_sense_context(p_excinf)
					&& (exc_get_iipm(p_excinf) == IIPM_ENAALL));
}

/*
 *	CPU例外が発生した時のコンテキストと割込み／CPUロック状態の参照
 *
 *	CPU例外が発生した時のシステム状態が，
 *	 ・カーネル実行中でなく，
 *	 ・タスクコンテキストであり，
 *	 ・全割込みロック状態でなく，
 *	 ・CPUロック状態でない
 *	時にtrue，そうでない時にfalseを返す（CPU例外がカーネル管理外の割込
 *	み処理中で発生した場合にもfalseを返す）．
 *
 *	SH2A-DUALでは，CPU例外が発生した時のIPM（ハードウェアの割込み優先度マ
 *	スク）がTIPM_LOCK(=TMIN_INTPRI)より低いことをチェックすることで，
 *	 ・カーネル実行中でないこと，
 *	 ・全割込みロック状態でないこと，
 *	 ・CPUロック状態でないこと
 *	の3つの条件をチェックしている（CPU例外が発生した時のlock_flagは参
 *	照していない）．これにより，（モデル上の）割込み優先度マスクを
 *	TIPM_LOCK以上に設定してタスクを実行している時にもfalseが返ってしま
 *	うが，判断を正確にするためのオーバヘッドが大きいことから，許容する
 *	ことにする．
 */
Inline bool_t
exc_sense_unlock(void *p_excinf)
{
	return(!exc_sense_context(p_excinf)
					&& exc_get_iipm(p_excinf) < IIPM_LOCK);
}

/*
 * Trapa以外の例外で登録されていない例外が発生すると呼び出される
 */
extern void default_exc_handler(void *p_excinf);

/*
 * 未登録の割込みが発生した場合に呼び出される
 * 　APIが標準的なものと異なるため、ユーザーが用意したもので
 * 　置き換える場合は、プロトタイプ宣言も削除する。
 */
#ifndef OMIT_DEFAULT_INT_HANDLER
extern void default_int_handler(INTNO intno);
#endif /* OMIT_DEFAULT_INT_HANDLER */

/*
 *	プロセッサ依存の初期化
 */
extern void prc_initialize(void);

/*
 *	プロセッサ依存の終了時処理
 */
extern void prc_exit(void);

/*
 *  ロック関係
 */

/*  ロックに用いるセマフォレジスタ  */
#define SEMR_LOCK	SEMR1

/*
 *  ジャイアントロックの初期化
 */
Inline void
x_initialize_giant_lock(LOCK *p_giant_lock)
{
		/*
		 *	SEMR == 0 ：使用中
		 *	SEMR == 1 ：空き
		 */
	sil_wrb_reg((uint8_t *)SEMR_LOCK, SEMR_SEMF_BIT);
	*p_giant_lock = (uint8_t *)SEMR_LOCK;
}

/*
 *  Test & Set命令
 *  　true ：使用中
 *  　false：空き
 */
Inline bool_t
test_and_set(uint8_t *p_semr)
{
	uint8_t semr = sil_reb_reg(p_semr);
	
	if (semr == 0) {
		return true;
	}
	return false;
}

/*
 *  ロック取得解放関数
 *  　x_lock_cpu()/x_unlock_cpu()を使うため，これらが定義された後で
 *  　定義する． 
 */

/*
 *  1段目のロック取得（タスク・非タスクコンテキスト共用）
 */
Inline void
x_acquire_lock(LOCK *p_lock)
{
	uint8_t *p_semr = *p_lock;

	while (test_and_set(p_semr)) {
		x_unlock_cpu();
		x_lock_cpu();
	}

	/* メモリの内容が書き換わる可能性がある */
	Asm("":::"memory");
}

#define t_acquire_lock(p_lock) x_acquire_lock(p_lock)
#define i_acquire_lock(p_lock) x_acquire_lock(p_lock)


/*
 *  ロックの解放（タスク・非タスクコンテキスト共用）
 */
Inline void
x_release_lock(LOCK *p_lock)
{
	/* メモリの内容が書き換わる可能性がある */
	Asm("":::"memory");
	sil_wrb_reg(*p_lock, SEMR_SEMF_BIT);
}

/*
 *  割込みを許可しないロック取得関数
 */
Inline void 
x_acquire_lock_without_preemption(LOCK *p_lock)
{
	uint8_t *p_semr = *p_lock;

	/*  使用中の間、ループを繰り返す  */
	while (test_and_set(p_semr));
	
	/* メモリの内容が書き換わる可能性がある */
	Asm("":::"memory");
}


/*
 *  スピンロック関係
 */

/*
 *  スピンロック機能の方式
 */
#define TTYPE_SPN NATIVE_SPN

/*
 *  ネイティブスピン方式の場合のスピンロックの最大数
 *  　-2しているのは、ロック操作とSILで使用する分
 */
#define TMAX_NATIVE_SPN   (32 - 2)

/*
 *  スピンロックの型
 */
typedef uint8_t *SPNLOCK;

/*
 *  スピンロックの初期化
 */
Inline void
x_initialize_spin(ID spnid, SPNLOCK *p_spn_lock)
{
	uint8_t *p_semr = semr_tbl[spnid + 2];
	
	*p_spn_lock = p_semr;
	sil_wrb_reg(p_semr, SEMR_SEMF_BIT);
}

/*
 *  スピンロックの取得
 */
Inline void
x_lock_spin(SPNLOCK *p_spn_lock)
{
	x_acquire_lock(p_spn_lock);
}

#define t_lock_spin(p_spn_lock)  x_lock_spin(p_spn_lock)
#define i_lock_spin(p_spn_lock)  x_lock_spin(p_spn_lock)

/*
 *  スピンロックの取得の試行
 */
Inline bool_t
x_try_lock_spin(SPNLOCK *p_spn_lock)
{
	uint8_t *p_semr = *p_spn_lock;

	if (test_and_set(p_semr)) {
		/* 失敗 */
		return(true);
	}
	else {
		/* 成功 */
		Asm("":::"memory");
		return(false);
	}
}

/*
 *  スピンロックの返却
 */
Inline void
x_unlock_spin(SPNLOCK *p_spn_lock)
{
	x_release_lock(p_spn_lock);
}


#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_PRC_CONFIG_H */
