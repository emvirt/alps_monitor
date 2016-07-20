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
 *  @(#) $Id: prc_config.h 540 2010-02-17 10:48:07Z ertl-honda $
 */


/*
 *		プロセッサ依存モジュール（SH4A用）
 *
 *  このインクルードファイルは，target_config.h（または，そこからインク
 *  ルードされるファイル）のみからインクルードされる．他のファイルから
 *  直接インクルードしてはならない．
 */

#ifndef TOPPERS_PRC_CONFIG_H
#define TOPPERS_PRC_CONFIG_H

/*
 * プロセッサの種類
 */
#define SH4A

#ifdef FPU_ENABLE

/*
 *  FPSCRのPRビットの初期値
 */
#ifdef FPU_DOUBLE
#define FPSCR_PR_INIT 0x00080000  /* 倍精度 */
#else  /* FPU_SINGLE FPU_SINGLE_ONLY */
#define FPSCR_PR_INIT 0x0000000   /* 単精度 */
#endif /* FPU_DOUBLE */

/*
 *  FPSCRのPRビット以外の設定初期値
 */
#ifndef FPSCR_INIT
#define FPSCR_INIT 0x00040000
#endif  /* FPSCR_INIT */

#endif /* FPU_ENABLE */

/*
 * 割込みハンドラ番号の内部・外部表現相互変換
 */
#define EXT_INHNO(iintno) (CAST(uint16_t, (iinhno) << 5)) /* 内部表現を外部表現に */
#define INT_INHNO(intno)  (CAST(uint16_t, (inhno) >> 5))  /* 外部表現を内部表現に */

/*
 * 割込み番号の内部・外部表現相互変換
 */
#define EXT_INTNO(iintno)   (CAST(uint16_t, (iintno) << 5)) /* 内部表現を外部表現に */
#define INT_INTNO(intno)    (CAST(uint16_t, (intno) >> 5))  /* 外部表現を内部表現に */

/*
 * アセンブラで割込み番号から inh_tbl を引くためのマクロ
 * regには，割込み番号を保持している
 * レジスタを指定する 
 */
#ifdef TOPPERS_ASM_MACRO
.macro index_intno reg
    shlr2 \reg
    shlr  \reg
.endm
#endif /* TOPPERS_ASM_MACRO */
     
/*
 * CPU例外ハンドラ番号に関する定義
 */ 
#define TMIN_EXCNO 0x040U
#define TMAX_EXCNO 0x820U
#define TNUM_EXC   0x42U
     
/*
 * CPU例外ハンドラ番号の内部・外部表現相互変換
 */
#define EXT_EXCNO(iexcno)  (CAST(uint16_t, (iexcno) << 5)) /* 内部表現を外部表現に */
#define INT_EXCNO(excno)   (CAST(uint16_t, (excno)  >> 5)) /* 外部表現を内部表現に */

/*
 * アセンブラでCPU例外番号から exch_tbl を引くためのマクロ
 * regには，EXPEVTレジスタの値を保持している
 * レジスタを指定する 
 */
#ifdef TOPPERS_ASM_MACRO
.macro index_excno reg
    shlr2 \reg
    shlr  \reg
.endm
#endif /* TOPPERS_ASM_MACRO */

#ifndef TOPPERS_MACRO_ONLY
  
#endif /* TOPPERS_MACRO_ONLY */

/*
 *  サポートする機能の定義
 */
#define SUPPORT_DIS_INT            /* dis_intをサポートする */
#define SUPPORT_ENA_INT            /* ena_intをサポートする */


/*
 *  キャッシュモードの設定
 *    P1(0x8..)              : ライトスルー
 *    P0(0x0..),U0,P3(0x3..) : コピーバック
 */
#ifdef ENABLE_CACHE_WRITE_BACK

#ifdef ENABLE_CACHE_COHERENCE
#define CCR_MODE  UINT_C(0x00000101) /* コヒーレンシ有効　ライトバック */
#else  /* ENABLE_CACHE_COHERENCE */
#define CCR_MODE  UINT_C(0x00010101) /* コヒーレンシ無効　ライトバック */
#endif /* ENABLE_CACHE_COHERENCE */

#else /* ENABLE_CACHE_WRITE_BACK */

#ifdef ENABLE_CACHE_COHERENCE
#define CCR_MODE  UINT_C(0x00000103) /* コヒーレンシ有効　ライトスルー */
#else  /* ENABLE_CACHE_COHERENCE */
#define CCR_MODE  UINT_C(0x00010103) /* コヒーレンシ無効　ライトスルー */
#endif /* ENABLE_CACHE_COHERENCE */

#endif /* ENABLE_CACHE_WRITE_BACK */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  プロセッサの特殊命令のインライン関数定義
 */
#include "prc_insn.h"

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

/* CPU間割込み用レジスタ  */
static const uint_t int_ici_tbl[TNUM_PRCID] = {
	C0INTICI,
#if TNUM_PRCID >= 2 
	C1INTICI,
#endif /* TNUM_PRCID >= 2 */
#if TNUM_PRCID >= 3
	C2INTICI,
#endif /* TNUM_PRCID >= 3 */
#if TNUM_PRCID >= 4
	C3INTICI,
#endif /* TNUM_PRCID >= 4 */
};

static const uint_t int_ici_clr_tbl[TNUM_PRCID] = {
	C0INTICICLR,
#if TNUM_PRCID >= 2
	C1INTICICLR,
#endif /* TNUM_PRCID >= 2 */
#if TNUM_PRCID >= 3
	C2INTICICLR,
#endif /* TNUM_PRCID >= 3 */
#if TNUM_PRCID >= 4
	C3INTICICLR,
#endif /* TNUM_PRCID >= 4 */
};

Inline void
ici_int(uint_t prcindex)
{
	sil_wrw_mem((void *)int_ici_tbl[prcindex], (1 << (prcindex<<2)));
}

Inline void
ici_int_clr(void)
{
	uint_t prcindex = x_prc_index();

	sil_wrw_mem((void *)int_ici_clr_tbl[prcindex],
				sil_rew_mem((void *)int_ici_tbl[prcindex]));
}

/*
 *  非タスクコンテキスト用のスタック初期値
 */
#define TOPPERS_ISTKPT(istk, istksz) ((STK_T *)((char *)(istk) + (istksz)))

/*
 *  割込み番号のマスク（下位16bitのみが有効）
 */
#define INTNO_MASK(intno) (intno & 0xffffU)

/*
 *  割込みハンドラ番号のマスク（下位16bitのみが有効）
 */
#define INHNO_MASK(intno) (intno & 0xffffU)

/*
 *  タスクコンテキストブロックの定義
 */
typedef struct task_context_block {
	void	*sp;		/* スタックポインタ */
	FP		pc;			/* プログラムカウンタ */
} CTXB;

/*
 *  sta_ker 内でのバリア同期
 */
extern void sta_ker_barrier_sync(void);

extern void x_config_int(INTNO intno, ATR intatr, PRI intpri, uint_t affinity_mask);
#endif /* TOPPERS_MACRO_ONLY */

/*
 *  割込み優先度マスク操作ライブラリ
 *  
 *  SH34では，ステータスレジスタ（SR）の4〜7ビットめの4ビットに割込み優先度
 *  マスク（ハードウェアの割込み優先度マスク，IPM）が置かれている．IPMを保
 *  存しておくために，割込み優先度の外部表現（-1から連続した負の値）を使う
 *  ことも可能であるが，余計な左右ビットシフトと符号反転が必要になる．これ
 *  を避けるために，IPMを保存する場合には，SRの4〜7ビットめを取り出した値
 *  を使うことにする．この値を割込み優先度マスクの内部表現と呼び，IIPMと書
 *  くことにする．
 */

/*
 *  割込み優先度マスクの外部表現と内部表現の変換
 *
 *  アセンブリ言語のソースファイルからインクルードする場合のために，CASTを使用
 */
#define EXT_IPM(iipm)    (-CAST(PRI,(iipm) >> 4))       /* 内部表現を外部表現に */
#define INT_IPM(ipm)     (CAST(uint16_t, -(ipm)) << 4)  /* 外部表現を内部表現に */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  IPM（ハードウェアの割込み優先度マスク，内部表現）の現在値の読出し
 */
Inline uint8_t
current_iipm(void)
{
	return((uint8_t)(current_sr() & 0xF0U));
}

/*
 *  IPM（ハードウェアの割込み優先度マスク，内部表現）の現在値の設定
 */
Inline void
set_iipm(uint8_t iipm)
{
	set_sr((current_sr() & ~0xF0U) | iipm);
}

/*
 *  TOPPERS標準割込み処理モデルの実現
 *
 *  SH34はステータスレジスタ(SR)内に割込み優先度マスク(IPM)を持っている．
 *  CPUロックフラグに相当機能を持たない．そのため，擬似的にCPUロックフ
 *  ラグを実現する．
 *
 *  まず，CPUロック状態を管理すための変数(lock_flag)を用意する．
 *
 *  CPUロックフラグがクリアされている間は，IPMをモデル上の割込み優先度
 *  マスクの値に設定する．この間は，モデル上の割込み優先度マスクは，
 *  IPMを用いる．
 * 
 *  それに対してCPUロックフラグがセットされいる間は，IPMを，カーネル管
 *  理外のものを除くすべての割込み要求をマスクする値(TIPM_LOCK)と，モデ
 *  ル上の割込み優先度マスクとの高い方に設定する．この間のモデル上の割
 *  込み優先度マスクは，そのための変数(saved_iipm, 内部表現で保持)を用
 *  意して保持する．
 */

/*
 *  コンテキストの参照
 *
 *  SH34では，コンテキストの判定に，割込みのネスト回数を管理する
 *  R7_BANK1を用いる．
 * 
 */
Inline bool_t
sense_context(void)
{
	uint32_t nest;

	Asm("stc r7_bank,%0" : "=r"(nest));
	return(nest > 0U);
}

/*
 *  CPUロック状態の参照
 */
Inline bool_t
x_sense_lock(void)
{
    return(get_my_p_tpcb()->lock_flag);
}

#define t_sense_lock()    x_sense_lock()
#define i_sense_lock()    x_sense_lock()

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  CPUロック状態での割込み優先度マスク
 *
 *  TIPM_LOCKは，CPUロック状態での割込み優先度マスク，すなわち，カーネ
 *  ル管理外のものを除くすべての割込みをマスクする値に定義する．
 */
#define TIPM_LOCK    TMIN_INTPRI

/*
 *  CPUロック状態での割込み優先度マスクの内部表現
 */
#define IIPM_LOCK    INT_IPM(TIPM_LOCK)

/*
 *  TIPM_ENAALL（割込み優先度マスク全解除）の内部表現
 */
#define IIPM_ENAALL  INT_IPM(TIPM_ENAALL)

#ifndef TOPPERS_MACRO_ONLY

/*
 *  CPUロックフラグ実現のための変数
 * 
 *  これらの変数は，CPUロック状態の時のみ書き換えてもよいとする．
 *  インライン関数中で，アクセスの順序が変化しないよう，volatile を指定． 
 */

/*
 *  CPUロック状態への移行
 *
 *  IPM（ハードウェアの割込み優先度マスク）を，saved_iipmに保存し，カー
 *  ネル管理外のものを除くすべての割込みをマスクする値（TIPM_LOCK）に設
 *  定する．また，lock_flagをtrueにする．
 *
 *  IPMが，最初からTIPM_LOCKと同じかそれより高い場合には，それを
 *  saved_iipmに保存するのみで，TIPM_LOCKには設定しない．これはモデル
 *  上の割込み優先度マスクが，TIPM_LOCKと同じかそれより高いレベルに設定
 *  されている状態にあたる．
 *
 *  この関数は，CPUロック状態（lock_flagがtrueの状態）で呼ばれることは
 *  ないものと想定している．
 */
Inline void
x_lock_cpu(void)
{
	uint8_t iipm;

	/*
	*  current_iipm()の返り値を直接saved_iipmに保存せず，一時変数iipm
	*  を用いているのは，current_iipm()を呼んだ直後に割込みが発生し，
	*  起動された割込み処理でsaved_iipmが変更される可能性があるためで
	*  ある．
	*/
	iipm = current_iipm();
	if (IIPM_LOCK > iipm) {
		set_iipm(IIPM_LOCK);
	}
	get_my_p_tpcb()->saved_iipm = iipm;
	get_my_p_tpcb()->lock_flag = true;
	/* クリティカルセクションの前後でメモリが書き換わる可能性がある */
	Asm("":::"memory");
}

#define t_lock_cpu()    x_lock_cpu()
#define i_lock_cpu()    x_lock_cpu()

/*
 *  CPUロック状態の解除
 *
 *  lock_flagをfalseにし，IPM（ハードウェアの割込み優先度マスク）を，
 *  saved_iipmに保存した値に戻す．
 *
 *  この関数は，CPUロック状態（lock_flagがtrueの状態）でのみ呼ばれるも
 *  のと想定している．
 */
Inline void
x_unlock_cpu(void)
{
	/* クリティカルセクションの前後でメモリが書き換わる可能性がある */
	Asm("":::"memory");
	get_my_p_tpcb()->lock_flag = false;
	set_iipm(get_my_p_tpcb()->saved_iipm);
}

#define t_unlock_cpu()    x_unlock_cpu()
#define i_unlock_cpu()    x_unlock_cpu()

/*
 *  chg_ipmで有効な割込み優先度の範囲の判定
 *
 *  TMIN_INTPRIの値によらず，chg_ipmでは，-15〜TIPM_ENAALL（＝0）の範囲
 *  に設定できることとする（ターゲット定義の拡張）．
 */
#define VALID_INTPRI_CHGIPM(intpri) \
				((-15 <= (intpri) && (intpri) <= TIPM_ENAALL))

/*
 * （モデル上の）割込み優先度マスクの設定
 *
 *  CPUロックフラグがクリアされている時は，ハードウェアの割込み優先度マ
 *  スクを設定する．CPUロックフラグがセットされている時は，saved_iipm
 *  を設定し，さらに，ハードウェアの割込み優先度マスクを，設定しようと
 *  した（モデル上の）割込み優先度マスクとTIPM_LOCKの高い方に設定する．
 */
Inline void
x_set_ipm(PRI intpri)
{
	uint8_t   iipm = INT_IPM(intpri);

	if (!get_my_p_tpcb()->lock_flag) {
	set_iipm(iipm);
	}
	else {
		get_my_p_tpcb()->saved_iipm = iipm;
#if TIPM_LOCK == -15
		/*
		*  TIPM_LOCKが-15の場合には，この時点でハードウェアの割込み優先
		*  度マスクが必ず15に設定されているため，設定する必要はない．
		*/
      
#else /* TIPM_LOCK == -15 */
		set_iipm(iipm > IIPM_LOCK ? iipm : IIPM_LOCK);
#endif /* TIPM_LOCK == -15 */
	}
}

#define t_set_ipm(intpri)    x_set_ipm(intpri)
#define i_set_ipm(intpri)    x_set_ipm(intpri)

/*
 * （モデル上の）割込み優先度マスクの参照
 *
 *  CPUロックフラグがクリアされている時はハードウェアの割込み優先度マ
 *  スクを，セットされている時はsaved_iipmを参照する．
 */
Inline PRI
x_get_ipm(void)
{
	uint8_t iipm;

	if (!get_my_p_tpcb()->lock_flag) {
		iipm = current_iipm();
	}
	else {
		iipm = get_my_p_tpcb()->saved_iipm;
	}
	return(EXT_IPM(iipm));
}

#define t_get_ipm()    x_get_ipm()
#define i_get_ipm()    x_get_ipm()

/*
 * 割込み優先度レジスタの設定
 */
Inline void
set_irc_ipr(uint32_t address, uint32_t offset, uint8_t val)
{
	uint32_t ipr_val;
	SIL_PRE_LOC;
	SIL_LOC_INT();

	ipr_val = sil_rew_mem((void *) address);
	ipr_val &= ~(0x0FU << offset);
	ipr_val |= (uint32_t)val << offset;    
	sil_wrw_mem((void *) address, ipr_val);

	SIL_UNL_INT();
}

/*
 *  タスクコンテキスト かつ CPUロック解除状態なら true
 *  t_sense_lock()内でPCBをアクセスするため，割込みを禁止して実行する．   
 */
Inline bool_t
sense_tskctx_unl(void)
{
	bool_t result;
	uint8_t iipm;

	/* CPUロック相当 */
	iipm = current_iipm();
	if (IIPM_LOCK > iipm) {
		set_iipm(IIPM_LOCK);
	}

	result = (!sense_context() && !t_sense_lock());

	/* CPUロック解除相当 */
	if (IIPM_LOCK > iipm) {
		set_iipm(iipm);
	}

	return(result);
}



/*
 *  非タスクコンテキスト かつ CPUロック解除状態なら true
 *  t_sense_lock()内でPCBをアクセスするため，割込みを禁止して実行する． 
 */
Inline bool_t
sense_intctx_unl(void)
{
	bool_t result;

	uint8_t iipm;

	/* CPUロック相当 */
	iipm = current_iipm();
	if (IIPM_LOCK > iipm) {
		set_iipm(IIPM_LOCK);
	}

	result = (sense_context() && !t_sense_lock());

	/* CPUロック解除相当 */
	set_iipm(iipm);

	return(result);
}

/*
 *  最高優先順位タスクへのディスパッチ（prc_support.S）
 *
 *  dispatchは，タスクコンテキストから呼び出されたサービスコール処理か
 *  ら呼び出すべきもので，タスクコンテキスト・CPUロック状態・ディスパッ
 *  チ許可状態・（モデル上の）割込み優先度マスク全解除状態で呼び出さな
 *  ければならない．
 */
extern void dispatch(void);

/*
 *  自タスクのマイグレーション（core_support.S）
 */
extern void dispatch_and_migrate(ID prcid);

/*
 *  現在のコンテキストを捨てマイグレーション（core_support.S）
 */
extern void exit_and_migrate(ID prcid);

/*
 *  ディスパッチャの動作開始（prc_support.S）
 *
 *  start_dispatchは，カーネル起動時に呼び出すべきもので，すべての割込
 *  みを禁止した状態（割込みロック状態と同等の状態）で呼び出さなければ
 *  ならない．
 */
extern void start_dispatch(void) NoReturn;

/*
 *  現在のコンテキストを捨ててディスパッチ（prc_support.S）
 *
 *  exit_and_dispatchは，ext_tskから呼び出すべきもので，タスクコンテキ
 *  スト・CPUロック状態・ディスパッチ許可状態・（モデル上の）割込み優先
 *  度マスク全解除状態で呼び出さなければならない．
 */
extern void exit_and_dispatch(void) NoReturn;

/*
 *  カーネルの終了処理の呼出し（prc_support.S）
 *
 *  call_exit_kernelは，カーネルの終了時に呼び出すべきもので，非タスク
 *  コンテキストに切り換えて，カーネルの終了処理（exit_kernel）を呼び出
 *  す．
 */
extern void call_exit_kernel(void) NoReturn;

/*
 *  タスクコンテキストの初期化
 *
 *  タスクが休止状態から実行できる状態に移行する時に呼ばれる．この時点
 *  でスタック領域を使ってはならない．
 *
 *  activate_contextを，インライン関数ではなくマクロ定義としているのは，
 *  この時点ではTCBが定義されていないためである．
 */
extern void    start_r(void);

#define activate_context(p_tcb)                                         \
{                                                                       \
	(p_tcb)->tskctxb.sp = (void *)((char *)((p_tcb)->p_tinib->stk)      \
                                        + (p_tcb)->p_tinib->stksz);     \
	(p_tcb)->tskctxb.pc = (void *) start_r;                             \
}

/*
 *  calltexは使用しない
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
 *  SH34の割込みアーキテクチャと割込み処理モデルの実現
 * 
 *  SH34の割込みは，内部周辺モジュール割込み，IRQ割込み, PINT割込みに分
 *  類でき，それぞれ扱いが異なる．そのため，割込みハンドラ番号から，ど
 *  の割込みに分類するためのマクロを用意する．
 * 
 *  PINT割込みは15bitのプロセッサへの割込み要求ラインで実現されているが，
 *  IRCへの割込み要求ラインはPINT0〜PINT7とPINT0〜PINT7といった2本になっ
 *  ている．そのため，カーネルでも個別には扱わず，2本の割込みとして扱う．
 *  PINTは，個別にPINTERレジスタにより許可禁止可能であるが，カーネルで
 *  は，PINTERレジスタの設定は取り扱わない．そのため，PINTERレジスタの
 *  設定は，ユーザープログラムで行う必要がある．同様にPINTの要求レベル
 *  を設定するICR2の設定もユーザープログラムで行う必要が△襦イ覆�，エ�ﾃ
 *  ジ割込みに設定した際の割込みハンドラの入り口でのIRR0のクリアは
 *  i_begin_int()で行う．PINT割込みかどうかを判定するマクロを用意する．
 *
 *  同様にIRQ割込みかを判定するマクロを用意する．
 */

/*
 *  PINT割込みかどうかを判定するためのマクロ
 */
#define INTNO_PINT(intno) ((0x700 == (intno)) || (0x720 == (intno)))
#define INTNO_PINT_OFFSET(intno) (((INTNO_MASK(intno)) - 0x700) >> 5)

/*
 *  IRQ割込みかどうかを判定するためのマクロ
 */
#define INTNO_IRQ(intno) ((0x600 <= (intno)) && ((intno) <= 0x6A0))
#define INTNO_IRQ_OFFSET(intno) (((INTNO_MASK(intno)) - 0x600) >> 5)
     
/*
 *  割込み優先度
 *
 *  各割込みの割込み優先度は割込み優先順設定レジスタ(IPRXX)によって設
 *  定する．内部周辺モジュールの場合，周辺モジュール毎に設定可能である．
 *  例えばSCIFは，ERI2, RXI2, BRI2, TXI2という4つの割込み要求ラインをサ
 *  ポートするが，全ての割込み要求ラインに対して同一の優先度しか設定で
 *  きない．そのため，各割込み要求ライン毎にどの割込み優先順設定レジス
 *  タを設定するかを定義したテーブルを用意する．また，割込み番号は連続
 *  していため，サポートしていない番号には，numberに-1を設定する．テー
 *  ブル
 */

/*
 *  各割込み番号毎の設定するIPRの情報管理のための構造体  
 */
typedef struct {
	/* IPRA:0, IPRB:1, IPRC:2, IPRD:3, IPRE:4, IPRF:5, IPRG:6 */
	uint32_t address; 
	/* bit5〜12:3, bit11〜8:2, bit7〜4:1, bit3〜0:0 */
	uint8_t  offset;
} IPR_INFO;
/*
 *  各割込み番号毎のIPRの情報管理テーブル
 */
extern const IPR_INFO ipr_info_tbl[TNUM_INH];

/*
 *  各割込み番号毎の設定するMSKCLRの情報管理のための構造体  
 */
typedef struct {
	uint32_t address; 
	uint32_t  mask;
} MSK_INFO;
/*
 *  各割込み番号毎のMSKCLRの情報管理テーブル
 */
extern const MSK_INFO mskclr_info_tbl[TNUM_INH];

/*
 *  割込みコントローラの初期化
 *  ToDo 一旦ローカル変数にコントローラのアドレスを入れる．
 */
Inline void
init_irc(void)
{
	/* 割込みコントロールレジスタ０の設定 */
	sil_wrw_mem((void*)ICR0, 0x00800000U);
	/* 割込みコントロールレジスタ１の設定 */
	sil_wrh_mem((void*)ICR1, 0x00000000U);

	/*
	 *  割込みマスクレジスタの初期化
	 */
	init_int2msk();
}

/*
 *  割込み番号・割込みハンドラ番号
 *
 *  割込みハンドラ番号(inhno)と割込み番号(intno)は，割込み発生時に割込
 *  み事象レジスタに設定される値を用いる．割込み事象レジスタは，INTEVT
 *  とINTEVT2の2種類があり，それぞれ設定される値が異なる．初期のSH34で
 *  はINTEVTを使用していたが，現在はINTEVT2を用いる．そのため，プロセ
 *  ッサの型番毎の定義ファイルを用意して，どちらのレジスタを用いるが設
 *  定することとする．また，サポートする割込みの最大数も異なるため，
 *  最大値もプロセッサの型番毎の定義ファイルに定義する．
 *
 *  INTENTに設定される値はスパースな値であるため，そのままの値を優先度
 *  管理等のテーブルのインデックスに用いると，無駄な領域が発生する．そ
 *  のため，カーネル内部では，INTENTに設定される値をシフトした値を用い
 *  る．シフトする値もプロセッサ毎に異なるためプロセッサの型番毎の定義
 *  ファイルに定義する．
 *
 *  内部表現の名前は，iintno,iinhnoとする．
 */

/*
 *  割込み番号の範囲の判定
 * 
 *  TMIN_INTNOとTMAX_INTNOの範囲であっても有効な番号でない場合があるた
 *  め，ipr_info_tbl[]で有効な番号かをチェックする
 */
#define VALID_INTNO(intno) \
        ((TMIN_INTNO <= (INTNO_MASK(intno))) && ((INTNO_MASK(intno)) <= TMAX_INTNO) && (ipr_info_tbl[INT_INTNO(INTNO_MASK(intno))].address != 0U))
#define VALID_INTNO_DISINT(intno)    VALID_INTNO(intno)
#define VALID_INTNO_CFGINT(intno)    VALID_INTNO(intno)

/*
 *  CPU例外ハンドラ番号
 *
 *  CPU例外ハンドラ番号としては，例外事象レジスタに設定する例外コードを
 *  用いる．サポートするCPU例外ハンドラ数はプロセッサの身嵋茲飽曚覆襪ﾎ
 *  で，プロセッサの型番毎の定義ファイルに定義する．
 *
 *  割込みハンドラ番号と同じく，CPU例外ハンドラ番号もスパースであるため，
 *  カーネル内部では値をシフトした内部表現を用いる． 
 */
#define VALID_EXCNO_DEFEXC(excno) \
        (TMIN_EXCNO <= (excno) && (excno) <= TMAX_EXCNO)

/*
 *  割込みハンドラ
 *
 *  割込みハンドラを登録する．割込みハンドラテーブル実体はコンフィギュ
 *  レータで静的に生成する
 */ 
extern const FP inh_tbl[TNUM_INH];

/*
 *  割込みハンドラの設定
 *
 *  ベクトル番号inhnoの割込みハンドラの起動番地inthdrに設定する．割込み
 *  ハンドラテーブル
 */
Inline void
x_define_inh(INHNO inhno, FP int_entry, uint_t affinity_mask)
{
  	assert(((TMIN_INHNO <= INHNO_MASK(inhno)) && (INHNO_MASK(inhno) <= TMAX_INHNO)));
}

/*
 *  割込みハンドラの出入口処理の生成マクロ
 *
 */
#define INT_ENTRY(inhno, inthdr)    inthdr
#define INTHDR_ENTRY(inhno, inhno_num, inthdr) extern void inthdr(void);



/*
 *  割込み要求禁止フラグ
 *
 *  SH34のIRCは割込み要求禁止フラグを持たない．割込みを禁止する場合は，
 *  割込み優先順設定レジスタ(IPRXX)を0に設定する必要がある．割込み優
 *  先順設定レジスタを0にすることで割込み要求禁止フラグを実現するとその
 *  間，割込み優先度を記憶しておく領域が必要となる．コンフィギュレータ
 *  でこのテーブルを用意する．なお，このテーブルは割込みの入り口で割込
 *  み要因に応じてSRに設定する必要があるため，内部表現とする．また，割
 *  込みハンドラの入り口での利便性のため，IPMだけではなく，割込みハンド
 *  ラ実行時のSRの設定も同時に持つ．そのため，IPMの値のみを取得したい場
 *  合は，下位8bitのみを参照する必要がある．内容はコンフィギュレータで
 *  静的に生成する．
 */
extern const uint32_t int_iipm_tbl[TNUM_INT];

/*
 *  割込み要求禁止フラグのセット
 *
 *  割込み優先順設定レジスタ(IPRA〜G)を0に設定する．
 *
 *  割込み属性が設定されていない割込み要求ラインに対して割込み要求禁止
 *  フラグをクリアしようとした場合には，falseを返す．
 */
Inline bool_t
x_disable_int(INTNO intno)
{
	uint16_t iintno  = INT_INTNO(INTNO_MASK(intno));
	uint32_t address = ipr_info_tbl[iintno].address;
	uint32_t offset  = ipr_info_tbl[iintno].offset;
	uint32_t iipm    = int_iipm_tbl[iintno];

	/*
	 *  割込み属性が設定されていない場合
	 */
	if ((iipm & 0x01U) != 0) {
		return(false);
	}
    
	/* 
	*  割込み番号に対応したIPRの値を0にする．
	*/
	set_irc_ipr(address, offset, 0x00);

	return(true);
}

#define t_disable_int(intno) x_disable_int(intno)
#define i_disable_int(intno) x_disable_int(intno)

/*
 *  割込み要求禁止フラグの解除
 *
 *  割込み優先順設定レジスタ(IPRA〜G)を元の値に戻す．
 *
 *  割込み属性が設定されていない割込み要求ラインに対して割込み要求禁止
 *  フラグをクリアしようとした場合には，falseを返す．
 */
Inline bool_t
x_enable_int(INTNO intno)
{
	uint16_t iintno  = INT_INTNO(INTNO_MASK(intno));
	uint32_t address = ipr_info_tbl[iintno].address;
	uint32_t offset  = ipr_info_tbl[iintno].offset;
	uint32_t iipm    = int_iipm_tbl[iintno];

	/*
	 *  割込み属性が設定されていない場合
	 */
	if ((iipm & 0x01U) != 0) {
		return(false);
	}

	/* 
	*  割込み番号に対応したIPRの値をセットする
	*/
	set_irc_ipr(address, offset, (-EXT_IPM(iipm & 0xF0U)));
    
	return(true);
}

#define t_enable_int(intno) x_enable_int(intno)
#define i_enable_int(intno) x_enable_int(intno)
/*
 * 割込みマスクレジスタの設定
 */
Inline void
set_target_irc(INTNO intno, uint_t cpuindex)
{
	uint16_t iintno  = INT_INTNO(INTNO_MASK(intno));
	uint32_t address = mskclr_info_tbl[iintno].address;
	uint32_t mask  = mskclr_info_tbl[iintno].mask;

	address = address + CALC_CPU_MSKCLR_OFFSET(cpuindex);
	sil_wrw_mem((void *) address, mask);
}


/*
 *  割込みハンドラ入口で必要なIRC操作
 *
 *  PINT・IRQ割込みでかつ，エッジトリガの場合は割込み要求をクリアする．
 *  エッジトリガか判断するのが面倒なので，常にクリアする．
 */
Inline void
i_begin_int(INTNO intno)
{
}

/*
 *  割込みハンドラの出口で必要なIRC操作
 */
Inline void
i_end_int(INTNO intno)
{
}

/*
 *  CPU例外ハンドラ関係
 */ 

/*
 *  例外ハンドラ引数 p_excinf から各種情報を取り出すためのマクロ
 */
#ifdef FPU_ENABLE

#ifdef FPU_SAVE_BANKED_REG

#define P_EXCINF_OFFSET_SSR     (12U + 14U + 16U)
#define P_EXCINF_OFFSET_SPC     (14U + 14U + 16U)
#define P_EXCINF_OFFSET_PR      (13U + 14U + 16U)
#define P_EXCINF_OFFSET_EXPEVT    0U 

#else

#define P_EXCINF_OFFSET_SSR     (12U + 14U)
#define P_EXCINF_OFFSET_SPC     (14U + 14U)
#define P_EXCINF_OFFSET_PR      (13U + 14U)
#define P_EXCINF_OFFSET_EXPEVT    0U

#endif /* FPU_SAVE_BANKED_REG */

#else

#define P_EXCINF_OFFSET_SSR     12U
#define P_EXCINF_OFFSET_SPC     14U
#define P_EXCINF_OFFSET_PR      13U
#define P_EXCINF_OFFSET_EXPEVT   0U

#endif /* FPU_ENABLE */

/*
 *  CPU例外ハンドラの登録用テーブル
 */
extern const FP exch_tbl[TNUM_EXC];

/*
 *  CPU例外ハンドラの設定
 */
Inline void
x_define_exc(EXCNO excno, FP exchdr)
{       
}

/*
 *  CPU例外ハンドラの入口処理の生成マクロ
 */
#define EXC_ENTRY(excno, exchdr)    exchdr
#define EXCHDR_ENTRY(excno, exchdr) extern void exchdr(void *p_excinf);

/*
 *  CPU例外の発生した時のコンテキストの参照
 *
 *  CPU例外の発生した時のコンテキストが，タスクコンテキストの時にfalse，
 *  そうでない時にtrueを返す．
 */
Inline bool_t
exc_sense_context(void *p_excinf)
{
	uint32_t nest;

	Asm("stc r7_bank,%0" : "=r"(nest));
	return(nest > 1U);
}

/*
 *  CPU例外の発生した時のIPM（ハードウェアの割込み優先度マスク，内部表
 *  現）の参照
 */
Inline uint16_t
exc_get_iipm(void *p_excinf)
{
	return(*(((uint32_t *) p_excinf) + P_EXCINF_OFFSET_SSR) & 0x00F0U);
}

/*
 *  CPU例外の発生した時のコンテキストと割込みのマスク状態の参照
 *
 *  CPU例外の発生した時のシステム状態が，カーネル実行中でなく，タスクコ
 *  ンテキストであり，割込みロック状態でなく，CPUロック状態でなく，（モ
 *  デル上の）割込み優先度マスク全解除状態である時にtrue
，そうでない時
 *  にfalseを返す（CPU例外がカーネル管理外の割込み処理中で発生した場合
 *  にもfalseを返す）．
 *
 *  SHでは，CPU例外の発生した時のIPM（ハードウェアの割込み優先度マスク）
 *  がすべての割込みを許可する状態であることをチェックすることで，カー
 *  ネル実行中でないこと，割込みロック状態でないこと，CPUロック状態でな
 *  いこと，（モデル上の）割込み優先度マスク全解除状態であることの4つの
 *  条件をチェックすることができる（CPU例外が発生した時のlock_flagを参
 *  照する必要はない）．
 */
Inline bool_t
exc_sense_intmask(void *p_excinf)
{
	return(!exc_sense_context(p_excinf)
                    && exc_get_iipm(p_excinf) == IIPM_ENAALL);
}

/*
 *  CPU例外の発生した時のコンテキストと割込み／CPUロック状態の参照
 *
 *  CPU例外の発生した時のシステム状態が，カーネル実行中でなく，タスクコ
 *  ンテキストであり，割込みロック状態でなく，CPUロック状態でない時に
 *  true，そうでない時にfalseを返す（CPU例外がカーネル管理外の割込み処
 *  理中で発生した場合にもfalseを返す）．
 *
 *  SH34では，CPU例外の発生した時のIPM（ハードウェアの割込み優先度マ
 *  スク）がTIPM_LOCKより低いことをチェックすることで，カーネル実行中で
 *  ないこと，割込みロック状態でないこと，CPUロック状態でないことの3つ
 *  の条件をチェックしている（CPU例外が発生した時のlock_flagは参照して
 *  いない）．これにより，（モデル上の）割込み優先度マスクをTIPM_LOCK以
 *  上に設定してタスクを実行している時にもfalseが返ってしまうが，判断を
 *  正確にするためのオーバヘッドが大きいことから，許容することにする．
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
 */
extern void default_int_handler(void *p_excinf);

/*
 *  str_ker()の前でマスタプロセッサで行う初期化
 */
extern void prc_mprc_initialize(void);

/*
 *  プロセッサ依存の初期化
 */
extern void prc_initialize(void);

/*
 *  プロセッサ依存の終了時処理
 */
extern void prc_terminate(void);

/*
 *  ロック取得時に使用するダミー変数
 */
extern  uint32_t dangling_dummy;

/*
 *  1段目のロック取得（タスク・非タスクコンテキスト共用）（通常版）
 */
Inline void
x_acquire_lock(LOCK *p_lock)
{
	bool_t    result;

	while(true){
		Asm("     movli.l	@%2,r0             \n"
		"\t   mov		#0,r3              \n"
		"\t   cmp/eq	r3,r0              \n"
		"\t   bf		1f                 \n"
		"\t   mov		#1,r0              \n"
		"\t   movco.l	r0,@%2             \n"
		"\t   bf		2f                 \n"  //失敗
		"\t   mov		#0,%1              \n"
		"\t   bra		3f                 \n"
		"\t    nop                         \n"
		"1:\t                              \n"  //ダングリング　
		"\t   movco.l	r0,%0              \n"
		"2:\t                              \n"  //失敗のパターン
		"\t   mov		#1,%1              \n"
		"3:\t                              \n"  //終了
		:"=m"(dangling_dummy),"=r"(result)
		:"r"(p_lock):"r0","r3", "cc");

		if(!result){
			return;
		}
		/* 割込みの許可 */
		x_unlock_cpu();
		x_lock_cpu();
	}
}

#define t_acquire_lock(p_lock) x_acquire_lock(p_lock)
#define i_acquire_lock(p_lock) x_acquire_lock(p_lock)

#if TTYPE_KLOCK != G_KLOCK

/*
 *  2段目以上のロック取得（タスク・非タスクコンテキスト共用）
 */
Inline bool_t
x_acquire_nested_lock(LOCK *p_lock)
{
	bool_t    result;
	PCB		*p_pcb;

	while(true){
		Asm("     movli.l	@%2,r0             \n"
		"\t   mov		#0,r3              \n"
		"\t   cmp/eq	r3,r0              \n"
		"\t   bf		1f                 \n"
		"\t   mov		#1,r0              \n"
		"\t   movco.l	r0,@%2             \n"
		"\t   bf		2f                 \n"  //失敗
		"\t   mov		#0,%1              \n"
		"\t   bra		3f                 \n"
		"\t    nop                         \n"
		"1:\t                              \n"  //ダングリング　
		"\t   movco.l	r0,%0              \n"
		"2:\t                              \n"  //失敗のパターン
		"\t   mov		#1,%1              \n"
		"3:\t                              \n"  //終了
		:"=m"(dangling_dummy),"=r"(result)
		:"r"(p_lock):"r0","r3", "cc");

		if(!result){
			return(false);
		}
		/* 割込みの許可 */
		x_unlock_cpu();
		x_lock_cpu();

		/*
		 *  マイグレーションする可能性があるのでここで毎回取得  
		 *  非タスクコンテキストの場合，マイグレーションしないため，
		 *  while前に実行してもよいが，1回でロックがとれた場合，
		 *  効率が悪いので，ここで取得する．
		 */
		p_pcb = get_my_p_pcb(); 
		if (p_pcb->p_firstlock == NULL) {
			return(true);
		}
	}
}

#define t_acquire_nested_lock(p_lock) x_acquire_nested_lock(p_lock)
#define i_acquire_nested_lock(p_lock) x_acquire_nested_lock(p_lock)

#endif /* TTYPE_KLOCK != G_KLOCK */

/*
 *  割込を許可しないロック取得関数
 */
Inline void 
x_acquire_lock_without_preemption(LOCK *p_lock)
{
	bool_t    result;

	while(true){
		Asm("     movli.l	@%2,r0             \n"
		"\t   mov		#0,r3              \n"
		"\t   cmp/eq	r3,r0              \n"
		"\t   bf		1f                 \n"
		"\t   mov		#1,r0              \n"
		"\t   movco.l	r0,@%2             \n"
		"\t   bf		2f                 \n"  //失敗
		"\t   mov		#0,%1              \n"
		"\t   bra		3f                 \n"
		"\t    nop                         \n"
		"1:\t                              \n"  //ダングリング　
		"\t   movco.l	r0,%0              \n"
		"2:\t                              \n"  //失敗のパターン
		"\t   mov		#1,%1              \n"
		"3:\t                              \n"  //終了
		:"=m"(dangling_dummy),"=r"(result)
		:"r"(p_lock):"r0","r3", "cc");

		if(!result){
			return;
		}
	}
}

/*
 *  ロックの解放（通常版）
 */
Inline void
x_release_lock(LOCK *p_lock)
{
	*p_lock = 0;
}

/*
 *  スピンロック機能のネイティブロック方式で使用する関数
 */

/*
 *  スピンロックの型（ネイティブロックの場合）
 */
typedef uint32_t SPNLOCK;

/*
 *  スピンロックIDから実際のロックのIDへの変換
 */
#define TO_NATIVE_ID(spnid)		(spnid * 10)

/*
 *  スピンロックの初期化
 */
Inline void
x_initialize_spin(ID spnid, SPNLOCK *p_spn_lock)
{
	*p_spn_lock = 0;
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
 *  スピンロックの取得の試行（通常ロック版）
 */
Inline bool_t
x_try_lock_spin(SPNLOCK *p_spn_lock)
{
	bool_t    result;

	Asm("     movli.l	@%2,r0             \n"
		"\t   mov		#0,r3              \n"
		"\t   cmp/eq	r3,r0              \n"
		"\t   bf		1f                 \n"
		"\t   mov		#1,r0              \n"
		"\t   movco.l	r0,@%2             \n"
		"\t   bf		2f                 \n"  //失敗
		"\t   mov		#0,%1              \n"
		"\t   bra		3f                 \n"
		"\t    nop                         \n"
		"1:\t                              \n"  //ダングリング　
		"\t   movco.l	r0,%0              \n"
		"2:\t                              \n"  //失敗のパターン
		"\t   mov		#1,%1              \n"
		"3:\t                              \n"  //終了
		:"=m"(dangling_dummy),"=r"(result)
		:"r"(p_spn_lock):"r0","r3", "cc");

	return result;
}

#define t_try_lock_spin(p_spn_lock) x_try_lock_spin(p_spn_lock)
#define i_try_lock_spin(p_spn_lock) x_try_lock_spin(p_spn_lock)

/*
 *  スピンロックの解放
 */
Inline void
x_unlock_spin(SPNLOCK *p_spn_lock)
{
  x_release_lock(p_spn_lock);
}

#define t_unlock_spin(p_spn_lock) x_unlock_spin(p_spn_lock)
#define i_unlock_spin(p_spn_lock) x_unlock_spin(p_spn_lock)

/*
 *  ジャイアントロックの初期化（G_LOCKの場合）
 */
Inline void
x_initialize_giant_lock(LOCK *p_giant_lock)
{
	*p_giant_lock = 0;
}



/*
 *  タスクロックの初期化
 */
Inline void
x_initialize_tsk_lock(LOCK *p_tsk_lock)
{
	*p_tsk_lock = 0;
}


/*
 *  オブジェクトロックの初期化
 */
Inline void
x_initialize_obj_lock(LOCK *p_obj_lock)
{
	*p_obj_lock = 0;
}


#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_PRC_CONFIG_H */
