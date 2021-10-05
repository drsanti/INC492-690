
#include "os.h"

int main(void)
{
    OS_Init();

    LED_PwmSet(LED_ID_0, 500, 0,   100);
    LED_PwmSet(LED_ID_1, 500, 100, 100);
    LED_PwmSet(LED_ID_2, 500, 200, 100);
    LED_PwmSet(LED_ID_3, 500, 300, 100);

    OS_Start();
}
