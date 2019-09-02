
#ifndef TIMERS_H
#define	XIMERS_H

#include <xc.h> 

void TimerSeg_Start();
void TimerSeg_Reset();
void TimerSeg_Stop();

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

