
#include "os.h"

int main(void)
{
    OS_Init();

    int x = 10;
    float y = 3.4456;

    char buff[25]; // ['A']['1']

    buff[0] = 'A';
    buff[1] = '7';
    buff[2] = NULL; // NULL Terminator (ASCII) -> 0, '\0', NULL
                    // NULL Terminator means end of the string.
        
    sprintf(buff, "x is %i\r\n", x);
    UART1_AsyncWriteString(buff);

    sprintf(buff, "y is %f", y);
    UART1_AsyncWriteString(buff);

    OS_Start();
}
