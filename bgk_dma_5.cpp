#include "InterruptIn.h"
#include "PinNames.h"
#include "mbed.h"

DigitalOut oled1(PA_8);
DigitalOut oled2(LED1);
typedef struct {
int vnyt; 
int vnesh; 
} message_t;

MemoryPool<message_t, 16> mpool;
Queue<message_t, 16> queue;
Thread thread;

void send_thread(void)
{
uint32_t i1 = 0;
uint32_t i2 = 0;
while (true) {
i1++; 
i2++;
message_t *message = mpool.alloc();
message->vnyt = i1%2;
message->vnesh = i2%2;
queue.put(message);
ThisThread::sleep_for(1000);
}
}



// main() runs in its own thread in the OS
int main()
{
    thread.start(send_thread);
    while (true) {
    osEvent evt = queue.get();
    if (evt.status == osEventMessage) {
    message_t *message = (message_t *)evt.value.p;
    if((message->vnesh)==0)
    {
        oled1=0;
    }
    else 
    {
        oled1=1;
    }
    if((message->vnyt)==0)
    {
        oled2=0;
    }
    else 
    {
        oled2=1;
    }
    mpool.free(message);
    }
    }
}
