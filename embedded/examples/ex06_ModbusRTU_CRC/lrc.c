#include "ecc.h"
uint8_t ModbusLRC (uint8_t *pData, uint16_t length)
{
    uint8_t lrc = 0 ;
    int16_t i;
    for (i = 0; i < length; i++)
    {
        lrc += *pData++;
    }
    return (uint8_t)(-lrc);
} 



