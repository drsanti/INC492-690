
#include "os.h"

void doIt(void *p) {
    LED2_Inv();
    LED_Inv(LED_ID_3);
}

int main(void)
{
    OS_Init();
    LED2_Inv();

    OS_TimerCreate(
        "Timer1",                // Name
        300,                     // No. of ticks (ms)
        1,                       // 1: repeat, 0: one time
        doIt                     // callback function (pointer to function)
    );

    OS_Start();
}
// Make the LED0 bliks 2Hz