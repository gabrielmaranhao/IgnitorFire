
#ifndef BLUETOOTH_H
#define	BLUETOOTH_H

#include <xc.h>

#include "my_usart.h"




void HC05_enviar_mensagem_conexao(){
    
    USARTwrite('#');
    USARTwrite(conectar);
    USARTwrite(0x01);
    USARTwrite(conectar+0x01);//checksum
    USARTwrite('X');
        
}

void HC05_enviar_mensagem_bases(char num){
    
    USARTwrite('#');
    USARTwrite(bases);
    USARTwrite(0x02);
    USARTwrite(bases+0x02);//checksum
    for(int p=1;p<=num;p++){
        USARTwrite(eeprom_read(num));
    }
    USARTwrite('X');
        
}

int  HC05_receber_mensagem(){
    
    char aux = 0;
    char aux2 = 0;
    
    tipo_mensagem = 0x00;
    code = 0x00;
    canais_mensagem = 0x00;
     
        aux2 = ReadUSART();
        if(aux2 == '#'){
            tipo_mensagem = ReadUSART();
            code = ReadUSART();

            aux = ReadUSART();
            if(aux == 'C'){ // mensagem com canais
                canais_mensagem = ReadUSART();
                if(ReadUSART() == 'K'){
                    if(ReadUSART() == tipo_mensagem + code + canais_mensagem){
                        USARTclean();
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
            else{ //aqui aux = checksum 
                if(aux == tipo_mensagem + code){ //checksum já feito
                    USARTclean();
                    return 1;    
                }
                //incluir Timer depois (else if))
            }
        }
        else if (aux2 == '$'){
            tipo_mensagem = 'P';
            code = ReadUSART();
            canais_mensagem = ReadUSART();
            USARTclean();
            return 1;
        }
    return 0;
}

int  HC05_receber_acionamento(){ // recebe via bluetoth
    
    if(ReadUSART() == '#'){
        tipo_mensagem = 'P';
        code = ReadUSART();
        canais_mensagem = ReadUSART();
        
        return 1;
    }
    else{
        return 0;
    }
    
    
    
}


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

