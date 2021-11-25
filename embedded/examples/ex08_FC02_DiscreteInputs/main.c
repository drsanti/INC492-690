#include <ecc.h>

uint8_t     buffer[256];        // RTU Data
uint16_t    counter = 0;        // Byte Counter
uint16_t    ticks = 0;          // Timer Ticks

uint8_t     buffer_r[256];      // Response buffer

uint16_t CalculateModbusCRC(uint8_t *pData, uint16_t length)
{
    static const uint16_t crcTable[] = {
        0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
        0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
        0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
        0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
        0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
        0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
        0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
        0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
        0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
        0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
        0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
        0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
        0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
        0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
        0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
        0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
        0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
        0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
        0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
        0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
        0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
        0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
        0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
        0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
        0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
        0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
        0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
        0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
        0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
        0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
        0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
        0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040 
    };

    uint16_t tmp;
    uint16_t crc = 0xFFFF;
    while(length--) {
        tmp = *pData++ ^ crc;  
        crc >>= 8; 
        crc ^= crcTable[tmp & 0xFF];
    }
    return (crc>>8) | (crc<<8);
}


void FunctionCode01() {

    uint16_t coilAddr = (uint16_t)(buffer[2]<<8);
    coilAddr |= (uint16_t)buffer[3];

    uint16_t quantity = (uint16_t)(buffer[4]<<8);
    quantity |= (uint16_t)buffer[5];

    uint8_t coils = 0x00;
    int8_t i;
    for(i=0; i<quantity; i++) {
        uint8_t id = (coilAddr-1) + i;
        uint8_t cs = Led_Get(id);
        coils |= cs<<i;
    }

    buffer_r[0] = buffer[0];            // Slave Address
    buffer_r[1] = buffer[1];            // Function Code
    buffer_r[2] = 1;                    // Byte Count
    buffer_r[3] = 0;                    // Data High
    buffer_r[4] = coils;                // Data Low
    uint16_t crc = CalculateModbusCRC(buffer_r, 5);
    buffer_r[5] = (uint8_t)(crc>>8);    // CRC Low
    buffer_r[6] = (uint8_t)(crc&0xFF);  // CRC High
    Uart1_WriteBytes((char *)buffer_r, 7);
}



void FunctionCode02() {

    uint16_t inputAddr = (uint16_t)(buffer[2]<<8);
    inputAddr |= (uint16_t)buffer[3];       // Starting Address

    uint16_t quantity = (uint16_t)(buffer[4]<<8);
    quantity |= (uint16_t)buffer[5];        // Quantity of Discrete Inputs

    uint8_t inputs = 0x00;
    int8_t i;
    for(i=0; i<quantity; i++) {
        uint8_t id = (inputAddr-1) + i;
        uint8_t di = Psw_Get(id);
        inputs |= di<<i;
    }

    buffer_r[0] = buffer[0];            // Slave Address
    buffer_r[1] = buffer[1];            // Function Code
    buffer_r[2] = 1;                    // Byte Count
    buffer_r[3] = 0;                    // Data High
    buffer_r[4] = inputs;               // Data Low
    uint16_t crc = CalculateModbusCRC(buffer_r, 5);
    buffer_r[5] = (uint8_t)(crc>>8);    // CRC Low
    buffer_r[6] = (uint8_t)(crc&0xFF);  // CRC High
    Uart1_WriteBytes((char *)buffer_r, 7);
}

void Interpreter(void) {

    if( counter == 8 ) {                                // FC 0x01-0x06
       
        uint16_t crcM = (buffer[6]<<8) | buffer[7];
        uint16_t crcC = CalculateModbusCRC(buffer, 6);

        if(crcM != crcC) {
            // Error 
        }
        else if( buffer[1] >= 1 &&  buffer[1] <= 4 ) {       // FC 0x01-0x04
            if(buffer[1] == 1) {
                FunctionCode01();       // Function Code 0x01
            }
            else if(buffer[1] == 2) {
                FunctionCode02();       // Function Code 0x02
            }       
        }
        else if( buffer[1] >= 5 &&  buffer[1] <= 6 ) {  // FC 0x05-0x06
            
            
        }
        else {
            // Error
        }
    }
    else if( counter >= 11 ) {                          // FC 0x0F-0x10
       
        uint8_t bc = buffer[6];
        uint16_t crcM = (buffer[7+bc]<<8) | buffer[8+bc];
        uint16_t crcC = CalculateModbusCRC(buffer, 7+bc);

        if(crcM != crcC) {
            // Error      
        }
        else if( buffer[1] >= 15 &&  buffer[1] <= 16 ){      // FC 0x0F-0x10
            
           
        }
        else {
            // Error    
        }
    }
    else {
        // Error    
    }

}

void Byte_Received(void* event) {
    uart_event_t* evt = (uart_event_t*)event;
    buffer[counter++] = evt->byte;
    ticks = 0;
}

void Timer_Callback(void* event) {
    if( counter >= 8 ) {
        if( ++ticks >= 5 ) {
            Uart1_SetRxCallback(NULL);              // UART1 RxD Unregister
            Interpreter();
            ticks = 0;
            counter = 0;
            Uart1_SetRxCallback(Byte_Received);     // UART1 RxD Register
        }
    }
}

int main(void) {
    System_Init();
    Uart2_PrintfAsync("FC02 Read Discrete Inputs\r\n");  // UART2
    Uart1_SetRxCallback(Byte_Received);
    Timer_Create(2, Timer_Callback);
    System_Start();
    return 0;
}
