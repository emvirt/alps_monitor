/*
 *  TOPPERS/FMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Flexible MultiProcessor Kernel
 *
 *  Copyright (C) 2008-2009 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_serial.h 370 2009-09-09 09:09:21Z ertl-honda $
 */

/*
 * シリアルI/Oデバイス（SIO）ドライバ（NIOS2_DEV用）
 */

#ifndef TOPPERS_TARGET_SERIAL_H
#define TOPPERS_TARGET_SERIAL_H

#include "nios2_system.h"

/*
 *  SIOの割込みハンドラのベクタ番号
 */
#define INHNO_SIO_PORT1  (0x10000 | JTAG_UART_1_INT)  /* 割込みハンドラ番号 */
#define INTNO_SIO_PORT1  (0x10000 | JTAG_UART_1_INT)  /* 割込み番号 */
#define INTPRI_SIO_PORT1 (-2)            /* 割込み優先度 */
#define INTATR_SIO_PORT1 0U              /* 割込み属性 */

#define INHNO_SIO_PORT2  (0x20000 | JTAG_UART_2_INT)  /* 割込みハンドラ番号 */
#define INTNO_SIO_PORT2  (0x20000 | JTAG_UART_2_INT)  /* 割込み番号 */
#define INTPRI_SIO_PORT2 (-2)            /* 割込み優先度 */
#define INTATR_SIO_PORT2 0U              /* 割込み属性 */

#define INHNO_SIO_PORT3  (0x30000 | JTAG_UART_3_INT)  /* 割込みハンドラ番号 */
#define INTNO_SIO_PORT3  (0x30000 | JTAG_UART_3_INT)  /* 割込み番号 */
#define INTPRI_SIO_PORT3 (-2)            /* 割込み優先度 */
#define INTATR_SIO_PORT3 0U              /* 割込み属性 */

#define INHNO_SIO_PORT4  (0x40000 | JTAG_UART_4_INT)  /* 割込みハンドラ番号 */
#define INTNO_SIO_PORT4  (0x40000 | JTAG_UART_4_INT)  /* 割込み番号 */
#define INTPRI_SIO_PORT4 (-2)            /* 割込み優先度 */
#define INTATR_SIO_PORT4 0U              /* 割込み属性 */

/*
 *  チップ依存モジュール（JTAG UART用）
 */
#include "nios2_gcc/jtag_uart.h"

#endif /* TOPPERS_TARGET_SERIAL_H */
