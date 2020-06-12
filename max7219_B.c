/* 
****************************************************************************************************** 
* Chip        : MAX7219 
* Author      : Burak HANÇERLI 
* Mail        : bhancerli@gmail.com 
* Description : MAX7219 LED Display Driver Routines - v1.1 
*    
*  
*  The Maxim MAX7219 is a led display driver. It can control up to 64 individual leds or eight 
*  7-segment-display. 
* 
*  Max7912 uses 3-wire to communicate with microcontroller. 
*    - DATA   : Used to transmit data. 
*    - CLOCK  : Used to generate clock signal which is required for transmitting each bite of data. 
*    - LOAD   : Used to load data to the Max7913's Dual-Port SRam. 
*                  
* 
* 
*  Advantages of Max7219 : 
*  - Adjustable intensity. (More or less bright leds) 
*  - Shutdown mode (much less power consumption) 
*  - Test mode (To see if digits are working or not) 
*  - Adjustable digit number. (1 to 8 seven-segment-display) 
*  - Changeable decode mode. (codeB-decode-mode or no-decode-mode) 
*      (Look over datasheet for more detailed information) 
* 
* 
*  DESCRIPTION OF USER FUNCTIONS 
* ------------------------------- 
*  init7219()                      : Required for initialize MAX7219. This function have has to be called before calling any function. 
*  write7219(digit, data)          : Writes data to the specified digit number. If Decimal Point needed on any digit, just add 
                                     128 to the data. 
                                     For example : write7219(1,3)   = writes "3"  to first digit. 
                                                   write7219(1,131) = writes "3." to first digit. 
*  shutdown7219(operatingMode)     : Set operatingMode = 0 to Shutdown mode 
*                                    Set operatingMode = 1 to Normal mode 
*  decode7219(decodeMode)          : Sets digit-decode mode. code-B or no-decode mode. Look up datasheet for detailed instructions. 
*                                    For example, if user sets decodeMode = 4, (4=0b00000100), then 3. digit will be 
*                                    decoded as code-B algorithm, but other pins don't have any decode mode. 
*  brightness7219(brightnessLevel) : Sets brightness level of digits. 
*                                    brightnessLevel = 0  ; minimum brightness level 
*                                    brightnessLevel = 15 ; maximum brigthness level 
*                                    
*  scanLimit7219(totalDigitNumber) : Sets number of connected digits to the MAX7219. When init7912() function is called, 
                                     number of digit will be setted to 4 as default. 
                                      
*  test7219(testMode)              : Sets 7-segment-display test-mode on or off. 
                                     testMode = 0 ; normal operation mode 
                                     testMode = 1 ; display test mode 
                                      
** THIS LIBRARY CAN BE USED, DEVELOPED OR SHARED WITH REFERRING THE AUTHOR.                                    
******************************************************************************************************/ 


// CONSTANTS // 
// - Connection Pins (CHANGE THESE PINS AS YOU WISH) 
#define CLK_B       PIN_a5
#define LOAD_B      PIN_e0 
#define SEND_DATA_B PIN_a3 

// - Mode Selection 
#define decode_B 0x09 
#define brightness_B 0x0A 
#define scanLimit_B 0x0B 
#define shutDown_B 0x0C 
#define dispTest_B 0x0F 

// Firt 4 bites (not used generally) 
#define firstBites_B 0x0 

// Wait function 
#define wait_B delay_ms(1) 

long serialData_2_B=0; 



void clock7219_B() // clock (CLK) pulse 
{ 
   output_low(CLK_B); 
   wait_B; 
   output_high(CLK_B); 
} 

void load7219_B()  // load (LOAD) pulse 
{ 
   output_low(LOAD_B); 
   wait_B; 
   output_high(LOAD_B); 
} 

void send7219_B(long data) // send 16 bit data word to the 7219 
{ 

   int count; 
   for (count=0; count<16; ++count) 
   { 
      output_bit(SEND_DATA_B, shift_left(&data,2,0)); // set data (DIN) level 
      clock7219_B(); // clock data in 
   } 
   load7219_B(); // latch the last 16 bits clocked 
} 

void dataMaker_B(byte mode, int dataIncoming) // Standart data package function 
{ 
   serialData_2_B=firstBites; 
   serialData_2_B<<=4; 
   serialData_2_B|=mode; 
   serialData_2_B<<=8; 
   serialData_2_B|=dataIncoming; 
   send7219_B(serialData_2_B); 
} 

void write7219_B(byte digit, int data) // Send data to digits 
{ 
   dataMaker_B(digit, data); 
} 

void shutdown7219_B(int operatingMode) 
{ 
   dataMaker_B(shutDown,operatingMode); 
} 

void decode7219_B(int decodeMode) 
{ 
   dataMaker_B(decode, decodeMode); 
} 

void brightness7219_B(int brightnessLevel) 
{ 
   dataMaker_B(brightness, brightnessLevel); 
} 

void scanLimit7219_B(int totalDigitNumber) 
{ 
   dataMaker_B(scanLimit, totalDigitNumber); 
} 

void test7219_B(int testMode) 
{ 
   dataMaker_B(dispTest, testMode); 
} 

void init7219_B() 
{ 
   dataMaker_B(shutDown, 1);     // No-Shutdown mode. Normal Operation mode. 
   dataMaker_B(decode, 0);      // All digits are programmed as code-B decode mode. 
   dataMaker_B(scanLimit, 7);    // Total digit number set to 4. 
   dataMaker_B(brightness, 7);  // Full brightness. 
}

