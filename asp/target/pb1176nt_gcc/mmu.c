#include "kernel_impl.h"
#include <sil.h>
#include "mmu.h"

/*
 * ����Υ١������ɥ쥹�ȥ�����
 */
#define SIZE_1M    0x00100000

#define DOMAIN_NO  1

#define PAGE_TABLE_SIZE  0x00004000 /* 4KB */

/*
 * �Ѵ��ơ��֥�ؤ���������
 * va   : ���ۥ��ɥ쥹
 * pa   : ʪ�����ɥ쥹
 * size : ������ 
 * s    : ��ͭ����
 * tex  : C B�Ȥ��Ȥ߹�碌���Ѳ�
 * ap   : ����������
 * c    : ����å���
 * c    : �Хåե�
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
 * �ڡ����ơ��֥�
 * TTBCR �� N �� 0 ����ꤷ�Ƥ��뤿��, 16KB �����Ǥʤ���Фʤ�ʤ�.
 */
uint32_t PageTable[PAGE_TABLE_SIZE] __attribute__((aligned(16384)));

/*
 * �ڡ����ơ��֥�Υ������������ꤹ��
 */
static void
mmu_set_section(uintptr_t va, uintptr_t pa,
                uint8_t s, uint8_t tex, uint8_t ap, 
                uint8_t c, uint8_t b)
{
    uint32_t ttb;
    uint32_t *sptr;

    /*
     * �ڡ����ơ��֥�Υ١������ɥ쥹�����
     */
    CP15_TTB0_READ(ttb);

    /*
     * VA���饻�������Υ��ɥ쥹�����
     */
    sptr  = (uint32_t *)((ttb & 0xFFFFC000) | (((va & 0xFFF00000) >> 20) << 2));
    *sptr = pa | (s << 16) | (tex << 12) | (ap << 10) | (c << 3) | (b << 2) | (1 << 1);
}


/*
 * ����˽���,����Υޥåԥ󥰤�Ԥ�
 * �ޥåԥ󥰤�ñ�̤�1MB 
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

    /* MBñ�̤ˤʤ�褦�˥����������� */
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

    /* �ץ�ե��å��Хåե��򥯥ꥢ */
    pb1176_pbuffer_flash();

    /*
     * TTBR0���Ѥ����Ѥ˻���
     */
    CP15_TTBCR_WRITE(0);

    /*
     * �Ѵ��ơ��֥�(TT)����ݤ���
     * C�ӥå�(Inner Cachable)�򥻥å� 
     */
    CP15_TTB0_WRITE((((uint32_t)PageTable)|CP15_TTB0_C));

    /* �ץ�ե��å��Хåե��򥯥ꥢ */
    pb1176_pbuffer_flash();

    /*
     * ������������ʪ�����ɥ쥹 = ���ۥ��ɥ쥹
     * �Υ󥭥�å���֥롤�Υ�Хåե��֥�Ȥ��� 
     */
    m_attribute.pa   = 0x00000000;
    m_attribute.va   = m_attribute.pa;
    m_attribute.size = 0x80000000;
    m_attribute.s    = 0;          /* 0:not-shared */
    m_attribute.ap   = 3;          /* �ե륢������ */
    m_attribute.tex  = 0;          /* 000: Strongly Ordered */
    m_attribute.c    = 0;
    m_attribute.b    = 0;

    mmu_map_memory (&m_attribute);

    m_attribute.pa   = 0x80000000;
    m_attribute.va   = m_attribute.pa;

    mmu_map_memory (&m_attribute);

    /*
     * RAM������(�ƥ����ȥ��ɥ쥹�ʤɤ˽��äƽ񤭴�����)
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
    m_attribute.ap   = 3;    /* �ե륢������ */
    m_attribute.tex  = 1;    /* Outer and Inner Noncachable */
    m_attribute.c    = 0;
    m_attribute.b    = 0;

    mmu_map_memory (&m_attribute);

    /*
     * �ɥᥤ���ֹ�򥻥å�
     */
    CP15_DOMAINS_WRITE(DOMAIN_NO);


    /*
     * CONTROL���ץ��å��� M�ӥåȡ�XP�ӥåȤ򥻥åȤ��ơ�
     * MMU��ͭ���ˤ���  
     */
    CP15_CONTROL_READ(bits);
    bits |= CP15_CONTROL_M_BIT | CP15_CONTROL_XP_BIT;
    CP15_CONTROL_WRITE(bits);
}

/*
 * icache ͭ��
 * eratta �ˤ���褦��, i cache �� invalidate ��1�٤Ǹ����ʤ����Ȥ�����Τ�,
 * ʣ����¹Ԥ���ɬ�פ�����.
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

/* dcache ͭ�� */
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
