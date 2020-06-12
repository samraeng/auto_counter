
void PCF8574_Escribe(int8 dato);
int8 PCF8574_Lee(void);


void teclado_ini(void)
{
   port_b_pullups (0b00000001);  //Activa la resistencia pull-up interna del pin RB0/INT   
   PCF8574_Escribe(0b11110000);  //Pone como salida (0) las Filas  y como entrada (1) las Columnas
   delay_ms(20);                  //para que al pulsar haya un cambio de estado y se provoque la INT
}


void PCF8574_Escribe(int8 dato)
{
      i2c_start();                      // Start condition
      i2c_write(PCF8574_Escritura);      // Device address
      i2c_write(dato);                  // dato
      i2c_stop();                       // Stop condition
}


int8 escanea_tecla1(void)
{
   int8 l,c;
   signed int8 dato1=-1;
   int8 fila=0b11111110;       //Primera fila (P0=1)

   for (l=0; l<=3; l++)        //Recorre las 4 filas (lineas)
   {
      PCF8574_Escribe(fila);
    //  delay_ms(20);          //Para que le de tiempo a escribir
    
      delay_ms(10);     
      dato1=PCF8574_Lee();      
      c=0;
      if (!bit_test(dato1,4)) c=1;             //Col 1
      else if (!bit_test(dato1,5)) c=2;        //Col 2
      else if (!bit_test(dato1,6)) c=3;        //Col 3
      else if (!bit_test(dato1,7)) c=4;        //Col 4
      if(c) break;                             //Si c es distinto de 0 sale del for
      rotate_left(&fila, 1);     //Siguientes filas, va desplazando el 0 hacia la izquierda   
   }   

   if(c)
      dato1=(l*4)+c;
   else
      dato1=-1;                      //Ninguna tecla pulsada

   PCF8574_Escribe(0b11110000);     //Vuelve a poner como salida (0) las Lineas  
                                    //y como entrada (1) las Columnas
   return dato1;
}

int8 escanea_tecla2(void)
{
   int8 l,c;
   signed int8 dato2=-1;
   int8 fila=0b11111110;       //Primera fila (P0=1)

   for (l=0; l<=3; l++)        //Recorre las 4 filas (lineas)
   {
      PCF8574_Escribe(fila);
    //  delay_ms(20);          //Para que le de tiempo a escribir
    
      delay_ms(10);     
      dato2=PCF8574_Lee();      
      c=0;
      if (!bit_test(dato2,4)) c=1;             //Col 1
      else if (!bit_test(dato2,5)) c=2;        //Col 2
      else if (!bit_test(dato2,6)) c=3;        //Col 3
      else if (!bit_test(dato2,7)) c=4;        //Col 4
      if(c) break;                             //Si c es distinto de 0 sale del for
      rotate_left(&fila, 1);     //Siguientes filas, va desplazando el 0 hacia la izquierda   
   }   

   if(c)
      dato2=(l*4)+c;
   else
      dato2=-1;                      //Ninguna tecla pulsada

   PCF8574_Escribe(0b11110000);     //Vuelve a poner como salida (0) las Lineas  
                                    //y como entrada (1) las Columnas
   return dato2;
}


char tecla_c(char codigo)
{
   char tabla_tecla[10]={'1','2','3','4','5','6','7','8','9','0'};
   return tabla_tecla[codigo-1];
}


int8 lee_teclado(int8 tecla_hex)
{
//   int8 tecla_hex;
   tecla_hex=escanea_tecla2();
   return tecla_hex;
}


int8 PCF8574_Lee(void)
{
      int8 dato;
      i2c_start();                   // Start condition
      i2c_write(PCF8574_Lectura);   // Device address   (Aqui INT pasa otra vez a 1)
      //dato = i2c_read();            //original
      dato = i2c_read(0);
      i2c_stop();                    // Stop condition
      return dato;
}
