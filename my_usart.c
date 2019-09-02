/*
 * File:   my_usart.c
 * Author: gabriel maranhao
 *
 * Created on 5 de Abril de 2017, 22:23
 */


#include <xc.h>
#include <pic18lf1320.h>
#include "my_usart.h"

/***************** USART - PROTOCOL**************/ 

void USARTconfig(){
    
  TXSTA = 0;           // Reset USART registers to POR state
  RCSTA = 0;
  
  RCSTAbits.SPEN = 1;
  
  TRISBbits.TRISB1 = 1;
  TRISBbits.TRISB4 = 1;
  
  //-----TRANSMIT--------
  TXSTAbits.SYNC = 0;  //Sync or async operation
  TXSTAbits.TX9 = 0;    //9-bit transmission off;
  TXSTAbits.TXEN = 1;     //enable transmit
  //---------------------
  
  //-----RCEIVE--------
  RCSTAbits.RX9 = 0;
  RCSTAbits.CREN = 1;  //enable reception
  //---------------------
  
  //------BAUD RATE---- datasheet
  TXSTAbits.BRGH = 1;
  BAUDCTLbits.BRG16 = 0;
  SPBRGH = 0;
  SPBRG = 0x81; // =9600
  //SPBRG = 0x40; // =19200
  
  //-------------------

  //-----INTERRUPTS----
  INTCONbits.PEIE = 1;  //Enable pheripheral interrupt (serial port is a pheripheral)
  INTCONbits.GIE = 1;
  
  IPR1bits.RCIP = 0; // low priority
  
  PIE1bits.RCIE = 0;    // Interrupt on receipt
  PIE1bits.TXIE = 0;     //TX interrupt off;
  //----------------
  
}
void USARTwrite(char data){
  while(!TXIF) continue;
  TXREG = data;
}
char USARTdataRdy(void){
  if(PIR1bits.RCIF)  // If RCIF is set
    return 1;  // Data is available, return TRUE
  return 0;  // Data not available, return FALSE
}
char ReadUSART(void){
   while (PIR1bits.RC1IF == 0) { // wait for char
       
        if (RCSTAbits.OERR == 1) {
            RCSTAbits.OERR = 0; // clear overrun if it occurs
            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;
        }
}  
return RCREG1;
}
void USARTclose(void){
  RCSTA &= 0b01001111;  // Disable the receiver
  TXSTAbits.TXEN = 0;   // and transmitter

  PIE1 &= 0b11001111;   // Disable both interrupts
}
void putch(char c){   //função "printf" utiliza dela para printar informaçoes
    while(!TXIF) continue;
    //LATCbits.LATC2 = 0;
    TXREG = c;
}
char getch(){
    return RCREG1;
}
/*
 -> Para printar HEX na porta USART:
 * printf("0x%02x",data);   onde "data" é a variavel que será printada em HEX
 */
/*----------------------------------------------*/

void USARTgetString ( char *buffer,  unsigned int len){
    int i;    // Length counter
    unsigned char data;
    
    
    for(i=0;i<len;i++)  // Only retrieve len characters
  {
    //while(!USARTdataRdy());// Wait for data to be received

    data = ReadUSART();    // Get a character from the USART
                           // and save in the string
    *buffer = data;
    buffer++;              // Increment the string pointer
  }
}

void USARTclean(){
    
       getch();
       getch();
       getch();
       getch();

            
}