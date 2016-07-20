#ifndef DUALOSCOM_CONFIG
#define DUALOSCOM_CONFIG

#include <stdbool.h>
#include <stdint.h>

#define SHM_PHYS_ADDRESS                   0x4000000UL /* 64MB */

#define DUALOSCOM_NUM_CHANNELS             2
#define DUALOSCOM_CHANNEL_0                0
#define DUALOSCOM_CHANNEL_1                1

#define DUALOSCOM_NUM_BLOCKS               {10,5}
#define DUALOSCOM_BLOCK_SIZES              {32,8} // must be multiple of 4 (alignment)
#define DUALOSCOM_RTOS_PROTECTED           {false,false}
#define DUALOSCOM_GPOS_PROTECTED           {false,false}

// channel size = 24  <-- write cursor, read cursor, filter_id x 2
//                + 2 * 4 * (num_blocks + 1) <-- 2 x FIFO queues
//                + num_blocks * (block_size + 4) <--- blocks with lock
#define DUALOSCOM_SHMEM_OFFSETS            {0,472} // TODO: cache-align offsets
#define DUALOSCOM_SHMEM_PENDING_EVENTS     604
#define DUALOSCOM_SHMEM_SIZE               620     // DUALOSCOM_SHMEM_PENDING_EVENTS + 8*num_channels

#define DUALOSCOM_NUM_GROUPS               3
#define DUALOSCOM_CHANNEL2GROUPS           {{true,true,false},{true,false,true}}
#define DUALOSCOM_GROUP_0                  0
#define DUALOSCOM_GROUP_1                  1
#define DUALOSCOM_GROUP_2                  2

#define DUALOSCOM_NUM_FILTERS              2
#define DUALOSCOM_FILTERS                  {filter_dummy, filter_even}
#define DUALOSCOM_FILTER_ID_DUMMY          0
#define DUALOSCOM_FILTER_ID_EVEN           1
#define DUALOSCOM_RTOS2GPOS_DEFAULT_FILTER DUALOSCOM_FILTER_ID_DUMMY
#define DUALOSCOM_GPOS2RTOS_DEFAULT_FILTER DUALOSCOM_FILTER_ID_DUMMY

#include <linux/ioctl.h>

// 'j' 00-3F   linux/joystick.h
#define DUALOSCOM_IOCTL_CMD_INIT           _IO('j', 0x40)
#define DUALOSCOM_IOCTL_CMD_SENDEVENT      _IOW('j', 0x41, int)
#define DUALOSCOM_IOCTL_CMD_WAITEVENT      _IOW('j', 0x42, int)
#define DUALOSCOM_IOCTL_CMD_WAITGROUP      _IOWR('j', 0x43, int)

extern bool filter_dummy(void *buffer, uint32_t size);
extern bool filter_even(void *buffer, uint32_t size);

#endif /* DUALOSCOM_CONFIG */
