#include "kernel_impl.h"
#include <sil.h>
#include "mmu.h"

/*
 * �������̃x�[�X�A�h���X�ƃT�C�Y
 */
#define SIZE_1M    0x00100000

#define DOMAIN_NO  1

#define PAGE_TABLE_SIZE  0x00004000 /* 4KB */

/*
 * �ϊ��e�[�u���ւ̐ݒ���e
 * va   : ���z�A�h���X
 * pa   : �����A�h���X
 * size : �T�C�Y 
 * ns   : Non-Secure access
 * s    : ���L�w��
 * tex  : C B�Ƃ̑g�ݍ��킹�ŕω�
 * ap   : �A�N�Z�X��
 * c    : �L���b�V��
 * c    : �o�b�t�@
 */
typedef struct{
    uintptr_t   va;
    uintptr_t   pa;
    uint32_t    size;
    uint8_t     ns;
    uint8_t     s;
    uint8_t     tex;
    uint8_t     ap;
    uint8_t     c;
    uint8_t     b;
}MEMORY_ATTRIBUTE;

/*
 * �y�[�W�e�[�u��
 * TTBCR �� N �� 0 ���w�肵�Ă��邽��, 16KB ���E�łȂ���΂Ȃ�Ȃ�.
 */
uint32_t PageTable[PAGE_TABLE_SIZE] __attribute__((aligned(16384)));

/*
 * �y�[�W�e�[�u���̃Z�N�V������ݒ肷��
 */
static void
mmu_set_section(uintptr_t va, uintptr_t pa,
                uint8_t ns, uint8_t s, uint8_t tex,
                uint8_t ap, uint8_t c, uint8_t b)
{
    uint32_t ttb;
    uint32_t *sptr;

    /*
     * �y�[�W�e�[�u���̃x�[�X�A�h���X���擾
     */
    CP15_TTB0_READ(ttb);

    /*
     * VA����Z�N�V�����̃A�h���X���擾
     */
    sptr  = (uint32_t *)((ttb & 0xFFFFC000) | (((va & 0xFFF00000) >> 20) << 2));
    *sptr = pa | (ns << 19) | (s << 16) | (tex << 12) | (ap << 10) | (c << 3) | (b << 2) | (1 << 1);
}

/*
 * �w��ɏ]��,�������̃}�b�s���O���s��
 * �}�b�s���O�̒P�ʂ�1MB 
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

    /* MB�P�ʂɂȂ�悤�ɃT�C�Y��ݒ� */
    size = (size + 0x000FFFFF) & ~0x000FFFFF;

    while(size > 0){
        mmu_set_section(va, pa,
                        m_attribute->ns,
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

    /* �v���t�F�b�`�o�b�t�@���N���A */
    pb1176_pbuffer_flash();

    /*
     * TTBR0��p����p�Ɏw��
     */
    CP15_TTBCR_WRITE(0);

    /*
     * �ϊ��e�[�u��(TT)���m�ۂ���
     * C�r�b�g(Inner Cachable)���Z�b�g 
     */
    CP15_TTB0_WRITE((((uint32_t)PageTable)|CP15_TTB0_C));

    /* �v���t�F�b�`�o�b�t�@���N���A */
    pb1176_pbuffer_flash();

    /*
     * �S�Z�N�V�����𕨗��A�h���X = ���z�A�h���X
     * �m���L���b�V���u���C�m���o�b�t�@�u���Ƃ��� 
     */
    m_attribute.pa   = 0x00000000;
    m_attribute.va   = m_attribute.pa;
    m_attribute.size = 0x80000000;
    m_attribute.ns   = 0;          /* ns=0: Secure access */
    m_attribute.s    = 0;          /* not-shared */
    m_attribute.ap   = 3;          /* �t���A�N�Z�X */
    m_attribute.tex  = 0;          /* 000 00 Strongly Ordered */
    m_attribute.c    = 0;
    m_attribute.b    = 0;

    mmu_map_memory (&m_attribute);

    /*
     * SRAM/RAM�̐ݒ�
     */
#ifdef TOPPERS_WITH_SAFEG
    m_attribute.pa   = 0x10300000;
#else
    m_attribute.pa   = 0x0;
#endif

    m_attribute.va   = m_attribute.pa;
    m_attribute.size = SIZE_1M*2;
    m_attribute.c    = 1;    /* cb:11 No Allocate on Write */
    m_attribute.b    = 1;

    mmu_map_memory (&m_attribute);

    /*
     * Set 1MB of shared memory between both worlds
     */
#ifdef TOPPERS_WITH_SAFEG
    m_attribute.pa   = 0x4000000; // 0x6000000 + SIZE_1M*2; // 0x6200000
    m_attribute.va   = m_attribute.pa;
    m_attribute.size = SIZE_1M;
    m_attribute.ns   = 1;    /* 1: Non-Secure access */
    m_attribute.s    = 1;    /* 1:shared */
    m_attribute.ap   = 3;    /* �t���A�N�Z�X */
    m_attribute.tex  = 1;    /* 100 Outer and Inner Noncachable */
    m_attribute.c    = 0;
    m_attribute.b    = 0;
#endif

    /*
     * TCM settings
     */
    m_attribute.pa   = TCM_BASE_ADDRESS;
    m_attribute.va   = m_attribute.pa;
    m_attribute.size = SIZE_1M;
    m_attribute.ns   = 1;    /* ns=1: Non-Secure access */
    m_attribute.s    = 0;    /* not-shared */
    m_attribute.ap   = 3;    /* �t���A�N�Z�X */
    m_attribute.tex  = 1;    /* Outer and Inner Noncachable */
    m_attribute.c    = 0;
    m_attribute.b    = 0;

    mmu_map_memory (&m_attribute);

    /*
     * �h���C���ԍ����Z�b�g
     */
    CP15_DOMAINS_WRITE(DOMAIN_NO);

    /*
     * CONTROL�R�v���Z�b�T�� M�r�b�g�CXP�r�b�g���Z�b�g���āC
     * MMU��L���ɂ���
     */
    CP15_CONTROL_READ(bits);
    bits |= CP15_CONTROL_M_BIT | CP15_CONTROL_XP_BIT;
    CP15_CONTROL_WRITE(bits);
}

/*
 * icache �L��
 * eratta �ɂ���悤��, i cache �� invalidate ��1�x�Ō����Ȃ����Ƃ�����̂�,
 * ��������s����K�v������.
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

/* dcache �L�� */
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
    dcache_enable();
    icache_enable();
}
