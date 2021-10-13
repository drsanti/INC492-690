#include <ecc.h>

void Rx_Callback( void *event ) 
{
    char id = ((uart_event_t *)event)->byte - 0x30;
    if(id >= 0 || id <=3 )
        Led_Flash( id, 200 );
}

int main(void) 
{
    System_Init();
    Uart1_SetRxCallback( Rx_Callback );
    System_Start();

    return 0;
}
