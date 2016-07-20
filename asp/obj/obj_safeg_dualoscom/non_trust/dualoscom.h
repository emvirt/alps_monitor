#ifndef DUALOSCOM
#define DUALOSCOM

#include "dualoscom_types.h"
#include "dualoscom_config.h"

/*
 * dualoscom_init
 *
 * Initializates the Dual-OS Communications system.
 *
 * Errors: DUALOSCOM_NOPERM, DUALOSCOM_PARAM, DUALOSCOM_TIMEOUT
 *
 */
int dualoscom_init(bool master, dualoscom_time_t timeout);

/*
 * dualoscom_filter_set
 *
 * Used by the receiver to set a communications filter.
 *
 * Errors: DUALOSCOM_NOPERM, DUALOSCOM_NOINIT, DUALOSCOM_PARAM
 *
 */
int dualoscom_filter_set(const dualoscom_channel_id_t channel_id,
                         const dualoscom_filter_id_t  filter_id);

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
                          dualoscom_block_id_t          *block_id);

/*
 * dualoscom_block_free
 *
 * Releases a block back to the channel’s pool where it belongs.
 *
 * Errors: DUALOSCOM_NOPERM, DUALOSCOM_NOINIT, DUALOSCOM_PARAM, DUALOSCOM_ALLOC
 *
 */
int dualoscom_block_free(const dualoscom_block_id_t block_id);

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
                              uint32_t                    *size);

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
int dualoscom_block_enqueue(const dualoscom_block_id_t block_id);

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
                            dualoscom_block_id_t          *block_id);

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
int dualoscom_channel_event_send(const dualoscom_channel_id_t channel_id);

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
                                 const dualoscom_time_t       timeout);

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
                               const dualoscom_time_t     timeout);

#endif /* DUALOSCOM_API */
