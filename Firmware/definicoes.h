/* 
 * File: definicoes.h  
 * Author: Gabriel Maranhão Soares
 * Comments: header de definições
 * Revision history: 1v
 */

#define	DEFINICOES_H

char tipo_mensagem = 0;
char code_mensagem = 0;
char canais_mensagem = 0;

char Cont_Seg = 0;
char Cont_Seg_Start = 0; // segundo aguardando
char Cont_Min = 0;
char Cont_Min_blue = 0;

void piscar_led_verde();
void piscar_led_vermelho();
void piscar_leds();

int16_t timeout_error = 0; 


#if RADIO

//char at_test[]          = "AT#";
//char at_boud[]          = "AT+B9600#";
//char at_func3[]         = "AT+FU3#";
//char at_request_baud[]  = "AT+RB#";
//char at_default[]       = "AT+DEFAULT#";

//void EnviarComandoAT_Radio(char* comando);
//int  TestRadio();
//void EnterModeAT();

//tipo de mensagens
#define start       'S'
#define resposta    'R'
#define done        'D'

#define alive       'A'
#define fogo        'F'


int  HC12_receber_mensagem();
void HC12_enviar_mensagem_status(char tipo, char code);
void HC12_enviar_mensagem_ativacao(char code_base, char canais);



#endif

#if BLUETOOTH

#define conectar    'C'
#define bases       'B'

#define OK          'K'
//#define fire        'F'
#define time        'T'

char code;

void HC05_enviar_mensagem_conexao();
void HC05_enviar_mensagem_bases(char num);
int  HC05_receber_mensagem();
int  HC05_receber_acionamento();

#endif

#if RECEPTOR

#define Ch1     LATAbits.LATA0 
#define Ch2     LATAbits.LATA2    
#define Ch3     LATBbits.LATB0 
#define Ch4     LATBbits.LATB2
#define Ch5     LATBbits.LATB3
#define Ch6     LATBbits.LATB5 

#define led_verde    LATBbits.LATB7 
#define led_vermelho LATBbits.LATB6 


#define ON_fire     1
#define OFF_fire    0

#define SETpin_HC LATAbits.LATA1 

char flag_start_rspondido = 0;
char flag_alive_respondido = 0;

char canal = 0x00;

void initializeReceptor(){
    
    //Canais 1 to 6 output
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA2 = 0;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB5 = 0;
    
    //leds output
    TRISBbits.TRISB6 = 0;   //red   
    TRISBbits.TRISB7 = 0;   //green
    
    //canais desligados
    Ch1 = OFF_fire;
    Ch2 = OFF_fire;
    Ch3 = OFF_fire;
    Ch4 = OFF_fire;
    Ch5 = OFF_fire;
    Ch6 = OFF_fire;
    
    
}

void acionar_Fogos (char canais)
{
    char index = 0x00;
    
    while(canais > 0b00000000){
        
        index = index + 1;
        canal = ((canais >> index)  & 0x01);
        
        if(canal == 0x01){
            
            switch (index){
                case 0x01:
                    Ch1 = ON_fire;
                    __delay_ms(1000);
                    Ch1 = OFF_fire;
                    canais = canais - 0b00000001;
                    break;
                case 0x02:
                    Ch2 = ON_fire;
                    __delay_ms(10);
                    Ch2 = OFF_fire;
                    canais = canais - 0b00000010;
                    break;
                case 0x03:
                    Ch3 = ON_fire;
                    __delay_ms(10);
                    Ch3 = OFF_fire;
                    canais = canais - 0b00000100;
                    break;
                case 0x04:
                    Ch4 = ON_fire;
                    __delay_ms(10);
                    Ch4 = OFF_fire;
                    canais = canais - 0b00001000;
                    break;
                case 0x05:
                    Ch5 = ON_fire;
                    __delay_ms(10);
                    Ch5 = OFF_fire;
                    canais = canais - 0b00010000;
                    break;
                case 0x06:
                    Ch6 = ON_fire;
                    __delay_ms(10);
                    Ch6 = OFF_fire;
                    canais = canais - 0b00100000;
                    break;
                default:
                    break;     
            }
        }
                     
    }
   
}


#endif

#if TRANSMISSOR

#define SETpin_HC LATAbits.LATA1

#define led_verde    LATAbits.LATA0 
#define led_vermelho LATAbits.LATA2  

#define HC_12   0 // radio
#define HC_05   1 // bluetooth

#define MuxDemux   LATBbits.LATB0

#define Channels 0b00000000;

int16_t num_bases = 0x0000;
char flag_resposta_start = 0;

char base_alive_bluetooth = 0;
char flag_bluetooth_conectado = 0;
char flag_fire = 0;

/*typedef struct
{
   char uniCode ; 
   char live;  
   
}FireBase;*/

void initializeTransmissor(){
    //leds
    TRISAbits.TRISA0 = 0;   //green
    TRISAbits.TRISA2 = 0;   //red
    
    //demux
    TRISBbits.TRISB0 = 0;  // output
}

void selectModule(char module){
    
    if(module == 0 ) MuxDemux =  0;
    if(module == 1 ) MuxDemux =  1;
}


#endif

void piscar_led_verde(){
    led_verde = 1;
    __delay_ms(150);
    led_verde = 0;
    __delay_ms(150);
}

void piscar_led_vermelho(){
     led_vermelho = 1;
    __delay_ms(150);
    led_vermelho = 0;
    __delay_ms(150);
}

void piscar_leds(){
    led_verde = 1;
    led_vermelho = 1;
    __delay_ms(200);
    led_verde = 0;
    led_vermelho = 0;
    __delay_ms(200);
}

#include <xc.h> 
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
#ifdef	__cplusplus
}
#endif /* __cplusplus */



