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
 *  @(#) $Id: chip_config.h 553 2010-03-16 02:51:56Z ertl-honda $
 */

/*
 *  チップ依存モジュール（MPCORE用）
 *
 *  カーネルのチップ依存部のインクルードファイル．kernel_impl.hのター
 *  ゲット依存部の位置付けとなる．
 */

#ifndef TOPPERS_CHIP_CONFIG_H
#define TOPPERS_CHIP_CONFIG_H

/*
 *  チップ依存部ののハードウェア資源の定義
 */
#include "mpcore.h"

#ifdef USE_IPI_DIS_HANDER_BYPASS
#define OMIT_KER_REQ_ON_IPI

#ifndef TOPPERS_MACRO_ONLY
/*
 *  カーネル終了処理要求
 */
extern void ext_ker_request(void);
#endif /* TOPPERS_MACRO_ONLY */
#endif /* USE_IPI_DIS_HANDER_BYPASS */

/*
 *  ベクタールーチンをカーネルで持つかの定義
 */
#define VECTOR_KERNEL

/*
 *  割込み番号のマスク（下位16bitのみが有効）
 */
#define INTNO_MASK(intno) (intno & 0xffffU)

/*
 *  割込みハンドラ番号のマスク（下位16bitのみが有効）
 */
#define INHNO_MASK(intno) (intno & 0xffffU)

/*
 *  割込みハンドラ番号と割込み番号に関する定義
 */
#define TMIN_INTNO  DIC_TMIN_INTNO
#define TMAX_INTNO  DIC_TMAX_INTNO
#define TNUM_INT    DIC_TNUM_INT

#define TMIN_INHNO  DIC_TMIN_INTNO
#define TMAX_INHNO  DIC_TMAX_INTNO
#define TNUM_INH    DIC_TNUM_INT

#ifndef TOPPERS_MACRO_ONLY

/*
 *  自プロセッサのAPCBへのポインタの取得
 *  インクルード順序の関係上，target_pcb.h には記述できないためここで定義する．
 */
Inline APCB*
get_my_p_apcb(void)
{
	return &((get_my_p_tpcb())->arm_pcb);
}

/*
 *  マスタプロセッサかを返す
 */
Inline bool_t
x_sense_mprc(void){
	if(x_prc_index() == (TOPPERS_MASTER_PRCID - 1)) {
		return true;
	}
	else {
		return false;
	}
}

/*
 *  ロック関係
 */

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

/*
 *  割込み番号の範囲の判定
 */
#define VALID_INTNO(intno) ((TMIN_INTNO <= INTNO_MASK(intno)) && (INTNO_MASK(intno) <= TMAX_INTNO))
#define VALID_INTNO_DISINT(intno)    VALID_INTNO(intno)
#define VALID_INTNO_CFGINT(intno)    VALID_INTNO(intno)
#define VALID_INTNO_ATTISR(intno)    VALID_INTNO(intno)

/*
 *  割込みハンドラの登録用テーブル（kernel_cfg.c）
 */
extern const FP* const p_inh_table[TNUM_PRCID];

/*
 *  割込みハンドラの設定
 * 
 *  割込みハンドラ番号inhnoの割込みハンドラの起動番地をinthdrに設定する
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
#define INTHDR_ENTRY(inhno, inhno_num, inthdr)

#endif /* TOPPERS_MACRO_ONLY */  

/*
 *  割込み優先度マスク操作ライブラリ
 *
 *  DICでは，DIC内の優先度レジスタにCPUの割込み優先度マスク（IPM）を設定する．  
 *  割込み優先度マスクは値が大きいほど低優先度である．
 *  割込み優先度マスクはの段階はARM11とCA9で異なる．
 *    ・ARM11(ARMv6) : 16段階（0x00 〜 0x0f）
 *    ・CA9(ARMv7)   : 32段階（0x00 〜 0x1f）  
 *  IPMを保存しておくために，割込み優先度の外部表現（-1から連続した負の値）
 *  を使うことも可能であるが，余計な計算が必要になる．これを避けるため，IPMを
 *  保存する場合には，DICで扱う優先度の形式とする．この値を割込み優先度マスク
 *  の内部表現と呼び，IIPMと書くことにする． 
 */

/*
 *  割込み優先度マスクのビットマスク
 */
#if __TARGET_ARCH_ARM == 6
#ifndef TOPPERS_MACRO_ONLY
#define IPM_BIT_MASK  0x0fU
#else /* TOPPERS_MACRO_ONLY */
#define IPM_BIT_MASK  0x0f
#endif /* TOPPERS_MACRO_ONLY */
#define IPM_BIT_OFFSET 4
#elif __TARGET_ARCH_ARM == 7
#ifndef TOPPERS_MACRO_ONLY
#define IPM_BIT_MASK  0x1fU
#else /* TOPPERS_MACRO_ONLY */
#define IPM_BIT_MASK  0x1f
#endif /* TOPPERS_MACRO_ONLY */
#define IPM_BIT_OFFSET 3
#endif /* __TARGET_ARCH_ARM == 7 */

/*
 *  割込み優先度マスクの外部表現と内部表現の変換
 *
 *  アセンブリ言語のソースファイルからインクルードする場合のために，型
 *  キャストしない定義も用意している．
 */
#ifndef TOPPERS_MACRO_ONLY
#define EXT_IPM(iipm)    ((PRI)(iipm - IPM_BIT_MASK))    /* 内部表現を外部表現に */
#define INT_IPM(ipm)    ((uint8_t)(ipm + IPM_BIT_MASK))  /* 外部表現を内部表現に */
#else /* TOPPERS_MACRO_ONLY */
#define EXT_IPM(iipm)    (iipm - IPM_BIT_MASK)           /* 内部表現を外部表現に */
#define INT_IPM(ipm)    (ipm + IPM_BIT_MASK)             /* 外部表現を内部表現に */
#endif /* TOPPERS_MACRO_ONLY */

/*
 *  TIPM_ENAALL（割込み優先度マスク全解除）の内部表現
 */
#define IIPM_ENAALL        (INT_IPM(TIPM_ENAALL))

#ifndef TOPPERS_MACRO_ONLY

/*
 *  IPM（ハードウェアの割込み優先度マスク，内部表現）の現在値の読出し
 */
Inline uint8_t
current_iipm(void)
{
	return(dic_cpu_current_priority());
}

/*
 *  IPM（ハードウェアの割込み優先度マスク，内部表現）の現在値の設定
 */
Inline void
set_iipm(uint8_t iipm)
{
	dic_cpu_set_priority(iipm);
}

/*
 * (モデル上の)割込み優先度マスクの設定
 */
Inline void
x_set_ipm(PRI intpri)
{
	set_iipm(INT_IPM(intpri));
}

#define t_set_ipm(intpri) x_set_ipm(intpri)
#define i_set_ipm(intpri) x_set_ipm(intpri)

/*
 *  (モデル上の)割込み優先度マスクの参照
 */
Inline PRI
x_get_ipm(void)
{
	// return(EXT_IPM(current_iipm()));
	return TIPM_ENAALL;
}

#define t_get_ipm() x_get_ipm()
#define i_get_ipm() x_get_ipm()

/*
 *  割込み属性アクセステーブル
 *  
 *  割込み属性テーブルへのポインタ．割込み属性テーブルは，割込み属性が設定
 *  されていれば"1"，設定されていなければ"0"となる
 */
extern const uint8_t* const p_cfgint_table[];

#define my_cfgint_table (p_cfgint_table[x_prc_index()])

/*
 * （モデル上の）割込み要求禁止フラグのセット
 *
 *  指定された，割込み番号の割込み要求禁止フラグのセットして，割込みを
 *  禁止する．
 *    
 *  割込み属性が設定されていない割込み要求ラインに対して割込み要求禁止
 *  フラグをクリアしようとした場合には，falseを返す．  
 */
Inline bool_t
x_disable_int(INTNO intno)
{
	if (my_cfgint_table[INTNO_MASK(intno)] == 0){
		return(false);
	}
    
	dic_disable_int(INTNO_MASK(intno));
	return(true);
}

#define t_disable_int(intno)  x_disable_int(intno)
#define i_disable_int(intno)  t_disable_int(intno)

/* 
 *  (モデル上の)割り要求禁止フラグの解除
 *
 *  指定された，割込み番号の割込み要求禁止フラグのクリアして，割込みを
 *  許可する．
 *
 *  割込み属性が設定されていない割込み要求ラインに対して割込み要求禁止
 *  フラグをクリアしようとした場合には，falseを返す．
 */
Inline bool_t
x_enable_int(INTNO intno)
{
	if (my_cfgint_table[INTNO_MASK(intno)] == 0){
		return(false);
	}
    
	dic_enable_int(INTNO_MASK(intno));
	return(true);
}

#define t_enable_int(intno) x_enable_int(intno)
#define i_enable_int(intno) x_enable_int(intno)

/*
 * 割込み要求のクリア
 */
Inline void
x_clear_int(INTNO intno)
{
	dic_clear_pending(INTNO_MASK(intno));
}

#define t_clear_int(intno) x_clear_int(intno) 
#define i_clear_int(intno) x_clear_int(intno) 

/*
 *  割込み要求のチェック
 */
Inline bool_t
x_probe_int(INTNO intno)
{
	return(dic_probe_int(INTNO_MASK(intno)));
}

#define t_probe_int(intno) x_probe_int(intno)
#define i_probe_int(intno) x_probe_int(intno)

/*
 *  割込み要求ラインの属性の設定
 *
 */
extern void x_config_int(INTNO intno, ATR intatr, PRI intpri, uint_t affinity_mask);

/*
 * 割込みハンドラの入り口で必要なIRC操作
 *
 */
Inline void
i_begin_int(INTNO intno)
{    
}

/*
 * 割込みハンドラの出口で必要なIRC操作
 */
Inline void
i_end_int(INTNO intno)
{
}

/*
 *  str_ker() の前でマスタプロセッサで行う初期化
 */
extern void chip_mprc_initialize(void);

/*
 *  チップ依存の初期化
 */
extern void chip_initialize(void);

/*
 *  チップ依存部の終了
 *
 *  システムを終了する時に使う．
 */
extern void chip_exit(void);

/*
 *  割込みハンドラ（chip_support.S）
 */
extern void interrupt_handler(void);

/*
 *  未定義の割込みが入った場合の処理
 */
extern void default_int_handler(void);


#endif /* TOPPERS_MACRO_ONLY */

/*
 *  ARM依存モジュール
 */
#include "arm_gcc/common/core_config.h"

#ifndef TOPPERS_MACRO_ONLY
/*
 *  ロック取得解放関数
 *  x_lock_cpu()/x_unlock_cpu()を使うため，コア依存モジュール（ARM用）を
 *  読み込んでから定義する． 
 */

/*
 *  ロック取得失敗時の割込み待ち
 *
 *  MPCoreでは，ロック取得失敗時にスピンを繰り返すのではなく，wfeにより，
 *  イベント待ちとすることが推奨されている．
 *  wfeによるイベント待ちの際，割込み応答性を低下させないため，割込みを許
 *  可した状態て，イベント待ちとするべきである．wfe命令自信は，割込みを禁
 *  止・許可を操作しないため，別途msr命令により割込みを許可する．
 *
 *  その際，msr実行により割込みが許可された直後(wfeを実行する前に)に割込
 *  みが入り，その後割込みハンドラからのリターン後にwfeになり，イベントを
 *  逃す可能性が考えられる．
 *
 *  しかしながら， 割込みハンドラからのリターン(正確には例外一般からのリタ
 *  ーン)を行うと，イベントレジスタがセットされ，wfe実行時にイベントレジス
 *  タがセットされていると，クリアしてwfeから即座にリターンするための問題
 *  ない． 
 */
  
/*
 *  1段目のロック取得（タスク・非タスクコンテキスト共用）
 */
Inline void
x_acquire_lock(LOCK *p_lock)
{
	int		locked;

	while(true) {
		Asm("   mov      r2, #0x01   \n"
			"\t ldrex    r1, [%1]    \n"
			"\t cmp      r1, #0x00   \n"
			"\t msrne    cpsr_c, %2  \n" /* 割込みの許可(bit0-7のみ書き換え) */
			"\t wfene                \n"
			"\t msrne    cpsr_c, %3  \n" /* 割込みの禁止(bit0-7のみ書き換え) */
			"\t strexeq  r1, r2,[%1] \n"
			"\t mov      %0, r1      \n"
			:"=r"(locked)
			:"r"(p_lock),"I"(CPSR_SVC|CPSR_ALWAYS_SET),"I"(CPSR_SVC|CPSR_CPULOCK|CPSR_ALWAYS_SET)
			:"r1","r2", "cc");

		if(locked == 0){
			/* ロック取得成功 */
			mpcore_data_memory_barrier();
			Asm("":::"memory");
			return;
		}
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
	PCB		*p_pcb;
	int		locked;

	while(true) {
		Asm("   mov      r2, #0x01   \n"
			"\t ldrex    r1, [%1]    \n"
			"\t cmp      r1, #0x00   \n"
			"\t msrne    cpsr_c, %2  \n" /* 割込みの許可(bit0-7のみ書き換え) */
			"\t wfene                \n"
			"\t msrne    cpsr_c, %3  \n" /* 割込みの禁止(bit0-7のみ書き換え) */
			"\t strexeq  r1, r2,[%1] \n"
			"\t mov      %0, r1      \n"
			:"=r"(locked)
			:"r"(p_lock),"I"(CPSR_SVC|CPSR_ALWAYS_SET),"I"(CPSR_SVC|CPSR_CPULOCK|CPSR_ALWAYS_SET)
			:"r1","r2", "cc");
		if(locked == 0){
			/* ロック取得成功 */
			mpcore_data_memory_barrier();
			Asm("":::"memory");
			return(false);
		}
		/*
		 * マイグレーションする可能性があるのでここで毎回取得  
		 * 非タスクコンテキストの場合，マイグレーションしないため，
		 * while前に実行してもよいが，1回でロックがとれた場合，
		 * 効率が悪いので，ここで取得する．
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
 *  ロックの解放（タスク・非タスクコンテキスト共用）
 */
Inline void
x_release_lock(LOCK *p_lock)
{
	Asm("":::"memory");
	mpcore_data_memory_barrier();
	*p_lock = 0;
	mpcore_data_sync_barrier();
	Asm("sev");
}

/*
 *  割込を許可しないロック取得関数
 */
Inline void 
x_acquire_lock_without_preemption(LOCK *p_lock)
{
	int		locked;

	while(true) {
		Asm("   mov      r2, #0x01   \n"
			"\t ldrex    r1, [%1]    \n"
			"\t cmp      r1, #0x00   \n"
			"\t wfene                \n"
			"\t strexeq  r1, r2,[%1] \n"
			"\t mov      %0, r1      \n"
			:"=r"(locked):"r"(p_lock):"r1","r2", "cc");
		if(locked == 0){
			/* ロック取得成功 */
			mpcore_data_memory_barrier();
			Asm("":::"memory");
			return;
		}
	}
}

/*
 *  スピンロック機能のネイティブロック方式で使用する関数
 */

/*
 *  スピンロックの型
 */
typedef uint32_t SPNLOCK;

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
 *  スピンロックの取得の試行
 */
Inline bool_t
x_try_lock_spin(SPNLOCK *p_spn_lock)
{
	int locked;

	Asm("   mov      r2, #0x01   \n"
		"\t ldrex    r1, [%1]    \n"
		"\t cmp      r1, #0x00   \n"
		"\t strexeq  r1, r2,[%1] \n"
		"\t mov      %0, r1      \n"
		:"=r"(locked):"r"(p_spn_lock):"r1","r2", "cc");

	if (locked == 0) {
		/* 成功した場合 */
		mpcore_data_memory_barrier();
		Asm("":::"memory");
		return(false);
	}

	return(true);
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

#endif /* TOPPERS_CHIP_CONFIG_H */
