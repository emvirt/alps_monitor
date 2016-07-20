/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 *
 *  Copyright (C) 2006-2007 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: mpcore.c 791 2008-03-11 12:00:46Z honda $
 */

#include "kernel_impl.h"

/*
 * GIC CPUインタフェース関連
 */

/*
 * CPUインタフェースの初期化
 */
void
gic_cpu_init(void)
{
    /* 最低優先度に設定 */
    gic_cpu_set_priority(GIC_MIN_PRIORITY);

    /* 割込み優先度の全ビット有効に */
    gic_cpu_set_bp(GIC_CPUIF_BPR_ALL);

    /* CPUインタフェースを有効に */
    sil_wrw_mem((void *)GIC_CPUIF_CR, GIC_CPUIF_CR_EN);

    /* EOI をセット */
    sil_wrw_mem((void *)GIC_CPUIF_EOI, sil_rew_mem((void *)GIC_CPUIF_IAR));
}

/*
 * CPUインタフェースの終了
 */
void
gic_cpu_stop(void)
{
    /* CPUインタフェースを無効に */
    sil_wrw_mem((void *)(GIC_CPUIF_CR), 0);
}


/*
 * GIC DISインタフェース関連
 */

/*
 * 割込み禁止
 */
void
gic_disable_int(uint8_t id)
{
    uint16_t offset_addr;
    uint16_t offset_bit;

    offset_addr = (id / 32) * 4;
    offset_bit  = id % 32;

    sil_wrw_mem((void *)(GIC_CE + offset_addr), (1 << offset_bit));
}

/*
 * 割込み許可
 */
void
gic_enable_int(uint8_t id)
{
    uint16_t offset_addr;
    uint16_t offset_bit;

    offset_addr = (id / 32) * 4;
    offset_bit  = id % 32;

    sil_wrw_mem((void *)(GIC_SE + offset_addr), (1 << offset_bit));
}

/*
 * 割込みペンディングクリア
 */
void
gic_clear_pending(uint8_t id)
{
    uint16_t offset_addr;
    uint16_t offset_bit;

    offset_addr = (id / 32) * 4;
    offset_bit  = id % 32;

    sil_wrw_mem((void *)(GIC_CP + offset_addr), (1 << offset_bit));
}

/*
 * 割込みペンディングセット
 */
void
gic_set_pending(uint8_t id){
    uint16_t offset_addr;
    uint16_t offset_bit;

    offset_addr = (id / 32) * 4;
    offset_bit  = id % 32;

    sil_wrw_mem((void *)(GIC_SP + offset_addr), (1 << offset_bit));
}

/*
 * 割込み要求のチェック
 */
bool_t
gic_probe_int(uint8_t id){
    uint32_t state;
    uint16_t offset_addr;
    uint16_t offset_bit;

    offset_addr = (id / 32) * 4;
    offset_bit  = id % 32;

    state = sil_rew_mem((void *)(GIC_SP + offset_addr));

    if((state & (1 << offset_bit)) == (1 << offset_bit)){
        return(true);
    }else{
        return(false);
    }
}

/*
 * 割込みコンフィギュレーション設定
 */
void
gic_config(uint8_t id, uint8_t config){
    uint16_t offset_addr;
    uint16_t offset_bit;
    uint32_t cfgr_reg_val;

    offset_addr = (id / 16) * 4;
    offset_bit  = (id % 16) * 2;

    cfgr_reg_val  = sil_rew_mem((void *)(GIC_ICR + offset_addr));
    cfgr_reg_val &= ~(0x03U << offset_bit);
    cfgr_reg_val |= (0x03U & config) << offset_bit;

    sil_wrw_mem((void *)(GIC_ICR + offset_addr), cfgr_reg_val);
}

/*
 * 割込み優先度のセット
 * 内部表現で渡す．
 */
void
gic_set_priority(uint8_t id, int pri){
    uint16_t offset_addr;
    uint16_t offset_bit;
    uint32_t pr_reg_val;

    offset_addr = (id / 4) * 4;
    offset_bit  = ((id % 4) * 8) + 4;

    pr_reg_val  = sil_rew_mem((void *)(GIC_IPR + offset_addr));
    pr_reg_val &= ~(0xfU << offset_bit);
    pr_reg_val |= (0xfU & pri) << (offset_bit);

    sil_wrw_mem((void *)(GIC_IPR + offset_addr), pr_reg_val);
}

/*
 * GIC割込みターゲットの設定
 * cpusはターゲットとするCPUのビットパターンで指定
 *   CPU0 : 0x01
 *   CPU1 : 0x02
 *   CPU2 : 0x04
 *   CPU3 : 0x08
 */
void
gic_set_target(uint8_t id, uint8_t cpus)
{
    uint32_t offset_addr;
    uint32_t offset_bit;
    uint32_t itr_reg_val;

    offset_addr = (id / 4) * 4;
    offset_bit  = (id % 4) * 8;

    itr_reg_val  = sil_rew_mem((void *)(GIC_ITR + offset_addr));
    itr_reg_val &= ~(0xf << offset_bit);
    itr_reg_val |= (cpus << offset_bit);


    sil_wrw_mem((void *)(GIC_ITR + offset_addr), itr_reg_val);
}

/*
 * GICインタフェースの初期化
 */
void
gic_init(void)
{
    int i;

    /* GICインタフェースを無効に */
    sil_wrw_mem((void *)(GIC_CR), 0);

    /* 割込みを全て禁止 */
    for(i = 0; i < GIC_TNUM_INT/32; i++){
        sil_wrw_mem((void *)(GIC_CE + (4 * i)), 0xffffffff);
    }

    /* ペンディングをクリア */
    for(i = 0; i < GIC_TNUM_INT/32; i++){
        sil_wrw_mem((void *)(GIC_CP + (4 * i)), 0xffffffff);
    }

    /* 優先度最低に設定  */
    for(i = 0; i < GIC_TNUM_INT/4; i++){
        sil_wrw_mem((void *)(GIC_IPR + (4 * i)), 0xffffffff);
    }

    /* モード初期化(1-N Level) */
    for(i = 0; i < GIC_TNUM_INT/16; i++){
        sil_wrw_mem((void *)(GIC_ICR + (4 * i)), 0x55555555);
    }

    /* GICインタフェースを有効に */
    sil_wrw_mem((void *)(GIC_CR), GIC_CR_ENABLE);
}

/*
 * GICインタフェースの終了
 */
void
gic_stop(void)
{
    /* GICインタフェースを無効に */
    sil_wrw_mem((void *)(GIC_CR), 0);
}
