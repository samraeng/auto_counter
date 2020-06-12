
void blink_enter(int8 digit_blink,int8 data,int1 on,int1 page);
void clear_dis(void);
int8  get_key(int8 code);
void display_set(int8 digit);


void set_distance(void)
{    
   dataMaker(0X08,0x67);//display p
   dataMaker(0X07,Font_B[index_program]);// display program number  
   clear_dis();

   delay_ms(100);
   read_program(index_program);num_key=0;flg_clear=0;
  while(flg_set_dis)
  { 

    ///////////////////////// blink set distance //////////////////  
  
    if(passcount_set>=0 && passcount_set<=50)
    {
      blink_enter(0x06,0x01,1,0);passcount_set++;
    
    }
  
    if(passcount_set>=50 && passcount_set<=100)
    {
       blink_enter(0x06,0x01,0,0);passcount_set++;

    }  
    if(passcount_set>100 )
    {
     passcount_set=0;
    }     
    /////////////////////////////////////////////////////////
    if(flg_key && cod_tecla1 != 0xff && (cod_tecla1 !=13))
    {
      flg_key=0;
      num_key++;
      set_key[num_key]=get_key(cod_tecla1);
     if(cod_tecla1!=8){ display_set(num_key);} 
    }
  //////////////////////////////////////////////////////////  
    if(flg_key && cod_tecla1 ==13 )
    {// flg_key=0;
      num_key=0;
      flg_clear=1;
      flg_set_dis=0;index_program=1;
      clear_dis();//flg_clear=0;
      read_program(index_program);
    }
 if(flg_ccp1)
 { //printf(" puls  =   %lu \r\n ",puls);
   display(puls);

   flg_ccp1=0;
 }
  
  
  }
}
void blink_enter(int8 digit_blink,int8 data,int1 on,int1 page)
{

switch (digit_blink)
{
  case 1:
         if(page==0 && on==1){dataMaker(0x01,data);}
         if(page==0 && on==0){dataMaker(0x01,0x00);}
         //////////////////////////////////////////////////
         if(page==1 && on==1){dataMaker_b(0x01,data);}
         if(page==1 && on==0){dataMaker_b(0x01,0x00);}         
         break;
  case 2:
         if(page==0 && on==1){dataMaker(0x02,data);}
         if(page==0 && on==0){dataMaker(0x02,0x00);}
         //////////////////////////////////////////////////
         if(page==1 && on==1){dataMaker_b(0x02,data);}
         if(page==1 && on==0){dataMaker_b(0x02,0x00);}   
         break;  
  case 3:
         if(page==0 && on==1){dataMaker(0x03,data);}
         if(page==0 && on==0){dataMaker(0x03,0x00);}
         //////////////////////////////////////////////////
         if(page==1 && on==1){dataMaker_b(0x03,data);}
         if(page==1 && on==0){dataMaker_b(0x03,0x00);}     
         break; 
  case 4:
         if(page==0 && on==1){dataMaker(0x04,data);}
         if(page==0 && on==0){dataMaker(0x04,0x00);}
         //////////////////////////////////////////////////
         if(page==1 && on==1){dataMaker_b(0x04,data);}
         if(page==1 && on==0){dataMaker_b(0x04,0x00);}     
         break;          
  case 5:
         if(page==0 && on==1){dataMaker(0x05,data);}
         if(page==0 && on==0){dataMaker(0x05,0x00);}
         //////////////////////////////////////////////////
         if(page==1 && on==1){dataMaker_b(0x05,data);}
         if(page==1 && on==0){dataMaker_b(0x05,0x00);}   
         break;
  case 6:
         if(page==0 && on==1){dataMaker(0x06,data);}
         if(page==0 && on==0){dataMaker(0x06,0x00);}
         //////////////////////////////////////////////////
         if(page==1 && on==1){dataMaker_b(0x06,data);}
         if(page==1 && on==0){dataMaker_b(0x06,0x00);}  
         break;  
  case 7:
         if(page==0 && on==1){dataMaker(0x07,data);}
         if(page==0 && on==0){dataMaker(0x07,0x00);}
         //////////////////////////////////////////////////
         if(page==1 && on==1){dataMaker_b(0x07,data);}
         if(page==1 && on==0){dataMaker_b(0x07,0x00);}   
         break; 
  case 8:
         if(page==0 && on==1){dataMaker(0x08,data);}
         if(page==0 && on==0){dataMaker(0x08,0x00);}
         //////////////////////////////////////////////////
         if(page==1 && on==1){dataMaker_b(0x08,data);}
         if(page==1 && on==0){dataMaker_b(0x08,0x00);}   
         break;           
  default :
  
          break;
          
}

}
void clear_dis(void)
{
   dataMaker(0X01,0x00);// 
   dataMaker(0X02,0x00);// 
   dataMaker(0X03,0x00);// 
   dataMaker(0X04,0x00);//
   dataMaker(0X05,0x00);// 
  
}

int8 get_key(int8 code)
{
int8 key;
switch (code) {

    case 1:
           
           key=1;
           break;

    case 2:
           key=4;
           break;
    case 3:
           key=7;
           break;
           
    case 4:
           key=0;
           break;

    case 5:
           key=2;
           break;
    case 6:
           key=5;
           break;           
    case 7:
           key=8;
           break;
    case 8:
           //key=0xff;
           index_program++;
           if(index_program>9)index_program=1;
            dataMaker(0X07,Font_B[index_program]);// display program number 
            read_program(index_program);
            if(index_program>9)index_program=1;
            num_key=0;
           break;
    case 9:
           key=3;
           break;
    case 10:
           key=6; 
           break;
    case 11:
           key=9; 
           break;
    case 12:
           //0x0c;
           num_key=0;
           flg_clear=1;           
           save_program(index_program);flg_clear=0;
           break;
    case 16:
           //0x0c;
           num_key=0;
           flg_clear=1;
           clear_dis(); flg_clear=0;
           break;            
    default:

            break; }
            
     return key;       
}
void display_set(int8 digit)
{
 
switch (digit) {

    case 1:
           if(!flg_clear)
           {
           num0=set_key[1];
           num10=num100=num1000=num10000=0;
           dataMaker(0x01,Font_B[num0]);
           }
           break;

    case 2:
           if(!flg_clear)
           {    
           num0=set_key[2];
           num10=set_key[1];
           num100=num1000=num10000=0;
           dataMaker(0x01,Font_B[num0]);
           dataMaker(0x02,Font_B[num10]);
           }
           break;
    case 3:
           if(!flg_clear)
           {           
           num0=set_key[3];
           num10=set_key[2];
           num100=set_key[1];
           num1000=num10000=0;
           dataMaker(0x01,Font_B[num0]);
           dataMaker(0x02,Font_B[num10]);
           dataMaker(0x03,Font_B[num100]);
           }
           break;
           
    case 4:
           if(!flg_clear)
           {    
           num0=set_key[4];
           num10=set_key[3];
           num100=set_key[2];
           num1000=set_key[1];
           num10000=0;
           dataMaker(0x01,Font_B[num0]);
           dataMaker(0x02,Font_B[num10]);
           dataMaker(0x03,Font_B[num100]);
           dataMaker(0x04,Font_B[num1000]);
           }
           break;

    case 5:
           if(!flg_clear)
           {    
           num0=set_key[5];
           num10=set_key[4];
           num100=set_key[3];
           num1000=set_key[2];
           num10000=set_key[1];
           dataMaker(0x01,Font_B[num0]);
           dataMaker(0x02,Font_B[num10]);
           dataMaker(0x03,Font_B[num100]);
           dataMaker(0x04,Font_B[num1000]);    
           dataMaker(0x05,Font_B[num10000]);
           }
           break;
          
    default:

            break; }
            
   program[index_program]=(num10000*10000)+(num1000*1000)+(num100*100)+(num10*10)+num0;
}

