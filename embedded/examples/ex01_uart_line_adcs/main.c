#include "os.h"

/*
Today topics:
    1) Analog-to-Digital Converter (ADC) 
    2) UART Line Received
    3) String to number conversion 
*/


/*
2) UART Line received
   "Line" means a string that ends with "\r\n".
*/

/*
To make a new line with the Terminal (Proteus),
Hit CTRL+ENT
*/

void UartLineCallback(void *event) {
    // Unpack the received event.
    uart_event_t *uart_event = (uart_event_t *)event;
    data_t uart_data = uart_event->data;

    // Get the line
    const char *line_data = (const char *)uart_data.buffer;

    // Print the received line.
    char buff[64];
    sprintf(buff, "Line: [%s]\r\n", line_data);
    Uart1_AsyncWriteString(buff);


    // Convert the received line (only numbers '0' to '9')
    
    // 
    // string to integer, e.g, "123" -> 123
    //
    /*
    int num = atoi(line_data);
    int res = num * 10;
    sprintf(buff, "num: [%i], res: (%i)\r\n", num, res);
    Uart1_AsyncWriteString(buff);
    */

    // 
    // string to real, e.g, "123.456" -> 123.346
    //
    /*
    float numf = atof(line_data);
    float resf = numf * 10.0;
    sprintf(buff, "num: [%f], res: (%f)\r\n", numf, resf);
    Uart1_AsyncWriteString(buff);
    */

    // 
    // Get ADC value from a channel specified by user input ('0','1','2','3')
    //
    int ch  = atoi(line_data);
    int adc = ADC_Get(ch);
    sprintf(buff, "CH: %i, RAW: %i\r\n", ch, adc);
    Uart1_AsyncWriteString(buff);

}

int main(void)
{
    OS_Init();

    //!! Register UART1 Line Received Callback
    OS_Uart1SetLineReceivedCallback(UartLineCallback);



    OS_Start();
}
