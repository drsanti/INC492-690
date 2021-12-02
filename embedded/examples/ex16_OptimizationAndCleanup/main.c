/************************************************************
 * File:    main.c                                          *
 * Description:                                             *
 *          ModbusRTU Slave (Code Optimization and Cleanup) *
 * Author:  Asst.Prof.Dr.Santi Nuratch                      *
 *          Embedded Computing and Control Laboratory       *
 *          ECC-Lab, INC, KMUTT, Thailand                   *
 ************************************************************
 * Update:  15 November 2021                                *
 ************************************************************

 ************************************************************
 * Functions Codes:
 *   + 0x01 - Read Coils
 *   + 0x02 - Read Discrete Inputs
 *   + 0x03 - Read Holding Registers
 *   + 0x04 - Read Input Registers
 *   + 0x05 - Write Single Coil
 *   + 0x0F - Write Multiple Coils
 *   + 0x10 - Write Multiple Registers
 ************************************************************
 * Error Codes:
 *   + 0x01 - Illegal Function
 *   + 0x02 - Illegal Data Address
 *   + 0x03 - Illegal Data Value
 *   + 0x04 - Slave Device Failure
 *   + 0x05 - Acknowledge
 *   + 0x06 - Slave Device Busy
 *   + 0x07 - Negative Acknowledge
 *   + 0x08 - Memory Parity Error
 *   + 0x0A - Gateway Path Unavailable
 *   + 0x0B - Gateway Target Device Failed to Respond
 ************************************************************
 *  Error Response Frame Format:
 *    [SlaveAddress] [0x80|FunctionCode] [ErrorCode] [CRCLow] [CRCHigh]
 * 
 ************************************************************ 
*/


#include <ecc.h>

#define SLAVE_ADDR 0x01

uint8_t     buffer[256];        // Request Frame Buffer (Sent from Master)
uint16_t    counter = 0;        // Received Byte Counter
uint16_t    ticks = 0;          // Timer Ticks
uint8_t     buffer_r[256];      // Response Frame Buffer (Sent to Master)
uint16_t    configs[4] = {0x1234, 0x3456, 0xABCD, 0x12EF};  // Used as Holding Registers


/**
 * Calculates and Returns 16-bit CRC (Modbus RTU CRC)
 */
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


/**
 * Returns Slave Address
 */
uint16_t GetSlaveAddress(void) {
    return (uint16_t)(buffer[0]);       // Slave Address
}

/**
 * Returns Function Code
 */
uint16_t GetFunctionCode(void) {
    return (uint16_t)(buffer[1]);       // Function Code
}

/**
 * Returns Staring Address of IO/Register
 */
uint16_t GetAddress(void) {
    return ((uint16_t)(buffer[2]<<8)) | ((uint16_t)buffer[3]);   // Starting Address of IOs
}

/**
 * Returns Quantity of IO/Register
 */
uint16_t GetQuantity(void) {
    return ((uint16_t)(buffer[4]<<8)) | ((uint16_t)buffer[5]);  // Quantity of IOs
}

/**
 * Returns Byte Counter
 */
uint16_t GetByteCount(void) {
    return (uint16_t)buffer[6];         // Byte Count
}

/**
 * Make ResponseFrame of FC01-02 and Sends to Master
 */
void MakeResponseFrameFC01_02(uint8_t byteCnt, uint8_t dataHigh, uint8_t dataLow) {
    buffer_r[0] = buffer[0];                // Slave Address
    buffer_r[1] = buffer[1];                // Function Code
    buffer_r[2] = byteCnt;                  // Byte Count
    buffer_r[3] = dataHigh;                 // Data High
    buffer_r[4] = dataLow;                  // Data Low
    uint16_t crc = CalculateModbusCRC(buffer_r, 5);
    buffer_r[5] = (uint8_t)(crc>>8);        // CRC Low
    buffer_r[6] = (uint8_t)(crc&0xFF);      // CRC High
    Uart1_WriteBytes((char *)buffer_r, 7);  // Send response frame to Master   
}

/**
 * Make ResponseFrame of FC03-04 and Sends to Master
 */
void MakeResponseFrameFC03_04(uint16_t quantity, uint16_t *pDataBuffer) {
    buffer_r[0] = buffer[0];            // Slave Address
    buffer_r[1] = buffer[1];            // Function Code
    buffer_r[2] = quantity*2;           // Byte Count
    
    uint8_t i;
    uint8_t j = 3;
    for(i=0; i<quantity; i++) {
        buffer_r[j+0] =  (uint8_t)(pDataBuffer[i]>>8);    // Data High 
        buffer_r[j+1] =  (uint8_t)(pDataBuffer[i]&0xFF);  // Data Low 
        j += 2; 
    }
    uint16_t crc = CalculateModbusCRC(buffer_r, 3 + (quantity*2));
    buffer_r[j+0] = (uint8_t)(crc>>8);          // CRC Low
    buffer_r[j+1] = (uint8_t)(crc&0xFF);        // CRC High
    Uart1_WriteBytes((char *)buffer_r, j+2);    // Send response frame to Master
}

/**
 * Make ResponseFrame of FC06-06, 0F-10 and Sends to Master
 */
void MakeResponseFrameFC05_06_0F_10(void) {
    buffer_r[0] = buffer[0];                // Slave Address
    buffer_r[1] = buffer[1];                // Function Code
    buffer_r[2] = buffer[2];                // Address High
    buffer_r[3] = buffer[3];                // Address Low
    buffer_r[4] = buffer[4];                // Quantity High
    buffer_r[5] = buffer[5];                // Quantity Low
    uint16_t crc = CalculateModbusCRC(buffer_r, 6);
    buffer_r[6] = (uint8_t)(crc>>8);        // CRC Low
    buffer_r[7] = (uint8_t)(crc&0xFF);      // CRC High
    Uart1_WriteBytes((char *)buffer_r, 8);   // Send response frame to Master
}

/**
 * Make ErrorResponseFrame and Sends to Master
 */
void MakeErrorResponseFrame(uint8_t errCode) {
    buffer_r[0] = buffer[0];                // Slave Address
    buffer_r[1] = 0x80|buffer[1];           // 1000 0000 (0x80) -- Function Code
    buffer_r[2] = errCode;                  // Error Code 
    uint16_t crc = CalculateModbusCRC(buffer_r, 3);
    buffer_r[3] = (uint8_t)(crc>>8);        // CRC Low
    buffer_r[4] = (uint8_t)(crc&0xFF);      // CRC High
    Uart1_WriteBytes((char *)buffer_r, 5);  // Send response frame to Master
}

/**
 * FC 0x01: Read Coils (LED<3:0>)
 */
void FunctionCode01() {
    uint16_t coilAddr = GetAddress();   // Starting Address of Coils
    if((coilAddr < 1) || (coilAddr > 4)) {
        // Error => 0x02: Illegal Data Address
        MakeErrorResponseFrame(0x02);
        return;
    }
    uint16_t quantity = GetQuantity();          // Quantity of Coils
    uint8_t coils = 0x00;
    int8_t i;
    for(i=0; i<quantity; i++) {
        uint8_t id = (coilAddr-1) + i;
        uint8_t cs = Led_Get(id);
        coils |= cs<<i;
    }
    MakeResponseFrameFC01_02(1, 0x00, coils);   // Make and Send response frame to Master
}

/**
 * FC 0x02: Read Discrete Inputs (PSW<3:0>)
 */
void FunctionCode02() {
    uint16_t inputAddr = GetAddress();      // Starting Address of Discrete Inputs
    uint16_t quantity  = GetQuantity();     // Quantity of Discrete Inputs
    uint8_t inputs = 0x00;
    int8_t i;
    for(i=0; i<quantity; i++) {
        uint8_t id = (inputAddr-1) + i;
        uint8_t di = Psw_Get(id);
        inputs |= di<<i;
    }
    MakeResponseFrameFC01_02(1, 0x00, inputs);  // Make and Send response frame to Master
}

/**
 * FC 0x03: Read Holding Registers (configs<3:0>)
 */
void FunctionCode03() {

    uint16_t holdingAddr = GetAddress();    // Starting Address of Holding Registers
    uint16_t quantity = GetQuantity();      // Quantity of Holding Registers
    uint16_t holdingBuffer[4];
    uint8_t i;
    for(i=0; i<quantity; i++) {
        holdingBuffer[i] = configs[i + holdingAddr-1];
    } 
    MakeResponseFrameFC03_04(quantity, holdingBuffer);
}

/**
 * FC 0x04: Read Input Registers (ADC<3:0>)
 */
void FunctionCode04() {
    uint16_t inregAddr = GetAddress();  // Starting Address of Input Registers
    uint16_t quantity = GetQuantity();  // Quantity of Input Registers
    uint16_t inregBuffer[4];
    uint8_t i;
    for(i=0; i<quantity; i++) {
        inregBuffer[i] = Adc_Get(i + inregAddr-1);
        Uart2_Printf("Adc[%d]: %d 0x%.4X\r\n", i + inregAddr-1, inregBuffer[i], inregBuffer[i]);
    }
    MakeResponseFrameFC03_04(quantity, inregBuffer);
}

/**
 * FC 0x05: Write Single Coil (LED<3:0>)
 */
void FunctionCode05() {
    uint16_t coilAddr = (uint16_t)(buffer[2]<<8);   // Starting Address of Coil
    coilAddr |= (uint16_t)buffer[3];

    // 0x0000 -- OFF
    // 0xFF00 -- ON
    uint16_t coilData = 0x0000;
    coilData |= (uint16_t)buffer[4]<<8;     // Data High
    coilData |= (uint16_t)buffer[5];        // Data Low
    Uart2_Printf("Coil Data: 0x%.4X\r\n", coilData);

    if(coilData == 0x0000) {
        Led_Clr(coilAddr - 1);              // Turn OFF
    }
    else if(coilData == 0xFF00) {
        Led_Set(coilAddr - 1);              // Turn ON
    }
    else {
        // Error
        Uart2_Printf("Coil Data Error!!\r\n");
        return;
    }
    MakeResponseFrameFC05_06_0F_10();
}

/**
 * FC 0x06: Write Single Register (configs<3:0>)
 */
void FunctionCode06() {

    uint16_t holdingAddr = GetAddress();        // Starting Address of Holding Register
    uint16_t holdingData = (uint16_t)(buffer[4]<<8);
    
    holdingData |= (uint16_t)buffer[5];         // Data of Holding Register
    if(holdingAddr != 1) {
        Uart2_Printf("Error!!\r\n");
        return;
    }

    configs[holdingAddr - 1] = holdingData;
    // configs[0]: f.t
    double freq = 10.0e3*((double)(configs[0]>>8))/255.0;
    uint16_t duration = (uint16_t)(5000.0*((double)(configs[0]&0xFF)/255.0));
    Uart2_Printf("f: %.3f Hz, t: %d mS\r\n", freq, duration);

    if(freq < 100 || freq > 1e3) {              // 100Hz - 1kHz
        // Error => 0x03: Illegal Data Value
        MakeErrorResponseFrame(0x03);
        return;
    }
    Beep_Play(duration, freq);              // Beep sound
    MakeResponseFrameFC05_06_0F_10();
}

/**
 * FC 0x0F: Write Multiple Coils (LED<3:0>)
 */
void FunctionCode0F() {
    uint16_t coilAddr = GetAddress();       // Staring Address of Coils
    uint16_t quantity = GetQuantity();      // Quantity of Coils
    uint16_t byteCount = GetByteCount();    // Byte Count
    uint8_t dataBuffer[32];
    uint8_t i;
    for(i=0; i<byteCount; i++) {
        dataBuffer[i] = buffer[7 + i];
    }

    uint8_t ledData = dataBuffer[1];
    for(i=0; i<quantity; i++) {                    
        if((ledData & 0x01) != 0) {
            Led_Set(coilAddr-1 + i);    // Turn ON
        }
        else {
            Led_Clr(coilAddr-1 + i);    // Turn OFF
        }
        ledData >>= 1;
    }
    MakeResponseFrameFC05_06_0F_10();
}


/**
 * FC 0x10: Write Multiple Registers (configs<3:0>)
 */
void FunctionCode10() {
    uint16_t holdingAddr = GetAddress();        // Staring Address of Registers
    uint16_t quantity = GetQuantity();          // Quantity of Registers
    uint8_t i;
    uint8_t j = 0;
    for(i=0; i<quantity; i++) {
        uint16_t d = 0x0000;
        d |= (uint16_t)buffer[7 + j + 0]<<8;
        d |= (uint16_t)buffer[7 + j + 1];
        configs[holdingAddr - 1 + i] = d;
        j+=2;
    }
    for(i=0; i<quantity; i++) {
        double duty = (double)configs[holdingAddr - 1 + i]/65535.0;
        Uart2_Printf("duty: %.2f\r\n", duty);
        Pwm_SetDutyRatio(holdingAddr - 1 + i, duty);
    }
    MakeResponseFrameFC05_06_0F_10();
}

/*
 * Processes Request Frame and Executes the Requested Function
 */
void Interpreter(void) {
    uint8_t slaveAddr = GetSlaveAddress();
    if(slaveAddr != SLAVE_ADDR) {
        Uart2_Printf("SlaveAddr: 0x%.2X, Received: 0x%.2X\r\n", SLAVE_ADDR, slaveAddr);
        return;
    }

    if( counter == 8 ) {                                // FC 0x01-0x06
        uint16_t crcM = (buffer[6]<<8) | buffer[7];
        uint16_t crcC = CalculateModbusCRC(buffer, 6);

        if(crcM != crcC) {
            // Error => 0x07: Negative Acknowledge
            MakeErrorResponseFrame(0x07);
            return;
        }
        else if( buffer[1] >= 1 &&  buffer[1] <= 4 ) {  // FC 0x01-0x04
            if(buffer[1] == 1) {
                FunctionCode01();       // Function Code 0x01
            }
            else if(buffer[1] == 2) {
                FunctionCode02();       // Function Code 0x02
            } 
            else if(buffer[1] == 3) {
                FunctionCode03();       // Function Code 0x03
            }  
            else if(buffer[1] == 4) {
                FunctionCode04();       // Function Code 0x04
            }     
        }
        else if( buffer[1] >= 5 &&  buffer[1] <= 6 ) {  // FC 0x05-0x06
            if(buffer[1] == 5) {
                FunctionCode05();       // Function Code 0x05
            } 
            else if(buffer[1] == 6) {
                FunctionCode06();       // Function Code 0x06
            }    
        }
        else {
            // Error => 0x01: Illegal Function
           MakeErrorResponseFrame(0x01);
           return;  
        }
    }
    else if( counter >= 11 ) {                          // FC 0x0F-0x10
        uint8_t bc = buffer[6];
        uint16_t crcM = (buffer[7+bc]<<8) | buffer[8+bc];
        uint16_t crcC = CalculateModbusCRC(buffer, 7+bc);

        if(crcM != crcC) {
            // Error => 0x07: Negative Acknowledge
            MakeErrorResponseFrame(0x07);
            return;     
        }
        else if( buffer[1] >= 15 &&  buffer[1] <= 16 ){      // FC 0x0F-0x10
            if(buffer[1] == 0x0F) {
                FunctionCode0F();       // Function Code 15 (0x0F)
            } 
            else if(buffer[1] == 0x10) {
                FunctionCode10();       // Function Code 16 (0x10)
            }   
        }
        else {
            // Error => 0x01: Illegal Function
            MakeErrorResponseFrame(0x01);
            return;
        }
    }
    else {
        // Error => 0x07: Negative Acknowledge
        MakeErrorResponseFrame(0x07); 
    }
}

/*
 * Received UART Data and Puts to Buffer
 */
void Byte_Received(void* event) {
    uart_event_t* evt = (uart_event_t*)event;
    buffer[counter++] = evt->byte;
    ticks = 0;
}

/*
 * Waits Request Frame and then Executes the Interpreter
 */
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

/**
 * Main Function
 */
int main(void) {
    System_Init();
    Uart2_PrintfAsync("ModbusRTU Slave Ready!\r\n");
    Uart1_SetRxCallback(Byte_Received);
    Timer_Create(2, Timer_Callback);
    Pwm_Init(100.0, 0.5);
    System_Start();
    return 0;
}

/* EOF */
