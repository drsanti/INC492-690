#include <ecc.h>

void Byte_Received(void* event)
{
    uart_event_t* evt = (uart_event_t*)event;
    unsigned char id = evt->byte - 0x30;
    if(id >= 0 && id <= 3)
    {
        Led_Inv(id);
    }
}

int main(void) 
{
    System_Init();
    Uart1_PrintfAsync("Hello Serial Port\r\n");
    Uart1_SetRxCallback(Byte_Received);
    System_Start();
    return 0;
}
