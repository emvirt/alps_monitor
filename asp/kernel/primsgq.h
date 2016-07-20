/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2008 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: primsgq.h 748 2008-03-07 17:18:06Z hiro $
 */

/*
 *		優先度データキュー機能
 */

#ifndef TOPPERS_PRIMSGQ_H
#define TOPPERS_PRIMSGQ_H

#include <queue.h>

/*
 *  優先度データ管理ブロック
 */
typedef struct primsg_management_block PMQMB;

struct primsg_management_block {
	PMQMB		*p_next;		/* 次のデータ */
	intptr_t	data;			/* データ本体 */
	uint_t		msgsz;			/* message size */
	PRI			datapri;		/* データ優先度 */
};

/*
 *  優先度データキュー初期化ブロック
 *
 *  この構造体は，同期・通信オブジェクトの初期化ブロックの共通部分
 *  （WOBJINIB）を拡張（オブジェクト指向言語の継承に相当）したもので，
 *  最初のフィールドが共通になっている．
 */
typedef struct primsgq_initialization_block {
	ATR			pmqatr;			/* message queue attributes */
	uint_t		pmqcnt;			/* max number of messages */
	uint_t		maxmsgsz;		/* max message size */
	PRI			maxmpri;		/* maximum priority of a message */
	PMQMB		*p_pmqmb;		/* head of the region of message blocks */
	char_t		*p_pmqbuff;		/* head of the buffer for the messages */
} PMQINIB;

/*
 *  優先度データキュー管理ブロック
 *
 *  この構造体は，同期・通信オブジェクトの管理ブロックの共通部分（WOBJCB）
 *  を拡張（オブジェクト指向言語の継承に相当）したもので，最初の2つの
 *  フィールドが共通になっている．
 *
 *  p_pmqinib: the initialization block. In particular it contains the
 *     pointer to the array of message blocks p_pmqinib->p_pmqmb
 *  count: number of message blocks in the queue at a given instant
 *  p_head: list of message blocks
 *  unused: index of the next free block in the array of message blocks
 *  p_freelist: when a message block is released it is inserted in this
 *  linked list. (Note: we could as well initialize the freelist at the
 *  beginning and we would not need the "unused" variable)
 */
typedef struct primsgq_control_block {
	QUEUE		swait_queue;	/* 優先度データキュー送信待ちキュー */
	const PMQINIB *p_pmqinib;	/* 初期化ブロックへのポインタ */
	QUEUE		rwait_queue;	/* 優先度データキュー受信待ちキュー */
	uint_t		count;			/* 優先度データキュー中のデータの数 */
	PMQMB		*p_head;		/* 最初のデータ */
	uint_t		unused;			/* 未使用データ管理ブロックの先頭 */
	PMQMB		*p_freelist;	/* 未割当てデータ管理ブロックのリスト */
} PMQCB;

/*
 *  優先度データキュー待ち情報ブロックの定義
 *
 *  この構造体は，同期・通信オブジェクトの待ち情報ブロックの共通部分
 *  （WINFO_WOBJ）を拡張（オブジェクト指向言語の継承に相当）したもので，
 *  最初の2つのフィールドが共通になっている．
 *  優先度データキューへの送信待ちと優先度データキューからの受信待ちで，
 *  同じ待ち情報ブロックを使う．
 */
typedef struct primsgq_waiting_information {
	WINFO		winfo;			/* 標準の待ち情報ブロック */
	PMQCB		*p_pmqcb;		/* 待っている優先度データキューの管理ブロック*/
	intptr_t	data;			/* 送受信データ */
	uint_t		msgsz;			/* message size */
	PRI			datapri;		/* データ優先度 */
} WINFO_PMQ;

/*
 *  優先度データキューIDの最大値（kernel_cfg.c）
 */
extern const ID	tmax_pmqid;

/*
 *  優先度データキュー初期化ブロックのエリア（kernel_cfg.c）
 */
extern const PMQINIB	pmqinib_table[];

/*
 *  優先度データキュー管理ブロックのエリア（kernel_cfg.c）
 */
extern PMQCB	pmqcb_table[];

/*
 *  優先度データキュー管理ブロックから優先度データキューIDを取り出すた
 *  めのマクロ
 */
#define	PMQID(p_pmqcb)	((ID)(((p_pmqcb) - pmqcb_table) + TMIN_PMQID))

/*
 *  優先度データキュー機能の初期化
 */
extern void	initialize_primsgq(void);

/*
 *  優先度データキュー管理領域へのデータの格納
 */
extern void	enqueue_primsg(PMQCB *p_pmqcb, intptr_t data, uint_t msgsz, PRI datapri);

/*
 *  優先度データキュー管理領域からのデータの取出し
 */
extern void	dequeue_primsg(PMQCB *p_pmqcb, intptr_t buffer, uint_t *p_msgsz, PRI *p_datapri);

/*
 *  優先度データキューへのデータ送信
 */
extern bool_t	send_primsg(PMQCB *p_pmqcb, intptr_t data, uint_t msgsz,
							PRI datapri, bool_t *p_reqdsp);

/*
 *  優先度データキューからのデータ受信
 */
extern bool_t	receive_primsg(PMQCB *p_pmqcb, intptr_t buffer,
							   uint_t *p_msgsz, PRI *p_datapri,
							   bool_t *p_reqdsp);

#endif /* TOPPERS_PRIMSGQ_H */
