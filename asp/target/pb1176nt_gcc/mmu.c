#include "kernel_impl.h"
#include <sil.h>
#include "mmu.h"

/*
 * メモリのベースアドレスとサイズ
 */
#define SIZE_1M    0x00100000

#define DOMAIN_NO  1

#define PAGE_TABLE_SIZE  0x00004000 /* 4KB */

/*
 * 変換テーブルへの設定内容
 * va   : 仮想アドレス
 * pa   : 物理アドレス
 * size : サイズ 
 * s    : 共有指定
 * tex  : C Bとの組み合わせで変化
 * ap   : アクセス権
 * c    : キャッシュ
 * c    : バッファ
 */
typedef struct{
    uintptr_t   va;
    uintptr_t   pa;
    uint32_t    size;
    uint8_t     s;
    uint8_t     tex;
    uint8_t     ap;
    uint8_t     c;
    uint8_t     b;
}MEMORY_ATTRIBUTE;


/*
 * ページテーブル
 * TTBCR で N に 0 を指定しているため, 16KB 境界でなければならない.
 */
uint32_t PageTable[PAGE_TABLE_SIZE] __attribute__((aligned(16384)));

/*
 * ページテーブルのセクションを設定する
 */
static void
mmu_set_section(uintptr_t va, uintptr_t pa,
                uint8_t s, uint8_t tex, uint8_t ap, 
                uint8_t c, uint8_t b)
{
    uint32_t ttb;
    uint32_t *sptr;

    /*
     * ページテーブルのベースアドレスを取得
     */
    CP15_TTB0_READ(ttb);

    /*
     * VAからセクションのアドレスを取得
     */
    sptr  = (uint32_t *)((ttb & 0xFFFFC000) | (((va & 0xFFF00000) >> 20) << 2));
    *sptr = pa | (s << 16) | (tex << 12) | (ap << 10) | (c << 3) | (b << 2) | (1 << 1);
}


/*
 * 指定に従い,メモリのマッピングを行う
 * マッピングの単位は1MB 
 */
static void
mmu_map_memory(MEMORY_ATTRIBUTE *m_attribute)
{
    uint32_t size;
    uintptr_t va;
    uintptr_t pa;
    
    size = m_attribute->size;
    va   = m_attribute->va;
    pa   = m_attribute->pa;

    /* MB単位になるようにサイズを設定 */
    size = (size + 0x000FFFFF) & ~0x000FFFFF;

    while(size > 0){
        mmu_set_section(va, pa,
                        m_attribute->s,
                        m_attribute->tex,
                        m_attribute->ap,
                        m_attribute->c,
                        m_attribute->b);

        va   += SIZE_1M;
        pa   += SIZE_1M;
        size -= SIZE_1M;
    }
}



void
pb1176_mmu_init(void){
    MEMORY_ATTRIBUTE m_attribute;

    uint32_t bits = 0;

    /* プリフェッチバッファをクリア */
    pb1176_pbuffer_flash();

    /*
     * TTBR0を用いる用に指定
     */
    CP15_TTBCR_WRITE(0);

    /*
     * 変換テーブル(TT)を確保する
     * Cビット(Inner Cachable)をセット 
     */
    CP15_TTB0_WRITE((((uint32_t)PageTable)|CP15_TTB0_C));

    /* プリフェッチバッファをクリア */
    pb1176_pbuffer_flash();

    /*
     * 全セクションを物理アドレス = 仮想アドレス
     * ノンキャッシャブル，ノンバッファブルとする 
     */
    m_attribute.pa   = 0x00000000;
    m_attribute.va   = m_attribute.pa;
    m_attribute.size = 0x80000000;
    m_attribute.s    = 0;          /* 0:not-shared */
    m_attribute.ap   = 3;          /* フルアクセス */
    m_attribute.tex  = 0;          /* 000: Strongly Ordered */
    m_attribute.c    = 0;
    m_attribute.b    = 0;

    mmu_map_memory (&m_attribute);

    m_attribute.pa   = 0x80000000;
    m_attribute.va   = m_attribute.pa;

    mmu_map_memory (&m_attribute);

    /*
     * RAMの設定(テキストアドレスなどに従って書き換える)
     */

#ifdef TOPPERS_WITH_SAFEG
    m_attribute.pa   = 0x6000000;
#else
    m_attribute.pa   = 0x0;
#endif

    m_attribute.va   = m_attribute.pa;
    m_attribute.size = SIZE_1M*2;
    m_attribute.c    = 1;    /* Inner Write-Back, no Allocate on Write */
    m_attribute.b    = 1;

    mmu_map_memory (&m_attribute);

    /*
     * Set 1MB of shared memory between both worlds
     */
#ifdef TOPPERS_WITH_SAFEG
    m_attribute.pa   = 0x4000000; // 0x6000000 + SIZE_1M*2; // 0x6200000
    m_attribute.va   = m_attribute.pa;
    m_attribute.size = SIZE_1M;
    m_attribute.s    = 1;    /* 0:shared */
    m_attribute.c    = 1;    /* Write-Through */
    m_attribute.b    = 0;
#endif

    /*
     * TCM settings
     */
    m_attribute.pa   = TCM_BASE_ADDRESS;
    m_attribute.va   = m_attribute.pa;
    m_attribute.size = SIZE_1M;
    m_attribute.s    = 0;    /* not-shared */
    m_attribute.ap   = 3;    /* フルアクセス */
    m_attribute.tex  = 1;    /* Outer and Inner Noncachable */
    m_attribute.c    = 0;
    m_attribute.b    = 0;

    mmu_map_memory (&m_attribute);

    /*
     * ドメイン番号をセット
     */
    CP15_DOMAINS_WRITE(DOMAIN_NO);


    /*
     * CONTROLコプロセッサの Mビット，XPビットをセットして，
     * MMUを有効にする  
     */
    CP15_CONTROL_READ(bits);
    bits |= CP15_CONTROL_M_BIT | CP15_CONTROL_XP_BIT;
    CP15_CONTROL_WRITE(bits);
}

/*
 * icache 有効
 * eratta にあるように, i cache の invalidate は1度で効かないことがあるので,
 * 複数回実行する必要がある.
 */
void
icache_enable(void)
{
    unsigned long int bits = 0;
    CP15_ICACHE_INVALIDATE();
    CP15_ICACHE_INVALIDATE();
    CP15_ICACHE_INVALIDATE();
    CP15_ICACHE_INVALIDATE();
    CP15_ICACHE_INVALIDATE();
    CP15_ICACHE_INVALIDATE();
    CP15_ICACHE_INVALIDATE();
    CP15_ICACHE_INVALIDATE();
    CP15_ICACHE_INVALIDATE();
    CP15_ICACHE_INVALIDATE();
    CP15_CONTROL_READ(bits);
    bits |= CP15_CONTROL_I_BIT;
    CP15_CONTROL_WRITE(bits);
}

/* dcache 有効 */
void
dcache_enable(void)
{
    unsigned long int bits = 0;
    CP15_DCACHE_CLEAN_AND_INVALIDATE();
    CP15_CONTROL_READ(bits);
    bits |= CP15_CONTROL_C_BIT;
    CP15_CONTROL_WRITE(bits);
}

void pb1176_cache_init(){
    icache_enable();
    dcache_enable();
}
