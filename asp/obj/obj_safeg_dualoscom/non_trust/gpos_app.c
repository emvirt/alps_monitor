#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "dualoscom.h"

bool filter_dummy(void *buffer, uint32_t size)
{
    return true;
}

bool filter_even(void *buffer, uint32_t size)
{
    uint32_t value;

    value = *((uint32_t *)buffer);

    if ((value % 2) == 0)  return true;
    return false;
}

int main()
{
    int ret;
    void *buffer;
    uint32_t size;
    dualoscom_block_id_t block_id;
    char msg[] = "hey!";

    printf("GPOS dualoscom_init\n");
    ret = dualoscom_init(true, 0);
    assert(ret == 0);

    printf("GPOS wait for message\n");
    ret = dualoscom_channel_event_wait(DUALOSCOM_CHANNEL_0, 0);
    assert(ret == 0);

    ret = dualoscom_block_dequeue(DUALOSCOM_CHANNEL_0, &block_id);
    assert(ret == 0);

    ret = dualoscom_block_getbuffer(block_id, &buffer, &size);
    assert(ret == 0);

    printf("MSG: %s\n", (char *)buffer);

    printf("GPOS send ack\n");
    ret = dualoscom_channel_event_send(DUALOSCOM_CHANNEL_0);
    assert(ret == 0);

    printf("GPOS wait for rpc\n");
    ret = dualoscom_channel_event_wait(DUALOSCOM_CHANNEL_0, 0);
    assert(ret == 0);

    printf("received params: %u and %u\n",
           *((uint32_t *)buffer), *((uint32_t *)(buffer + 4)));

    *((uint32_t *)buffer) = *((uint32_t *)buffer) * *((uint32_t *)(buffer + 4));
    printf("GPOS send rpc result (%u)\n", *((uint32_t *)buffer));

    ret = dualoscom_channel_event_send(DUALOSCOM_CHANNEL_0);
    assert(ret == 0);

    printf("GPOS wait for rpc\n");
    ret = dualoscom_channel_event_wait(DUALOSCOM_CHANNEL_0, 0);
    assert(ret == 0);

    printf("received params: %u and %u\n",
           *((uint32_t *)buffer), *((uint32_t *)(buffer + 4)));

    *((uint32_t *)buffer) = *((uint32_t *)buffer) * *((uint32_t *)(buffer + 4));
    printf("GPOS send rpc result (%u)\n", *((uint32_t *)buffer));

    ret = dualoscom_channel_event_send(DUALOSCOM_CHANNEL_0);
    assert(ret == 0);

    printf("GPOS dualoscom_block_free\n");
    ret = dualoscom_block_free(block_id);
    assert(ret == 0);

    printf("Send a message to the RTOS by channel 1\n");
    ret = dualoscom_block_alloc(DUALOSCOM_CHANNEL_1, &block_id);
    assert(ret == 0);

    ret = dualoscom_block_getbuffer(block_id, &buffer, &size);
    assert(ret == 0);

    assert(sizeof(msg) < size);
    memcpy(buffer, (void *)msg, sizeof(msg));

    ret = dualoscom_block_enqueue(block_id);
    assert(ret == 0);

    ret = dualoscom_channel_event_send(DUALOSCOM_CHANNEL_1);
    assert(ret == 0);

    printf("Send several messages with a filter on\n");

    ret = dualoscom_block_alloc(DUALOSCOM_CHANNEL_0, &block_id);
    assert(ret == 0);

    ret = dualoscom_block_getbuffer(block_id, &buffer, &size);
    assert(ret == 0);

    *((uint32_t *)buffer) = 1;

    ret = dualoscom_block_enqueue(block_id);
    if(ret != 0) printf("data %u filtered\n",*((uint32_t *)buffer));

    ret = dualoscom_block_alloc(DUALOSCOM_CHANNEL_0, &block_id);
    assert(ret == 0);

    ret = dualoscom_block_getbuffer(block_id, &buffer, &size);
    assert(ret == 0);

    *((uint32_t *)buffer) = 2;

    ret = dualoscom_block_enqueue(block_id);
    if(ret != 0) printf("data %u filtered\n",*((uint32_t *)buffer));

    ret = dualoscom_block_alloc(DUALOSCOM_CHANNEL_0, &block_id);
    assert(ret == 0);

    ret = dualoscom_block_getbuffer(block_id, &buffer, &size);
    assert(ret == 0);

    *((uint32_t *)buffer) = 3;

    ret = dualoscom_block_enqueue(block_id);
    if(ret != 0) printf("data %u filtered\n",*((uint32_t *)buffer));

    ret = dualoscom_channel_event_send(DUALOSCOM_CHANNEL_0);
    assert(ret == 0);

    return 0;
}
