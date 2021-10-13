#include <ecc.h>

void Rx_Callback( void *event ) 
{
    Led_Flash( LED_ID_0, 200 );
}

int main(void) 
{

    System_Init();

    Uart1_SetRxCallback( Rx_Callback );

    System_Start();

    return 0;
}
