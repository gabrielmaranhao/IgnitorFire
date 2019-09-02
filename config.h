/* 
 * File:  config.h 
 * Author: Gabriel Maranhão Soares
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.


#pragma config OSC = HS  // osc = HS
#pragma config CONFIG3H = 0x0080   // MCLRE = on
#pragma config BOR = OFF                                  
#pragma config WDT = OFF
#pragma config CPB = OFF
#pragma config CPD = OFF
#pragma config LVP = OFF
#pragma config PWRT = OFF

#define _XTAL_FREQ 20000000  //External Clock FOSC 



#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

