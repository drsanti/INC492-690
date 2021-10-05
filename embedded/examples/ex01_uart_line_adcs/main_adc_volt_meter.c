#include "os.h"

/*
Today topics:
    1) Analog-to-Digital Converter (ADC) 
    2) UART Line Received
    3) String to number conversion 
*/




/*

- Print the value of the ADC0 every 0.5 seconds.
- Print the voltage of ADC0 every 0.5 seconds.
    0) Create the time base.
    1) Get the value of ADC0.
    2) Convert the numeric value to a string.
    3) Print it out to the UART1.
*/


/*
The ADC modul (all channels) has a 10-bit resulution.

Example of 2-bit:
00  0
01  1
10  2
11  3

Example of 8-bit:
0000 0000 0
0000 0001 1
..
1111 1111 255

Example of 10-bit:
00 0000 0000  0
00 0000 0001  1
..
11 1111 1111  1023

*/

void ADC_Reader(void * event) {

    // 1
    int adc0 = ADC0_Get();

    // 2 - Print the RAW value (0-1023)
    char buff[32];
    //sprintf(buff, "ADC0: %i\r\n", adc0);
    //Uart1_AsyncWriteString(buff);


    //
    // RAW to Voltage
    //
    // 2.1 Print the vltage (0-3.3 volts)
    // INPUT VOLTAGE          RAW VALUE
    //     0                     0
    //   3.3                  1023
    //   f(t)               0-1023      

    // 3
    float v = 3.3*((float)adc0/1023.0);
    // sprintf(buff, "ADC0: %i, VIN0: %f volts\r\n", adc0, v);
    // sprintf(buff, "ADC0: %.4i, VIN0: %.3f volts\r\n", adc0, v);
    sprintf(buff, "ADC0: %4i, VIN0: %.3f volts\r\n", adc0, v);
    Uart1_AsyncWriteString(buff);
    
    

        
}     
   



int main(void)
{
    OS_Init();

    // 0
    OS_TimerCreate("T1", 500, 1, ADC_Reader);


    OS_Start();
}
