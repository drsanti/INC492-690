#include "os.h"


// When a numeric key is press, print it out.

void UartByte_callback(void *event)  {
    // Unpack the received event, the Uart1 event
    uart_event_t  *evt = (uart_event_t *)event;
    
    // Take the event data
    data_t event_data = evt->data;

    // Take only the first byte in the buffer
    char byte_data = event_data.buffer[0];

    // Check if it's a numeric key
    if(byte_data >= '0' && byte_data <= '9') {
        
        char str[32];
        sprintf(str, "You press: %c\r\n", byte_data);
        UART1_AsyncWriteString(str);
    }

}

int main(void)
{
    OS_Init();

    OS_Uart1SetRxCallback(UartByte_callback);

    
    OS_Start();
}
