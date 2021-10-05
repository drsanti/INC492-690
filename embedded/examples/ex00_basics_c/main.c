
#include "os.h"




void LED_0_1_Callback(void *p) {
    LED_Inv(LED_ID_0);
    LED_Inv(LED_ID_1);
}


void LED3_Callback (void *p) {
    LED_Inv(LED_ID_3);
}

int main(void)
{
    OS_Init();
    LED_On(0);
    LED_Off(1);
    LED_Off(2);
    LED_Off(3);

    double dbl = 22.0/7.0;
    char buff[64];
    sprintf(buff, "addr: 0x%X, size: %d, value: %f\r\n", (unsigned int)&dbl, sizeof(dbl), dbl);
    UART1_AsyncWriteString(buff);


    OS_TimerCreate(
        "Timer1",                // Name
        500,                     // No. of ticks (ms)
        1,                       // 1: repeat, 0: one time
        LED_0_1_Callback        // callback function (pointer to function)
    );

     OS_TimerCreate(
        "Timer2",                   // Name
        1000,                        // No. of ticks (ms)
        1,                          // 1: repeat, 0: one time
        LED3_Callback               // callback function (pointer to function)
    );

    OS_Start();
}
// Make the LED0 bliks 2Hz