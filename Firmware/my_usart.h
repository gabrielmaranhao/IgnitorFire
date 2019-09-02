 
#ifndef MY_USART_H
#define	MY_USART_H

#include <xc.h>


void USARTconfig();
void USARTwrite(char data);
char USARTdataRdy(void);
char ReadUSART(void);
void USARTclose(void);
void putch(char c);
void USARTgetString ( char *buffer,  unsigned int len);
void USARTclean();


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

