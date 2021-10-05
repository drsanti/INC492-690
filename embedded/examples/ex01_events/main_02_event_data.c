#include "os.h"


// When SW<id> is pressed => Toggle LED<id>

void swx_callback(void *event) 
{
    // Unpack the received event.
    event_t  *evt = (event_t *)event;
    switch_t *psw = (switch_t *)evt->sender;

    // Take the id of the pressed switch.
    int id = psw->id; // 0, 1, 2, 3

    // Toggle the LED<id>   // id: 0, 1, 2, 3
    LED_Inv(id);
}

int main(void)
{
    OS_Init();

    //-----[Code 1]--------------------------
    //OS_SwitchSetCallback(0, swx_callback);
    //OS_SwitchSetCallback(1, swx_callback);
    //OS_SwitchSetCallback(2, swx_callback);
    //OS_SwitchSetCallback(3, swx_callback);

    //
    // [Code 2] is the same as [Code 1]
    //
    
    //-----[Code 2]--------------------------
    int i;
    for(i=PSW_ID_0; i<=PSW_ID_3; i++) {
        //In this case, i = 0, 1, 2, 3
        OS_SwitchSetCallback(i, swx_callback); 
    }
    
    OS_Start();
}
