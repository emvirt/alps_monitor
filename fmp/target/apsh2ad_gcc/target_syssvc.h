/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2007 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *	Copyright (C) 2010 by Industrial Technology Institute,
 *								Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id$
 */

/*
 *  システムサービスのターゲット依存部（APSH2AD用）
 *
 *  システムサービスのターゲット依存部のインクルードファイル．このファ
 *  イルの内容は，コンポーネント記述ファイルに記述され，このファイルは
 *  無くなる見込み．
 */

#ifndef TOPPERS_TARGET_SYSSVC_H
#define TOPPERS_TARGET_SYSSVC_H

/*
 *  ターゲットシステムのハードウェア資源の定義
 */
#include "apsh2ad.h"

/*
 *  プロセッサ依存の定義
 */
#include "sh2a_dual_gcc/prc_syssvc.h"

/*
 *  トレースログに関する設定
 */
#ifdef TOPPERS_TRACE_ENABLE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_TRACE_ENABLE */

/*
 *  起動メッセージのターゲットシステム名
 */
#define TARGET_NAME    "APSH2AD(SH7205)"

/*
 *  起動メッセージにターゲット依存部の著作権表示を
 *  追加するためのマクロ．
 */
#ifdef PRC_COPYRIGHT
#define TARGET_COPYRIGHT	PRC_COPYRIGHT
#endif /* PRC_COPYRIGHT */

/*
 *  システムログの低レベル出力のための文字出力
 *
 *  ターゲット依存の方法で，文字cを表示/出力/保存する．
 */
extern void	target_fput_log(char_t c);

/*
 *  シリアルポート数の定義
 */
#define TNUM_PORT        1        /* サポートするシリアルポートの数 */
#define TNUM_SIOP        TNUM_PORT


/*
 *  ボーレート
 */
#define BPS_SETTING 		38400
#define SCIF0_BPS_SETTING  (((PCLOCK / 32) / BPS_SETTING) - 1)
#define SCIF1_BPS_SETTING  (((PCLOCK / 32) / BPS_SETTING) - 1)

/*
 *  クロックソース
 */
/*  内部クロック／SCK 端子は入力端子（入力信号は無視）  */
#define SCIF0_CKE_SETTING	0x00U
#define SCIF1_CKE_SETTING	0x00U


#define SIO_INIT_DLY		1000000	/* 起動時の待ち時間(1ビット分) */

/*
 *  システムログタスク関連の定数の定義
 *
 *  デフォルト値の通り．
 */

/*
 *  システムログが使用するポート数と実際のポート数の整合性をチェック
 *  
 *  　ターゲットボードにシリアルポートのコネクタが１つしかないため、
 *  　ターゲット依存部のデフォルトではシステムログ機能をグローバル方式
 *  　で使用することを前提としている。（ソフトウェア自体はTNUM_PORTの
 *  　定義を2に変更すれば、2ポート使用できるが、ハードウェア側もコネク
 *  　タを追加する必要がある。）グローバル方式を指定するG_SYSLOGマクロ
 *  　の定義は、アプリケーション側のMakefileで行う。サンプルプログラム
 *  　では、G_SYSLOGマクロが定義されていないため、デフォルトのままビル
 *  　ドするとmake depend時に以下のエラーメッセージが表示されるが、
 *  　G_SYSLOGマクロの定義を追加することにより、正しくビルドできるよう
 *  　になる。
 *  　G_SYSLOGの定義方法は、ユーザーズマニュアルfmp/user.txtの
 *  　　「6.1 Makefileの変数定義」
 *  　　　「(M) システムログ機能のグローバル方式の指定」を参照
 */
#if !defined(G_SYSLOG) && (TNUM_PORT < TNUM_PRCID)
#error !defined(G_SYSLOG) && (TNUM_PORT < TNUM_PRCID) : check G_SYSLOG and TNUM_PORT !
#endif /* defined(G_SYSLOG) && (TNUM_PORT == 1) */

#endif /* TOPPERS_TARGET_SYSSVC_H */
