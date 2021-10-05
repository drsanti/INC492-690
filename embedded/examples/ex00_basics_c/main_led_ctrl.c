
#include "os.h"

int main(void)
{
    OS_Init();
    
    // LED control functions
    LED0_On();              // turn on LED0
    LED_On(LED_ID_3);       // turn on LED3
    LED0_Off();             // turn off LED0
    LED_Inv(LED_ID_3);      // Invert or Toggle LED3

    // What about the LED?

    OS_Start();
}
