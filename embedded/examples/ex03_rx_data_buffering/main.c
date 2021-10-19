#include <ecc.h>

uint8_t data_buffer[128];
uint16_t data_counter = 0;
uint8_t data_ready = 0;

void Byte_Received(void* event)
{
    uart_event_t* evt = (uart_event_t*)event;
    data_buffer[data_counter++] = evt->byte;
    if(evt->byte == ';' || evt->byte == '\n')  {
        Uart1_SetRxCallback(NULL);
        data_buffer[data_counter] = 0;
        data_counter = 0;
        data_ready = 1;
    }
}

void Timer_Callback(void* event) {
    if(data_ready == 1)
    {
        data_ready = 0;   
        Uart1_PrintfAsync("Received data: %s\r\n", data_buffer);  
        Uart1_SetRxCallback(Byte_Received);
    }
}

int main(void) 
{
    System_Init();
    Uart1_PrintfAsync("EX03 Ready\r\n");
    Uart1_SetRxCallback(Byte_Received);
    Timer_Create(5, Timer_Callback);
    System_Start();
    return 0;
}
