#include <ecc.h>

uint8_t buffer[256];
uint16_t counter = 0;
uint16_t ticks = 0;

void Byte_Received(void* event)
{
    uart_event_t* evt = (uart_event_t*)event;
    buffer[counter++] = evt->byte;
    ticks = 0;
}

void Timer_Callback(void* event) {
    if(counter > 0)
    {
        ticks++;
        if(ticks >= 5)
        {
            Uart1_SetRxCallback(NULL);

            int16_t i;
            Uart2_Printf("Received data: ");
            for(i=0; i<counter; i++) 
            {
                Uart2_Printf("%.2X ", buffer[i]); 
                Uart1_Put(buffer[i]);  
            }
            Uart2_Printf("\r\n");

            Uart1_SetRxCallback(Byte_Received);

            ticks = 0;
            counter = 0;
        }
    }
}

int main(void) 
{
    System_Init();

    Uart2_PrintfAsync("Ex04 Modbus Slave Ready\r\n");

    Uart1_SetRxCallback(Byte_Received);
    Timer_Create(2, Timer_Callback);
    System_Start();
    return 0;
}
