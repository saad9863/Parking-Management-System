//
// * File:   Q4.c
// * Author: Saad
// *
// * Created on July 9, 2020, 3:19 PM
// */
// PIC18F4550 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator (HS))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h>
#define X_1  PORTBbits.RB3  // variables defined for the key pad  X is column and Y is row 
#define X_2  PORTAbits.RA1
#define X_3  PORTAbits.RA0
#define Y_1  PORTBbits.RB4
#define Y_2  PORTBbits.RB5
#define Y_3  PORTBbits.RB6
#define Y_4  PORTBbits.RB7
#define RS   PORTEbits.RE0  // connecting LCD with these PORTS 
#define RW   PORTEbits.RE1
#define en   PORTEbits.RE2
#define ldt  PORTD
unsigned char msg1[] = " OCCUPIED";
unsigned char msg2[] = " VACANT";
unsigned char eemsg[] = " PARKING NUMBER";
unsigned char errmsg1[] = " PRE-OCCUPIED ";
unsigned char errmsg2[] = " NICE ";
unsigned char A,B,C,D,E,F;                   // variables defined for checking the occupied and vacant number i.e. either less than 10 or between 10 and 100 or equal to 10
int a,b,c,key;                // this a,b,c will be shown as numbers before occupied and vacant and key is the numbers on mobile keypad i.e. 1 to 9
int address;		      // address that will be added using the keypad
int o = 0;			// this is occupied variable that will be used as the number that will be used in A,B,C,D,E,F conditions that is number of cars in the parking lot i.e. this is the number that is incremented
int v = 0;
int keycounter = 0;		// this is the number of times we have pressed the mobile keypad for the address i.e. if 2 times button is pressed key counter =2
int keypadfull = 0;

void delay(unsigned int j)
{	
    unsigned int i=0;
	for(i=0;i<j;i++);
}

void lcdcmd(unsigned char cmd)
{
    ldt = cmd;
    RS = 0;
    en = 1;
    delay(3);
    en = 0;
    
}
void lcddata(unsigned char data)
{
    ldt = data;
    RS = 1;
    en = 1;
    delay(3);
    en = 0;
}
void addresscal()
{
    address = (a*100) + (b*10) +(c*1) ;
}

void userentry()
{
    X_1 = 1; X_2 = 0; X_3 = 0; 	

	if (Y_1 == 1)
    { 
        delay(100);
        while (Y_1==1);
        key = 1; 
        lcddata('1');
        delay(10);
        keycounter = keycounter + 1;
        if(keycounter == 1)
	{
	a = 1;
	} 
        else if(keycounter == 2)
        {
            b = 1;
        } 
        else if(keycounter == 3)
        {
            c = 1;
        }
    }
	
    if (Y_2 == 1) 
    { 
        delay(100);
        while (Y_2==1);
        key = 4; 
        lcddata('4');
        delay(10);
        keycounter = keycounter + 1; 
        if(keycounter == 1)
        {
            a = 4;
        } 
        else if(keycounter == 2)
        {
            b = 4;
        } 
        else if(keycounter == 3)
        {
            c = 4;
        }
    }
	
    if (Y_3 == 1) 
    { 
        delay(100); 
        while (Y_3==1); 
        key = 7; 
        lcddata('7');
        delay(10);
        keycounter = keycounter + 1;
        if(keycounter == 1)
        {
            a = 7;
        } 
        else if(keycounter == 2)
        {
            b = 7;
        }
        else if(keycounter == 3)
        {
            c = 7;
        }
    }
	
    if (Y_4 == 1) 
    { 
        delay(100); 
        while (Y_4==1);
        key = 0; 
        lcddata('0');
        delay(10);
        keycounter = keycounter + 1; 
        if(keycounter == 1)
        {
            a = 0;
        } 
        else if(keycounter == 2)
        {
            b = 0;
        } 
        else if(keycounter == 3)
        {
            c = 0;
        }
    }
    
    
    X_1 = 0; X_2 = 1; X_3 = 0;
    
    if (Y_1 == 1) 
    { 
        delay(100); 
        while (Y_1==1);
        key = 2;
        lcddata('2');
        delay(10);
        keycounter = keycounter + 1;
        if(keycounter == 1)
        {
            a = 2;
        } 
        else if(keycounter == 2)
        {
            b = 2;
        } 
        else if(keycounter == 3)
        {
            c = 2;
        }
    }
	
    if (Y_2 == 1) 
    { 
        delay(100);
        while (Y_2==1); 
        key = 5; 
        lcddata('5');
        delay(10);
        keycounter = keycounter + 1;
        if(keycounter == 1)
        {
            a = 5;
        } 
        else if(keycounter == 2)
        {
            b = 5;
        } 
        else if(keycounter == 3)
        {
            c = 5;
        }
    }
	
    if (Y_3 == 1)
    { 
        delay(100);
        while (Y_3==1);
        key = 8; 
        lcddata('8');
        delay(10);
        keycounter = keycounter + 1; 
        if(keycounter == 1)
        {
            a = 8;
        } 
        else if(keycounter == 2)
        {
            b = 8;
        } 
        else if(keycounter == 3)
        {
            c = 8;
        }
    }
	
    if (Y_4 == 1)
    { 
        delay(100);
        while (Y_4==1);
        key = 0;
        lcddata('0');
        delay(10);
        keycounter = keycounter + 1;
        if(keycounter == 1)
        {
            a = 0;
        } 
        else if(keycounter == 2)
        {
            b = 0;
        } 
        else if(keycounter == 3)
        {
            c = 0;
        }
    }
    
    
    X_1 = 0; X_2 = 0; X_3 = 1;
    
    if (Y_1 == 1)
    { 
        delay(100);
        while (Y_1==1);
        key = 3; 
        lcddata('3');
        delay(10);
        keycounter = keycounter + 1;
        if(keycounter == 1)
        {
            a = 3;
        } 
        else if(keycounter == 2)
        {
            b = 3;
        } 
        else if(keycounter == 3)
        {
            c = 3;
        }
    }
	
    if (Y_2 == 1) 
    { 
        delay(100); 
        while (Y_2==1);
        key = 6; 
        lcddata('6');
        delay(10);
        keycounter = keycounter + 1;
        if(keycounter == 1)
        {
            a = 6;
        } 
        else if(keycounter == 2)
        {
            b = 6;
        } 
        else if(keycounter == 3)
        {
            c = 6;
        }
    }
	
    if (Y_3 == 1) 
    { 
        delay(100); 
        while (Y_3==1); 
        key = 9;
        lcddata('9');
        delay(10);
        keycounter = keycounter + 1; 
        if(keycounter == 1)
        {
            a = 9;
        } 
        else if(keycounter == 2)
        {
            b = 9;
        } 
        else if(keycounter == 3)
        {
            c = 9;
        }
    }
	
    if (Y_4 == 1) 
    { 
            delay(100);
            while(Y_4==1);
            key = 0; 
            lcddata('0');
            delay(10);
            keycounter = keycounter + 1; 
            if(keycounter == 1)
            {
                a = 0;
            } 
            else if(keycounter == 2)
            {
                b = 0;
            }
            else if(keycounter == 3)
            {
                c = 0;
            }
    
        
    }    
    
    
    if(keycounter == 3)
    {
        addresscal();
        keypadfull = 1;
    }
    
}
void EEPROM_Write (int address, int data)
{
    /*Write Operation*/
    EEADR=address;	/* Write address to the EEADR register*/
    EEDATA=data;	/* Copy data to the EEDATA register for write */
    EECON1bits.EEPGD=0;	/* Access data EEPROM memory*/
    EECON1bits.CFGS=0;	/* Access flash program or data memory*/
    EECON1bits.WREN=1;	/* Allow write to the memory*/
    INTCONbits.GIE=0;	/* Disable global interrupt*/
    
    /* Below sequence in EECON2 Register is necessary 
    to write data to EEPROM memory*/
    EECON2=0x55;
    EECON2=0xaa;
    
    EECON1bits.WR=1;	/* Start writing data to EEPROM memory*/
    INTCONbits.GIE=1;	/* Enable interrupt*/
    
    while(PIR2bits.EEIF==0);/* Wait for write operation complete */
    PIR2bits.EEIF=0;	/* Reset EEIF for further write operation */
    
}
int EEPROM_Read (int address)
{
    /*Read operation*/
    EEADR=address;	/* Read data at location 0x00*/
    EECON1bits.WREN=0;	/* WREN bit is clear for Read operation*/  
    EECON1bits.EEPGD=0;	/* Access data EEPROM memory*/
    EECON1bits.RD=1;	/* To Read data of EEPROM memory set RD=1*/
    return(EEDATA);
}

void inttocharo(int value)   // basically this is a function that is used to convert our integer mobile keypad values into character ones for occupied numbers on LCD
{
    if(value < 10 && value < 100)
    {
        A = ' ';
        B = ' ';
    if(value == 0){ C = '0';}else if(value == 1){ C = '1';}else if(value == 2){ C = '2';}else if(value == 3){ C = '3';}else if(value == 4){ C = '4';}
    else if(value == 5){ C = '5';}else if(value == 6){ C = '6';}else if(value == 7){ C = '7';}else if(value == 8){ C = '8';}else if(value == 9){ C = '9';}
    }
    else if(value == 10)
    {
        A = ' ';
        B = '1';
        C = '0';
    }
    else if(value > 10 && value < 100)
    {
        A = ' ';
    if(value == 11){ B = '1'; C ='1';}else if(value == 12){ B ='1'; C = '2';}else if(value == 13){ B ='1'; C = '3';}else if(value == 14){ B ='1'; C = '4';}
    else if(value == 15){ B ='1'; C = '5';}else if(value == 16){ B ='1'; C = '6';}else if(value == 17){ B ='1'; C = '7';}else if(value == 18){ B ='1'; C = '8';}else if(value == 19){ B ='1'; C = '9';}
    else if(value == 20){ B ='2'; C = '0';}else if(value == 21){ B ='2'; C = '1';}else if(value == 22){ B ='2'; C = '2';}else if(value == 23){ B ='2'; C = '3';}else if(value == 24){ B ='2'; C = '4';}
    else if(value == 25){ B ='2'; C = '5';}else if(value == 26){ B ='2'; C = '6';}else if(value == 27){ B ='2'; C = '7';}else if(value == 28){ B ='2'; C = '8';}else if(value == 29){ B ='2'; C = '9';}
    else if(value == 30){ B ='3'; C = '0';}else if(value == 31){ B ='3'; C = '1';}else if(value == 32){ B ='3'; C = '2';}else if(value == 33){ B ='3'; C = '3';}else if(value == 34){ B ='3'; C = '4';}
    else if(value == 35){ B ='3'; C = '5';}else if(value == 36){ B ='3'; C = '6';}else if(value == 37){ B ='3'; C = '7';}else if(value == 38){ B ='3'; C = '8';}else if(value == 39){ B ='3'; C = '9';}
    else if(value == 40){ B ='4'; C = '0';}else if(value == 41){ B ='4'; C = '1';}else if(value == 42){ B ='4'; C = '2';}else if(value == 43){ B ='4'; C = '3';}else if(value == 44){ B ='4'; C = '4';}
    else if(value == 45){ B ='4'; C = '5';}else if(value == 46){ B ='4'; C = '6';}else if(value == 47){ B ='4'; C = '7';}else if(value == 48){ B ='4'; C = '8';}else if(value == 49){ B ='4'; C = '9';}
    else if(value == 50){ B ='5'; C = '0';}else if(value == 51){ B ='5'; C = '1';}else if(value == 52){ B ='5'; C = '2';}else if(value == 53){ B ='5'; C = '3';}else if(value == 54){ B ='5'; C = '4';}
    else if(value == 55){ B ='5'; C = '5';}else if(value == 56){ B ='5'; C = '6';}else if(value == 57){ B ='5'; C = '7';}else if(value == 58){ B ='5'; C = '8';}else if(value == 59){ B ='5'; C = '9';}
    else if(value == 60){ B ='6'; C = '0';}else if(value == 61){ B ='6'; C = '1';}else if(value == 62){ B ='6'; C = '2';}else if(value == 63){ B ='6'; C = '3';}else if(value == 64){ B ='6'; C = '4';}
    else if(value == 65){ B ='6'; C = '5';}else if(value == 66){ B ='6'; C = '6';}else if(value == 67){ B ='6'; C = '7';}else if(value == 68){ B ='6'; C = '8';}else if(value == 69){ B ='6'; C = '9';}
    else if(value == 70){ B ='7'; C = '0';}else if(value == 71){ B ='7'; C = '1';}else if(value == 72){ B ='7'; C = '2';}else if(value == 73){ B ='7'; C = '3';}else if(value == 74){ B ='7'; C = '4';}
    else if(value == 75){ B ='7'; C = '5';}else if(value == 76){ B ='7'; C = '6';}else if(value == 77){ B ='7'; C = '7';}else if(value == 78){ B ='7'; C = '8';}else if(value == 79){ B ='7'; C = '9';}
    else if(value == 80){ B ='8'; C = '0';}else if(value == 81){ B ='8'; C = '1';}else if(value == 82){ B ='8'; C = '2';}else if(value == 83){ B ='8'; C = '3';}else if(value == 84){ B ='8'; C = '4';}
    else if(value == 85){ B ='8'; C = '5';}else if(value == 86){ B ='8'; C = '6';}else if(value == 87){ B ='8'; C = '7';}else if(value == 88){ B ='8'; C = '8';}else if(value == 89){ B ='8'; C = '9';}
    else if(value == 90){ B ='9'; C = '0';}else if(value == 91){ B ='9'; C = '1';}else if(value == 92){ B ='9'; C = '2';}else if(value == 93){ B ='9'; C = '3';}else if(value == 94){ B ='9'; C = '4';}
    else if(value == 95){ B ='9'; C = '5';}else if(value == 96){ B ='9'; C = '6';}else if(value == 97){ B ='9'; C = '7';}else if(value == 98){ B ='9'; C = '8';}else if(value == 99){ B ='9'; C = '9';}
    }
    else if(value == 100)
    {
        A = '1';
        B = '0';
        C = '0';
    }
}
void inttocharv (int value) // basically this is a function that is used to convert our integer mobile keypad values into character ones for vacant numbers on LCD
{
    if(value < 10 && value < 100)
    {
        D = ' ';
        E = ' ';
    if(value == 0){ F = '0';}else if(value == 1){ F = '1';}else if(value == 2){ F = '2';}else if(value == 3){ F = '3';}else if(value == 4){ F = '4';}
    else if(value == 5){ F = '5';}else if(value == 6){ F = '6';}else if(value == 7){ F = '7';}else if(value == 8){ F = '8';}else if(value == 9){ F = '9';}
    }
    else if(value == 10)
    {
        D = ' ';
        E = '1';
        F = '0';
    }
    else if(value > 10 && value < 100)
    {
        D = ' ';
    if(value == 11){ E = '1'; F ='1';}else if(value == 12){ E ='1'; F = '2';}else if(value == 13){ E ='1'; F = '3';}else if(value == 14){ E ='1'; F = '4';}
    else if(value == 15){ E ='1'; F = '5';}else if(value == 16){ E ='1'; F = '6';}else if(value == 17){ E ='1'; F = '7';}else if(value == 18){ E ='1'; F = '8';}else if(value == 19){ E ='1'; F = '9';}
    else if(value == 20){ E ='2'; F = '0';}else if(value == 21){ E ='2'; F = '1';}else if(value == 22){ E ='2'; F = '2';}else if(value == 23){ E ='2'; F = '3';}else if(value == 24){ E ='2'; F = '4';}
    else if(value == 25){ E ='2'; F = '5';}else if(value == 26){ E ='2'; F = '6';}else if(value == 27){ E ='2'; F = '7';}else if(value == 28){ E ='2'; F = '8';}else if(value == 29){ E ='2'; F = '9';}
    else if(value == 30){ E ='3'; F = '0';}else if(value == 31){ E ='3'; F = '1';}else if(value == 32){ E ='3'; F = '2';}else if(value == 33){ E ='3'; F = '3';}else if(value == 34){ E ='3'; F = '4';}
    else if(value == 35){ E ='3'; F = '5';}else if(value == 36){ E ='3'; F = '6';}else if(value == 37){ E ='3'; F = '7';}else if(value == 38){ E ='3'; F = '8';}else if(value == 39){ E ='3'; F = '9';}
    else if(value == 40){ E ='4'; F = '0';}else if(value == 41){ E ='4'; F = '1';}else if(value == 42){ E ='4'; F = '2';}else if(value == 43){ E ='4'; F = '3';}else if(value == 44){ E ='4'; F = '4';}
    else if(value == 45){ E ='4'; F = '5';}else if(value == 46){ E ='4'; F = '6';}else if(value == 47){ E ='4'; F = '7';}else if(value == 48){ E ='4'; F = '8';}else if(value == 49){ E ='4'; F = '9';}
    else if(value == 50){ E ='5'; F = '0';}else if(value == 51){ E ='5'; F = '1';}else if(value == 52){ E ='5'; F = '2';}else if(value == 53){ E ='5'; F = '3';}else if(value == 54){ E ='5'; F = '4';}
    else if(value == 55){ E ='5'; F = '5';}else if(value == 56){ E ='5'; F = '6';}else if(value == 57){ E ='5'; F = '7';}else if(value == 58){ E ='5'; F = '8';}else if(value == 59){ E ='5'; F = '9';}
    else if(value == 60){ E ='6'; F = '0';}else if(value == 61){ E ='6'; F = '1';}else if(value == 62){ E ='6'; F = '2';}else if(value == 63){ E ='6'; F = '3';}else if(value == 64){ E ='6'; F = '4';}
    else if(value == 65){ E ='6'; F = '5';}else if(value == 66){ E ='6'; F = '6';}else if(value == 67){ E ='6'; F = '7';}else if(value == 68){ E ='6'; F = '8';}else if(value == 69){ E ='6'; F = '9';}
    else if(value == 70){ E ='7'; F = '0';}else if(value == 71){ E ='7'; F = '1';}else if(value == 72){ E ='7'; F = '2';}else if(value == 73){ E ='7'; F = '3';}else if(value == 74){ E ='7'; F = '4';}
    else if(value == 75){ E ='7'; F = '5';}else if(value == 76){ E ='7'; F = '6';}else if(value == 77){ E ='7'; F = '7';}else if(value == 78){ E ='7'; F = '8';}else if(value == 79){ E ='7'; F = '9';}
    else if(value == 80){ E ='8'; F = '0';}else if(value == 81){ E ='8'; F = '1';}else if(value == 82){ E ='8'; F = '2';}else if(value == 83){ E ='8'; F = '3';}else if(value == 84){ E ='8'; F = '4';}
    else if(value == 85){ E ='8'; F = '5';}else if(value == 86){ E ='8'; F = '6';}else if(value == 87){ E ='8'; F = '7';}else if(value == 88){ E ='8'; F = '8';}else if(value == 89){ E ='8'; F = '9';}
    else if(value == 90){ E ='9'; F = '0';}else if(value == 91){ E ='9'; F = '1';}else if(value == 92){ E ='9'; F = '2';}else if(value == 93){ E ='9'; F = '3';}else if(value == 94){ E ='9'; F = '4';}
    else if(value == 95){ E ='9'; F = '5';}else if(value == 96){ E ='9'; F = '6';}else if(value == 97){ E ='9'; F = '7';}else if(value == 98){ E ='9'; F = '8';}else if(value == 99){ E ='9'; F = '9';}
    }
    else if(value == 100)
    {
        D = '1';
        E = '0';
        F = '0';
    }
    
}
void parkinglot()
{
    o = 0;                                      // now in this we are checking wether the parking lot has cars in it. So as the number of cars enter than OCCUPIED is incremented  
    v = 0;					// and as the cars leaves the Parking lot than vacant is incremented 
    for(int i=0; i<100; i++)			//Now this is the loop that is used to read 1 or 0 from EEPROM memory to check wether the car is present or not. If the car is present than 1 is already placed.
    {						// and if no car is present at the address than we have 0 present over there that means a car can be parked over here.
        int highlow = EEPROM_Read (i) ;		// but if their is already a 1 placed their that means we cannot park the car over there. So invalid message is displaced
        
        if(highlow == 1)
        {
            o = o + 1;
        }
        else if(highlow == 0)
        {
            v = v + 1;
        }
    }
    
    inttocharo(o);
    inttocharv(v);
    
}
void  __interrupt () ISR(void)
{
    keypadfull = 0;
    keycounter = 0;
    if(INT0IF == 1)			//NOW this is the FLAG that is used to check the enter button in our proteus that is this is used for entry of cars
    {
    INT0IF = 0;
    lcdcmd(0x01); // clearing lcd
    delay(5);
    lcdcmd(0x06); // enable auto shifting of cursor
    delay(5);
    lcdcmd(0x80); // frist line
    delay(5);
    for(int i=0; i<15; i++)
    {
        lcddata(eemsg[i]);     // this message says enter parking number for either parking the car or removing the car
        delay(5);
    }
    lcdcmd(0xC0); //second line
    delay(5);
    lcddata(' ');
    delay(5);
    lcddata(' ');
    delay(5);
    while(keypadfull == 0)           // Here if not all 3 values are pressed on the mobile key then we can still enter the address that is why we call user entry
    {
        userentry();
    }
    delay(5000);
    if(EEPROM_Read (address) == 0)	// Here we are 
    {
       EEPROM_Write (address, 1); 
    }
    else if(EEPROM_Read (address) == 1)
    {
        lcdcmd(0x01); // clearing lcd
        delay(5);
        lcdcmd(0x06); // enable auto shifting of cursor
        delay(5);
        lcdcmd(0x80); // first line 
        for(int i=0; i<13; i++)
        {
            lcddata(errmsg1[i]);		// That is already a car was present over here so error message saying INVALID
            delay(4000);
        }
        
        delay(5000);
    }
    parkinglot();				//Here we call parking lot to check the number of OCCUPIED and VACANCY
    lcdcmd(0x01); // clearing lcd
    delay(5);
    lcdcmd(0x06); // enable auto shifting of cursor
    delay(5);
    lcdcmd(0x80); // frist line
    delay(5);
    lcddata(' ');
    delay(5);
    lcddata(A);				//THESE A,B,C are the values of after they are compared with the less than 10 and 100 and greater than 10 and 100 and equal to 100.
    delay(5);
    lcddata(B);
    delay(5);
    lcddata(C);
    delay(5);
    for(int i=0; i<9; i++)
    {
        lcddata(msg1[i]);		// message saying OCCUPIED after the three A,B,C values 	
        delay(5);
    }
    lcdcmd(0xC0); //second line
    delay(5);
    lcddata(' ');
    delay(5);
    lcddata(D);
    delay(5);
    lcddata(E);
    delay(5);
    lcddata(F);
    delay(5);
     for(int i=0; i<7; i++)
    {
        lcddata(msg2[i]);		// message saying VACANCY after the three D,E,F values 
        delay(5);
    }
    
    }
    else if(INT1IF == 1)		//Now this is the flag that is used for Exit of cars from our parking lot
    {
    INT1IF = 0;    
    lcdcmd(0x01); // clearing lcd
    delay(5);
    lcdcmd(0x06); // enable auto shifting of cursor
    delay(5);
    lcdcmd(0x80); // frist line
    delay(5);
    for(int i=0; i<15; i++)
    {
        lcddata(eemsg[i]);		//this is the message that displays ENTER PARKING NUMBER
        delay(5);
    }
    lcdcmd(0xC0); //second line
    delay(5);
    lcddata(' ');
    delay(5);
    lcddata(' ');
    delay(5);
    keycounter = 0;
    keypadfull = 0;
    while(keypadfull == 0)
    {
        userentry();
    }
    delay(5000);
    if(EEPROM_Read (address) == 1)
    {
       EEPROM_Write (address, 0); 
    }
    else if(EEPROM_Read (address) == 0)
    {
        lcdcmd(0x01); // clearing lcd
        delay(5);
        lcdcmd(0x06); // enable auto shifting of cursor
        delay(5);
        lcdcmd(0x80); // frist line 
        for(int i=0; i<8; i++)
        {
            lcddata(errmsg1[i]);
            delay(5);
        }
        delay(5000);
    }
    parkinglot();
    lcdcmd(0x01); // clearing lcd
    delay(5);
    lcdcmd(0x06); // enable auto shifting of cursor
    delay(5);
    lcdcmd(0x80); // frist line
    delay(5);
    lcddata(' ');
    delay(5);
    lcddata(A);
    delay(5);
    lcddata(B);
    delay(5);
    lcddata(C);
    delay(5);
    for(int i=0; i<9; i++)
    {
        lcddata(msg1[i]);
        delay(5);
    }
    lcdcmd(0xC0); //second line
    delay(5);
    lcddata(' ');
    delay(5);
    lcddata(D);
    delay(5);
    lcddata(E);
    delay(5);
    lcddata(F);
    delay(5);
     for(int i=0; i<7; i++)
    {
        lcddata(msg2[i]);
        delay(5);
    }
    }
    else if(INT2IF == 1)
    {
        INT2IF = 0;
        for(int i=0; i<100 ;i++)
        {
            EEPROM_Write (i, 0);
        }
        
        parkinglot();
        lcdcmd(0x01); // clearing lcd
    delay(5);
    lcdcmd(0x06); // enable auto shifting of cursor
    delay(5);
    lcdcmd(0x80); // frist line
    delay(5);
    lcddata(' ');
    delay(5);
    lcddata(A);
    delay(5);
    lcddata(B);
    delay(5);
    lcddata(C);
    delay(5);
    for(int i=0; i<9; i++)
    {
        lcddata(msg1[i]);
        delay(5);
    }
    lcdcmd(0xC0); //second line
    delay(5);
    lcddata(' ');
    delay(5);
    lcddata(D);
    delay(5);
    lcddata(E);
    delay(5);
    lcddata(F);
    delay(5);
     for(int i=0; i<7; i++)
    {
        lcddata(msg2[i]);
        delay(5);
    }
        
    }
}
void main(void) 
{
    ADCON1 = 0B00001111;
    TRISA0 = 0; //keypad output
    TRISA1 = 0; //keypad output
    TRISB3 = 0; //keypad output
    TRISB4 = 1; //keypad input
    TRISB5 = 1; //keypad input
    TRISB6 = 1; //keypad input
    TRISB7 = 1; //keypad input
    TRISE  = 0;
    TRISD  = 0;
    INT0IE = 1;
    INT1IE = 1;
    INT2IE = 1;
    GIE    = 1;
    PEIE   = 1;
    
    lcdcmd(0x38); // mode of lcd 
    delay(5);
    lcdcmd(0x0E); // display ON
    delay(5);
    
    parkinglot();
    lcdcmd(0x01); // clearing lcd
    delay(5);
    lcdcmd(0x06); // enable auto shifting of cursor
    delay(5);
    lcdcmd(0x80); // frist line
    delay(5);
    lcddata(' ');
    delay(5);
    lcddata(A);
    delay(5);
    lcddata(B);
    delay(5);
    lcddata(C);
    delay(5);
    for(int i=0; i<9; i++)
    {
        lcddata(msg1[i]);
        delay(5);
    }
    lcdcmd(0xC0); //second line
    delay(5);
    lcddata(' ');
    delay(5);
    lcddata(D);
    delay(5);
    lcddata(E);
    delay(5);
    lcddata(F);
    delay(5);
     for(int i=0; i<7; i++)
    {
        lcddata(msg2[i]);
        delay(5);
    }
        
    while(1);
    
}
