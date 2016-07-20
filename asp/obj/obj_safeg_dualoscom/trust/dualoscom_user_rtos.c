#include "dualoscom_config.h"
#include "dualoscom_types.h"
// #include "pb1176t.h"

static void *memset (void *str, int c, size_t len )
{
    register char *st = (char *)str;
    while (len-- > 0) *st++ = c;
    return (void *)str;
}

/*
 * INTERNAL TYPES
 */
typedef struct {
    uint32_t *write;
    uint32_t *read;
    uint32_t *buffer;
    uint32_t  num_elements;
} dualoscom_fifo_t;

typedef struct {
    bool_t                   mutex_protection;
    uint32_t                 block_size;
    uint32_t                 num_blocks;
    void                    *blocks_buffer;
    dualoscom_fifo_t         rtos2gpos_fifo;
    dualoscom_fifo_t         gpos2rtos_fifo;
    dualoscom_filter_id_t   *rtos2gpos_filter_id;
    dualoscom_filter_id_t   *gpos2rtos_filter_id;
} dualoscom_channel_t;

typedef struct {
    bool_t initialized;
    dualoscom_filter_t  filters[DUALOSCOM_NUM_FILTERS];
    dualoscom_channel_t channels[DUALOSCOM_NUM_CHANNELS];
} dualoscom_user_t;

/*
 * MODULE VARIABLES
 */
static dualoscom_user_t the_dualoscom_user = {
    .initialized = false,
    .filters     = DUALOSCOM_FILTERS,
};

/*
 * FIFO
 */
static inline int __dualoscom_fifo_full(dualoscom_fifo_t *fifo)
{
//    uint32_t fifo_write;
//    uint32_t fifo_read;

//     volatile uint32_t result = 0;

//    fifo_write = *fifo->write;
//    assert(fifo_write >= 0);
//    assert(fifo_write < fifo->num_elements);

//     CP15_PM_CONTROL_WRITE(0);
//     CP15_PM_CYCLE_WRITE(0);
//     CP15_PM_CONTROL_WRITE(1);

//    fifo_read = *fifo->read;
//    assert(fifo_read >= 0);
//    assert(fifo_read < fifo->num_elements);

//     CP15_PM_CYCLE_READ(result);
//     syslog(LOG_NOTICE, "%u cycles", result); // +5ms is approximate

//    return ((fifo_write + 1) % fifo->num_elements) == fifo_read;

    return ((*fifo->write + 1) % fifo->num_elements) == *fifo->read;
}

static inline int __dualoscom_fifo_empty(dualoscom_fifo_t *fifo)
{
    return (*fifo->write == *fifo->read);
}

static inline int __dualoscom_fifo_enqueue(dualoscom_fifo_t *fifo, uint32_t value)
{
//    uint32_t fifo_write;

//    if (__dualoscom_fifo_full(fifo)) return -1;

//     volatile uint32_t result = 0;
// 
//     CP15_PM_CONTROL_WRITE(0);
//     CP15_PM_CYCLE_WRITE(0);
//     CP15_PM_CONTROL_WRITE(1);

//    fifo_write = *fifo->write;
//    assert(fifo_write >= 0);
//    assert(fifo_write < fifo->num_elements);

//     CP15_PM_CYCLE_READ(result);
//     syslog(LOG_NOTICE, "%u cycles", result); // +5ms is approximate

//    fifo->buffer[fifo_write] = value;
//    *fifo->write = (fifo_write + 1) % fifo->num_elements;

    if (__dualoscom_fifo_full(fifo)) return -1;

    fifo->buffer[*fifo->write] = value;
    *fifo->write = (*fifo->write + 1) % fifo->num_elements;

    return 0;
}

static inline int __dualoscom_fifo_dequeue(dualoscom_fifo_t *fifo, uint32_t *value)
{
//    uint32_t fifo_read;

//    if (__dualoscom_fifo_empty(fifo)) return -1;

//     volatile uint32_t result = 0;
// 
//     CP15_PM_CONTROL_WRITE(0);
//     CP15_PM_CYCLE_WRITE(0);
//     CP15_PM_CONTROL_WRITE(1);

//    fifo_read = *fifo->read;
//    assert(fifo_read >= 0);
//    assert(fifo_read < fifo->num_elements);

//     CP15_PM_CYCLE_READ(result);
//     syslog(LOG_NOTICE, "%u cycles deq", result); // +5ms is approximate
    
//    *value = fifo->buffer[*fifo->read];
//    *fifo->read = (*fifo->read + 1) % fifo->num_elements;

    if (__dualoscom_fifo_empty(fifo)) return -1;

    *value = fifo->buffer[*fifo->read];
    *fifo->read = (*fifo->read + 1) % fifo->num_elements;

    return 0;
}

/*
 * DEBUG
 */
// #define DUALOSCOM_DEBUG

#ifdef DUALOSCOM_DEBUG

#define DEBUG(enable,x,args...) \
    if(enable) syslog(LOG_NOTICE, "DEBUG (%s): " x, __func__ , ##args);

// #define DUALOSCOM_DEBUG_INIT
// #define DUALOSCOM_DEBUG_ALLOC
// #define DUALOSCOM_DEBUG_GETBUFFER
// #define DUALOSCOM_DEBUG_ENQUEUE
// #define DUALOSCOM_DEBUG_DEQUEUE

static void __dualoscom_print_blocks(dualoscom_channel_t *ch)
{
    int i, j;

    for(i=0; i<ch->num_blocks; i++) {
        syslog(LOG_NOTICE, "blocks(%d):", i);
        for(j=0; j<ch->block_size; j++) {
            syslog(LOG_NOTICE, " 0x%X", *((unsigned char *) (ch->blocks_buffer + i*(4 + ch->block_size) + 4 + j)));
        }
    }
}

static void __dualoscom_print_locks(dualoscom_channel_t *ch)
{
    int i;

    for(i=0; i<ch->num_blocks; i++) {
        syslog(LOG_NOTICE, "locks(%d): 0x%X", i, *((uint32_t *)(ch->blocks_buffer + i*(4 + ch->block_size))));
    }
}

static void __dualoscom_print_fifos(dualoscom_channel_t *ch)
{
    int i;

    syslog(LOG_NOTICE, "rtos2gpos_fifo.write: %u", *ch->rtos2gpos_fifo.write);
    syslog(LOG_NOTICE, "rtos2gpos_fifo.read: %u", *ch->rtos2gpos_fifo.read);
    syslog(LOG_NOTICE, "rtos2gpos_fifo.buffer: 0x%X", (uint32_t)ch->rtos2gpos_fifo.buffer);
    syslog(LOG_NOTICE, "rtos2gpos_fifo.num_elements: %u", (uint32_t)ch->rtos2gpos_fifo.num_elements);
    syslog(LOG_NOTICE, "rtos2gpos_fifo:");
    for(i=0; i<ch->num_blocks+1; i++) {
        syslog(LOG_NOTICE, " 0x%X", *(ch->rtos2gpos_fifo.buffer + i*4));
    }

    syslog(LOG_NOTICE, "gpos2rtos_fifo.write: %u", *ch->gpos2rtos_fifo.write);
    syslog(LOG_NOTICE, "gpos2rtos_fifo.read: %u", *ch->gpos2rtos_fifo.read);
    syslog(LOG_NOTICE, "gpos2rtos_fifo.buffer: 0x%X", (uint32_t)ch->gpos2rtos_fifo.buffer);
    syslog(LOG_NOTICE, "gpos2rtos_fifo.num_elements: %u", (uint32_t)ch->gpos2rtos_fifo.num_elements);
    syslog(LOG_NOTICE, "gpos2rtos_fifo:");
    for(i=0; i<ch->num_blocks+1; i++) {
        syslog(LOG_NOTICE, " 0x%X", *(ch->gpos2rtos_fifo.buffer + i*4));
    }
}

static void __dualoscom_print_channel(dualoscom_channel_t *ch)
{
    syslog(LOG_NOTICE, "mutex_protection: %s", (ch->mutex_protection) ? "true" : "false");
    syslog(LOG_NOTICE, "block_size: %u", ch->block_size);
    syslog(LOG_NOTICE, "num_blocks: %u", ch->num_blocks);
    syslog(LOG_NOTICE, "blocks_buffer: 0x%X", (uint32_t)ch->blocks_buffer);
    syslog(LOG_NOTICE, "rtos2gpos_filter_id: %u", *ch->rtos2gpos_filter_id);
    syslog(LOG_NOTICE, "gpos2rtos_filter_id: %u", *ch->gpos2rtos_filter_id);
}

static void __dualoscom_print_all()
{
    int i;
    dualoscom_channel_t *ch;

    syslog(LOG_NOTICE, "initialized: %s", (the_dualoscom_user.initialized)?"true":"false");

    for(i=0; i<DUALOSCOM_NUM_FILTERS; i++)
        syslog(LOG_NOTICE, "filter(%d): 0x%X", i, (uint32_t)the_dualoscom_user.filters[i]);

    for(i=0; i<DUALOSCOM_NUM_CHANNELS; i++) {
        ch = &the_dualoscom_user.channels[i];
        syslog(LOG_NOTICE, "CHANNEL %d", i);
        __dualoscom_print_channel(ch);
        __dualoscom_print_fifos(ch);
        __dualoscom_print_locks(ch);
        __dualoscom_print_blocks(ch);
    }
}

#else
#define DEBUG(enable,x,args...)
#endif

/*
 * KERNEL INTERFACE
 */
extern int __dualoscom_init_kernel(dualoscom_time_t timeout, void *shmem);
extern int __dualoscom_kernel_sendevent(dualoscom_channel_id_t channel_id);
extern int __dualoscom_kernel_waitevent(dualoscom_channel_id_t channel_id, dualoscom_time_t timeout);
extern int __dualoscom_kernel_waitgroup(dualoscom_group_id_t group_id, dualoscom_time_t timeout);

/*
 * dualoscom_init
 *
 * Initializates the Dual-OS Communications system.
 *
 * Errors: DUALOSCOM_NOPERM, DUALOSCOM_PARAM, DUALOSCOM_TIMEOUT
 *
 */
int dualoscom_init(bool_t master, dualoscom_time_t timeout)
{
    int ret, i;
    void *shmem;
    dualoscom_channel_t *ch;
    bool_t protection[DUALOSCOM_NUM_CHANNELS]   = DUALOSCOM_RTOS_PROTECTED;
    uint32_t num_blocks[DUALOSCOM_NUM_CHANNELS] = DUALOSCOM_NUM_BLOCKS;
    uint32_t block_size[DUALOSCOM_NUM_CHANNELS] = DUALOSCOM_BLOCK_SIZES;
    uint32_t offsets[DUALOSCOM_NUM_CHANNELS]    = DUALOSCOM_SHMEM_OFFSETS;

    if (!master) return 0; // TODO: complete this

    // 1.- create shared memory object and map it
    shmem = (void *)SHM_PHYS_ADDRESS;

    // 2.- initialize all channels
    memset(shmem, 0, DUALOSCOM_SHMEM_SIZE);
    for(i=0; i<DUALOSCOM_NUM_CHANNELS; i++) {
        ch = &the_dualoscom_user.channels[i];
        // local variables
        ch->mutex_protection = protection[i];
        ch->block_size = block_size[i];
        ch->num_blocks = num_blocks[i];
        ch->rtos2gpos_fifo.num_elements = ch->num_blocks + 1;
        ch->gpos2rtos_fifo.num_elements = ch->num_blocks + 1;
        // initialize pointers to shared memory
        ch->rtos2gpos_fifo.write  = shmem + offsets[i];
        ch->rtos2gpos_fifo.read   = ch->rtos2gpos_fifo.write  + 4;
        ch->rtos2gpos_fifo.buffer = ch->rtos2gpos_fifo.read   + 4;
        ch->gpos2rtos_fifo.write  = ch->rtos2gpos_fifo.buffer + 4 * (ch->num_blocks + 1);
        ch->gpos2rtos_fifo.read   = ch->gpos2rtos_fifo.write  + 4;
        ch->gpos2rtos_fifo.buffer = ch->gpos2rtos_fifo.read   + 4;
        ch->rtos2gpos_filter_id   = ch->gpos2rtos_fifo.buffer + 4 * (ch->num_blocks + 1);
        ch->gpos2rtos_filter_id   = ch->rtos2gpos_filter_id   + 4;
        ch->blocks_buffer         = ch->gpos2rtos_filter_id   + 4;
        // set default GPOS2RTOS filter
        *(ch->gpos2rtos_filter_id) = DUALOSCOM_GPOS2RTOS_DEFAULT_FILTER;
    }

    // 3.- init events and wait for the GPOS to initialize (with a timeout)
    ret = __dualoscom_init_kernel(timeout, shmem);
    assert(ret == 0);

    the_dualoscom_user.initialized = true;

#ifdef DUALOSCOM_DEBUG_INIT
__dualoscom_print_all();
#endif

    return DUALOSCOM_SUCCESS;
}

/*
 * dualoscom_filter_set
 *
 * Used by the receiver to set a communications filter.
 *
 * Errors: DUALOSCOM_NOPERM, DUALOSCOM_NOINIT, DUALOSCOM_PARAM
 *
 */
int dualoscom_filter_set(const dualoscom_channel_id_t channel_id,
                         const dualoscom_filter_id_t  filter_id)
{
    if (the_dualoscom_user.initialized == false) return DUALOSCOM_NOINIT;
    if (channel_id >= DUALOSCOM_NUM_CHANNELS) return DUALOSCOM_PARAM;
    if (filter_id >= DUALOSCOM_NUM_FILTERS) return DUALOSCOM_PARAM;

    *the_dualoscom_user.channels[channel_id].gpos2rtos_filter_id = filter_id;

    return DUALOSCOM_SUCCESS;
}

/*
 * dualoscom_block_alloc
 *
 * Allocates a block from a channel’s pool. This function never blocks the
 * calling task or process.
 *
 * Errors: DUALOSCOM_NOPERM, DUALOSCOM_NOINIT, DUALOSCOM_PARAM, DUALOSCOM_FULL
 *
 */
int dualoscom_block_alloc(const dualoscom_channel_id_t   channel_id,
                          dualoscom_block_id_t          *block_id)
{
    int i, ret;
    dualoscom_channel_t *ch;
    uint32_t *lock;

    if (the_dualoscom_user.initialized == false) return DUALOSCOM_NOINIT;
    if (channel_id >= DUALOSCOM_NUM_CHANNELS) return DUALOSCOM_PARAM;
    if (block_id == NULL) return DUALOSCOM_PARAM;

    ch = &the_dualoscom_user.channels[channel_id];

    for(i=0; i < ch->num_blocks; i++) {
        lock = ch->blocks_buffer + i*(4 + ch->block_size);
        // atomic: if (*lock == 0) *lock = 1, ret = true
        ret = __sync_bool_compare_and_swap (lock, 0, 1);
        if (ret == false) continue;
        block_id->block_id = (uint32_t)i;
        block_id->channel_id = channel_id;
#ifdef DUALOSCOM_DEBUG_ALLOC
        __dualoscom_print_locks(ch);
#endif
        return DUALOSCOM_SUCCESS;
    }
#ifdef DUALOSCOM_DEBUG_ALLOC
    __dualoscom_print_locks(ch);
#endif
    return DUALOSCOM_FULL;
}

/*
 * dualoscom_block_free
 *
 * Releases a block back to the channel’s pool where it belongs.
 *
 * Errors: DUALOSCOM_NOPERM, DUALOSCOM_NOINIT, DUALOSCOM_PARAM, DUALOSCOM_ALLOC
 *
 */
int dualoscom_block_free(const dualoscom_block_id_t block_id)
{
    dualoscom_channel_t *ch;
    uint32_t *lock;

    if (the_dualoscom_user.initialized == false) return DUALOSCOM_NOINIT;
    if (block_id.channel_id >= DUALOSCOM_NUM_CHANNELS) return DUALOSCOM_PARAM;

    ch = &the_dualoscom_user.channels[block_id.channel_id];

    if (block_id.block_id >= ch->num_blocks) return DUALOSCOM_PARAM;

    lock = ch->blocks_buffer + block_id.block_id*(4 + ch->block_size);
    if (*lock == 0) return DUALOSCOM_ALLOC;

    *lock = 0;

#ifdef DUALOSCOM_DEBUG_ALLOC
    __dualoscom_print_locks(ch);
#endif

    return DUALOSCOM_SUCCESS;
}

/*
 * dualoscom_block_getbuffer
 *
 * To obtain a pointer to the beginning of the memory region of a block.
 *
 * Errors: DUALOSCOM_NOPERM, DUALOSCOM_NOINIT, DUALOSCOM_PARAM, DUALOSCOM_ALLOC
 *
 */
int dualoscom_block_getbuffer(const dualoscom_block_id_t   block_id,
                              void                       **buffer,
                              uint32_t                    *size)
{
    dualoscom_channel_t *ch;
    uint32_t *lock;

    if (the_dualoscom_user.initialized == false) return DUALOSCOM_NOINIT;
    if (block_id.channel_id >= DUALOSCOM_NUM_CHANNELS) return DUALOSCOM_PARAM;

    ch = &the_dualoscom_user.channels[block_id.channel_id];

    if (block_id.block_id >= ch->num_blocks) return DUALOSCOM_PARAM;

    lock = ch->blocks_buffer + block_id.block_id*(4 + ch->block_size);
    if (*lock == 0) return DUALOSCOM_ALLOC;

    *buffer = (void *)(lock + 1);
    *size   = ch->block_size;

#ifdef DUALOSCOM_DEBUG_GETBUFFER
    __dualoscom_print_channel(ch);
    syslog(LOG_NOTICE, "buffer: 0x%X\n", (uint32_t)*buffer);
    syslog(LOG_NOTICE, "size: %u\n", (uint32_t)*size);
#endif

    return DUALOSCOM_SUCCESS;
}

/*
 * dualoscom_block_enqueue
 *
 * Enqueues a block to a channel’s FIFO. Note that the channel is implicit in
 * the block identifier.
 *
 * Errors: DUALOSCOM_NOPERM, DUALOSCOM_NOINIT, DUALOSCOM_PARAM,
 *         DUALOSCOM_FILTER, DUALOSCOM_ALLOC.
 *
 */
int dualoscom_block_enqueue(const dualoscom_block_id_t block_id)
{
    dualoscom_channel_t *ch;
    uint32_t *lock;
    int ret;
    dualoscom_filter_id_t filter_id;
    dualoscom_filter_t filter;
    void *buffer;

    if (the_dualoscom_user.initialized == false) return DUALOSCOM_NOINIT;
    if (block_id.channel_id >= DUALOSCOM_NUM_CHANNELS) return DUALOSCOM_PARAM;

    ch = &the_dualoscom_user.channels[block_id.channel_id];

    // TODO: mutex

    // filtering
//     volatile uint32_t result = 0;
// 
//     CP15_PM_CONTROL_WRITE(0);
//     CP15_PM_CYCLE_WRITE(0);
//     CP15_PM_CONTROL_WRITE(1);

    filter_id = *(ch->rtos2gpos_filter_id);
    assert(filter_id < DUALOSCOM_NUM_FILTERS);

//     CP15_PM_CYCLE_READ(result);
//     syslog(LOG_NOTICE, "%u cycles", result); // +5ms is approximate

    filter = the_dualoscom_user.filters[filter_id];
    buffer = ch->blocks_buffer + block_id.block_id*(4 + ch->block_size) + 4;
    if (filter(buffer, ch->block_size) == false) return DUALOSCOM_FILTER;

    lock = ch->blocks_buffer + block_id.block_id*(4 + ch->block_size);
    if (*lock == 0) return DUALOSCOM_ALLOC;

    ret = __dualoscom_fifo_enqueue(&ch->rtos2gpos_fifo, block_id.block_id);
    assert(ret == 0);

#ifdef DUALOSCOM_DEBUG_ENQUEUE
    __dualoscom_print_fifos(ch);
#endif

    return 0;
}

/*
 * dualoscom_block_dequeue
 *
 * Dequeues a block from a channel’s FIFO. This function never blocks the
 * calling task or process.
 *
 * Errors: DUALOSCOM_NOPERM, DUALOSCOM_NOINIT, DUALOSCOM_PARAM,
 *         DUALOSCOM_EMPTY, DUALOSCOM_ALLOC
 *
 */
int dualoscom_block_dequeue(const dualoscom_channel_id_t   channel_id,
                            dualoscom_block_id_t          *block_id)
{
    dualoscom_channel_t *ch;
    int ret;
    uint32_t *lock;

    // TODO: mutex and filtering
    if (the_dualoscom_user.initialized == false) return DUALOSCOM_NOINIT;
    if (channel_id >= DUALOSCOM_NUM_CHANNELS) return DUALOSCOM_PARAM;
    if (block_id == NULL) return DUALOSCOM_PARAM;

    ch = &the_dualoscom_user.channels[channel_id];

    ret = __dualoscom_fifo_dequeue(&ch->gpos2rtos_fifo, &block_id->block_id);
    if (ret == -1) return DUALOSCOM_EMPTY;

    assert(block_id->block_id < ch->num_blocks);
    block_id->channel_id  = channel_id;

    lock = ch->blocks_buffer + block_id->block_id*(4 + ch->block_size);
    if (*lock == 0) return DUALOSCOM_ALLOC;

#ifdef DUALOSCOM_DEBUG_ENQUEUE
    __dualoscom_print_fifos(ch);
#endif

    return 0;
}

/*
 * dualoscom_channel_event_send
 *
 * Sends an asynchronous channel event notification. If a notification
 * was already sent and has not been acknowledged yet by the receiver, the
 * function will return as if the notification was sent successfully.
 *
 * Errors: DUALOSCOM_NOPERM, DUALOSCOM_NOINIT, DUALOSCOM_PARAM
 *
 */
int dualoscom_channel_event_send(const dualoscom_channel_id_t channel_id)
{
    int ret;

    if (the_dualoscom_user.initialized == false) return DUALOSCOM_NOINIT;
    if (channel_id >= DUALOSCOM_NUM_CHANNELS) return DUALOSCOM_PARAM;

    ret = __dualoscom_kernel_sendevent(channel_id);
    assert(ret == 0);

    return 0;
}

/*
 * dualoscom_channel_event_wait
 *
 * This function makes the calling task - or process - wait for an
 * asynchronous event notification on a specific channel. If an event was
 * pending, the function acknowledges it and returns immediately.
 * Otherwise, the calling thread is put in waiting state until an event
 * arrives or a timeout occurs. The units in which the timeout is
 * expressed are implementation-dependent.
 *
 * Errors: DUALOSCOM_NOPERM, DUALOSCOM_NOINIT, DUALOSCOM_PARAM,
 *         DUALOSCOM_TIMEOUT
 *
 */
int dualoscom_channel_event_wait(const dualoscom_channel_id_t channel_id,
                                 const dualoscom_time_t       timeout)
{
    int ret;

    if (the_dualoscom_user.initialized == false) return DUALOSCOM_NOINIT;
    if (channel_id >= DUALOSCOM_NUM_CHANNELS) return DUALOSCOM_PARAM;

    ret = __dualoscom_kernel_waitevent(channel_id, timeout);
    assert(ret == 0);

    return 0;
}

/*
 * dualoscom_group_event_wait
 *
 * This function makes the calling task - or process - wait for an
 * asynchronous event notification on a specific group. If an event was
 * pending, the function acknowledges it and returns immediately.
 * Otherwise, the calling thread is put in waiting state until an event
 * arrives or a timeout occurs. The units in which the timeout is
 * expressed are implementation-dependent.
 *
 * Errors: DUALOSCOM_NOPERM, DUALOSCOM_NOINIT, DUALOSCOM_PARAM,
 *         DUALOSCOM_TIMEOUT
 *
 */
int dualoscom_group_event_wait(const dualoscom_group_id_t group_id,
                               const dualoscom_time_t     timeout)
{
    int ret;

    if (the_dualoscom_user.initialized == false) return DUALOSCOM_NOINIT;
    if (group_id >= DUALOSCOM_NUM_GROUPS) return DUALOSCOM_PARAM;

    ret = __dualoscom_kernel_waitgroup(group_id, timeout);
    assert(ret == 0);

    return 0;
}
