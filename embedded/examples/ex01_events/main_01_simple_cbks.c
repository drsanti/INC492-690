#include "os.h"


// When SW0 is pressed => LED0 ON
// When SW1 is pressed => LED0 OFF

void sw0_callback(void *event) 
{
    LED_On(LED_ID_0);
}
void sw1_callback(void *event) 
{
    LED_Off(LED_ID_0);
}

int main(void)
{
    OS_Init();

    /*
    // Add an event listenner
    // Subscribe to the event
    // Register event
    // Add a callback function
    */
    OS_SwitchSetCallback(PSW_ID_0, sw0_callback);
    OS_SwitchSetCallback(PSW_ID_1, sw1_callback);

    
    OS_Start();
}
