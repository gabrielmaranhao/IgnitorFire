/*
 * File:   timers.c
 * Author: gabri
 *
 * Created on 10 de Junho de 2017, 16:21
 */


#include <xc.h>

#include "timers.h"

void TimerSeg_Start(){
   
    //Config   
    T0CONbits.T08BIT = 0;   // 16bit
    T0CONbits.T0CS = 0;     // internal clock
    T0CONbits.PSA = 0;      //prescaler assigned
    //1:256
    T0CONbits.T0PS0 = 1;
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS2 = 1;
    
    
    //Interrupts
    INTCONbits.GIE = 1;
    INTCONbits.TMR0IE = 1;
    
    INTCON2bits.TMR0IP = 1; // high priority
    
    //Load
    // 19531 decimal = 0x4c4c
    //0xffff - 0x4c4b = 0xb3b4
    TMR0H = 0xb3;
    TMR0L = 0xb4;
   
    //Start
    T0CONbits.TMR0ON = 1;
}

void TimerSeg_Reset(){
    TMR0H = 0xb3;
    TMR0L = 0xb4;
}

void TimerSeg_Stop(){
    TMR0H = 0xb3;
    TMR0L = 0xb4;
    T0CONbits.TMR0ON = 0;
}
