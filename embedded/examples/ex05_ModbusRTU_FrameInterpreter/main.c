#include <ecc.h>

uint8_t     buffer[256];    // RTU Data
uint16_t    counter = 0;    // Byte Counter
uint16_t    ticks = 0;      // Timer Ticks

void Interpreter(void) {

    uint16_t idx = 0;
    Uart2_Printf("\r\n");
    Uart2_Printf("Slave Address: %.2X\r\n", buffer[idx++]);
    Uart2_Printf("Function Code: %.2X\r\n", buffer[idx++]);
    Uart2_Printf("Address High:  %.2X\r\n", buffer[idx++]);
    Uart2_Printf("Address Low:   %.2X\r\n", buffer[idx++]);

    if( counter == 8 ) {    // FC 0x01-0x06
       
        if( buffer[1] >= 1 &&  buffer[1] <= 4 ) {       // FC 0x01-0x04
            
            Uart2_Printf("Quantity High: %.2X\r\n", buffer[idx++]);
            Uart2_Printf("Quantity Low:  %.2X\r\n", buffer[idx++]);
        }
        else if( buffer[1] >= 5 &&  buffer[1] <= 6 ) {  // FC 0x05-0x06
            
            Uart2_Printf("Data High:     %.2X\r\n", buffer[idx++]);
            Uart2_Printf("Data Low:      %.2X\r\n", buffer[idx++]);
        }
        else {
            // Error
        }
    }
    else if( counter >= 11 ) {                          // FC 0x0F-0x10
       
        if( buffer[1] >= 15 &&  buffer[1] <= 16 ){      // FC 0x0F-0x10
            
            Uart2_Printf("Quantity High: %.2X\r\n", buffer[idx++]);
            Uart2_Printf("Quantity Low:  %.2X\r\n", buffer[idx++]);
            Uart2_Printf("Byte Count:    %.2X\r\n", buffer[idx++]);
        }
        else {
            // Error    
        }
    }
    else {
        // Error    
    }

}


void Byte_Received(void* event) {
    uart_event_t* evt = (uart_event_t*)event;
    buffer[counter++] = evt->byte;
    ticks = 0;
}

void Timer_Callback(void* event) {
    if( counter >= 8 ) {
        if( ++ticks >= 5 ) {
            Uart1_SetRxCallback(NULL);              // UART1 RxD Unregister
            Interpreter();
            ticks = 0;
            counter = 0;
            Uart1_SetRxCallback(Byte_Received);     // UART1 RxD Register
        }
    }
}

int main(void) {
    System_Init();
    Uart2_PrintfAsync("Modbus Interpreter Ready\r\n");  // UART2
    Uart1_SetRxCallback(Byte_Received);
    Timer_Create(2, Timer_Callback);
    System_Start();
    return 0;
}
