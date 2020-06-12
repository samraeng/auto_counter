#include <16F877a.h>
#device ADC=10
#use delay(crystal=20000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)


#include "max7219.c"
#include "max7219_B.c"

#use fast_io(c)

//#use fast_io(a)

#use I2C(master,sda=PIN_C4,scl=PIN_C3,FORCE_HW)

#define PCF8574_Lectura   0b01000001   // Fija=0100  A2A1A0=001   Modo lectura=1

//#define PCF8574_Lectura   0b01001111   // Fija=0100  A2A1A0=001   Modo lectura=1

#define PCF8574_Escritura 0b01000000   // Fija=0100  A2A1A0=000   Modo escritura=0 (=PCF8574P)

int8 portd;
#locate portd = 0x008
#bit    relay1=portd.0
#bit    relay2=portd.1
#bit    relay3=portd.2
#bit    relay4=portd.3
#bit    relay5=portd.4
#bit    relay6=portd.5
#bit    relay7=portd.6
#bit    relay8=portd.7


////////////////////////////////// keyboard /////////////////////
signed char cod_tecla1;
signed char tecla_hex;
int8 const num[13]={0xff,0x01,0x04,0x07,0x00,0x02,0x05,0x08,0xff,0x03,0x06,0x09};
//////////////////////////////////////////////////////////////////
VOID get_keyboard(void);

const unsigned char Font_B[16]={0x7E,0x30,0x6D,0x79,0x33,0x5B,0x5F,0x70,
                        0x7F,0x7B,0x77,0x1F,0x4E,0x3D,0x4F,0x47};

VOID K_DIGIT(UNSIGNED INT16 DATA);
VOID K_DIGIT2(UNSIGNED INT16 DATA2);


VOID DISPLAY(INT8 MODE);
VOID DISPLAY_max2(INT8 MODE2);


unsigned int8 scan_key(void);

////////////////////////max2//////
int8 ten_thousand2=0;
INT8 Thousand2=0;
INT8 Hundred2=0;
INT8 Ten2=0;
INT8 ONE2=0;
//////////////////////////////////
INT8 F_MODE;
int1 flg_key=0; 
int1 flg_key8=0;


////////////////////// function set distance  ////////////
int1 flg_set_dis=0;
 //int8 key;
 int8 set_key[10];
 //int8 num_digit[9];
 int8 num_key=0;
 int1 flg_clear=0;
 int8 index_program=1;
 unsigned int16 program[10];
 int8 num0,num10,num100,num1000,num10000;
 int8 passcount_set=0;
///////////////////////// main function/////////////////////
//int8 n=8;
int1 flg_enable=0;
int1 bit_blink;
int1 flg_fw=0;
int1 flg_rw=0;
int1 flg_dir=0;
int1 flg_ccp1=0;
int1 flg_set=0;
int1 flg_auto_mode=0;

int8 select_mode=0;
int8 count_time=0; 
int16 time_stop=0;
UNSIGNED INT16 puls=0;
unsigned int16 dis1,dis2,dis3,dis4;
//////////////////////// function prototype //////////////// 
void display(unsigned int16 p);
void save_program(int8 program_num);
void read_program(int8 num_prg); 
VOID get_keyboard(void);
void set_distance(void);
void display_mode(int8 mode);
void auto_mode(void);
#include <set_d.c>
//#include <i2c_pcf8574a.c>
#include <i2c_pcf85741.c>
#INT_EXT
void  int_rb0(void) 
{
    delay_us(10);

   cod_tecla1 = escanea_tecla1();
   tecla_hex = escanea_tecla2();   
  //printf("Digito[] = \r\n %2.2x",cod_tecla1);
 
 //  printf("Digi to[] =  %2u \r\n ",cod_tecla1);
   flg_key=1;
}

#INT_CCP1

void ccp1_isr(){

    if(flg_dir==0) puls++;
    if(flg_dir==1 && puls>0){ puls--; if(puls<program[index_program])flg_set=0;}
 
   flg_ccp1=1;
  // printf(" puls  =   %lu \r\n ",puls);
  // printf(" index_program  =   %u \r\n ",index_program);
  // printf(" program  =   %lu \r\n ",program[index_program]);
}
#INT_TIMER1
void  TIMER1_isr(void) 
{
  count_time++;
  if(count_time>=9)
  {
   count_time=0;
   bit_blink=~bit_blink;
   time_stop++;
     if(time_stop>=60)
     {

       flg_enable=0;
       relay1=0;
  
       time_stop=1000;
     
     }
  }
}

void main(void)
{  
   port_b_pullups(0x0f);   
   //OUTPUT_LOW(CLK);OUTPUT_LOW(LOAD);OUTPUT_LOW(SEND_DATA);
   delay_ms(10);
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);
   
  ////////////////////////////init max1
     enable_interrupts(global);     
      enable_interrupts(int_ext); enable_interrupts(INT_TIMER1);

      set_tris_b(0xFF);ext_int_edge( H_TO_L );
      set_tris_d(0x00);
    ///////////////////////// interrupt ccp ///////////////////
     enable_interrupts(int_ccp1);   
     setup_ccp1(CCP_CAPTURE_FE);
     ////////////////////////  setup timer1 //////////////////
     setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);      //104 ms overflow
     ///////////////////////////////////////////////////////////
   
     init7219();
      dataMaker(0X01, 0X00);
      dataMaker(0X02, 0X00);
      dataMaker(0X03, 0X00);
      dataMaker(0X04, 0X00);
      dataMaker(0X05, 0X00);
      dataMaker(0X06, 0X00);
      dataMaker(0X07, 0X00);
      dataMaker(0X08, 0X00);
////////////////////////////////init max 2

 //  OUTPUT_LOW(CLK_B);OUTPUT_LOW(LOAD);OUTPUT_LOW(SEND_DATA);
   delay_ms(10);
       
      init7219_B();
      dataMaker_B(0X01, 0X7e);
      dataMaker_B(0X02, 0X7e);
      dataMaker_B(0X03, 0X7e);
      dataMaker_B(0X04, 0X7e);      
      dataMaker_B(0X05, 0X7e);
      dataMaker_B(0X06, 0X7e);
      dataMaker_B(0X07, 0X7e);
      dataMaker_B(0X08, 0X7e); 
      delay_ms(1000);
      dataMaker_B(0X01, 0X00);
      dataMaker_B(0X02, 0X00);
      dataMaker_B(0X03, 0X00);
      dataMaker_B(0X04, 0X00);      
      dataMaker_B(0X05, 0X00);
      dataMaker_B(0X06, 0X00);
      dataMaker_B(0X07, 0X00);
      dataMaker_B(0X08, 0X00);       
      
//////////////////////////////////// keyboard /////////////////
teclado_ini();
///////////////////////////////////  test relay ///////////////

 relay1=relay2=relay3=relay4=relay5=relay6=relay7=relay8=0;
 read_program(index_program);
 
 //dis1=60000*0.1;
 //printf(" dis10percen  =   %lu \r\n ",dis1); 
while(TRUE){
 
//////////////////////////////////// get data keyboard //////////////////
if(flg_key)
{
  
  //////////////////////////////// select mode //////////////////////////
  if(cod_tecla1==16)
  { flg_key=0;
    select_mode++ ;//printf(" index_program  =   %u \r\n ",select_mode); 
    display_mode(select_mode);
    
    if(select_mode>=4)select_mode=0;
  }
  if(cod_tecla1==13)/////////////  power ready ////////////
  {          
      relay1=1;
      flg_enable=1;
      time_stop=0;
     if(read_eeprom(59)==1)
     {
     puls=(read_eeprom(54)*10000)+(read_eeprom(55)*1000)+(read_eeprom(56)*100)+(read_eeprom(57)*10)+read_eeprom(58);
     // puls=program[index_program];
      display(puls);
     } 
  }
  if(cod_tecla1==14 && flg_enable && !flg_set && select_mode==2)/////////////// fw joc mode  //////////////////
  {
    flg_fw=1;
    relay4=1;
    flg_dir=0;
    
  }
  if(cod_tecla1==14 && flg_enable && select_mode==3)////////////////////////// fw joc mode 3 (set program)
  {
    flg_fw=1;
    relay4=1;
    flg_dir=0;    
  }
  if(cod_tecla1==14 && flg_enable && select_mode==1)/////////////////////////// start auto mode  ///////////  
  {

    flg_auto_mode=1;
    auto_mode();
  } 
  if(cod_tecla1==15 && flg_enable && puls!=0)/////////////// rw joc //////////////////
  {
    flg_rw=1;
    relay5=1;
    flg_dir=1;
    
  }   

//-------------------- set distance program ------------
  if(cod_tecla1==12 && select_mode==3)
  {
   flg_set_dis=1;flg_key=0;
   set_distance();
  }
//----------------------  select program -------------
  if(cod_tecla1==8 && flg_key8==0)
  {
  
  flg_key8=1;  
  index_program++;
  read_program(index_program);//printf(" dis1  dis2  dis3    =   %lu %lu %lu \r\n ",dis1,dis2,dis3,);
  
  flg_key=0;
  if(index_program>9)index_program=0;
  }
//-------------------release key ----------------------
 if(cod_tecla1==0xff)
 {
    flg_key8=0;
    flg_key=0;
    if(flg_fw && (select_mode==2 || select_mode==3) && select_mode!=1)///////////// fw joc mode (select mode=2 or 3)
    {
     relay4=0;flg_fw=0;
    }
    if(flg_rw && (select_mode==2|| select_mode==3) && select_mode!=1)////////////// rw joc mode (select mode=2 or 3)
    {
     relay5=0;flg_rw=0;
    }
 }
 //----------------- ecoder input puls -----------------
 if(flg_ccp1)
 { //printf(" puls  =   %lu \r\n ",puls);
   display(puls);
  // if(puls>=program[index_program]){}
   if(puls>=program[index_program] && select_mode==2)
   {
    relay4=0;
    flg_set=1;
   }
   if(puls==0)
   {
    relay5=0;flg_set=0;
   }
   flg_ccp1=0;
 }
 
  
}
   ///////////////////////// blink enable //////////////////  
  
    if(bit_blink && flg_enable)
    {       
    blink_enter(7,0x01,1,1);     
    }
    if(!bit_blink )
    {
       blink_enter(7,0x01,0,1);
    
    }     
   ///////////////////////// blink mode set ////////////////
    if(bit_blink && !flg_enable && select_mode==0)
    {       
    blink_enter(8,0x01,1,1);     
    }   
    if(!bit_blink && !flg_enable && select_mode==0)
    {
     blink_enter(8,0x01,0,1);
    
    } 
 
 
      
      

   } 
}

VOID K_DIGIT2(UNSIGNED INT16 DATA2)
{
    ten_thousand2=(data2/10000);
    Thousand2 = ((data2% 10000)/1000);
    Hundred2  = (((data2 % 10000)%1000)/100);
    Ten2      = ((((data2 % 10000)%1000)%100)/10);
    One2      = (((((data2 % 10000)%1000)%100)%10)/1); 

    write_eeprom(54,ten_thousand2);
    write_eeprom(55,Thousand2);
    write_eeprom(56, Hundred2);
    write_eeprom(57, ten2);
    write_eeprom(58, one2); 
    write_eeprom(59,1);
}

VOID DISPLAY_max2(INT8 MODE2)
{
    switch (MODE2) {

    case 1:
    

             dataMaker_B(0X01, Font_B[ONE2]);
             dataMaker_B(0X02, 0X00);
             dataMaker_B(0X03, 0X00);
             dataMaker_B(0X04, 0X00);  
             dataMaker_B(0X05, 0X00); 
             break;

    case 2:
 
             ////////////////////////////max2/////// 

             dataMaker_B(0X01, Font_B[ONE2]);
             dataMaker_B(0X02, Font_B[TEN2]);
             dataMaker_B(0X03, 0X00);
             dataMaker_B(0X04, 0X00);
             dataMaker_B(0X05, 0X00);
           break;
    case 3:

             /////////////////////////////////

             dataMaker_B(0X01, Font_B[ONE2]);
             dataMaker_B(0X02, Font_B[TEN2]);
             dataMaker_B(0X03, Font_B[HUNDRED2]);
             dataMaker_B(0X04, 0X00);
             dataMaker_B(0X05, 0X00);
           break;
    case 4:
        

             dataMaker_B(0X01, Font_B[ONE2]);
             dataMaker_B(0X02, Font_B[TEN2]);
             dataMaker_B(0X03, Font_B[HUNDRED2]);
             dataMaker_B(0X04, Font_B[THOUSAND2]);   
             dataMaker_B(0X05, 0x00);  
           break;
    case 5:
        

             dataMaker_B(0X01, Font_B[ONE2]);
             dataMaker_B(0X02, Font_B[TEN2]);
             dataMaker_B(0X03, Font_B[HUNDRED2]);
             dataMaker_B(0X04, Font_B[THOUSAND2]);  
             dataMaker_B(0X05, Font_B[ten_THOUSAND2]); 
           break;           
                

    default:

            break; }

}
void read_program(int8 num_prg)
{   dataMaker(0X08,0x67); 
   dataMaker(0X07,Font_B[index_program]);
  switch (num_prg)
  {
    case 1:
           if(read_eeprom(5)==1)
           {
            num0=read_eeprom(0);
            num10=read_eeprom(1);
            num100=read_eeprom(2);
            num1000=read_eeprom(3);
            num10000=read_eeprom(4);
           }
           else
           {
           num0=num10=num100=num1000=num10000=0;
           }
           dataMaker(0x01,Font_B[num0]);
           dataMaker(0x02,Font_B[num10]);
           dataMaker(0x03,Font_B[num100]);
           dataMaker(0x04,Font_B[num1000]);    
           dataMaker(0x05,Font_B[num10000]);           

          break;

    case 2:
           if(read_eeprom(11)==1)
           {
            num0=read_eeprom(6);
            num10=read_eeprom(7);
            num100=read_eeprom(8);
            num1000=read_eeprom(9);
            num10000=read_eeprom(10);
           }
           else
           {
           num0=num10=num100=num1000=num10000=0;
           }
           dataMaker(0x01,Font_B[num0]);
           dataMaker(0x02,Font_B[num10]);
           dataMaker(0x03,Font_B[num100]);
           dataMaker(0x04,Font_B[num1000]);    
           dataMaker(0x05,Font_B[num10000]); 
          break;
    case 3:
           if(read_eeprom(17)==1)
           {
            num0=read_eeprom(12);
            num10=read_eeprom(13);
            num100=read_eeprom(14);
            num1000=read_eeprom(15);
            num10000=read_eeprom(16);
           }
           else
           {
           num0=num10=num100=num1000=num10000=0;
           }
           dataMaker(0x01,Font_B[num0]);
           dataMaker(0x02,Font_B[num10]);
           dataMaker(0x03,Font_B[num100]);
           dataMaker(0x04,Font_B[num1000]);    
           dataMaker(0x05,Font_B[num10000]); 
          break;
    case 4:
           if(read_eeprom(23)==1)
           {
            num0=read_eeprom(18);
            num10=read_eeprom(19);
            num100=read_eeprom(20);
            num1000=read_eeprom(21);
            num10000=read_eeprom(22);
           }
           else
           {
           num0=num10=num100=num1000=num10000=0;
           }
           dataMaker(0x01,Font_B[num0]);
           dataMaker(0x02,Font_B[num10]);
           dataMaker(0x03,Font_B[num100]);
           dataMaker(0x04,Font_B[num1000]);    
           dataMaker(0x05,Font_B[num10000]); 
          break;          
    case 5:
           if(read_eeprom(29)==1)
           {
            num0=read_eeprom(24);
            num10=read_eeprom(25);
            num100=read_eeprom(26);
            num1000=read_eeprom(27);
            num10000=read_eeprom(28);
           }
           else
           {
           num0=num10=num100=num1000=num10000=0;
           }
           dataMaker(0x01,Font_B[num0]);
           dataMaker(0x02,Font_B[num10]);
           dataMaker(0x03,Font_B[num100]);
           dataMaker(0x04,Font_B[num1000]);    
           dataMaker(0x05,Font_B[num10000]); 
          break;
    case 6:
           if(read_eeprom(35)==1)
           {
            num0=read_eeprom(30);
            num10=read_eeprom(31);
            num100=read_eeprom(32);
            num1000=read_eeprom(33);
            num10000=read_eeprom(34);
           }
           else
           {
           num0=num10=num100=num1000=num10000=0;
           }
           dataMaker(0x01,Font_B[num0]);
           dataMaker(0x02,Font_B[num10]);
           dataMaker(0x03,Font_B[num100]);
           dataMaker(0x04,Font_B[num1000]);    
           dataMaker(0x05,Font_B[num10000]);
          break;
    case 7:
           if(read_eeprom(41)==1)
           {
            num0=read_eeprom(36);
            num10=read_eeprom(37);
            num100=read_eeprom(38);
            num1000=read_eeprom(39);
            num10000=read_eeprom(40);
           }
           else
           {
           num0=num10=num100=num1000=num10000=0;
           }
           dataMaker(0x01,Font_B[num0]);
           dataMaker(0x02,Font_B[num10]);
           dataMaker(0x03,Font_B[num100]);
           dataMaker(0x04,Font_B[num1000]);    
           dataMaker(0x05,Font_B[num10000]);
          break;
    case 8:
          if(read_eeprom(47)==1)
           {
            num0=read_eeprom(42);
            num10=read_eeprom(43);
            num100=read_eeprom(44);
            num1000=read_eeprom(45);
            num10000=read_eeprom(46);
           }
           else
           {
           num0=num10=num100=num1000=num10000=0;
           }
           dataMaker(0x01,Font_B[num0]);
           dataMaker(0x02,Font_B[num10]);
           dataMaker(0x03,Font_B[num100]);
           dataMaker(0x04,Font_B[num1000]);    
           dataMaker(0x05,Font_B[num10000]);
          break;           
    case 9:
          if(read_eeprom(53)==1)
           {
            num0=read_eeprom(48);
            num10=read_eeprom(49);
            num100=read_eeprom(50);
            num1000=read_eeprom(51);
            num10000=read_eeprom(52);
           }
           else
           {
           num0=num10=num100=num1000=num10000=0;
           }
           dataMaker(0x01,Font_B[num0]);
           dataMaker(0x02,Font_B[num10]);
           dataMaker(0x03,Font_B[num100]);
           dataMaker(0x04,Font_B[num1000]);    
           dataMaker(0x05,Font_B[num10000]);
          break;           
          
          
     default:

            break;
  }
  program[index_program]=(num10000*10000)+(num1000*1000)+(num100*100)+(num10*10)+num0;
  dis1=program[index_program]*0.1;
  dis2=program[index_program]*0.25;
  
  dis3=program[index_program]*0.7;
  dis4=program[index_program]*0.9;
 // dis2=program[index_program]*0.25;
  
  //printf(" program  =  %u %lu \r\n ",index_program,program[index_program]);
}
void save_program(int8 program_num)
{
  switch (program_num)
  {
    case 1:
           write_eeprom(0,num0);
           write_eeprom(1,num10);
           write_eeprom(2,num100);
           write_eeprom(3,num1000);
           write_eeprom(4,num10000);
           write_eeprom(5,1);/////////// program 1 save
          break;
    case 2:
           write_eeprom(6,num0);
           write_eeprom(7,num10);
           write_eeprom(8,num100);
           write_eeprom(9,num1000);
           write_eeprom(10,num10000);
           write_eeprom(11,1);/////////// program 2 save    
          break;
    case 3:
           write_eeprom(12,num0);
           write_eeprom(13,num10);
           write_eeprom(14,num100);
           write_eeprom(15,num1000);
           write_eeprom(16,num10000);
           write_eeprom(17,1);/////////// program 3 save     
          break;
    case 4:
           write_eeprom(18,num0);
           write_eeprom(19,num10);
           write_eeprom(20,num100);
           write_eeprom(21,num1000);
           write_eeprom(22,num10000);
           write_eeprom(23,1);/////////// program 4 save     
          break;          
    case 5:
           write_eeprom(24,num0);
           write_eeprom(25,num10);
           write_eeprom(26,num100);
           write_eeprom(27,num1000);
           write_eeprom(28,num10000);
           write_eeprom(29,1);/////////// program 5 save     
          break;
    case 6:
           write_eeprom(30,num0);
           write_eeprom(31,num10);
           write_eeprom(32,num100);
           write_eeprom(33,num1000);
           write_eeprom(34,num10000);
           write_eeprom(35,1);/////////// program 6 save     
          break;          
    case 7:
           write_eeprom(36,num0);
           write_eeprom(37,num10);
           write_eeprom(38,num100);
           write_eeprom(39,num1000);
           write_eeprom(40,num10000);
           write_eeprom(41,1);/////////// program 7 save      
          break;
    case 8:
           write_eeprom(42,num0);
           write_eeprom(43,num10);
           write_eeprom(44,num100);
           write_eeprom(45,num1000);
           write_eeprom(46,num10000);
           write_eeprom(47,1);/////////// program 8 save      
          break;    
    case 9:
           write_eeprom(48,num0);
           write_eeprom(49,num10);
           write_eeprom(50,num100);
           write_eeprom(51,num1000);
           write_eeprom(52,num10000);
           write_eeprom(53,1);/////////// program 9 save    
          break;   
          
    default:

            break;    
  }
}

void display(unsigned int16 p)
{
      IF(p>0 && p<10) F_MODE=1;
      IF(p>=10 && p< 100) F_MODE=2;
      IF(p>=100 && p<1000) F_MODE=3;
      IF(p>=1000 && p<10000) F_MODE=4;
      if(p>=1000) f_mode=5;
      
     // K_DIGIT(K);
      K_DIGIT2(p);DISPLAY_max2(f_mode);
}

void display_mode(int8 mode)
{
  switch (mode){
  
      case 1:
              dataMaker_B(0X08, 0X77);// display A
              
             break;
      case 2:
             dataMaker_B(0X08, 0X38);// display j
             break;
      case 3:
             dataMaker_B(0X08, 0X67);// display b
             break;
      default:
      
             break;
             
  }
}
void auto_mode(void)
{   relay4=1;flg_set=0;
    relay3=1;flg_dir=0;
   while(flg_auto_mode)
   {
    ////////////////////////////// blink active function ////////////   
     if(bit_blink && flg_enable)
     {       
     blink_enter(7,0x01,1,1);     
     }
     if(!bit_blink )
     {
       blink_enter(7,0x01,0,1);     
     } 
      //////////////////////////////////////////////////////////////
      if(flg_ccp1)
      { 
       display(puls);
       flg_ccp1=0;
       if(puls>=program[index_program])
       {
         relay8=0;relay3=0;relay2=0;relay4=0;
         flg_set=1;
       }
       ///////////////////////// speed 1///////////////////////////////
       if(puls>=dis1 && puls <=dis2 && flg_set==0)
       {
        relay3=1; 
        relay4=1;
        relay2=1;
       }
       ////////////////////////// speed 2 //////////////////////////
       if(puls>dis2 && puls<=dis3 && flg_set==0)
       {
        relay3=1; 
        relay4=1;
        relay2=1;
        relay8=1;
       }
       /////////////////////////// speed 3 /////////////////////////////
       if(puls>dis3 && puls<=dis4 && flg_set==0)
       {
         relay8=0;
         relay2=1;
         relay4=1;relay3=1;
       }
       //////////////////////////// speed4 ///////////////////////////
        if( puls>dis4 && flg_set==0)
       {
         relay2=0;
         relay4=1;relay3=1;
       }      
       
      }
      ///////////////////////////  
   }
}
