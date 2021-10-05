// Variables

// signed 8-bit integer
char v1;
char v2 = 'A';

// signed 16-bit integer
short v3;
short v4 = 123;

// signed <16-bit, 32-bit, 64-bit> integer
int v5;
int v6 = 5+6;

// signed <32-bit, 64-bit> integer
long v7;
long v8 = 122;

// real number <16-bit, 32-bit, 64-bit>
float v9 = +1.24335;
float v10 = 22.0f/7.0f;

// real number <16-bit, 32-bit, 64-bit, 128-bit>
double v11;
double v12 = 1.243545646;
double v13 = 22.0/7.0;

//-------------------------------

// signed variables
signed char   vs14; // => char   vs14;
signed short  vs15;
signed int    vs16;
signed long   vs17;

// unsigned variables
unsigned char   vu14;
unsigned short  vu15;
unsigned int    vu16;
unsigned long   vu17;

//--------------
// ** signed vs unsigned
char          x = -10; // +/-
unsigned char y = +123;

// Variable size (No. of byte)
//  - bit      1-bit
//  - Nibble   4-bit
//  - Byte     8-bit

// char 8-bit
// 0000 0000 = 0
// 1111 1111 = 255

//  1111 1111
//             +
//  0000 0001
// 10000 0000
unsigned char v18 = 257;// v18=0;



// main function is an entry point.
int main(void) {

    return 1;
}
