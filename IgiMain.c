//Define equipamento
#define RECEPTOR           0
#define TRANSMISSOR        1

///-----------------------------------------
#if RECEPTOR 

#define RADIO       1     
#define BLUETOOTH   0

#endif

#if TRANSMISSOR

#define RADIO       1     
#define BLUETOOTH   1

#endif

#include <xc.h>
#include <p18lf1320.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pic18.h>

#include "config.h"
#include "definicoes.h"
#include "my_usart.h"
#include "timers.h"

#if RADIO

#include "radioHC12.h"

#endif

#if BLUETOOTH

#include "bluetooth.h"

#endif



void interrupt ISR(void){
    
    if(INTCONbits.TMR0IF){  // timer 1 seg
        INTCONbits.TMR0IF = 0;
        TimerSeg_Reset();
        
        Cont_Seg = Cont_Seg + 1;  // incrementa a cada segundo, é zerado
        Cont_Seg_Start = Cont_Seg_Start + 1; //contador desde o começo
        if(Cont_Seg == 60) {
            Cont_Seg = 0;
            Cont_Min = Cont_Min +1;
            Cont_Min_blue  = Cont_Min;
        }     
    }
#if TRANSMISSOR
    if(PIR1bits.RCIF){
        if (MuxDemux == 0){ // rádio
            if(HC12_receber_mensagem()){
                USARTclean();
                if(tipo_mensagem == start){
                    eeprom_write(num_bases,code_mensagem);
                    num_bases = num_bases + 0x0001;
                                        
                    HC12_enviar_mensagem_status(resposta,code_mensagem);
                    
                    flag_resposta_start = 1;
    
                }
                else if(tipo_mensagem == alive){
                    
                    base_alive_bluetooth = code_mensagem; // enviar codigo da base via bluetooth
                    
                    //avisar para o celualr que continua ativa                                    
                }
            }
        }
        else if (MuxDemux == 1){ // bluetooth
            if(HC05_receber_mensagem()){
                USARTclean();
                if(tipo_mensagem == OK){
                    led_vermelho = 0;
                    flag_bluetooth_conectado = 1;  
                }
                if(tipo_mensagem == 'P' && flag_bluetooth_conectado == 1 && (code == eeprom_read(0x00)||code == eeprom_read(0x01))){
                    selectModule(HC_12);
                    flag_fire = 1;
                    HC12_enviar_mensagem_ativacao(code,canais_mensagem);
                } 
            }
        }
    PIR1bits.RCIF = 0;
    }
        
#endif
    
    
#if RECEPTOR 
    if(PIR1bits.RCIF){
        if(HC12_receber_mensagem()){
            USARTclean();
            if(tipo_mensagem == resposta){
                if(code_mensagem == eeprom_read(0x00)){ // resposta da base
                    
                    flag_start_rspondido = 1;
                    flag_alive_respondido = 1;
                    
                }
                
            }
            else if(code_mensagem == eeprom_read(0x00)){
                
                switch(canais_mensagem){
                    case 0b00000001:
                        Ch1 = ON_fire;
                        __delay_ms(100);
                        Ch1 = OFF_fire;
                        
                        break;                 
                    case 0b00000010:
                        Ch2 = ON_fire;
                        __delay_ms(100);
                        Ch2 = OFF_fire;
                        
                        break;
                    case 0b00000100:
                        Ch3 = ON_fire;
                        __delay_ms(100);
                        Ch3 = OFF_fire;
                        
                        break;
                    case 0b00001000:
                        Ch4 = ON_fire;
                        __delay_ms(100);
                        Ch4 = OFF_fire;
                        
                        break;
                    case 0b00010000:
                        Ch5 = ON_fire;
                        __delay_ms(100);
                        Ch5 = OFF_fire;
                        
                        break;
                    case 0b00100000:
                        Ch6 = ON_fire;
                        __delay_ms(100);
                        Ch6 = OFF_fire;
                        
                        break;    
                }                
            }
            
        }
        PIR1bits.RCIF = 0;
    }
#endif 
}


//-----EEPROM functions-------
void Busy_eep ( void )
{
	while(EECON1bits.WR);
}
unsigned char Read_b_eep( unsigned int badd )
{
	EEADR = (badd & 0x0ff);
  	EECON1bits.CFGS = 0;
	EECON1bits.EEPGD = 0;
	EECON1bits.RD = 1;
	Nop();							//Nop may be required for latency at high frequencies
	Nop();							//Nop may be required for latency at high frequencies
	return ( EEDATA );              // return with read byte
}
void Write_b_eep( unsigned int badd,unsigned char bdat )
{
	char GIE_BIT_VAL = 0;
	EEADR = (badd & 0x0ff);
  	EEDATA = bdat;
  	EECON1bits.EEPGD = 0;
	EECON1bits.CFGS = 0;
	EECON1bits.WREN = 1;
	GIE_BIT_VAL = INTCONbits.GIE;
	INTCONbits.GIE = 0;
	EECON2 = 0x55;
	EECON2 = 0xAA;
	EECON1bits.WR = 1;
	while(EECON1bits.WR);				//Wait till the write completion
	INTCONbits.GIE = GIE_BIT_VAL;
	EECON1bits.WREN = 0;
}
//-------------------------


void main(void) {
    
    ADCON1 = 0x7F; // digital pins
   
    USARTconfig();
    __delay_us(4);
    TimerSeg_Start(); //inicia contagem
    
    #if RECEPTOR 
    //------grava receptor
        eeprom_write(0x00,0x42); // A,B,C,D... mudar código para cada receptor
    //---------  
    #endif

    RCONbits.IPEN = 1;  // enable priority levels on interrupts
    
      
        
#if RECEPTOR
          
	initializeReceptor();
    piscar_leds();
    piscar_leds();   
    
    PIE1bits.RCIE = 1;   // Interrupt on receipt
    
    while(1){       
    
        if(Cont_Seg_Start >= 1 && flag_start_rspondido == 0){
            piscar_led_vermelho();
                
            HC12_enviar_mensagem_status(start,eeprom_read(0x00));
            
            Cont_Seg_Start = 0;

        }
        if (Cont_Seg_Start  >= 3 &&  flag_start_rspondido == 1){
           
            piscar_led_verde();
            piscar_led_verde();
            
            Cont_Seg_Start = 0;
            
        
        }
        //if (Cont_Min >= 2 && flag_alive_respondido == 1){
            
            //HC12_enviar_mensagem_status(alive,eeprom_read(0x00));
           
          //  Cont_Min = 0;
        //}

    }
#endif
          
#if TRANSMISSOR
    initializeTransmissor();
    selectModule(HC_12);
    
    //zerado memória
    for(char z = 0x00;z <= 0x010;z++){
        eeprom_write(z,0xff);
    }
       
    piscar_leds();
    piscar_leds();
    
    PIE1bits.RCIE = 1;   // Interrupt on rece ipt
    
    while(1){ 
        
        if(flag_fire == 1){
            
            USARTclean();
            
            led_vermelho = 1;
            flag_bluetooth_conectado = 0;
            
            flag_fire = 0;
            __delay_ms(30);
            selectModule(HC_05);
        }
        if(Cont_Min_blue >= 1 && flag_bluetooth_conectado == 0){ // modo bluetooh
            USARTclean();
            led_vermelho = 1;
            selectModule(HC_05);
            Cont_Min_blue = 0;

        }
        if(Cont_Seg_Start >= 5 && flag_resposta_start == 1){
            
            for(int k=1;k<=num_bases;k++) piscar_led_verde();
            
            Cont_Seg_Start = 0;
            
        }
      
    }
   
#endif
     
}
