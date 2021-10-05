/************************************************************
 * File:    beep.h                                          *
 * Author:  Dr.Santi Nuratch                                *
 *          Embedded Computing and Control Laboratory       *
 * Update:  05 July 2017, 12.36 PM                          *
 ************************************************************/
#ifndef __BEEP_H__
#define __BEEP_H__

#include "config.h"

void Beep_Init(void);
void Beep_FreqSet(float freq);
void Beep_PowerSet(float power /* 0.0 - 1*/); 
void Beep(unsigned int period);
void Beep_Service(void);
#endif