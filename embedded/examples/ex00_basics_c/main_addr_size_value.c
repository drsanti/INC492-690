
#include "os.h"

int main(void)
{
    OS_Init();
    
    
    char c1 = 'A';

    float f1 = 12.3446;


    // 1. address
    // 2. size (in bytes(s))
    // 3. value
    char buff[584651436546];

    // prints the addr, size and value in the same line.
    sprintf(buff, "addr: 0x%X, size: %d, value: %c\r\n", &c1, sizeof(c1), c1);
    UART1_AsyncWriteString(buff);

    // prints the addr (in RAM) of c1
    sprintf(buff, "addr: 0x%X\r\n", &c1);
    UART1_AsyncWriteString(buff);

    // prints the size of c1 (in byte)
    sprintf(buff, "size: %d\r\n", sizeof(c1));
    UART1_AsyncWriteString(buff);

    // prints the value (character) of c1
    sprintf(buff, "size: %c\r\n", c1);
    UART1_AsyncWriteString(buff);


    // prints the addr, size and value of the f1 in the same line.
    sprintf(buff, "addr: 0x%X, size: %d, value: %f\r\n", &f1, sizeof(f1), f1);
    UART1_AsyncWriteString(buff);


    OS_Start();
}
