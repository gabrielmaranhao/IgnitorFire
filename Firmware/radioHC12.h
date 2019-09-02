
#ifndef RADIOHC12_H
#define	RADIOHC12_H

#include <xc.h>

/*
void EnterModeAT(){
    SETpin_HC = 0;
    __delay_ms(40);
}

void EnviarComandoAT_Radio(char* comando){
    
    EnterModeAT();
    
    char aux = 0;
    
    while(comando[aux] != '#'){
        putch(comando[aux]);
        aux = aux + 1;   
    }     
}

int TestRadio(){
    
    char aux;
  
    EnviarComandoAT_Radio(at_test);
    //USARTwrite('\r');
    
   
    if(ReadUSART() == 'O'){
        if(ReadUSART() == 'K'){    
           return 1; 
        }
        else return 0;
    }
    else return 0; 
}
*/

void HC12_enviar_mensagem_status(char tipo, char code_base){
    
    USARTwrite('#');
    USARTwrite(tipo);
    USARTwrite(code_base);
    USARTwrite('K');
    USARTwrite(tipo+code_base);
    USARTwrite('X');
        
}

void HC12_enviar_mensagem_ativacao(char code_base, char canais){
    
    USARTwrite('$');
    USARTwrite(code_base);
    USARTwrite(canais);
    
}

int HC12_receber_mensagem(){
    
    char aux = 0;
    char aux2 = 0;
    
    tipo_mensagem = 0x00;
    code_mensagem = 0x00;
    canais_mensagem = 0x00;
     
        aux2 = ReadUSART();
        if(aux2 == '#'){
            tipo_mensagem = ReadUSART();
            code_mensagem = ReadUSART();

            aux = ReadUSART();
            if(aux == 'C'){ // mensagem com canais
                canais_mensagem = ReadUSART();
                if(ReadUSART() == 'K'){

                    if(ReadUSART() == tipo_mensagem + code_mensagem + canais_mensagem){
                        return 1;
                    }
                    else{
                        //checksum error
                        return 0;
                    }
                }
                else{
                     //receiver error
                     return 0;
                }
            }
            else if(aux == 'K'){ // mensagem sem canais
                if(ReadUSART() == tipo_mensagem + code_mensagem){ //checksum já feito
                    return 1;    
                }
                else{
                     //checksum error
                     return 0;     
                }   
            }  
        }
        else if (aux2 == '$'){
            tipo_mensagem = 'P';
            code_mensagem = ReadUSART();
            canais_mensagem = ReadUSART();
            return 1;
            
        }
    return 0;
}


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

