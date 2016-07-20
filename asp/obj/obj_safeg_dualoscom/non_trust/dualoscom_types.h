#ifndef DUALOSCOM_TYPES
#define DUALOSCOM_TYPES

#include <stdbool.h>   /* bool type (for filter function) */
#include <stdint.h>    /* uint32_t */

/*
 * ERRORS
 */
#define DUALOSCOM_SUCCESS   (0) // success
#define DUALOSCOM_NOPERM   (-1) // not enough permissions.
#define DUALOSCOM_NOINIT   (-2) // communications system not initialized yet.
#define DUALOSCOM_PARAM    (-3) // incorrect parameter.
#define DUALOSCOM_FULL     (-4) // there are no free blocks.
#define DUALOSCOM_ENQ      (-5) // the block is enqueued.
#define DUALOSCOM_FILTER   (-6) // the block was discarded
#define DUALOSCOM_EMPTY    (-7) // no block is enqueued.
#define DUALOSCOM_ALLOC    (-8) // the block is not allocated.
#define DUALOSCOM_TIMEOUT  (-9) // a timeout occurred

/*
 * TYPES
 */
typedef uint32_t dualoscom_filter_id_t;

typedef uint32_t dualoscom_channel_id_t;

typedef struct {
    dualoscom_channel_id_t channel_id;
    uint32_t block_id;
} dualoscom_block_id_t;

typedef uint32_t dualoscom_group_id_t;

typedef uint32_t dualoscom_time_t;

typedef bool (*dualoscom_filter_t)(void *buffer, uint32_t size);

#endif /* DUALOSCOM_TYPES */
